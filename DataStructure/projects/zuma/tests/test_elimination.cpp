#include <string>
#include <vector>

#include "core/EliminationEngine.h"
#include "core/Track.h"
#include "test_harness.h"

namespace {

std::string ToString(const zuma::core::Track& track) {
    std::string text;
    for (const zuma::core::Ball& ball : track) {
        text += ball.color;
    }
    return text;
}

zuma::core::Track MakeTrack(const std::string& colors) {
    zuma::core::Track track;
    for (char color : colors) {
        track.Append(zuma::core::Ball{color});
    }
    return track;
}

std::vector<char> Colors(const std::vector<zuma::core::EliminationRound>& rounds) {
    std::vector<char> colors;
    for (const zuma::core::EliminationRound& round : rounds) {
        colors.push_back(round.color);
    }
    return colors;
}

std::vector<int> Counts(const std::vector<zuma::core::EliminationRound>& rounds) {
    std::vector<int> counts;
    for (const zuma::core::EliminationRound& round : rounds) {
        counts.push_back(static_cast<int>(round.count));
    }
    return counts;
}

}  // namespace

void RunEliminationTests() {
    using zuma::core::Ball;
    using zuma::core::EliminationEngine;
    using zuma::core::EliminationRound;
    using zuma::core::Track;

    CASE("消除 ① 不足 3 个不消除");
    {
        Track track = MakeTrack("RB");
        const Track::iterator seed = track.Insert(1, Ball{'R'});  // RRB
        const std::vector<EliminationRound> rounds = EliminationEngine::Resolve(track, seed);
        CHECK(rounds.empty());
        CHECK_EQ(ToString(track), std::string("RRB"));
    }

    CASE("消除 ② 恰好 3 个");
    {
        Track track = MakeTrack("RRB");
        const Track::iterator seed = track.Insert(3, Ball{'R'});  // RRRB
        const std::vector<EliminationRound> rounds = EliminationEngine::Resolve(track, seed);
        CHECK_EQ(rounds.size(), std::size_t(1));
        CHECK_EQ(rounds[0].round, 1);
        CHECK_EQ(rounds[0].count, std::size_t(3));
        CHECK_EQ(rounds[0].color, 'R');
        CHECK_EQ(rounds[0].gainedScore, 30);  // 3 * 10 * 1
        CHECK_EQ(ToString(track), std::string("B"));
    }

    CASE("消除 ③ 插入点两侧各 1 个同色（锚点两侧合并）");
    {
        Track track = MakeTrack("RCCR");
        const Track::iterator seed = track.Insert(3, Ball{'C'});  // RCCCR
        const std::vector<EliminationRound> rounds = EliminationEngine::Resolve(track, seed);
        CHECK_EQ(rounds.size(), std::size_t(1));
        CHECK_EQ(rounds[0].count, std::size_t(3));
        CHECK_EQ(ToString(track), std::string("RR"));  // 剩余 2 个 R 不足 3，连锁终止
    }

    CASE("消除 ④ 两轮连锁（普通消除 + 1 次连锁）");
    {
        Track track = MakeTrack("RRGGRR");
        const Track::iterator seed = track.Insert(3, Ball{'G'});  // RRGGGRR
        const std::vector<EliminationRound> rounds = EliminationEngine::Resolve(track, seed);
        CHECK_EQ(rounds.size(), std::size_t(2));
        CHECK_EQ(Counts(rounds), std::vector<int>({3, 4}));
        CHECK_EQ(Colors(rounds), std::vector<char>({'G', 'R'}));
        CHECK_EQ(rounds[1].gainedScore, 80);  // 4 * 10 * 2
        CHECK(track.empty());
    }

    CASE("消除 ⑤ 三轮连锁");
    {
        Track track = MakeTrack("RRGGBBGGRR");
        const Track::iterator seed = track.Insert(5, Ball{'B'});  // RRGGBBBGGRR
        const std::vector<EliminationRound> rounds = EliminationEngine::Resolve(track, seed);
        CHECK_EQ(rounds.size(), std::size_t(3));
        CHECK_EQ(Counts(rounds), std::vector<int>({3, 4, 4}));
        CHECK_EQ(Colors(rounds), std::vector<char>({'B', 'G', 'R'}));
        CHECK_EQ(rounds[2].gainedScore, 120);  // 4 * 10 * 3
        CHECK(track.empty());
    }

    CASE("消除 ⑦ 链首 / 链尾边界（无左邻或无右邻）");
    {
        Track head = MakeTrack("BRR");
        const Track::iterator headSeed = head.Insert(2, Ball{'R'});  // BRRR
        const std::vector<EliminationRound> headRounds = EliminationEngine::Resolve(head, headSeed);
        CHECK_EQ(headRounds.size(), std::size_t(1));
        CHECK_EQ(ToString(head), std::string("B"));

        Track tail = MakeTrack("RRB");
        const Track::iterator tailSeed = tail.Insert(3, Ball{'R'});  // RRRB
        const std::vector<EliminationRound> tailRounds = EliminationEngine::Resolve(tail, tailSeed);
        CHECK_EQ(tailRounds.size(), std::size_t(1));
        CHECK_EQ(ToString(tail), std::string("B"));
    }

    CASE("消除 ⑧ 颜色全不同不消除");
    {
        Track track = MakeTrack("RGB");
        const Track::iterator seed = track.Insert(2, Ball{'Y'});  // RYGB（插入到位置 2 之前）
        const std::vector<EliminationRound> rounds = EliminationEngine::Resolve(track, seed);
        CHECK(rounds.empty());
        CHECK_EQ(ToString(track), std::string("RYGB"));
    }

    CASE("CollectRun: 返回值与链上顺序一致");
    {
        Track track = MakeTrack("RRRG");
        const std::vector<Track::iterator> run =
            EliminationEngine::CollectRun(track, track.PositionOf(2));
        CHECK_EQ(run.size(), std::size_t(3));
        CHECK(run[0] == track.PositionOf(1));
        CHECK(run[1] == track.PositionOf(2));
        CHECK(run[2] == track.PositionOf(3));
    }

    CASE("消除 ⑥ 消除后链空（配合回合判定即胜利）");
    {
        Track track = MakeTrack("RRR");
        const Track::iterator seed = track.Insert(4, Ball{'R'});  // RRRR
        const std::vector<EliminationRound> rounds = EliminationEngine::Resolve(track, seed);
        CHECK_EQ(rounds.size(), std::size_t(1));
        CHECK_EQ(rounds[0].count, std::size_t(4));
        CHECK(track.empty());
    }
}
