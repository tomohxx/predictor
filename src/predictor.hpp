#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <cstdint>
#include <filesystem>
#include <valarray>
#include <vector>
#include "constant.hpp"

namespace pred {
  struct Value {
    int64_t all = 0;
    std::valarray<int64_t> states;

    Value()
        : states(INT64_C(0), K) {}
    Value& operator+=(const Value& rhs);
    Value& operator-=(const Value& rhs);
    std::valarray<double> props();
    std::valarray<int> props100();
  };

  const Value operator+(const Value& lhs, const Value& rhs);
  const Value operator-(const Value& lhs, const Value& rhs);
  class Predictor {
  private:
    // state specifier

    static constexpr int A = 0;
    static constexpr int B = 1;
    static constexpr int B1 = 1;
    static constexpr int B2 = 2;
    static constexpr int C = 3;
    static constexpr int D = 4;

    // length

    static constexpr int SIMPLE = 9;
    static constexpr int HONOR = 7;

    // alias

    using Hand = std::vector<int>;
    using Hands = std::vector<Hand>;
    using Pattern = std::vector<int>;
    using Combin = std::vector<std::vector<int>>;
    using Index = std::vector<int>;
    using State1d = std::vector<int64_t>;
    using State2d = std::vector<State1d>;
    using State3d = std::vector<State2d>;
    using State4d = std::vector<State3d>;

    // hand pattern

    Hands win_s_3_0;
    Hands win_h_3_0;
    Hands win_s_3_2;
    Hands win_h_3_2;
    Hands rdy_s_3_2;
    Hands rdy_s_3_1;
    Hands rdy_h_3_1;

    Hands win_s_2_0;
    Hands win_h_2_0;
    Hands rdy_s_2_1;
    Hands rdy_h_2_1;

    Hands win_s_6_0;
    Hands win_h_6_0;
    Hands win_s_6_2;
    Hands win_h_6_2;
    Hands rdy_s_6_5;
    Hands rdy_s_6_1;
    Hands rdy_h_6_1;

    // number of hand patterns (size: 5)

    const Pattern k_win_s_3_0 = {1, 16, 127, 627, 2098};      // total: 2869
    const Pattern k_win_h_3_0 = {1, 7, 21, 35, 35};           // total: 99
    const Pattern k_win_s_3_2 = {9, 135, 996, 4475, 0};       // total: 5615
    const Pattern k_win_h_3_2 = {7, 42, 105, 140, 0};         // total: 294
    const Pattern k_rdy_s_3_2 = {15, 202, 1261, 4678, 0};     // total: 6156
    const Pattern k_rdy_s_3_1 = {9, 249, 2406, 12533, 40070}; // total: 55267
    const Pattern k_rdy_h_3_1 = {7, 63, 210, 350, 315};       // total: 945

    // number of hand patterns (size: 7)

    const Pattern k_win_s_2_0 = {1, 9, 36, 84, 126, 126, 84};     // total: 466
    const Pattern k_win_h_2_0 = {1, 7, 21, 35, 35, 21, 7};        // total: 127
    const Pattern k_rdy_s_2_1 = {9, 72, 252, 504, 630, 504, 252}; // total: 2223
    const Pattern k_rdy_h_2_1 = {7, 42, 105, 140, 105, 42, 7};    // total: 448

    // number of hand patterns (size: 5)

    const Pattern k_win_s_6_0 = {1, 0, 7, 0, 10};    // total: 18
    const Pattern k_win_h_6_0 = {1, 0, 0, 0, 0};     // total: 1
    const Pattern k_win_s_6_2 = {9, 0, 36, 0, 0};    // total: 45
    const Pattern k_win_h_6_2 = {7, 0, 0, 0, 0};     // total: 7
    const Pattern k_rdy_s_6_5 = {0, 21, 0, 60, 0};   // total: 81
    const Pattern k_rdy_s_6_1 = {9, 0, 144, 0, 126}; // total: 279
    const Pattern k_rdy_h_6_1 = {7, 0, 0, 0, 0};     // total: 7

    // combination
    const Combin combin = {
        {1, 0, 0, 0, 0}, // 0Cr
        {1, 1, 0, 0, 0}, // 1Cr
        {1, 2, 1, 0, 0}, // 2Cr
        {1, 3, 3, 1, 0}, // 3Cr
        {1, 4, 6, 4, 1}, // 4Cr
    };

    // yaochuuhai
    const Index index = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33};

    // read hand patterns and waiting patterns (only for tempai) from the files
    void read_file(Hands& hands, std::filesystem::path file);

    void count1(State1d& f, const Hands& hands, const Pattern& kind, const int* wall, int length) const;
    void count2(State1d& f, State2d& g, const Hands& hands, const Pattern& kind, const int* wall, int river, int length) const;

    static void add1_lh(State2d& lhs, const State2d& rhs, int m);
    static int64_t add2_lh(const State2d& lhs, const State3d& rhs, int i, int m);
    static void add1_sp(State2d& lhs, const State2d& rhs, int m);
    static int64_t add2_sp(const State2d& lhs, const State3d& rhs, int i, int m);
    // return |A|, |B|, |C|, |D|
    static State2d synthesize(const State3d& f, int mask, int m, void (*add1)(State2d&, const State2d&, int));
    static Value calc(const State3d& f, const State4d& g, int m, int specifier,
                      void (*add1)(State2d&, const State2d&, int),
                      int64_t (*add2)(const State2d&, const State3d&, int, int));

  public:
    Predictor()
        : win_s_3_0(2869, Hand(SIMPLE, 0)),
          win_h_3_0(99, Hand(HONOR, 0)),
          win_s_3_2(5615, Hand(SIMPLE + 1, 0)),
          win_h_3_2(294, Hand(HONOR + 1, 0)),
          rdy_s_3_2(6156, Hand(SIMPLE + 1, 0)),
          rdy_s_3_1(55267, Hand(SIMPLE + 1, 0)),
          rdy_h_3_1(945, Hand(HONOR + 1, 0)),
          win_s_2_0(466, Hand(SIMPLE, 0)),
          win_h_2_0(127, Hand(HONOR, 0)),
          rdy_s_2_1(2223, Hand(SIMPLE + 1, 0)),
          rdy_h_2_1(448, Hand(HONOR + 1, 0)),
          win_s_6_0(18, Hand(SIMPLE, 0)),
          win_h_6_0(1, Hand(HONOR, 0)),
          win_s_6_2(45, Hand(SIMPLE, 0)),
          win_h_6_2(7, Hand(HONOR, 0)),
          rdy_s_6_5(81, Hand(SIMPLE + 1, 0)),
          rdy_s_6_1(279, Hand(SIMPLE + 1, 0)),
          rdy_h_6_1(7, Hand(HONOR + 1, 0)) {}
    void initialize(const std::string& dir);
    Value calc_lh(const std::vector<int>& wall, const std::vector<int>& river, int m) const;
    Value calc_sp(const std::vector<int>& wall, const std::vector<int>& river) const;
    Value calc_lh_sp(const std::vector<int>& wall, const std::vector<int>& river) const;
    Value calc_to(const std::vector<int>& wall, const std::vector<int>& river) const;
    std::vector<Value> operator()(const std::vector<int>& wall, const std::vector<int>& river, int m);
  };
}

#endif
