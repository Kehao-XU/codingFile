#include "core/Game.h"

#include <set>
#include <utility>

namespace zuma::core {

Game::Game(const Level& level, const ColorRegistry& colors)
    : level_(level),
      colors_(colors),
      feed_(level.feed),
      ammo_(level.ammo) {
    for (char color : level_.active) {
        track_.Append(Ball{color});
    }
    CollectColorWarnings();
    CheckOutcome();  // 关卡的 ACTIVE/FEED 同时为空时立即判胜
}

TurnResult Game::Fire(std::size_t position) {
    TurnResult result;
    if (status_ != GameStatus::Playing) {
        result.ok = false;
        result.message = "本局已结束，无法继续操作（可输入 UNDO 撤销，或输入 Q 退出）";
        return result;
    }

    const std::size_t length = track_.size();
    if (position < 1 || position > length + 1) {
        result.ok = false;
        result.message = "位置 " + std::to_string(position) + " 越界：当前轨道长度 " +
                         std::to_string(length) + "，合法范围 1.." + std::to_string(length + 1) +
                         "（非法输入不消耗彩球、不增长轨道）";
        return result;
    }

    PushHistory();
    const char color = ammo_.Current();
    const Track::iterator inserted = track_.Insert(position, Ball{color});

    result.ball = color;
    result.inserted = true;
    result.position = position;
    ApplyEndOfTurn(result, true, inserted);
    return result;
}

TurnResult Game::Drop() {
    TurnResult result;
    if (status_ != GameStatus::Playing) {
        result.ok = false;
        result.message = "本局已结束，无法继续操作（可输入 UNDO 撤销，或输入 Q 退出）";
        return result;
    }

    PushHistory();
    result.ball = ammo_.Current();
    result.isDrop = true;
    ApplyEndOfTurn(result, false, track_.end());
    return result;
}

void Game::ApplyEndOfTurn(TurnResult& result, bool hasSeed, Track::iterator seed) {
    // (3) 消除阶段：仅发射触发；丢弃不消除。
    if (hasSeed) {
        result.rounds = EliminationEngine::Resolve(track_, seed, ScoreBoard::kBaseScorePerBall);
        result.scoreGained = 0;
        for (const EliminationRound& round : result.rounds) {
            score_.AddElimination(round.count, round.round);
            result.scoreGained += round.gainedScore;
        }
    }

    // (4) 自动增长：等待区非空时加入一个球到入口端。
    if (std::optional<Ball> incoming = feed_.Pop()) {
        track_.Append(*incoming);
        result.fed = true;
        result.fedColor = incoming->color;
    }

    // (5) 更新手持球（弹药循环推进）与 (6) 回合数。
    ammo_.Advance();
    score_.RegisterTurn();

    // (7) 胜负判定
    CheckOutcome();
    result.statusAfter = status_;
}

void Game::CheckOutcome() {
    if (status_ == GameStatus::Quit) {
        return;
    }
    if (track_.empty() && feed_.empty()) {
        status_ = GameStatus::Won;
        return;
    }
    if (track_.size() > level_.maxLength) {
        status_ = GameStatus::Lost;
        return;
    }
    status_ = GameStatus::Playing;
}

void Game::PushHistory() {
    history_.push_back(Snapshot());
    if (history_.size() > kMaxHistory) {
        history_.erase(history_.begin());
    }
}

bool Game::Undo() {
    if (history_.empty()) {
        return false;
    }
    const GameState state = history_.back();
    history_.pop_back();
    Restore(state);
    return true;
}

GameState Game::Snapshot() const {
    GameState state;
    state.track.reserve(track_.size());
    for (const Ball& ball : track_) {
        state.track.push_back(ball.color);
    }
    state.feed = feed_.Snapshot();
    state.ammoCursor = ammo_.Cursor();
    state.currentBall = ammo_.Current();
    state.nextBall = ammo_.Next();
    state.score = score_.Score();
    state.turn = score_.Turn();
    state.maxCombo = score_.MaxCombo();
    state.eliminatedTotal = score_.EliminatedTotal();
    state.status = status_;
    return state;
}

void Game::Restore(const GameState& state) {
    track_.Clear();
    for (char color : state.track) {
        track_.Append(Ball{color});
    }
    feed_.Restore(state.feed);
    ammo_.Restore(state.ammoCursor, state.currentBall, state.nextBall);
    score_.Restore(state.score, state.turn, state.maxCombo, state.eliminatedTotal);
    status_ = state.status;
}

const GameViewData& Game::View() const {
    viewCache_.levelName = level_.name;
    viewCache_.levelSourcePath = level_.sourcePath;
    viewCache_.maxLength = level_.maxLength;
    viewCache_.track = &track_;
    viewCache_.feed = &feed_.Remaining();
    viewCache_.currentBall = ammo_.Current();
    viewCache_.nextBall = ammo_.Next();
    viewCache_.score = score_.Score();
    viewCache_.turn = score_.Turn();
    viewCache_.maxCombo = score_.MaxCombo();
    viewCache_.status = status_;
    viewCache_.canUndo = CanUndo();
    return viewCache_;
}

void Game::CollectColorWarnings() {
    std::set<char> seen;
    const std::string allColors = level_.active + level_.feed + level_.ammo;
    for (char color : allColors) {
        if (!seen.insert(color).second) {
            continue;
        }
        if (!colors_.Contains(color)) {
            warnings_.push_back(std::string("颜色 '") + color +
                                "' 未在颜色表中注册，将以默认样式显示（不影响游戏规则）");
        }
    }
}

}  // namespace zuma::core
