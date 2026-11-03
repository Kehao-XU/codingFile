#pragma once

#include <cstddef>
#include <vector>

#include "core/Track.h"

namespace zuma::core {

// 一轮消除的记录。round == 1 表示普通消除，round >= 2 表示第 (round-1) 次连锁。
struct EliminationRound {
    int round = 0;
    std::size_t count = 0;
    char color = '?';
    int gainedScore = 0;
};

// 消除与连锁消除引擎。无状态、不依赖计分板，保证可独立单元测试。
class EliminationEngine {
public:
    // 以 seed 为中心反复执行"收集极大同色段 -> 长度 >= 3 则删除"，
    // 删除后若断点两侧同色则以断点右侧球为新种子继续（连锁）。
    // 返回值按轮次升序排列；track 未被修改时返回空。
    static std::vector<EliminationRound> Resolve(Track& track, Track::iterator seed,
                                                 int baseScorePerBall = 10);

    // 收集包含 seed 的极大同色连续段（按链上顺序）。
    static std::vector<Track::iterator> CollectRun(Track& track, Track::iterator seed);

private:
    static constexpr std::size_t kMinRunToEliminate = 3;
};

}  // namespace zuma::core
