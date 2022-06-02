#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>
#include "judwin.hpp"
#include "judrdy.hpp"

template <int N, class Func>
void dealtile(int n, int m, int l, Func func)
{
  static std::vector<int> hd(N);

  if (n == N) {
    func(hd);
  }
  else {
    for (int i = std::min(4, m); i >= std::max(0, m - l); --i) {
      hd[n] = i;
      dealtile<N>(n + 1, m - i, l - 4, func);
    }
  }
}

template <int N>
std::ofstream& print(std::ofstream& fout, std::vector<int>& hd)
{
  for (int j = 0; j < N; ++j) {
    fout << hd[j] << " ";
  }
  return fout;
}

bool issp(int* begin, const int* end)
{
  for (; begin != end; ++begin) {
    if (*begin != 0 && *begin != 2) return false;
  }
  return true;
}

int isrh(int* begin, const int* end, std::function<int(int*, const int*)> func)
{
  int wait = 0;

  for (auto itr = begin; itr != end; ++itr) {
    if (*itr < 4) {
      ++(*itr);
      if (func(begin, end)) wait ^= 1 << (end - begin);
      --(*itr);
    }
    wait >>= 1;
  }
  return wait;
}

int main()
{
  {
    std::ofstream fout("win_s_3_0.txt");

    std::cout << "3N Win (Simple)" << std::endl;

    for (int i = 0; i < 5; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i, 32, [&](std::vector<int>& hd) {
        if (iswh0(hd.data())) {
          print<9>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_h_3_0.txt");

    std::cout << "3N Win (Honor)" << std::endl;

    for (int i = 0; i < 5; ++i) {
      int cnt = 0;
      dealtile<7>(0, 3 * i, 24, [&](std::vector<int>& hd) {
        if (iswhs(hd.data())) {
          print<7>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_s_3_2.txt");

    std::cout << "3N+2 Win (Simple)" << std::endl;

    for (int i = 0; i < 4; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i + 2, 32, [&](std::vector<int>& hd) {
        if (int wait = isrh2(hd.data()); iswh2(hd.data())) {
          print<9>(fout, hd) << wait << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_h_3_2.txt");

    std::cout << "3N+2 Win (Honor)" << std::endl;

    for (int i = 0; i < 4; ++i) {
      int cnt = 0;
      dealtile<7>(0, 3 * i + 2, 24, [&](std::vector<int>& hd) {
        if (iswhs(hd.data())) {
          int p = 0;
          for (; p < 7 && hd[p] != 2; ++p) {
          }
          print<7>(fout, hd) << (1UL << p) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_s_3_2.txt");

    std::cout << "3N+2 Rdy (Simple)" << std::endl;

    for (int i = 0; i < 4; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i + 2, 32, [&](std::vector<int>& hd) {
        if (int wait = isrh2(hd.data()); wait && !iswh2(hd.data())) {
          print<9>(fout, hd) << wait << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_s_3_1.txt");

    std::cout << "3N+1 Rdy (Simple)" << std::endl;

    for (int i = 0; i < 5; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i + 1, 32, [&](std::vector<int>& hd) {
        if (int wait = isrh1(hd.data()); wait) {
          print<9>(fout, hd) << wait << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_h_3_1.txt");

    std::cout << "3N+1 Rdy (Honor)" << std::endl;

    for (int i = 0; i < 5; ++i) {
      int cnt = 0;
      dealtile<7>(0, 3 * i + 1, 24, [&](std::vector<int>& hd) {
        if (int wait = isrh(&hd[0], &hd[7], [](auto begin, auto) { return iswhs(begin); }); wait) {
          print<7>(fout, hd) << wait << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_s_2_0.txt");

    std::cout << "2N Win (Simple)" << std::endl;

    for (int i = 0; i < 7; ++i) {
      int cnt = 0;
      dealtile<9>(0, 2 * i, 32, [&](std::vector<int>& hd) {
        if (issp(&hd[0], &hd[9])) {
          print<9>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_h_2_0.txt");

    std::cout << "2N Win (Honor)" << std::endl;

    for (int i = 0; i < 7; ++i) {
      int cnt = 0;
      dealtile<7>(0, 2 * i, 24, [&](std::vector<int>& hd) {
        if (issp(&hd[0], &hd[9])) {
          print<7>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_s_2_1.txt");

    std::cout << "2N+1 Rdy (Simple)" << std::endl;

    for (int i = 0; i < 7; ++i) {
      int cnt = 0;
      dealtile<9>(0, 2 * i + 1, 32, [&](std::vector<int>& hd) {
        if (int wait = isrh(&hd[0], &hd[9], [](auto begin, auto end) { return issp(begin, end); }); wait) {
          print<9>(fout, hd) << wait << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_h_2_1.txt");

    std::cout << "2N+1 Rdy (Honor)" << std::endl;

    for (int i = 0; i < 7; ++i) {
      int cnt = 0;
      dealtile<7>(0, 2 * i + 1, 24, [&](std::vector<int>& hd) {
        if (int wait = isrh(&hd[0], &hd[7], [](auto begin, auto end) { return issp(begin, end); }); wait) {
          print<7>(fout, hd) << wait << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_s_6_0.txt");

    std::cout << "6N (3N, 2N) Win (Simple)" << std::endl;

    for (int i = 0; i < 5; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i, 32, [&](std::vector<int>& hd) {
        if (iswh0(hd.data()) && issp(&hd[0], &hd[9])) {
          print<9>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_h_6_0.txt");

    std::cout << "6N (3N, 2N) Win (Honor)" << std::endl;

    for (int i = 0; i < 1; ++i) {
      int cnt = 0;
      dealtile<7>(0, 3 * i, 24, [&](std::vector<int>& hd) {
        if (iswh0(hd.data()) && issp(&hd[0], &hd[7])) {
          print<7>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_s_6_2.txt");

    std::cout << "6N+2 (3N+2, 2N) Win (Simple)" << std::endl;

    for (int i = 0; i < 3; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i + 2, 32, [&](std::vector<int>& hd) {
        if (iswh2(hd.data()) && issp(&hd[0], &hd[9])) {
          print<9>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("win_h_6_2.txt");

    std::cout << "6N+2 (3N+2, 2N) Win (Honor)" << std::endl;

    for (int i = 0; i < 1; ++i) {
      int cnt = 0;
      dealtile<7>(0, 3 * i + 2, 24, [&](std::vector<int>& hd) {
        if (iswhs(hd.data()) && issp(&hd[0], &hd[7])) {
          print<7>(fout, hd) << "\n";
          ++cnt;
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_s_6_5.txt");

    std::cout << "6N+5 (3N+2, 2N+1) Rdy (Simple)" << std::endl;

    for (int i = 0; i < 4; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i + 2, 32, [&](std::vector<int>& hd) {
        if (int wait = isrh2(hd.data()); wait) {
          if (isrh(&hd[0], &hd[9], [](auto begin, auto end) { return issp(begin, end); })) {
            print<9>(fout, hd) << wait << "\n";
            ++cnt;
          }
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_s_6_1.txt");

    std::cout << "6N+1 (3N+1, 2N+1) Rdy (Simple)" << std::endl;

    for (int i = 0; i < 5; ++i) {
      int cnt = 0;
      dealtile<9>(0, 3 * i + 1, 32, [&](std::vector<int>& hd) {
        if (int wait = isrh1(hd.data()); wait) {
          if (isrh(&hd[0], &hd[9], [](auto begin, auto end) { return issp(begin, end); })) {
            print<9>(fout, hd) << wait << "\n";
            ++cnt;
          }
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  {
    std::ofstream fout("rdy_h_6_1.txt");

    std::cout << "6N+1 (3N+1, 2N+1) Rdy (Honor)" << std::endl;

    for (int i = 0; i < 1; ++i) {
      int cnt = 0;
      dealtile<7>(0, 3 * i + 1, 24, [&](std::vector<int>& hd) {
        if (int wait = isrh(&hd[0], &hd[7], [](auto begin, auto) { return iswhs(begin); }); wait) {
          if (isrh(&hd[0], &hd[7], [](auto begin, auto end) { return issp(begin, end); })) {
            print<7>(fout, hd) << wait << "\n";
            ++cnt;
          }
        }
      });
      std::cout << i << "\t" << cnt << std::endl;
    }
  }

  return 0;
}
