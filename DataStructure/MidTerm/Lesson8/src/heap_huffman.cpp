// Lesson8/src/heap_huffman.cpp
// 四件事：Catalan 计数、最小堆插入、堆排序、霍夫曼编码。
// 对应学案 08 第五节。
// 构建：g++ -O2 -std=c++17 src/heap_huffman.cpp -o output/heap_huffman.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/heap_huffman.cpp。

#include <cstdio>

// ---------------- 1. Catalan ----------------
// TODO: 用递推求 b[0..n]。b[0] = 1，b[k] = Σ(i=0..k-1) b[i] * b[k-1-i]，返回
// b[n]。
//       自测：b[0..8] = 1, 1, 2, 5, 14, 42, 132, 429, 1430。
long long catalanRec(int n, long long *b) {
    // TODO
    b[0] = 1;
    for (int i = 1; i <= n; ++i) {
        long long s = 0;
        for (int j = 0; j < i; ++j) {
            s += b[j] * b[i - j - 1];
        }
        b[i] = s;
    }
    return b[n];
}

// TODO: 用通项 C(2n, n) / (n + 1) 直接算。注意边算边除，别让中间值溢出。
//       自测：与递推版对每个 n 都应相等。
long long catalanFormula(int n) {
    // TODO
    long long c = 1;
    for (int i = 1; i <= n; ++i) {
        c = c * (n + i) / i;
    }
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

    // TODO: 把 x 放到数组末尾（下标 size），对新位置做一次 siftUp，然后
    // ++size。
    //       注意先放再上浮，size 在最后自增。
    void insert(int x) {
        // TODO
        a[size] = x;
        siftUp(size);
        ++size;
    }

    void print() const {
        std::printf("  [");
        for (int i = 0; i < size; ++i)
            std::printf("%d%s", a[i], i + 1 < size ? " " : "");
        std::printf("]\n");
    }

  private:
    // TODO: 从下标 j 开始向上调整，直到父结点不大于自己或到达根。
    //       要点：先把 a[j]
    //       暂存，一路把父结点往下挪，最后再填回，避免反复交换。
    void siftUp(int j) {
        // TODO
        int temp = a[j];
        int current = j, prev = (j - 1) / 2;
        while (current > 0 && temp < a[prev]) {
            a[current] = a[prev];
            current = prev;
            prev = (prev - 1) / 2;
        }
        a[current] = temp;
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
// TODO: 在 a[0..n-1] 内把 a[i] 向下调整（最大堆语义）。
//       先暂存 a[i]，每轮在左右子女里挑较大的，比它大就上移，直到比不下或到底。
void siftDown(int *a, int i, int n) {
    // TODO
    int temp = a[i];
    int j = 2 * i + 1; // 左子女
    while (j < n) {
        if (j + 1 < n && a[j + 1] > a[j])
            ++j; // 选较大的子女
        if (temp >= a[j])
            break;
        a[i] = a[j];
        i = j;
        j = 2 * j + 1;
    }
    a[i] = temp;
}

// TODO: 堆排序（升序）。先从最后一个非叶结点 (n-2)/2 开始倒着 siftDown
// 建最大堆，
//       再反复把堆顶换到末尾、待调整范围缩小一格。
//       自测：{53,17,78,9,45,65,87,23} 排序后应为 9 17 23 45 53 65 78 87。
void heapSort(int *a, int n) {
    // TODO
    for (int i = (n - 2) / 2; i >= 0; --i)
        siftDown(a, i, n);
    for (int i = n - 1; i > 0; --i) {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        siftDown(a, 0, i);
    }
}

void printArray(const char *tag, const int *a, int n) {
    std::printf("%s", tag);
    for (int i = 0; i < n; ++i)
        std::printf("%d%s", a[i], i + 1 < n ? " " : "");
    std::printf("\n");
}

void taskHeapSort() {
    std::printf("=== 任务 3：堆排序 ===\n");
    int a[] = {53, 17, 78, 9, 45, 65, 87, 23};
    const int n = sizeof(a) / sizeof(a[0]);
    printArray("原始：    ", a, n);

    for (int i = (n - 2) / 2; i >= 0; --i)
        siftDown(a, i, n);
    printArray("建最大堆后：", a, n);

    heapSort(a, n);
    printArray("排序后：  ", a, n);
    std::printf("\n");
}

// ---------------- 4. 霍夫曼 ----------------
// TODO: 用数组法构造霍夫曼树，返回 WPL。
//       结点 0..n-1 是叶子（权值 w[i]），n..2n-2 是内部结点。
//       每轮在所有 parent == -1 的结点里找权值最小和次小的两个，合并成新结点
//       k： val[k] = 两者之和，parent 设为 k，lc[k] 记小的、rc[k]
//       记大的，并打印 "  合并 %d 与 %d -> %d\n"。WPL
//       就是所有内部结点权值之和。 调用前 val/parent/lc/rc 已初始化，叶子的 val
//       已填好。
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

    // TODO: 合并 n-1 轮，得到 wpl。
    int wpl = 0;

    // TODO: 输出编码。对每个叶子从下往上走到根，左子女记 '0'、右子女记 '1'，
    //       收集完逆序打印。格式："    %c : " 后跟编码，再跟 "   (权值 %d)\n"。
    std::printf("  编码：\n");

    int sumWL = 0; // 另一种算法：Σ w_i × 深度
    for (int i = 0; i < n; ++i) {
        int depth = 0;
        for (int p = parent[i]; p != -1; p = parent[p])
            ++depth;
        sumWL += w[i] * depth;
    }
    std::printf("  WPL（内部结点权值和）= %d，WPL（Σ 权值×深度）= %d\n", wpl,
                sumWL);
    std::printf("  等长编码总长 = (7+5+4+2)×2 = 36\n\n");
}

int main() {
    taskCatalan();
    taskHeapInsert();
    taskHeapSort();
    taskHuffman();
    return 0;
}
