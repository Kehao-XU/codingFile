#include "core/EliminationEngine.h"

#include <iterator>

namespace zuma::core {

std::vector<Track::iterator> EliminationEngine::CollectRun(Track& track, Track::iterator seed) {
    std::vector<Track::iterator> run;
    if (seed == track.end()) {
        return run;
    }

    const char color = seed->color;

    // 向左扩展。leftPart 为「由近及远」的顺序，反转后即与链上顺序一致。
    std::vector<Track::iterator> leftPart;
    Track::iterator left = seed;
    while (left != track.begin()) {
        Track::iterator previous = std::prev(left);
        if (previous->color != color) {
            break;
        }
        left = previous;
        leftPart.push_back(left);
    }

    run.assign(leftPart.rbegin(), leftPart.rend());
    run.push_back(seed);

    // 向右扩展
    Track::iterator right = seed;
    while (std::next(right) != track.end()) {
        Track::iterator next = std::next(right);
        if (next->color != color) {
            break;
        }
        right = next;
        run.push_back(right);
    }

    return run;
}

std::vector<EliminationRound> EliminationEngine::Resolve(Track& track, Track::iterator seed,
                                                         int baseScorePerBall) {
    std::vector<EliminationRound> rounds;
    int roundIndex = 0;

    while (true) {
        const std::vector<Track::iterator> run = CollectRun(track, seed);
        if (run.size() < kMinRunToEliminate) {
            break;
        }

        ++roundIndex;

        const char color = run.front()->color;
        const std::size_t count = run.size();
        // 先取断点两侧的邻居：std::list 的 erase 不影响其它节点的迭代器。
        const Track::iterator left =
            (run.front() == track.begin()) ? track.end() : std::prev(run.front());
        const Track::iterator right = std::next(run.back());

        track.Erase(run.front(), right);

        rounds.push_back(
            EliminationRound{roundIndex, count, color,
                             static_cast<int>(count) * baseScorePerBall * roundIndex});

        if (left != track.end() && right != track.end() && left->color == right->color) {
            seed = right;  // 断点两侧同色：合并成新段，进入下一轮连锁
        } else {
            break;
        }
    }

    return rounds;
}

}  // namespace zuma::core
