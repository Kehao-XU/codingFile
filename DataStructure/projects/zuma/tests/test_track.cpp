#include <iterator>
#include <string>

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

}  // namespace

void RunTrackTests() {
    using zuma::core::Ball;
    using zuma::core::Track;

    CASE("Track: 空链插入位置 1");
    {
        Track track = MakeTrack("");
        CHECK(track.empty());
        CHECK_EQ(track.size(), std::size_t(0));
        track.Insert(1, Ball{'R'});
        CHECK_EQ(ToString(track), std::string("R"));
    }

    CASE("Track: 中间插入 / 入口端追加");
    {
        Track track = MakeTrack("RG");
        track.Insert(2, Ball{'B'});
        CHECK_EQ(ToString(track), std::string("RBG"));

        track.Append(Ball{'Y'});
        CHECK_EQ(ToString(track), std::string("RBGY"));

        track.Insert(5, Ball{'P'});  // == size()+1，等价于追加到入口端
        CHECK_EQ(ToString(track), std::string("RBGYP"));

        track.Insert(1, Ball{'W'});  // 插到最深处
        CHECK_EQ(ToString(track), std::string("WRBGYP"));
    }

    CASE("Track: 位置定位边界");
    {
        Track track = MakeTrack("RG");
        CHECK(track.PositionOf(0) == track.end());  // 0 视为非法
        CHECK(track.PositionOf(3) == track.end());  // size()+1 == end()
        CHECK(track.PositionOf(4) == track.end());  // 越界也落到 end()
        CHECK(track.PositionOf(1)->color == 'R');
        CHECK(track.PositionOf(2)->color == 'G');
    }

    CASE("Track: 区间删除与迭代器稳定性");
    {
        Track track = MakeTrack("RRGGRR");
        const Track::iterator first = track.PositionOf(3);
        const Track::iterator last = track.PositionOf(5);
        const Track::iterator left = std::prev(first);
        const Track::iterator right = last;

        const Track::iterator returned = track.Erase(first, last);
        CHECK(returned == right);
        CHECK_EQ(ToString(track), std::string("RRRR"));
        // 区间之外节点的迭代器在 erase 后依然有效（消除/连锁依赖这一点）
        CHECK(left->color == 'R');
        CHECK(right->color == 'R');
    }

    CASE("Track: 三连段检测");
    {
        CHECK_FALSE(MakeTrack("RRB").HasRunOfThreeOrMore());
        CHECK(MakeTrack("RRRB").HasRunOfThreeOrMore());
        CHECK_FALSE(MakeTrack("").HasRunOfThreeOrMore());
        CHECK(MakeTrack("BRRRR").HasRunOfThreeOrMore());
        CHECK_FALSE(MakeTrack("RGRGR").HasRunOfThreeOrMore());
    }
}
