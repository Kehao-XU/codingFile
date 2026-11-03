// Lesson6/src/recursion.cpp
// 三个实验：Hanoi 的递归规模、Fib 的重复计算、n 皇后的回溯。
// 对应学案 06 第七节。
// 构建：g++ -O2 -std=c++17 src/recursion.cpp -o output/recursion.exe

#include <cstdio>
#include <ctime>

// ---------------- Hanoi ----------------
long long hanoiCalls = 0;
long long hanoiMoves = 0;

void hanoi(int n, char from, char via, char to) {
    ++hanoiCalls;
    if (n == 0) return;                 // 出口：没有盘子可移
    hanoi(n - 1, from, to, via);        // ① n-1 个先挪到中转柱
    ++hanoiMoves;                       // ② 最大的那个直接搬
    hanoi(n - 1, via, from, to);        // ③ n-1 个再挪到目标柱
}

// ---------------- Fibonacci ----------------
long long fibCalls = 0;

long long fibRec(int n) {
    ++fibCalls;
    if (n <= 1) return n;
    return fibRec(n - 1) + fibRec(n - 2);
}

long long fibIter(int n, long long &steps) {
    steps = 0;
    if (n <= 1) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        long long c = a + b;
        a = b;
        b = c;
        ++steps;
    }
    return b;
}

// ---------------- n 皇后 ----------------
int n, solutions;
int col[16], md[32], sd[32];            // 列、主对角线、次对角线占用标记

void queen(int row) {
    if (row == n) {                     // 放满 n 行，得到一个解
        ++solutions;
        return;
    }
    for (int j = 0; j < n; ++j) {
        int m = n + row - j - 1;        // 主对角线编号
        int s = row + j;                // 次对角线编号
        if (col[j] || md[m] || sd[s]) continue;   // 被攻击，换下一列
        col[j] = md[m] = sd[s] = 1;               // 落子
        queen(row + 1);
        col[j] = md[m] = sd[s] = 0;               // 撤销（回溯）
    }
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
