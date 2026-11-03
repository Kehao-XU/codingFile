#pragma once

#include <cstddef>
#include <deque>
#include <string>
#include <vector>

#include "core/AmmoGenerator.h"
#include "core/ColorRegistry.h"
#include "core/EliminationEngine.h"
#include "core/FeedQueue.h"
#include "core/GameState.h"
#include "core/GameStatus.h"
#include "core/Level.h"
#include "core/ScoreBoard.h"
#include "core/Track.h"

namespace zuma::core {

// 一个回合的完整执行记录，供渲染、调试与将来的回放使用。
struct TurnResult {
    bool ok = true;                    // false = 操作非法，游戏状态零变化
    std::string message;               // 失败原因
    bool isDrop = false;               // true = 本回合是丢弃
    char ball = '?';                   // 本回合发射/丢弃的球
    bool inserted = false;             // 是否成功插入
    std::size_t position = 0;          // 实际插入位置
    std::vector<EliminationRound> rounds;
    int scoreGained = 0;
    bool fed = false;                  // 本回合是否从等待区入球
    char fedColor = '?';
    GameStatus statusAfter = GameStatus::Playing;

    int chainRounds() const { return static_cast<int>(rounds.size()); }

    std::size_t eliminated() const {
        std::size_t total = 0;
        for (const EliminationRound& round : rounds) {
            total += round.count;
        }
        return total;
    }
};

// 表现层渲染所需的只读视图（不含任何可变接口）。
struct GameViewData {
    std::string levelName;
    std::string levelSourcePath;
    std::size_t maxLength = 0;
    const Track* track = nullptr;
    const std::deque<Ball>* feed = nullptr;
    char currentBall = '?';
    char nextBall = '?';
    int score = 0;
    int turn = 0;
    int maxCombo = 0;
    GameStatus status = GameStatus::Playing;
    bool canUndo = false;
};

// 规则引擎（应用层）：编排回合流水线、判定胜负、维护撤销历史。
class Game {
public:
    static constexpr std::size_t kMaxHistory = 64;  // 历史栈容量，超限丢弃最旧快照

    Game(const Level& level, const ColorRegistry& colors);

    // 发射并插入当前球；位置越界返回 ok=false 且状态零变化。
    TurnResult Fire(std::size_t position);
    // 丢弃当前球：不触发消除，但仍是有效回合（入球 + 推进弹药）。
    TurnResult Drop();

    bool CanUndo() const noexcept { return !history_.empty(); }
    bool Undo();

    void Quit() noexcept { status_ = GameStatus::Quit; }

    const GameViewData& View() const;
    GameStatus Status() const noexcept { return status_; }
    const std::vector<std::string>& Warnings() const noexcept { return warnings_; }

    // 当前合法的插入位置上限（= 轨道长度 + 1）。
    std::size_t LegalPositionMax() const noexcept { return track_.size() + 1; }

    GameState Snapshot() const;
    void Restore(const GameState& state);

private:
    void PushHistory();
    void ApplyEndOfTurn(TurnResult& result, bool hasSeed, Track::iterator seed);
    void CheckOutcome();
    void CollectColorWarnings();

    Level level_;
    const ColorRegistry& colors_;
    Track track_;
    FeedQueue feed_;
    AmmoGenerator ammo_;
    ScoreBoard score_;
    GameStatus status_ = GameStatus::Playing;
    std::vector<GameState> history_;  // 用作栈（LIFO）
    std::vector<std::string> warnings_;
    mutable GameViewData viewCache_;
};

}  // namespace zuma::core
