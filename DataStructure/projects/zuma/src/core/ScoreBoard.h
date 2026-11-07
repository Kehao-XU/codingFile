#pragma once

#include <cstddef>

namespace zuma::core {

// 计分板。计分规则（自定义，需在项目报告中说明）：
//   第 r 轮消除 k 个球  ->  + k * kBaseScorePerBall * r
//   丢弃 / 未消除的发射  ->  +0
//   每个有效回合（成功发射或丢弃）-> 回合数 +1
class ScoreBoard {
public:
    static constexpr int kBaseScorePerBall = 10;

    void AddElimination(std::size_t count, int round);
    void RegisterTurn();

    int Score() const noexcept { return score_; }
    int Turn() const noexcept { return turn_; }
    int MaxCombo() const noexcept { return maxCombo_; }
    int EliminatedTotal() const noexcept { return eliminatedTotal_; }

    void Restore(int score, int turn, int maxCombo, int eliminatedTotal);

private:
    int score_ = 0;
    int turn_ = 0;
    int maxCombo_ = 0;
    int eliminatedTotal_ = 0;
};

}  // namespace zuma::core
