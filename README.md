# codingFile

个人学习代码仓库。按“课程 / 练习 / 文档 / 归档”分区，避免所有东西堆在根目录。

## 目录

| 目录 | 内容 |
|---|---|
| `DataStructure/` | 数据结构课程：期中复习、Lab、课程项目、Python 基础、课件工具 |
| `practice/` | 零散练习，按主题分类（链表、DFS、文件 I/O、杂项等） |
| `docs/` | 跨项目文档：问题排查、环境说明、自学计划 |
| `archive/` | 不再维护但保留的个人草稿 |

## DataStructure

- `DataStructure/MidTerm/`：第 1～8 讲学案 + `src/` + `solution/` + `log/` + `viz/`，入口是 `MidTerm/期中总复习.md`
- `DataStructure/labs/`：Lab01～Lab07，每个 lab 一个主题
- `DataStructure/projects/zuma/`：课程 Project 1
- `DataStructure/python-basics/`：早期 Python 基础练习
- `DataStructure/slides/`：课件/PPT 生成工具
- `DataStructure/tools/`：课件抽取、动画几何检查
- `DataStructure/HANDOFF.md`：数据结构部分的交接笔记

更细的导航见 `DataStructure/README.md`。

## 运行约定

- C++ 使用 `g++ -O2 -std=c++17`，源码和生成物分开。
- `output/`、`build/`、`*.exe`、`__pycache__/`、IDE 配置不入库。
- `DataStructure/MidTerm/LessonN` 的构建方式见各讲学案的“动手”一节。
- 文件 I/O 练习在 `practice/cpp/file-io/` 下运行，数据在 `data/`。

## 说明

- `practice/` 里的代码是早期练习，部分注释是 GBK 编码，直接看可能有乱码。
- 仓库不再包含自动上传脚本；提交和推送手动执行。
