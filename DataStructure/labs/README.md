# Labs

| Lab | 主题 | 主要文件 |
|---|---|---|
| `lab01-array-basics` | 数组/哈希基础 | `LongestConsecutiveList.cpp`、`OnlyChar*.cpp`、`SumOfTwoNums.cpp` |
| `lab02-array-two-pointers` | 数组双指针 | `MidOfConsecutiveArray.cpp`、`RemoveValueInArray.cpp`、`SquareOfConsecutiveArray.cpp` |
| `lab03-linked-list` | 链表 | `1_ReverseList.cpp`、`2_ListLoopDetect.cpp`、`3_PalindromeListDetect.cpp` |
| `lab04-string` | 字符串 | `DNARepeat.cpp`、`FirstCharInString*.cpp`、`LoopString*.cpp` |
| `lab05-stack-queue` | 栈/队列/单调栈 | `DailyTemperature.cpp`、`MaxInWindow*.cpp`、`StringCode.cpp` |
| `lab06-recursion` | 递归/广义表 | `DigitsOfNumber.cpp`、`GenList*.cpp`、`Subset.cpp` |
| `lab07-tree` | 二叉树 | `ClosestSharedAncestor.cpp`、`ConstructTree.cpp`、`RightSideOfTree.cpp` |

每个 lab 的源码都在各自的 `src/` 下。没有统一构建脚本，按文件单独编译：

```bash
g++ -O2 -std=c++17 src/xxx.cpp -o /tmp/xxx
```
