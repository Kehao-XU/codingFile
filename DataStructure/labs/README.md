# Labs

每个 lab 一个目录，源码在 `src/`，可执行文件在 `output/`。`output/` 已被 `.gitignore` 忽略，重新编译即可生成。

| Lab | 主题 | 主要文件 |
|---|---|---|
| `lab01-array-basics` | 数组/哈希基础 | `LongestConsecutiveList.cpp`、`OnlyChar*.cpp`、`SumOfTwoNums.cpp` |
| `lab02-array-two-pointers` | 数组双指针 | `MidOfConsecutiveArray.cpp`、`RemoveValueInArray.cpp`、`SquareOfConsecutiveArray.cpp` |
| `lab03-linked-list` | 链表 | `1_ReverseList.cpp`、`2_ListLoopDetect.cpp`、`3_PalindromeListDetect.cpp` |
| `lab04-string` | 字符串 | `DNARepeat.cpp`、`FirstCharInString*.cpp`、`LoopString*.cpp` |
| `lab05-stack-queue` | 栈队列/单调栈 | `DailyTemperature.cpp`、`MaxInWindow*.cpp`、`StringCode.cpp` |
| `lab06-recursion` | 递归/广义表 | `DigitsOfNumber.cpp`、`GenList*.cpp`、`Subset.cpp` |
| `lab07-tree` | 二叉树 | `ClosestSharedAncestor.cpp`、`ConstructTree.cpp`、`RightSideOfTree.cpp` |
| `lab08-tree-heap` | 哈夫曼/堆/日程合并 | `huffmanDecode.cpp`、`logsMerge.cpp`、`schedule.cpp` |
| `lab09-bst` | 二叉搜索树 | `lab9-01.cpp`（范围和）、`lab9-02.cpp`（第 k 小）、`lab9-03.cpp`（迭代器） |

编译单个文件：

```bash
g++ -O2 -std=c++17 -Wall src/xxx.cpp -o output/xxx.exe
```

在 VS Code 里打开某个 `.cpp`，按 `Ctrl+Shift+B` 就会自动编译到该 lab 的 `output/` 并运行（任务见 `.vscode/tasks.json`）。

lab09 的三份代码按作业要求命名（`lab9-01.cpp` 等），提交时直接从这个目录取，压缩成「学号-姓名-lab9.zip」即可。
