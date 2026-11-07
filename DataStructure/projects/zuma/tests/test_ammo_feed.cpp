#include <string>
#include <vector>

#include "core/AmmoGenerator.h"
#include "core/FeedQueue.h"
#include "test_harness.h"

void RunAmmoFeedTests() {
    using zuma::core::AmmoGenerator;
    using zuma::core::FeedQueue;

    CASE("Ammo: 初始当前球与下一个球");
    {
        AmmoGenerator ammo("BBGYRRGBYPRG");
        CHECK_EQ(ammo.Current(), 'B');
        CHECK_EQ(ammo.Next(), 'B');
        CHECK_EQ(ammo.Cursor(), std::size_t(2));
    }

    CASE("Ammo: 逐字符推进");
    {
        AmmoGenerator ammo("BBGYRRGBYPRG");
        ammo.Advance();  // 第 1 个有效回合后
        CHECK_EQ(ammo.Current(), 'B');
        CHECK_EQ(ammo.Next(), 'G');

        ammo.Advance();  // 第 2 个有效回合后
        CHECK_EQ(ammo.Current(), 'G');
        CHECK_EQ(ammo.Next(), 'Y');
    }

    CASE("Ammo: 周期 = 字符串长度（无限循环，永不耗尽）");
    {
        const std::string pattern = "BBGYRRGBYPRG";
        AmmoGenerator ammo(pattern);

        std::vector<char> sequence;
        for (std::size_t index = 0; index < pattern.size(); ++index) {
            sequence.push_back(ammo.Current());
            ammo.Advance();
        }
        CHECK_EQ(std::string(sequence.begin(), sequence.end()), pattern);

        // 再推进 2 个整周期，状态应回到初始
        for (std::size_t index = 0; index < pattern.size() * 2; ++index) {
            ammo.Advance();
        }
        CHECK_EQ(ammo.Current(), 'B');
        CHECK_EQ(ammo.Next(), 'B');
        CHECK_EQ(ammo.Cursor(), std::size_t(2));
    }

    CASE("Ammo: 长度为 1 时当前球与下一个球恒相同");
    {
        AmmoGenerator ammo("R");
        CHECK_EQ(ammo.Current(), 'R');
        CHECK_EQ(ammo.Next(), 'R');
        for (int index = 0; index < 5; ++index) {
            ammo.Advance();
            CHECK_EQ(ammo.Current(), 'R');
        }
    }

    CASE("Feed: 先进先出");
    {
        FeedQueue feed("GBY");
        CHECK_EQ(feed.size(), std::size_t(3));
        CHECK(feed.Pop()->color == 'G');
        CHECK(feed.Pop()->color == 'B');
        CHECK_EQ(feed.size(), std::size_t(1));
        CHECK(feed.Pop()->color == 'Y');
        CHECK(feed.empty());
        CHECK_FALSE(feed.Pop().has_value());
    }

    CASE("Feed: 空等待区");
    {
        FeedQueue feed("");
        CHECK(feed.empty());
        CHECK_FALSE(feed.Pop().has_value());
        CHECK(feed.Remaining().empty());
    }

    CASE("Feed: 快照与恢复保持队首顺序");
    {
        FeedQueue feed("RGBY");
        const std::vector<char> snapshot = feed.Snapshot();
        CHECK_EQ(snapshot, std::vector<char>({'R', 'G', 'B', 'Y'}));

        feed.Pop();
        feed.Pop();
        feed.Pop();
        feed.Restore(snapshot);
        CHECK_EQ(feed.size(), std::size_t(4));
        CHECK(feed.Pop()->color == 'R');
        CHECK(feed.Pop()->color == 'G');
    }
}
