// Lesson8/src/heap_ops.cpp
// 补充练习：最小堆的建堆、插入、删除堆顶、判堆，以及 1 基下标对照。
// 对应学案 08 第三节。
// 构建：g++ -O2 -std=c++17 src/heap_ops.cpp -o output/heap_ops.exe

#include <cstdio>

// ---------- 0 基下标版本 ----------
void siftUpMin(int *a, int j) {
    int temp = a[j];
    while (j > 0 && temp < a[(j - 1) / 2]) {
        a[j] = a[(j - 1) / 2];
        j = (j - 1) / 2;
    }
    a[j] = temp;
}

void siftDownMin(int *a, int i, int n) {
    int temp = a[i];
    int j = 2 * i + 1;
    while (j < n) {
        if (j + 1 < n && a[j + 1] < a[j]) ++j;  // 选较小的子女
        if (temp <= a[j]) break;
        a[i] = a[j];
        i = j;
        j = 2 * i + 1;
    }
    a[i] = temp;
}

void buildMinHeap(int *a, int n) {
    for (int i = (n - 2) / 2; i >= 0; --i)
        siftDownMin(a, i, n);
}

bool isMinHeap(const int *a, int n) {
    for (int i = 0; i < n; ++i) {
        int l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && a[l] < a[i]) return false;
        if (r < n && a[r] < a[i]) return false;
    }
    return true;
}

int deleteMin(int *a, int &n) {
    int top = a[0];
    a[0] = a[--n];          // 末尾搬到堆顶
    siftDownMin(a, 0, n);   // 待调整范围已经减 1
    return top;
}

// ---------- 1 基下标版本 ----------
void siftDownMin1(int *a, int i, int n) {
    int temp = a[i];
    int j = 2 * i;
    while (j <= n) {
        if (j + 1 <= n && a[j + 1] < a[j]) ++j;
        if (temp <= a[j]) break;
        a[i] = a[j];
        i = j;
        j = 2 * i;
    }
    a[i] = temp;
}

void buildMinHeap1(int *a, int n) {
    for (int i = n / 2; i >= 1; --i)  // 1 基：最后一个非叶结点是 n/2
        siftDownMin1(a, i, n);
}

bool isMinHeap1(const int *a, int n) {
    for (int i = 1; i <= n; ++i) {
        int l = 2 * i, r = 2 * i + 1;
        if (l <= n && a[l] < a[i]) return false;
        if (r <= n && a[r] < a[i]) return false;
    }
    return true;
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
    std::printf("=== 任务 1：建最小堆 ===\n");
    int a[] = {53, 17, 78, 9, 45, 65, 87, 23};
    const int n0 = sizeof(a) / sizeof(a[0]);
    printArray("原始：      ", a, n0);
    buildMinHeap(a, n0);
    printArray("建最小堆后：", a, n0);
    std::printf("是否最小堆：%s\n\n", isMinHeap(a, n0) ? "是" : "不是");

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

    std::printf("=== 任务 3：判堆 ===\n");
    int b[] = {9, 17, 23, 45, 53, 65, 78, 87};
    const int nb = sizeof(b) / sizeof(b[0]);
    printArray("b = ", b, nb);
    std::printf("b 是否最小堆：%s\n", isMinHeap(b, nb) ? "是" : "不是");
    b[nb - 1] = 5;
    printArray("把末尾改成 5：", b, nb);
    std::printf("现在是否最小堆：%s\n\n", isMinHeap(b, nb) ? "是" : "不是");

    std::printf("=== 任务 4：1 基下标对照 ===\n");
    int c[16] = {0};
    const int nc = 8;
    int raw[8] = {53, 17, 78, 9, 45, 65, 87, 23};
    for (int i = 0; i < nc; ++i) c[i + 1] = raw[i];
    buildMinHeap1(c, nc);
    printArray1("建堆后（a[1..8]）：", c, nc);
    std::printf("是否最小堆：%s\n", isMinHeap1(c, nc) ? "是" : "不是");
    std::printf("下标关系：i=1 是根；i=6 的父是 a[3]=%d，左子女 2i=12 越界\n",
                c[3]);
    return 0;
}
