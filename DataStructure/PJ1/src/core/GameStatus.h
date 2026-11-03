#pragma once

namespace zuma::core {

// 一局游戏的总体状态。Quit 表示玩家主动退出（不计胜负）。
enum class GameStatus { Playing, Won, Lost, Quit };

}  // namespace zuma::core
