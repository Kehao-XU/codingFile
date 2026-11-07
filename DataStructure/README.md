# DataStructure

数据结构课程目录。

## 目录

| 路径 | 内容 |
|---|---|
| `MidTerm/` | 第 1～8 讲学案、练习骨架、参考答案、实测日志、可视化动画 |
| `labs/` | Lab01～Lab07，见 `labs/README.md` |
| `projects/zuma/` | 课程 Project 1（Zuma），CMake 工程 |
| `python-basics/` | 早期 Python 基础练习 |
| `slides/` | 课件/PPT 生成工具（`build_ppt.py`、`slides.json`、`_raw/`） |
| `tools/` | `extract_pptx.py`、`viz_check.js` |
| `HANDOFF.md` | 本课程的历史交接笔记 |

## MidTerm

入口：`MidTerm/期中总复习.md`。每讲的结构：

```
LessonN/
├─ 学案NN_*.md
├─ src/          # 练习骨架
├─ solution/     # 参考答案
├─ log/          # 实测输出
└─ output/       # 编译产物，已忽略
```

动画入口：`MidTerm/viz/index.html`，直接用浏览器打开。

## Labs

见 `labs/README.md`。每个 lab 只有 `src/`，没有统一构建脚本；按题单独编译即可。

## 项目

`projects/zuma/` 使用 CMake：

```bash
cd DataStructure/projects/zuma
cmake -S . -B build
cmake --build build
```

具体运行方式见该目录的 `README.md`。

## 工具

- 抽课件文本：`python tools/extract_pptx.py 课件/*.pptx --brief`
- 动画几何检查：`node tools/viz_check.js`
