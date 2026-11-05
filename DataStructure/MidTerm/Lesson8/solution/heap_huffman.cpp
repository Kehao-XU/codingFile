// Lesson8/src/heap_huffman.cpp
// 四件事：Catalan 计数、最小堆插入、堆排序、霍夫曼编码。
// 对应学案 08 第五节。
// 构建：g++ -O2 -std=c++17 src/heap_huffman.cpp -o output/heap_huffman.exe

#include <cstdio>

// ---------------- 1. Catalan ----------------
long long catalanRec(int n, long long *b) {   // 递推：b[n] = Σ b[i]*b[n-1-i]
    b[0] = 1;
    for (int k = 1; k <= n; ++k) {
        long long s = 0;
        for (int i = 0; i < k; ++i) s += b[i] * b[k - 1 - i];
        b[k] = s;
    }
    return b[n];
}

long long catalanFormula(int n) {             // 通项：C(2n,n)/(n+1)
    long long c = 1;
    for (int i = 1; i <= n; ++i) c = c * (n + i) / i;   // 先算 C(2n,n)
    return c / (n + 1);
}

void taskCatalan() {
    std::printf("=== 任务 1：n 个结点的不同二叉树棵数 ===\n");
    std::printf("%4s %14s %14s\n", "n", "递推 b[n]", "通项");
    long long b[16];
    catalanRec(10, b);
    for (int n = 0; n <= 8; ++n)
        std::printf("%4d %14lld %14lld\n", n, b[n], catalanFormula(n));
    std::printf("\n");
}

// ---------------- 2. 最小堆 ----------------
class MinHeap {
public:
    explicit MinHeap(int cap) : cap(cap), size(0), a(new int[cap]) {}
    ~MinHeap() { delete[] a; }

    void insert(int x) {
        a[size] = x;
        siftUp(size);
        ++size;
    }

    void print() const {
        std::printf("  [");
        for (int i = 0; i < size; ++i) std::printf("%d%s", a[i], i + 1 < size ? " " : "");
        std::printf("]\n");
    }

private:
    void siftUp(int j) {                       // 新元素一路上浮
        int i = (j - 1) / 2;                   // 父结点
        int temp = a[j];
        while (j > 0 && a[i] > temp) {
            a[j] = a[i];
            j = i;
            i = (i - 1) / 2;
        }
        a[j] = temp;
    }

    int cap, size;
    int *a;
};

void taskHeapInsert() {
    std::printf("=== 任务 2：最小堆的插入（SiftUp）===\n");
    const int data[] = {4, 2, 5, 8, 3, 6, 10, 14};
    MinHeap h(32);
    for (int i = 0; i < 8; ++i) {
        h.insert(data[i]);
        std::printf("插入 %2d 后：", data[i]);
        h.print();
    }
    std::printf("\n");
}

// ---------------- 3. 堆排序 ----------------
void siftDown(int *a, int i, int n) {          // 在 a[0..n-1] 内把 a[i] 沉下去
    int temp = a[i];
    int j = 2 * i + 1;                         // 左子女
    while (j < n) {
        if (j + 1 < n && a[j + 1] > a[j]) ++j; // 选较大的子女
        if (temp >= a[j]) break;
        a[i] = a[j];
        i = j;
        j = 2 * j + 1;
    }
    a[i] = temp;
}

void heapSort(int *a, int n) {
    for (int i = (n - 2) / 2; i >= 0; --i)     // 建最大堆：从最后一个非叶结点开始
        siftDown(a, i, n);
    for (int i = n - 1; i > 0; --i) {          // 每次把堆顶换到末尾
        int t = a[0];
        a[0] = a[i];
        a[i] = t;
        siftDown(a, 0, i);                     // 待调整范围缩小到 i
    }
}

void printArray(const char *tag, const int *a, int n) {
    std::printf("%s", tag);
    for (int i = 0; i < n; ++i) std::printf("%d%s", a[i], i + 1 < n ? " " : "");
    std::printf("\n");
}

void taskHeapSort() {
    std::printf("=== 任务 3：堆排序 ===\n");
    int a[] = {53, 17, 78, 9, 45, 65, 87, 23};
    const int n = sizeof(a) / sizeof(a[0]);
    printArray("原始：    ", a, n);

    for (int i = (n - 2) / 2; i >= 0; --i) siftDown(a, i, n);
    printArray("建最大堆后：", a, n);

    heapSort(a, n);
    printArray("排序后：  ", a, n);
    std::printf("\n");
}

// ---------------- 4. 霍夫曼 ----------------
// 用数组存 2n-1 个结点：0..n-1 是叶，n..2n-2 是内部结点。
// 每次扫描所有"还没有父结点"的结点，取权值最小的两个合并。
void taskHuffman() {
    std::printf("=== 任务 4：霍夫曼编码 ===\n");
    const char ch[] = {'A', 'T', 'C', 'S'};
    const int w[] = {7, 5, 4, 2};
    const int n = 4, total = 2 * n - 1;

    int val[16], parent[16], lc[16], rc[16];
    for (int i = 0; i < n; ++i) {
        val[i] = w[i];
        parent[i] = -1;
    }
    for (int i = n; i < total; ++i) {
        val[i] = 0;
        parent[i] = -1;
        lc[i] = rc[i] = -1;
    }

    int wpl = 0;
    for (int k = n; k < total; ++k) {
        int m1 = -1, m2 = -1;                        // 最小、次小
        for (int i = 0; i < k; ++i) {
            if (parent[i] != -1) continue;
            if (m1 == -1 || val[i] < val[m1]) { m2 = m1; m1 = i; }
            else if (m2 == -1 || val[i] < val[m2]) { m2 = i; }
        }
        val[k] = val[m1] + val[m2];
        parent[m1] = parent[m2] = k;
        lc[k] = m1;                                  // 小的在左，记 0
        rc[k] = m2;
        wpl += val[k];                               // 内部结点权值之和 = WPL
        std::printf("  合并 %d 与 %d -> %d\n", val[m1], val[m2], val[k]);
    }

    std::printf("  编码：\n");
    for (int i = 0; i < n; ++i) {
        char code[16];
        int len = 0, cur = i;
        while (parent[cur] != -1) {                  // 从叶往上走到根
            int p = parent[cur];
            code[len++] = (lc[p] == cur) ? '0' : '1';
            cur = p;
        }
        std::printf("    %c : ", ch[i]);
        for (int j = len - 1; j >= 0; --j) std::printf("%c", code[j]);
        std::printf("   (权值 %d)\n", w[i]);
    }

    int sumWL = 0;                                   // 另一种算法：Σ w_i × 深度
    for (int i = 0; i < n; ++i) {
        int depth = 0;
        for (int p = parent[i]; p != -1; p = parent[p]) ++depth;
        sumWL += w[i] * depth;
    }
    std::printf("  WPL（内部结点权值和）= %d，WPL（Σ 权值×深度）= %d\n", wpl, sumWL);
    std::printf("  等长编码总长 = (7+5+4+2)×2 = 36\n\n");
}

int main() {
    taskCatalan();
    taskHeapInsert();
    taskHeapSort();
    taskHuffman();
    return 0;
}
