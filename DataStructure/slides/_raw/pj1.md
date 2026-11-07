# PJ1 — Zuma

## 项目目标
C++17 命令行祖玛：读取关卡，玩家发射或丢弃当前球；插入后 ≥3 个同色连续球消除并继续连锁；每个有效回合等待区入 1 球；轨道与等待区同时清空判胜，回合末链长超过 `MAX_LENGTH` 判负；弹药按 `AMMO` 无限循环。自定义计分为第 r 轮消 k 球得 `k×10×r`，另支持 UNDO（最多 64 步）、HINT 占位和清屏/无颜色降级。

## 分层架构
| 层 | 目录 | 职责 | 关键类 |
|---|---|---|---|
| 表现层 | `cli/` | 命令解析、终端渲染、关卡菜单与主循环 | `CommandParser`、`ConsoleView`、`ConsoleApp` |
| 应用/规则层 | `core/` | 回合流水线、合法性、胜负、撤销编排 | `Game`、`TurnResult` |
| 领域层 | `core/` | 轨道、消除、供弹、关卡、计分、快照 | `Track`、`EliminationEngine`、`FeedQueue`、`AmmoGenerator`、`LevelLoader`、`ScoreBoard`、`GameState`、`ColorRegistry` |
| 基础设施 | `util/` | 文本处理、ANSI/UTF-8/终端探测 | `StringUtil`、`ConsoleColor` |

依赖方向为 `main → ConsoleApp → Game → 领域对象 → util`；`core` 不写标准输出、不包含 CLI 头文件。`Game::View()` 返回只读 `GameViewData`（含 `Track`/等待区指针），`TurnResult` 承载回合日志，避免表现层直接改状态。

`ARCHITECTURE.md` 约 60KB，核心是目标、D1–D12 规则裁定、分层、数据结构选型、详细设计、测试计划与扩展；正文与代码总体一致，12.3 还主动列出了 12 项实现差异。但仍有未同步点，见“可改进点”。

## 模块与职责
- `CommandParser`：纯函数，把一行文本转成 `Command{type,arg}`；只判语法，位置越界由 `Game` 判。
- `ConsoleView`：主要渲染入口；`BeginFrame` 清屏，`Render*` 负责状态、回合日志、结束画面、菜单、帮助和错误（`ConsoleApp` 另有少量换行输出）。
- `ConsoleApp`：从参数或 `levels/` 及上级目录发现关卡，选关、开局、结束后重玩/换关/退出。
- `Game`：`Fire/Drop` 的唯一入口；持有 `Track/FeedQueue/AmmoGenerator/ScoreBoard/history_`，非法操作零副作用。
- `LevelLoader`：`KEY=VALUE` 解析，处理 BOM、CRLF、注释和行尾注释；校验必填字段、颜色字母、`MAX_LENGTH` 1..999、`ACTIVE≤MAX_LENGTH`，重复键和初始三连只警告。
- `GameState`：轨道、等待区、弹药指针、手持球、计分和状态的纯值快照，供撤销（文档另预留存档/回放）。

## 核心算法与数据结构
- **Track**：`std::list<Ball>` 双向链表；位置 1 基，1 为最深处、N 为入口端。`PositionOf` 及封装它的 `Insert` 定位为 O(N)，链表节点插入/`Append` 本身 O(1)，`Erase` 一次删除区间 O(k)；迭代器稳定，便于连锁时持有断点两侧。
- **EliminationEngine**：`CollectRun` 从 seed 向左右扩展出极大同色段；`Resolve` 循环“收集→不足 3 结束→记录左右邻→区间删除→产出 `EliminationRound`”，若删除后左右同色则以右侧为 seed 继续。最坏 O(L²)，单轮得分 `count×10×round`。
- **FeedQueue**：`std::deque` 实现 FIFO，`Pop` 空时返回 `nullopt`；`Snapshot/Restore` 保持队首顺序。
- **AmmoGenerator**：`AMMO` 字符串加取模游标；初始 `current=p[0]`、`next=p[len>1?1:0]`、`cursor=(len>1?2:1)%len`，`Advance` 循环推进，长度 1 时恒为同色。
- **Game 回合顺序**：校验 → 快照入历史 → 插入/丢弃 → 消除（仅发射）→ 等待区入球 → 推进弹药 → 回合数 +1 → `CheckOutcome`。胜负先判胜后判负，且只在回合末比较长度，插入造成的瞬时超限不算失败。
- **计分**：`ScoreBoard` 累加分数、回合数、最大连锁和累计消除数；`maxCombo` 取最大轮次。

## 测试覆盖
| 测试文件 | 覆盖内容 | 用例数（大约） |
|---|---|---:|
| `test_track.cpp` | 空链/中间/入口插入、`PositionOf` 边界、区间删除与迭代器稳定、三连检测 | 5 |
| `test_elimination.cpp` | 不足 3 不消、恰好 3、插入点两侧合并、2/3 轮连锁、链首链尾、`CollectRun`、清空 | 9 |
| `test_ammo_feed.cpp` | 初始球、逐次推进、周期与长度 1、FIFO、空队列、快照恢复 | 7 |
| `test_level_loader.cpp` | 正常/注释/空行/小写/BOM+CRLF、缺字段、非法 `MAX_LENGTH`/颜色、`ACTIVE` 超限、三连警告、未知字段/缺等号/空文件、重复键、真实关卡 | 11 |
| `test_command_parser.cpp` | FIRE 及缩写/全角空格、非法语法、各无参命令、多余参数、空行/未知命令 | 5 |
| `test_game_turns.cpp` | 越界零副作用、DROP、回合顺序、N+1、空等待区、瞬时超限、判负/胜利/开局胜、撤销与连续撤销、Quit、未注册色、64 步上限、空轨道、动态合法范围 | 15 |
| `test_console_view.cpp` | 清屏开关、每帧自洽、颜色开关与未注册色、回合日志、等待区截断、结束画面、菜单与警告 | 9 |

`test_harness.h` 是自建轻量框架：`inline` 全局 `g_checks/g_failures`，`CASE` 设置当前用例名，`CHECK/CHECK_EQ/CHECK_FALSE` 记录文件与行号，失败后继续执行，`test_main.cpp` 汇总并返回非零码；各 `Run*` 手工调用，测试启动时向上查找 `levels/`。共 8 个 `test_*.cpp`（含 `test_main.cpp`），61 个 CASE；本地 g++ 15.2.0 在临时目录编译实测 **391 项检查、0 失败**。

## 设计优点
- 分层边界清楚：规则集中在 `core::Game`，`cli` 只做解析/渲染，核心逻辑可脱离终端测试。
- 回合语义集中：`Fire/Drop` 共用 `ApplyEndOfTurn`，避免两条路径的规则漂移；非法输入先校验后改状态。
- 数据结构与语义匹配：链表、FIFO 队列、环形缓冲、栈式历史、哈希颜色表各有明确理由，且 `Track` 封装容器。
- 关卡解析较健壮：BOM/CRLF/注释/行号报错/警告分级，未知颜色有兜底；关键规则有回归测试。

## 可改进点
1. **结束后的 UNDO 实际不可达**：`Game::Fire/Drop` 在非 `Playing` 时提示“可输入 UNDO 撤销”，但一局结束后 `ConsoleApp` 已退出主循环，`AskPostGameAction` 只接受 R/N/Q；该提示在 CLI 中永远不会出现，玩家也无法撤销败局。建议结束菜单接受 U，或删除该提示并明确赛后不可撤销。
2. **文档与代码仍有偏差**：`ARCHITECTURE.md` 5.8 称超过 4096 字符的行会截断，实际 `ReadLine` 无长度上限；第 9 节称预留 `IHintProvider`/`ILevelSource`，README 也提到 `IHintProvider`，但源码只有 HINT 文案，没有接口声明；7.1 列出 ScoreBoard 单测，实际没有 `test_scoreboard.cpp`（仅间接覆盖）；4.2 的“L≤16”与 `MAX_LENGTH` 可到 999 矛盾。建议逐条对齐，或在文档标注“未实现/仅规划”。
3. **构建未强制分层**：CMake 把 `core/cli/util` 都编进同一个 `zuma_core` 静态库，依赖方向只靠约定；可拆成 `zuma_domain`（core+util）和 `zuma_cli`，禁止 CLI 头文件进入 domain。
4. **补测试与去重**：增加 `ConsoleApp` 集成测试（选关、结束菜单、EOF）和 `ScoreBoard` 直测；给 `getline` 加行长上限；把 `LevelLoader` 内重复的三连检测抽成共享函数，避免与 `Track::HasRunOfThreeOrMore` 逻辑漂移。

## 一句话总结
这是一个分层清楚、规则裁定明确、以 391 项测试兜底的教学级 C++ 祖玛实现，主要短板是赛后撤销不可达与少量文档未同步。
