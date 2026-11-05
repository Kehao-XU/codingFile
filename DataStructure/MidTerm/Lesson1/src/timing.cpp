// Lesson1/src/timing.cpp
// 目的：用计时实验验证"阶"，而不是验证常数因子。
// 对应学案 01 第七节任务 2。
// 构建：g++ -O2 -std=c++17 timing.cpp -o ../output/timing.exe
// 运行：../output/timing.exe > ../log/timing.txt
//
// 练习骨架：TODO 处自己补，参考答案在 solution/timing.cpp。

#include <cstdio>
#include <ctime>

// TODO: 写一个 O(n) 的工作量函数：循环 n 次，返回值必须真的依赖循环。
//       提示：写成 s += i 时，-O2 会直接折叠成公式 n(n-1)/2，测出来就不是循环了。
long long workOn(int n) {
    // TODO
}

// TODO: 写一个 O(n^2) 的工作量函数：双层循环各跑 n 次，同样不能让编译器折叠。
long long workOn2(int n) {
    // TODO
}

volatile long long sink;  // 防止编译器把无副作用的调用整体优化掉

// TODO: 把 f(n) 重复调用 reps 次，用 std::clock() 计时，返回单次平均耗时（秒）。
//       要点：结果要写进 sink，否则 -O2 会把整个循环删掉；
//             返回的是 (t2 - t1) / CLOCKS_PER_SEC / reps。
double perCall(long long (*f)(int), int n, int reps) {
    // TODO
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
