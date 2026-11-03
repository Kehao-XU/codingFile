#include "core/ScoreBoard.h"

namespace zuma::core {

void ScoreBoard::AddElimination(std::size_t count, int round) {
    if (round < 1) {
        round = 1;
    }
    score_ += static_cast<int>(count) * kBaseScorePerBall * round;
    eliminatedTotal_ += static_cast<int>(count);
    if (round > maxCombo_) {
        maxCombo_ = round;
    }
}

void ScoreBoard::RegisterTurn() {
    ++turn_;
}

void ScoreBoard::Restore(int score, int turn, int maxCombo, int eliminatedTotal) {
    score_ = score;
    turn_ = turn;
    maxCombo_ = maxCombo;
    eliminatedTotal_ = eliminatedTotal;
}

}  // namespace zuma::core
