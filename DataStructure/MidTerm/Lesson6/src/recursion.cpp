// Lesson6/src/recursion.cpp
// 三个实验：Hanoi 的递归规模、Fib 的重复计算、n 皇后的回溯。
// 对应学案 06 第七节。
// 构建：g++ -O2 -std=c++17 src/recursion.cpp -o output/recursion.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/recursion.cpp。

#include <cstdio>
#include <ctime>

// ---------------- Hanoi ----------------
long long hanoiCalls = 0;
long long hanoiMoves = 0;

// TODO: 递归解 Hanoi。每次进入函数先 ++hanoiCalls（含 n == 0 的空调用）；
//       n == 0 直接返回；否则按三步走：n-1 个移到 via，最大盘移动到 to，
//       再把这 n-1 个从 via 移到 to。每真正搬动一个盘子 ++hanoiMoves。
//       自测：n 个盘子的移动次数应为 2^n - 1，递归调用次数应为 2^(n+1) - 1。
void hanoi(int n, char from, char via, char to) {
    // TODO
}

// ---------------- Fibonacci ----------------
long long fibCalls = 0;

// TODO: 朴素递归求 Fib。每次进入函数先 ++fibCalls；n <= 1 时直接返回 n。
//       自测：n = 30 时递归调用次数应达到 2,692,537 次，这就是重复计算的代价。
long long fibRec(int n) {
    // TODO
}

// TODO: 迭代求 Fib，避免重复计算。steps 记录循环体执行了多少次（n <= 1 时为 0）。
//       自测：Fib(30) 与递归版结果相同，但 steps 只有 29。
long long fibIter(int n, long long &steps) {
    // TODO
}

// ---------------- n 皇后 ----------------
int n, solutions;
int col[16], md[32], sd[32];            // 列、主对角线、次对角线占用标记

// TODO: 逐行放置皇后。行号 row 从 0 到 n-1，放满 n 行时 ++solutions 并返回。
//       每一列 j 都要检查：同列 col[j]、主对角线 md[n + row - j - 1]、
//       次对角线 sd[row + j] 是否已被占用；能放就置位、递归下一行、再复位（回溯）。
//       自测：n = 8 时应有 92 个解。
void queen(int row) {
    // TODO
}

int solveQueens(int size) {
    n = size;
    solutions = 0;
    for (int i = 0; i < 16; ++i) col[i] = 0;
    for (int i = 0; i < 32; ++i) md[i] = sd[i] = 0;
    queen(0);
    return solutions;
}

int main() {
    std::printf("=== 任务 1：Hanoi 的递归规模 ===\n");
    std::printf("%4s %14s %14s %14s %14s\n", "n", "实际移动", "2^n-1", "递归调用", "2^(n+1)-1");
    for (int k = 1; k <= 20; ++k) {
        hanoiCalls = hanoiMoves = 0;
        hanoi(k, 'A', 'B', 'C');
        long long expectMoves = (1LL << k) - 1;
        long long expectCalls = (1LL << (k + 1)) - 1;
        std::printf("%4d %14lld %14lld %14lld %14lld\n",
                    k, hanoiMoves, expectMoves, hanoiCalls, expectCalls);
    }

    std::printf("\n=== 任务 2：Fib 的重复计算 ===\n");
    std::printf("%4s %14s %18s %14s\n", "n", "Fib(n)", "递归调用次数", "迭代循环次数");
    const int ns[] = {10, 15, 20, 25, 30};
    for (int i = 0; i < 5; ++i) {
        fibCalls = 0;
        long long v = fibRec(ns[i]);
        long long steps = 0;
        fibIter(ns[i], steps);
        std::printf("%4d %14lld %18lld %14lld\n", ns[i], v, fibCalls, steps);
    }

    std::printf("\n=== 任务 3：n 皇后 ===\n");
    std::printf("%4s %10s %12s\n", "n", "解数", "耗时(ms)");
    for (int k = 1; k <= 8; ++k) {
        std::clock_t t1 = std::clock();
        int cnt = solveQueens(k);
        std::clock_t t2 = std::clock();
        std::printf("%4d %10d %12.3f\n", k, cnt,
                    1000.0 * (t2 - t1) / CLOCKS_PER_SEC);
    }
    return 0;
}
