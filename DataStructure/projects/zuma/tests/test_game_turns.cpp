#include <string>
#include <vector>

#include "core/ColorRegistry.h"
#include "core/Game.h"
#include "test_harness.h"

namespace {

zuma::core::Level MakeLevel(const std::string& active, const std::string& feed,
                            const std::string& ammo, std::size_t maxLength) {
    zuma::core::Level level;
    level.name = "单元测试关";
    level.sourcePath = "unit-test.txt";
    level.active = active;
    level.feed = feed;
    level.ammo = ammo;
    level.maxLength = maxLength;
    return level;
}

std::string TrackText(const zuma::core::Game& game) {
    std::string text;
    for (const zuma::core::Ball& ball : *game.View().track) {
        text += ball.color;
    }
    return text;
}

bool SameState(const zuma::core::GameState& lhs, const zuma::core::GameState& rhs) {
    return lhs.track == rhs.track && lhs.feed == rhs.feed && lhs.ammoCursor == rhs.ammoCursor &&
           lhs.currentBall == rhs.currentBall && lhs.nextBall == rhs.nextBall &&
           lhs.score == rhs.score && lhs.turn == rhs.turn && lhs.maxCombo == rhs.maxCombo &&
           lhs.eliminatedTotal == rhs.eliminatedTotal && lhs.status == rhs.status;
}

}  // namespace

void RunGameTurnTests() {
    using zuma::core::ColorRegistry;
    using zuma::core::Game;
    using zuma::core::GameState;
    using zuma::core::GameStatus;
    using zuma::core::TurnResult;

    const ColorRegistry colors;

    CASE("Game: 越界发射零副作用（D9）");
    {
        Game game(MakeLevel("RRB", "Y", "RG", 6), colors);
        const GameState before = game.Snapshot();

        for (std::size_t position : {std::size_t(0), std::size_t(5), std::size_t(99)}) {
            const TurnResult result = game.Fire(position);
            CHECK_FALSE(result.ok);
            CHECK_FALSE(result.message.empty());
        }

        CHECK(SameState(game.Snapshot(), before));
        CHECK_FALSE(game.CanUndo());  // 非法操作不入历史栈
        CHECK_EQ(game.View().turn, 0);
        CHECK_EQ(game.View().score, 0);
    }

    CASE("Game: 丢弃不消除，但仍是有效回合（D5）");
    {
        Game game(MakeLevel("RRB", "Y", "RGY", 10), colors);
        const TurnResult result = game.Drop();

        CHECK(result.ok);
        CHECK(result.isDrop);
        CHECK_EQ(result.ball, 'R');
        CHECK(result.rounds.empty());
        CHECK_EQ(result.scoreGained, 0);
        CHECK(result.fed);
        CHECK_EQ(result.fedColor, 'Y');
        CHECK_EQ(TrackText(game), std::string("RRBY"));
        CHECK_EQ(game.View().turn, 1);
        CHECK_EQ(game.View().score, 0);
        CHECK_EQ(game.View().currentBall, 'G');  // R -> G -> Y
        CHECK_EQ(game.View().nextBall, 'Y');
    }

    CASE("Game: 回合顺序 = 插入 -> 消除 -> 入球 -> 更新手持 -> 判定");
    {
        Game game(MakeLevel("RRB", "Y", "RG", 10), colors);
        const TurnResult result = game.Fire(3);  // RRRB -> 消除 3 个 R -> B -> 入球 Y -> BY

        CHECK(result.ok);
        CHECK(result.inserted);
        CHECK_EQ(result.position, std::size_t(3));
        CHECK_EQ(result.rounds.size(), std::size_t(1));
        CHECK_EQ(result.rounds[0].count, std::size_t(3));
        CHECK_EQ(result.rounds[0].color, 'R');
        CHECK_EQ(result.scoreGained, 30);
        CHECK(result.fed);
        CHECK_EQ(result.fedColor, 'Y');
        CHECK_EQ(TrackText(game), std::string("BY"));
        CHECK_EQ(game.View().turn, 1);
        CHECK_EQ(game.View().score, 30);
        CHECK_EQ(game.View().currentBall, 'G');
        CHECK_EQ(game.View().nextBall, 'R');
        CHECK(game.Status() == GameStatus::Playing);
    }

    CASE("Game: 位置 size()+1 表示放到入口端");
    {
        Game game(MakeLevel("RGB", "", "RG", 10), colors);
        const TurnResult result = game.Fire(4);
        CHECK(result.ok);
        CHECK_EQ(TrackText(game), std::string("RGBR"));
        CHECK_FALSE(result.fed);  // 等待区为空
    }

    CASE("Game: 等待区为空时不再入球");
    {
        Game game(MakeLevel("RRB", "", "RG", 10), colors);
        game.Fire(3);
        CHECK_EQ(TrackText(game), std::string("B"));
        CHECK_EQ(game.View().feed->size(), std::size_t(0));
    }

    CASE("Game: 插入造成的瞬时超限不判负（D6）");
    {
        Game game(MakeLevel("RRB", "", "RR", 3), colors);  // 满链
        const TurnResult result = game.Fire(3);            // RRRB 瞬时 4 个 > 3
        CHECK(result.ok);
        CHECK_EQ(TrackText(game), std::string("B"));
        CHECK(game.Status() == GameStatus::Playing);  // 回合末长度 1 <= 3
    }

    CASE("Game: 回合末超过上限判负（D6）");
    {
        Game game(MakeLevel("BG", "YYYY", "RG", 4), colors);
        CHECK(game.Fire(1).ok);  // RBG -> 入球 Y -> RBGY（4 <= 4）
        CHECK(game.Status() == GameStatus::Playing);
        CHECK_EQ(TrackText(game), std::string("RBGY"));

        game.Drop();  // 入球 Y -> RBGYY（5 > 4）
        CHECK_EQ(TrackText(game), std::string("RBGYY"));
        CHECK(game.Status() == GameStatus::Lost);

        const TurnResult afterLost = game.Fire(1);
        CHECK_FALSE(afterLost.ok);  // 结束后不再接受操作
    }

    CASE("Game: 链与等待区同时清空判胜（D7）");
    {
        Game game(MakeLevel("RRGGRR", "", "GGG", 8), colors);  // levels/demo.txt 的局面
        const TurnResult result = game.Fire(3);

        CHECK(result.ok);
        CHECK_EQ(result.rounds.size(), std::size_t(2));  // 普通消除 + 1 次连锁
        CHECK_EQ(result.eliminated(), std::size_t(7));
        CHECK_EQ(result.scoreGained, 110);  // 3*10*1 + 4*10*2
        CHECK(TrackText(game).empty());
        CHECK(game.Status() == GameStatus::Won);
        CHECK_EQ(game.View().score, 110);
        CHECK_EQ(game.View().maxCombo, 2);
    }

    CASE("Game: ACTIVE 与 FEED 同时为空 -> 开局即胜利");
    {
        Game game(MakeLevel("", "", "RG", 5), colors);
        CHECK(game.Status() == GameStatus::Won);
    }

    CASE("Game: 撤销回到上一回合开始前，可连续撤销");
    {
        Game game(MakeLevel("RRB", "YYY", "RRG", 10), colors);
        const GameState state0 = game.Snapshot();

        game.Fire(3);
        const GameState state1 = game.Snapshot();
        game.Drop();
        const GameState state2 = game.Snapshot();

        CHECK(game.CanUndo());
        CHECK(game.Undo());                       // 回到 state1
        CHECK(SameState(game.Snapshot(), state1));
        CHECK(game.Undo());                       // 回到 state0
        CHECK(SameState(game.Snapshot(), state0));
        CHECK_FALSE(game.CanUndo());
        CHECK_FALSE(game.Undo());
        CHECK_EQ(TrackText(game), std::string("RRB"));
        CHECK_EQ(game.View().turn, 0);
        CHECK(game.Status() == GameStatus::Playing);

        // 撤销后可以继续正常游玩
        const TurnResult result = game.Fire(3);
        CHECK(result.ok);
        CHECK(SameState(game.Snapshot(), state1));
        CHECK(SameState(state0, state0));
        CHECK_FALSE(SameState(state0, state2));
    }

    CASE("Game: Quit 后不再接受操作");
    {
        Game game(MakeLevel("RRB", "Y", "RG", 6), colors);
        game.Quit();
        CHECK(game.Status() == GameStatus::Quit);
        CHECK_FALSE(game.Fire(1).ok);
        CHECK_FALSE(game.Drop().ok);
    }

    CASE("Game: 未注册颜色给出警告但不影响规则");
    {
        Game game(MakeLevel("ZR", "", "RG", 5), colors);
        CHECK_EQ(game.Warnings().size(), std::size_t(1));
        CHECK(game.Warnings().front().find('Z') != std::string::npos);
        CHECK(game.Fire(1).ok);
    }

    CASE("Game: 历史栈容量上限");
    {
        Game game(MakeLevel("RRB", "YYYYYYYYYYYYYYYYYYYY", "RRG", 40), colors);
        for (int index = 0; index < 80; ++index) {
            CHECK(game.Drop().ok);
        }
        CHECK_EQ(game.View().turn, 80);
        CHECK_EQ(game.View().score, 0);
        int undone = 0;
        while (game.Undo()) {
            ++undone;
        }
        CHECK_EQ(undone, 64);  // kMaxHistory
    }

    CASE("Game: 空轨道时仍可发射（新球成为位置 1）");
    {
        Game game(MakeLevel("", "RG", "RG", 5), colors);
        CHECK(game.Status() == GameStatus::Playing);  // 等待区非空，不算胜利
        CHECK_EQ(game.LegalPositionMax(), std::size_t(1));

        const TurnResult result = game.Fire(1);
        CHECK(result.ok);
        CHECK_EQ(result.position, std::size_t(1));
        CHECK_EQ(result.eliminated(), std::size_t(0));
        CHECK_EQ(TrackText(game), std::string("RR"));  // 发射的 R + 等待区进入的 R
        CHECK_EQ(game.View().currentBall, 'G');
        CHECK_EQ(game.View().turn, 1);
        CHECK_FALSE(game.Fire(0).ok);
        CHECK_FALSE(game.Fire(4).ok);  // 链长 2，合法范围 1..3
    }

    CASE("Game: 合法位置范围随链长变化");
    {
        Game game(MakeLevel("RRB", "", "RG", 10), colors);
        CHECK_EQ(game.LegalPositionMax(), std::size_t(4));
        CHECK_FALSE(game.Fire(5).ok);
        CHECK(game.Fire(3).ok);  // RRRB -> 消除 3 个 R -> B
        CHECK_EQ(game.LegalPositionMax(), std::size_t(2));
    }
}
