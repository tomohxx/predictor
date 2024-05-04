#include <algorithm>
#include <fstream>
#include <stdexcept>
#include "predictor.hpp"

namespace pred {
  Value& Value::operator+=(const Value& rhs)
  {
    all += rhs.all;
    states += rhs.states;
    return *this;
  }

  Value& Value::operator-=(const Value& rhs)
  {
    all -= rhs.all;
    states -= rhs.states;
    return *this;
  }

  const Value operator+(const Value& lhs, const Value& rhs)
  {
    return Value(lhs) += rhs;
  }

  const Value operator-(const Value& lhs, const Value& rhs)
  {
    return Value(lhs) -= rhs;
  }

  std::valarray<double> Value::props() const
  {
    std::valarray<double> ret(0., K);

    if (all > 0) {
      for (int i = 0; i < K; ++i) {
        ret[i] = static_cast<double>(states[i]) / all;
      }
    }
    return ret;
  }

  std::valarray<int> Value::props100() const
  {
    std::valarray<int> ret(0, K);

    if (all > 0) {
      for (int i = 0; i < K; ++i) {
        ret[i] = states[i] * 100 / all;
      }
    }
    return ret;
  }

  int convert(const int* first, const int* last)
  {
    int ret = 0;

    for (auto i = last - first; i > 0; --i) {
      ret = ret << 1 | !!(*(first + i - 1));
    }
    return ret;
  }

  void Predictor::read_file(Hands& hands, std::filesystem::path file)
  {
    std::ifstream fin(file);

    if (!fin) {
      throw std::runtime_error("Reading file does not exist: " + file.string());
    }

    for (auto& hand : hands) {
      for (auto& elem : hand) {
        fin >> elem;
      }
    }
  }

  void Predictor::count1(State1d& f, const Hands& hands, const Pattern& kind, const int* wall, const int length) const
  {
    auto itr = hands.begin();

    std::fill(f.begin(), f.end(), 0);

    for (std::size_t i = 0; i < f.size(); ++i) {
      for (int j = 0; j < kind[i]; ++j) {
        int s = 1;

        for (int k = 0; k < length; ++k) {
          s *= combin[wall[k]][(*itr)[k]];
        }
        f[i] += s;
        ++itr;
      }
    }
  }

  void Predictor::count2(State1d& f, State2d& g, const Hands& hands, const Pattern& kind, const int* wall, const int river, const int length) const
  {
    auto itr = hands.begin();

    std::fill(f.begin(), f.end(), 0);
    std::for_each(g.begin(), g.end(), [](auto& v) { std::fill(v.begin(), v.end(), 0); });

    for (std::size_t i = 0; i < f.size(); ++i) {
      for (int j = 0; j < kind[i]; ++j) {
        int s = 1;
        const int wait = (*itr)[length];

        for (int k = 0; k < length; ++k) {
          s *= combin[wall[k]][(*itr)[k]];
        }

        if ((wait & (~river)) == wait) {
          for (int k = 0; k < length; ++k) {
            if (wait & (1 << k)) g[k][i] += s;
          }
          f[i] += s;
        }
        ++itr;
      }
    }
  }

  void Predictor::add1_lh(State2d& lhs, const State2d& rhs, const int m)
  {
    // update ld
    for (int j = m; j >= 0; --j) {
      int64_t tmp = 0;

      for (int k = 0; k <= j; ++k) {
        tmp += lhs[A][k] * rhs[D][j - k] + lhs[D][k] * rhs[A][j - k];
      }
      for (int k = 0; k <= j - 1; ++k) {
        tmp += lhs[B1][k] * rhs[C][j - k - 1] + lhs[C][k] * rhs[B1][j - k - 1] + lhs[B2][k] * rhs[B2][j - k - 1];
      }
      lhs[D][j] = tmp;
    }

    // update lc
    for (int j = m - 1; j >= 0; --j) {
      int64_t tmp = 0;

      for (int k = 0; k <= j; ++k) {
        tmp += lhs[A][k] * rhs[C][j - k] + lhs[C][k] * rhs[A][j - k];
      }
      lhs[C][j] = tmp;
    }

    // update lb1
    for (int j = m - 1; j >= 0; --j) {
      int64_t tmp = 0;

      for (int k = 0; k <= j; ++k) {
        tmp += lhs[A][k] * rhs[B1][j - k] + lhs[B1][k] * rhs[A][j - k];
      }
      lhs[B1][j] = tmp;
    }

    // update lb2
    for (int j = m - 1; j >= 0; --j) {
      int64_t tmp = 0;

      for (int k = 0; k <= j; ++k) {
        tmp += lhs[A][k] * rhs[B2][j - k] + lhs[B2][k] * rhs[A][j - k];
      }
      lhs[B2][j] = tmp;
    }

    // update la
    for (int j = m; j >= 0; --j) {
      int64_t tmp = 0;

      for (int k = 0; k <= j; ++k) {
        tmp += lhs[A][k] * rhs[A][j - k];
      }
      lhs[A][j] = tmp;
    }
  }

  int64_t Predictor::add2_lh(const State2d& lhs, const State3d& rhs, const int i, const int m)
  {
    // update ld

    int64_t ret = 0;

    for (int k = 0; k <= m; ++k) {
      ret += lhs[A][k] * rhs[D][i][m - k];
    }
    for (int k = 0; k <= m - 1; ++k) {
      ret += lhs[B1][k] * rhs[C][i][m - k - 1] + lhs[B2][k] * rhs[B2][i][m - k - 1];
    }
    return ret;
  }

  void Predictor::add1_sp(State2d& lhs, const State2d& rhs, const int m)
  {
    // update lb
    for (int j = m; j >= 0; --j) {
      int64_t tmp = 0;

      for (int k = 0; k <= j; ++k) {
        tmp += lhs[A][k] * rhs[B][j - k] + lhs[B][k] * rhs[A][j - k];
      }
      lhs[B][j] = tmp;
    }

    // update la
    for (int j = m; j >= 0; --j) {
      int64_t tmp = 0;

      for (int k = 0; k <= j; ++k) {
        tmp += lhs[A][k] * rhs[A][j - k];
      }
      lhs[A][j] = tmp;
    }
  }

  int64_t Predictor::add2_sp(const State2d& lhs, const State3d& rhs, const int i, const int m)
  {
    // update lb

    int64_t ret = 0;

    for (int k = 0; k <= m; ++k) {
      ret += lhs[A][k] * rhs[B][i][6 - k] + lhs[B][k] * rhs[A][i][6 - k];
    }
    return ret;
  }

  Predictor::State2d Predictor::synthesize(const State3d& f, const int mask, const int m, void (*add1)(State2d&, const State2d&, int))
  {
    State2d ret(4);
    int flag = false;

    for (int i = 0; i < 4; ++i) {
      if (mask & (1 << i)) {
        if (flag) {
          (*add1)(ret, f[i], m);
        }
        else {
          ret = f[i];
          flag = true;
        }
      }
    }
    return ret;
  }

  Value Predictor::calc(const State3d& f, const State4d& g, const int m, const int specifier,
                        void (*add1)(State2d&, const State2d&, int),
                        int64_t (*add2)(const State2d&, const State3d&, int, int))
  {
    Value ret;

    ret.all = synthesize(f, 15, m, add1)[specifier][m];

    for (int i = 0; i < 3; ++i) {
      const auto tmp = synthesize(f, 15 ^ (1 << i), m, add1);

      for (int j = 0; j < 9; ++j) {
        ret.states[9 * i + j] = add2(tmp, g[i], j, m);
      }
    }

    const auto tmp = synthesize(f, 7, m, add1);

    for (int j = 0; j < 7; ++j) {
      ret.states[27 + j] = add2(tmp, g[3], j, m);
    }
    return ret;
  }

  void Predictor::initialize(const std::string& dir)
  {
    read_file(win_s_3_0, std::filesystem::path(dir) / "win_s_3_0.txt");
    read_file(win_h_3_0, std::filesystem::path(dir) / "win_h_3_0.txt");
    read_file(win_s_3_2, std::filesystem::path(dir) / "win_s_3_2.txt");
    read_file(win_h_3_2, std::filesystem::path(dir) / "win_h_3_2.txt");
    read_file(rdy_s_3_2, std::filesystem::path(dir) / "rdy_s_3_2.txt");
    read_file(rdy_s_3_1, std::filesystem::path(dir) / "rdy_s_3_1.txt");
    read_file(rdy_h_3_1, std::filesystem::path(dir) / "rdy_h_3_1.txt");

    read_file(win_s_2_0, std::filesystem::path(dir) / "win_s_2_0.txt");
    read_file(win_h_2_0, std::filesystem::path(dir) / "win_h_2_0.txt");
    read_file(rdy_s_2_1, std::filesystem::path(dir) / "rdy_s_2_1.txt");
    read_file(rdy_h_2_1, std::filesystem::path(dir) / "rdy_h_2_1.txt");

    read_file(win_s_6_0, std::filesystem::path(dir) / "win_s_6_0.txt");
    read_file(win_h_6_0, std::filesystem::path(dir) / "win_h_6_0.txt");
    read_file(win_s_6_2, std::filesystem::path(dir) / "win_s_6_2.txt");
    read_file(win_h_6_2, std::filesystem::path(dir) / "win_h_6_2.txt");
    read_file(rdy_s_6_5, std::filesystem::path(dir) / "rdy_s_6_5.txt");
    read_file(rdy_s_6_1, std::filesystem::path(dir) / "rdy_s_6_1.txt");
    read_file(rdy_h_6_1, std::filesystem::path(dir) / "rdy_h_6_1.txt");
  }

  Value Predictor::calc_lh(const std::vector<int>& wall, const std::vector<int>& river, const int m) const
  {
    // number of states in the n'th group (n, s, m)
    State3d f(4, State2d(5, State1d(5, 0)));
    // number of states waiting for a tile (n, s, i, m)
    State4d g(4, State3d(5, State2d(9, State1d(5, 0))));

    for (int i = 0; i < 3; ++i) {
      const auto river_ = convert(&river[9 * i], &river[9 * i + 9]);

      count1(f[i][A], win_s_3_0, k_win_s_3_0, &wall[9 * i], SIMPLE);                    // A
      count1(f[i][B1], win_s_3_2, k_win_s_3_2, &wall[9 * i], SIMPLE);                   // B1
      count2(f[i][B2], g[i][B2], win_s_3_2, k_win_s_3_2, &wall[9 * i], river_, SIMPLE); // B2
      count2(f[i][C], g[i][C], rdy_s_3_2, k_rdy_s_3_2, &wall[9 * i], river_, SIMPLE);   // C
      count2(f[i][D], g[i][D], rdy_s_3_1, k_rdy_s_3_1, &wall[9 * i], river_, SIMPLE);   // D
    }

    const auto river_ = convert(&river[27], &river[K]);

    count1(f[3][A], win_h_3_0, k_win_h_3_0, &wall[27], HONOR);                    // A
    count1(f[3][B1], win_h_3_2, k_win_h_3_2, &wall[27], HONOR);                   // B1
    count2(f[3][B2], g[3][B2], win_h_3_2, k_win_h_3_2, &wall[27], river_, HONOR); // B2
    // f[3][C] and g[3][C] will not be updated
    count2(f[3][D], g[3][D], rdy_h_3_1, k_rdy_h_3_1, &wall[27], river_, HONOR); // D

    return calc(f, g, m, D, add1_lh, add2_lh);
  }

  Value Predictor::calc_sp(const std::vector<int>& wall, const std::vector<int>& river) const
  {
    // number of states in the n'th group (n, s, m)
    State3d f(4, State2d(2, State1d(7, 0)));
    // number of states waiting for a tile (n, s, i, m)
    State4d g(4, State3d(2, State2d(9, State1d(7, 0))));

    for (int i = 0; i < 3; ++i) {
      const auto river_ = convert(&river[9 * i], &river[9 * i + 9]);

      count1(f[i][A], win_s_2_0, k_win_s_2_0, &wall[9 * i], SIMPLE);                  // A
      count2(f[i][B], g[i][B], rdy_s_2_1, k_rdy_s_2_1, &wall[9 * i], river_, SIMPLE); // B
    }

    const auto river_ = convert(&river[27], &river[K]);

    count1(f[3][A], win_h_2_0, k_win_h_2_0, &wall[27], HONOR);                  // A
    count2(f[3][B], g[3][B], rdy_h_2_1, k_rdy_h_2_1, &wall[27], river_, HONOR); // B

    return calc(f, g, 6, B, add1_sp, add2_sp);
  }

  Value Predictor::calc_lh_sp(const std::vector<int>& wall, const std::vector<int>& river) const
  {
    // number of states in the n'th group (n, s, m)
    State3d f(4, State2d(5, State1d(5, 0)));
    // number of states waiting for a tile (n, s, i, m)
    State4d g(4, State3d(5, State2d(9, State1d(5, 0))));

    for (int i = 0; i < 3; ++i) {
      const auto river_ = convert(&river[9 * i], &river[9 * i + 9]);

      count1(f[i][A], win_s_6_0, k_win_s_6_0, &wall[9 * i], SIMPLE);  // A
      count1(f[i][B1], win_s_6_2, k_win_s_6_2, &wall[9 * i], SIMPLE); // B1
      // f[i][B2] and g[i][B2] will not be updated
      count2(f[i][C], g[i][C], rdy_s_6_5, k_rdy_s_6_5, &wall[9 * i], river_, SIMPLE); // C
      count2(f[i][D], g[i][D], rdy_s_6_1, k_rdy_s_6_1, &wall[9 * i], river_, SIMPLE); // D
    }

    const auto river_ = convert(&river[27], &river[K]);

    count1(f[3][A], win_h_6_0, k_win_h_6_0, &wall[27], HONOR);  // A
    count1(f[3][B1], win_h_6_2, k_win_h_6_2, &wall[27], HONOR); // B1
    // f[3][B2] and g[3][B2] will not be updated
    // f[3][C] and g[3][C] will not be updated
    count2(f[3][D], g[3][D], rdy_h_6_1, k_rdy_h_6_1, &wall[27], river_, 7); // D

    return calc(f, g, 4, D, add1_lh, add2_lh);
  }

  Value Predictor::calc_to(const std::vector<int>& wall, const std::vector<int>& river) const
  {
    Value ret;

    for (const int pair : index) {
      for (const int none : index) {
        if (pair == none) continue;

        int s = 1;

        for (const int i : index) {
          if (i == pair) {
            s *= combin[wall[i]][2];
          }
          else if (i != none) {
            s *= combin[wall[i]][1];
          }
        }

        if (!river[none]) {
          ret.all += s;
          ret.states[none] += s;
        }
      }
    }

    for (const int i : index) {
      if (river[i]) return ret;
    }

    int s = 1;

    for (const int i : index) {
      s *= combin[wall[i]][1];
    }
    ret.all += s;

    for (const int i : index) {
      if (!river[i]) ret.states[i] += s;
    }

    return ret;
  }

  std::vector<Value> Predictor::operator()(const std::vector<int>& wall, const std::vector<int>& river, const int m) const
  {
#ifdef CHECK_WALL
    if (wall.size() != K) {
      throw std::runtime_error("Size error of \'wall\'");
    }

    if (river.size() != K) {
      throw std::runtime_error("Size error of \'river\'");
    }

    for (const auto elem : wall) {
      if (elem < 0 || elem > 4) {
        throw std::runtime_error("An abnoraml data exsits in \'wall\'");
      }
    }

    for (const auto elem : river) {
      if (elem < 0 || elem > 4) {
        throw std::runtime_error("An abnoraml data exsits in \'river\'");
      }
    }

    if (m < 0 || m > 4) {
      throw std::runtime_error("Range error of \'m\'");
    }
#endif

    auto ret_lh = calc_lh(wall, river, m);
    auto ret_sp = m == 4 ? calc_sp(wall, river) : Value();
    auto ret_lh_sp = m == 4 ? calc_lh_sp(wall, river) : Value();
    auto ret_to = m == 4 ? calc_to(wall, river) : Value();

    ret_sp -= ret_lh_sp;

    return {ret_lh + ret_sp + ret_to, ret_lh, ret_sp, ret_to};
  }
}
