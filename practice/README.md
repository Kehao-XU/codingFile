# practice

早期练习归档，按主题分类。

| 目录 | 内容 |
|---|---|
| `cpp/linked-list/` | 链表：成对交换、有序插入、两数相加、相交结点、有序表合并/差集 |
| `cpp/dfs/` | DFS 题目与先序遍历 |
| `cpp/file-io/` | 文件读写、字数统计、合并文件、随机数文件最值 |
| `cpp/freshman-review/` | 大一程序设计复习题 |
| `cpp/lru-cache/` | 哈希表 + 双向链表实现 LRU |
| `cpp/matrix/` | 魔方阵判定 |
| `cpp/polynomial/` | 一元多项式加法/乘法 |
| `cpp/string/` | 字符串加密 |
| `cpp/misc/` | Hello World 等一次性小文件 |
| `python/` | Python 小脚本 |

## 文件 I/O 练习

在 `cpp/file-io/` 目录下编译运行，数据文件在 `data/`：

```bash
cd practice/cpp/file-io
g++ -O2 -std=c++17 count-letters.cpp -o count-letters
./count-letters
```

部分旧代码的注释是 GBK 编码，在 UTF-8 编辑器里可能显示乱码，不影响编译。
