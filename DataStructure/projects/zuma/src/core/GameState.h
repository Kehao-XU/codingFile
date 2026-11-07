#pragma once

#include <cstddef>
#include <vector>

#include "core/GameStatus.h"

namespace zuma::core {

// 一局游戏的完整快照（Memento 模式）。纯值语义，可直接拷贝，供撤销/重做与将来的存档使用。
struct GameState {
    std::vector<char> track;  // 轨道内容，按位置 1..N 顺序
    std::vector<char> feed;   // 等待区，队首在前
    std::size_t ammoCursor = 0;
    char currentBall = '?';
    char nextBall = '?';
    int score = 0;
    int turn = 0;
    int maxCombo = 0;
    int eliminatedTotal = 0;
    GameStatus status = GameStatus::Playing;
};

}  // namespace zuma::core
