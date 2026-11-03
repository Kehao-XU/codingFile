# 祖玛彩球连锁消除（命令行版）

数据结构课程 Project 1。命令行版祖玛游戏：彩球沿轨道排成链，玩家把手中彩球插入链上指定位置，
形成 3 个及以上同色连续球即消除，并可继续触发连锁消除；每个有效回合等待区会放入一个彩球。

- 需求：[Project 1 - Zuma.md](<./Project 1 - Zuma.md>)
- 架构设计：[ARCHITECTURE.md](./ARCHITECTURE.md)

---

## 1. 目录结构

```
PJ1/
├─ ARCHITECTURE.md      架构设计说明书
├─ README.md            本文件（运行说明）
├─ CMakeLists.txt       CMake 构建（可选）
├─ build.ps1            Windows 一键构建脚本（g++）
├─ levels/              关卡文件：level1.txt / level2.txt（题目提供）+ demo.txt（教学关）
├─ src/
│  ├─ main.cpp          程序入口
│  ├─ core/             领域层：Track / EliminationEngine / FeedQueue / AmmoGenerator /
│  │                    ScoreBoard / Level / LevelLoader / Game / ColorRegistry / GameState
│  ├─ cli/              表现层：CommandParser / ConsoleView / ConsoleApp
│  └─ util/             基础设施：StringUtil（文本工具）/ ConsoleColor（ANSI 与编码）
└─ tests/               单元测试（自建轻量框架，无第三方依赖）
```

## 2. 构建

### 方式 A：MSYS2 / MinGW-w64 / Linux（g++）

```bash
# 在 PJ1 目录下
g++ -std=c++17 -Wall -Wextra -O2 -Isrc \
    src/main.cpp src/core/*.cpp src/cli/*.cpp src/util/*.cpp -o zuma.exe

# 单元测试
g++ -std=c++17 -Wall -Wextra -O2 -Isrc \
    src/core/*.cpp src/cli/*.cpp src/util/*.cpp tests/*.cpp -o zuma_tests.exe
```

Windows 上可直接用脚本（自动编译游戏与测试，并把 `levels/` 复制到 `build/`）：

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

### 方式 B：CMake（任意平台）

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure     # 可选：运行单元测试
```

构建产物：`zuma`（游戏）、`zuma_tests`（单元测试）。

## 3. 运行

```bash
./zuma                      # 自动在当前目录（及上级目录）的 levels/ 中查找关卡，弹出关卡菜单
./zuma levels/level1.txt    # 直接进入指定关卡
./zuma levels/level1.txt levels/level2.txt     # 只把这两关作为菜单
./zuma --no-clear           # 关闭清屏，改为逐帧追加（保留完整历史记录）
./zuma --clear              # 强制每帧清屏重绘
./zuma --no-color           # 关闭 ANSI 彩色输出（输出被重定向时会自动关闭）
```

**屏幕刷新方式**：默认在支持 ANSI 的交互终端上**每帧清屏重绘**——每条指令处理完后清屏，屏幕上只显示
「本回合发生了什么（回合日志）+ 当前状态面板」，不会把历次界面都留在屏幕上，因此任何时候看到的
内容都是自洽的。若输出被重定向（管道/文件）或终端不支持 ANSI，会自动退化为**逐帧追加**模式，
方便录制、粘贴与自动化校验；也可用 `--no-clear` / `--clear` 显式指定。

> 提示：若在 Windows 的 `cmd`/PowerShell 中运行，程序会自动把控制台切到 UTF-8 并开启 ANSI，
> 保证中文与彩色显示正常；若终端不支持，会自动退化为纯字母显示 `[R]`。

## 4. 操作命令

| 命令 | 缩写 | 说明 |
| --- | --- | --- |
| `FIRE <n>` | `F <n>` | 把当前球插入到**位置 n 之前**（新球成为新的第 n 个）。`n` 合法范围 `1 .. 轨道长度+1`，`n = 长度+1` 表示放到入口端 |
| `DROP` | `D` | 丢弃当前球（不触发消除，但仍算一个有效回合，等待区照常入球） |
| `SHOW` | `S` | 重新显示当前状态 |
| `UNDO` | `U` | 撤销上一回合（可连续撤销，最多 64 步） |
| `HINT` | — | 提示（预留扩展接口，尚未启用） |
| `HELP` | `H` / `?` | 显示帮助与规则摘要 |
| `QUIT` | `Q` / `EXIT` | 退出本局 |

位置编号：`1` 为轨道最深处，`N` 为入口端（`N = 轨道长度`）。

## 5. 规则说明

1. **回合顺序**：玩家发射或丢弃 → （仅发射时）普通消除与所有连锁消除 → 等待区入 1 个球
   （等待区为空则不加）→ 更新当前球与下一个球 → 判定胜负。
2. **消除**：插入后与插入球相连的同色连续段达到 3 个即消除；消除后若断点两侧同色并重新合并成
   3 连以上，继续连锁消除，直到不能再消除。
3. **自动增长**：每个有效回合（成功的发射或丢弃）结束时从等待区加入 1 个彩球到入口端。
4. **弹药**：按关卡 `AMMO` 字符串循环生成（第 1 个字符为初始当前球，第 2 个为初始下一个球，
   取到末尾后回到开头），**无限生成，不存在弹药耗尽**。
5. **胜负**：轨道与等待区同时清空 → 胜利；回合结束时轨道长度 > `MAX_LENGTH` → 失败。
   *插入造成的瞬时超限不算失败*，只要回合结束时不超过上限即可。
6. **非法输入**：不消耗彩球、不增长轨道、不推进回合数、不影响得分，程序继续运行。
7. **计分**（自定义规则）：第 r 轮消除 k 个球得 `k × 10 × r` 分（普通消除 r=1，连锁 r≥2）；
   丢弃与未消除的发射得 0 分；每个有效回合计 1 回合。

### 关卡文件格式

```text
# 以 # 开头的行为注释；支持行尾注释、空行、UTF-8 BOM 与 CRLF
NAME=新手关            # 可选，缺省用文件名
ACTIVE=RRBBRR          # 开局已在轨道的彩球
FEED=GBYPRGBGYRBPYG    # 之后依次进入轨道的彩球
AMMO=BBGYRRGBYPRG      # 弹药循环规律（至少 1 个字符）
MAX_LENGTH=12          # 轨道允许的最大彩球数（1..999）
```

校验规则：字段缺失/未知字段/非法颜色字符/`MAX_LENGTH` 非 1..999/`ACTIVE` 长度超过上限 都会
导致加载失败并给出**行号 + 原因**；`ACTIVE` 自带 3 连、字段重复出现只给出警告。

## 6. 数据结构使用一览（对应评分点）

| 数据结构 | 使用位置 | 选择原因 |
| --- | --- | --- |
| 双向链表 `std::list` | 轨道球链 `Track` | 任意位置插入 O(1)、消除时一次区间删除、迭代器稳定（连锁需持有断点两侧迭代器） |
| 队列 `std::deque`（FIFO 语义） | 等待区 `FeedQueue` | 等待区严格先进先出；用 deque 是为了能只读遍历以便显示 |
| 环形缓冲（取模读指针） | 弹药 `AmmoGenerator` | 天然表达"取完从头再来"的无限循环 |
| 栈 `std::vector` 尾部进出 | 撤销历史 `Game::history_` | 撤销是后进先出；容量上限 64 |
| 哈希表 `std::unordered_map` | 颜色表 `ColorRegistry` | 颜色字符 → 显示样式快速查表，未注册颜色走兜底 |
| 向量 `std::vector` | 关卡菜单、连锁记录、状态快照 | 少量元素、需顺序遍历/下标访问 |

## 7. 测试

```bash
./zuma_tests              # 自动向上查找项目目录以定位 levels/
./zuma_tests <项目目录>    # 也可显式指定
```

覆盖：轨道插入/删除/边界、普通消除与多轮连锁（含链首链尾边界）、弹药循环周期、等待区 FIFO
与快照、关卡文件解析与各类非法输入、命令解析、回合流水线（含"非法输入零副作用"与
"瞬时超限不判负"两条关键规则）、撤销/连续撤销、历史栈上限、胜负判定，以及展示层的清屏开关、
每帧内容自洽性、颜色/无颜色输出、等待区截断。

当前共 **391 项检查，全部通过**（`-Wall -Wextra` 零警告）。

### 演示建议

| 演示点 | 操作 |
| --- | --- |
| 普通消除 + 连锁消除 + 胜利 | 选 `演示关`（`levels/demo.txt`）→ `FIRE 3` |
| 发射插入 / 丢弃 | `FIRE 3` / `DROP` |
| 无限弹药 | 连续 `DROP` 若干次，观察当前球/下一个球按 AMMO 循环 |
| 非法输入零副作用 | `FIRE abc`、`FIRE 0`、`FIRE 99`、`FLY 3`、空行 |
| 撤销 | `UNDO` |
| 失败 | `levels/level1.txt` 中持续 `DROP`，回合末链长 13 > 上限 12 时失败 |
| 界面刷新 | 任意指令后屏幕只保留「本回合日志 + 当前状态」；`--no-clear` 对比查看完整历史 |

## 8. 扩展预留

`UNDO` 已实现；`HINT` 预留 `IHintProvider` 接口；关卡来源可扩展（随机关卡）；状态快照
`GameState` 可直接用于存档与回放。详见 [ARCHITECTURE.md](./ARCHITECTURE.md) 第 9 节。
