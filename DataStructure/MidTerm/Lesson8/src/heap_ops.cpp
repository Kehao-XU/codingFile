// Lesson8/src/heap_ops.cpp
// 补充练习：最小堆的建堆、插入、删除堆顶、判堆，以及 1 基下标对照。
// 对应学案 08 第三节。
// 构建：g++ -O2 -std=c++17 src/heap_ops.cpp -o output/heap_ops.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/heap_ops.cpp。

#include <cstdio>

// ---------- 0 基下标版本 ----------
// 父：(i-1)/2，左子女：2i+1，右子女：2i+2
// TODO 1：最小堆的向上调整。把 a[j] 暂存，比父小就把父往下挪。
void siftUpMin(int *a, int j) {
    // TODO
    if (j == 0) return;

    int temp = a[j];
    int cur = j, prev = (j - 1) / 2;
    while (cur > 0 && a[prev] > temp) {
        a[cur] = a[prev];
        cur = prev;
        prev = (prev - 1) / 2;
    }
    a[cur] = temp;
}

// TODO 2：最小堆的向下调整。每轮选较小的子女，比它大就上移。
void siftDownMin(int *a, int i, int n) {
    // TODO
}

// TODO 3：自底向上建堆，从最后一个非叶结点 (n-2)/2 开始。
void buildMinHeap(int *a, int n) {
    // TODO
}

// TODO 4：判断 a[0..n-1] 是否是最小堆。
bool isMinHeap(const int *a, int n) {
    // TODO
    return false;
}

// TODO 5：删除堆顶并返回它。末尾元素搬到堆顶，n 减 1 后下沉。
//         n 用引用，调用后堆的大小会变。
int deleteMin(int *a, int &n) {
    // TODO
    return 0;
}

// ---------- 1 基下标版本（考题常见约定） ----------
// 父：i/2，左子女：2i，右子女：2i+1，a[1..n]
// TODO 6
void siftDownMin1(int *a, int i, int n) {
    // TODO
}

// TODO 7
void buildMinHeap1(int *a, int n) {
    // TODO
}

// TODO 8
bool isMinHeap1(const int *a, int n) {
    // TODO
    return false;
}

// ---------- 打印 ----------
void printArray(const char *tag, const int *a, int n) {
    std::printf("%s", tag);
    for (int i = 0; i < n; ++i)
        std::printf("%d%s", a[i], i + 1 < n ? " " : "");
    std::printf("\n");
}

void printArray1(const char *tag, const int *a, int n) {
    std::printf("%s", tag);
    for (int i = 1; i <= n; ++i)
        std::printf("%d%s", a[i], i < n ? " " : "");
    std::printf("\n");
}

int main() {
    // ---------- 任务 1：建最小堆 ----------
    std::printf("=== 任务 1：建最小堆 ===\n");
    int a[] = {53, 17, 78, 9, 45, 65, 87, 23};
    const int n0 = sizeof(a) / sizeof(a[0]);
    printArray("原始：      ", a, n0);
    buildMinHeap(a, n0);
    printArray("建最小堆后：", a, n0);
    std::printf("是否最小堆：%s\n\n", isMinHeap(a, n0) ? "是" : "不是");

    // ---------- 任务 2：插入 + 连续删除堆顶 ----------
    std::printf("=== 任务 2：插入与删除堆顶 ===\n");
    int h[32];
    int hn = 0;
    const int data[] = {4, 2, 5, 8, 3, 6, 10, 14};
    for (int i = 0; i < 8; ++i) {
        h[hn] = data[i];
        siftUpMin(h, hn);
        ++hn;
    }
    printArray("依次插入 4,2,5,8,3,6,10,14 后：", h, hn);
    std::printf("连续 deleteMin：");
    while (hn > 0) {
        int x = deleteMin(h, hn);
        std::printf("%d%s", x, hn > 0 ? " " : "");
    }
    std::printf("\n\n");

    // ---------- 任务 3：判堆 ----------
    std::printf("=== 任务 3：判堆 ===\n");
    int b[] = {9, 17, 23, 45, 53, 65, 78, 87};
    const int nb = sizeof(b) / sizeof(b[0]);
    printArray("b = ", b, nb);
    std::printf("b 是否最小堆：%s\n", isMinHeap(b, nb) ? "是" : "不是");
    b[nb - 1] = 5;  // 故意破坏
    printArray("把末尾改成 5：", b, nb);
    std::printf("现在是否最小堆：%s\n\n", isMinHeap(b, nb) ? "是" : "不是");

    // ---------- 任务 4：1 基下标 ----------
    std::printf("=== 任务 4：1 基下标对照 ===\n");
    int c[16] = {0};  // c[0] 不用
    const int nc = 8;
    for (int i = 0; i < nc; ++i) c[i + 1] = a[i];
    // 注意：a 已经建过一次堆，这里重新用原始数据更直观
    int raw[8] = {53, 17, 78, 9, 45, 65, 87, 23};
    for (int i = 0; i < nc; ++i) c[i + 1] = raw[i];
    buildMinHeap1(c, nc);
    printArray1("建堆后（a[1..8]）：", c, nc);
    std::printf("是否最小堆：%s\n", isMinHeap1(c, nc) ? "是" : "不是");
    std::printf("下标关系：i=1 是根；i=6 的父是 a[3]=%d，左子女 2i=12 越界\n",
                c[3]);
    return 0;
}
