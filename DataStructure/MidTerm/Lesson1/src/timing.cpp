// Lesson1/src/timing.cpp
// 目的：用计时实验验证"阶"，而不是验证常数因子。
// 对应学案 01 第七节任务 2。
// 构建：g++ -O2 -std=c++17 timing.cpp -o ../output/timing.exe
// 运行：../output/timing.exe > ../log/timing.txt

#include <cstdio>
#include <ctime>

// 注意：这里用 s = s * 31 + i 而不是 s += i。
// 若写成求和，编译器会把整个循环折叠成公式 n(n-1)/2，测出来的就不是循环了。
long long workOn(int n) {
    long long s = 0;
    for (int i = 0; i < n; i++)
        s = s * 31 + i;
    return s;
}

long long workOn2(int n) {
    long long s = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            s = s * 31 + j;
    return s;
}

volatile long long sink;  // 防止编译器把无副作用的调用整体优化掉

// 重复 reps 次，返回单次调用的平均耗时（秒）
double perCall(long long (*f)(int), int n, int reps) {
    std::clock_t t1 = std::clock();
    for (int r = 0; r < reps; r++)
        sink = f(n);
    std::clock_t t2 = std::clock();
    return double(t2 - t1) / CLOCKS_PER_SEC / reps;
}

int main() {
    const int ns[] = {2000, 4000, 8000, 16000};
    const int count = sizeof(ns) / sizeof(ns[0]);

    std::printf("%8s %12s %12s %10s %10s\n",
                "n", "O(n)/us", "O(n^2)/ms", "t_2n/t_n", "t_2n/t_n");
    std::printf("%8s %12s %12s %10s %10s\n",
                "", "(linear)", "(quadratic)", "(linear)", "(quadratic)");

    double prev1 = 0.0, prev2 = 0.0;
    for (int i = 0; i < count; i++) {
        int n = ns[i];
        double t1 = perCall(workOn, n, 10000) * 1e6;   // 微秒
        double t2 = perCall(workOn2, n, 1) * 1e3;       // 毫秒
        if (i == 0)
            std::printf("%8d %12.3f %12.3f %10s %10s\n", n, t1, t2, "-", "-");
        else
            std::printf("%8d %12.3f %12.3f %10.2f %10.2f\n",
                        n, t1, t2, t1 / prev1, t2 / prev2);
        prev1 = t1;
        prev2 = t2;
    }

    std::printf("\n结论：n 翻倍时 O(n) 的比值趋近 2，O(n^2) 的比值趋近 4；\n");
    std::printf("      绝对值随机器变化，比值不变。\n");
    return 0;
}
