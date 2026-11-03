// Lesson2/src/seqlist.cpp
// 三个实验：顺序表平均移动次数、三对角矩阵地址映射、稀疏矩阵快速转置。
// 对应学案 02 第六节。
// 构建：g++ -O2 -std=c++17 src/seqlist.cpp -o output/seqlist.exe

#include <cstdio>
#include <cstdlib>

// ---------- 顺序表 ----------
class SeqList {
public:
    explicit SeqList(int capacity)
        : cap(capacity), last(-1), data(new int[capacity]) {}

    ~SeqList() { delete[] data; }

    int size() const { return last + 1; }

    // 在第 i 个位置插入 x，moves 累加元素搬移次数
    bool insert(int i, int x, long long &moves) {
        if (i < 0 || i > last + 1) return false;  // 合法范围 0..n
        if (last + 1 == cap) return false;
        for (int j = last; j >= i; --j) {         // 从后往前搬
            data[j + 1] = data[j];
            ++moves;
        }
        data[i] = x;
        ++last;
        return true;
    }

    // 删除第 i 个元素，moves 累加元素搬移次数
    bool remove(int i, long long &moves) {
        if (i < 0 || i > last) return false;      // 合法范围 0..n-1
        for (int j = i; j < last; ++j) {          // 从前往后搬
            data[j] = data[j + 1];
            ++moves;
        }
        --last;
        return true;
    }

private:
    int cap;
    int last;
    int *data;
};

static void experimentMoveCost() {
    std::printf("=== 实验 1：顺序表平均移动次数（满表，各位置等概率）===\n");
    std::printf("%8s %14s %14s %12s %12s\n",
                "n", "insert/theory", "insert/measured", "del/theory", "del/measured");

    const int sizes[] = {10, 100, 1000, 10000};
    for (int s = 0; s < 4; ++s) {
        int n = sizes[s];
        SeqList L(n + 1);
        long long dummy = 0;
        for (int v = 0; v < n; ++v) L.insert(L.size(), v, dummy);  // 建满表

        // 插入：对每个位置各试一次，试完立刻删掉以复原
        long long total = 0;
        for (int i = 0; i <= n; ++i) {
            long long mv = 0;
            L.insert(i, -1, mv);
            total += mv;
            long long back = 0;
            L.remove(i, back);
        }
        double insertMeasured = double(total) / (n + 1);

        // 删除：对每个位置各试一次，删完立刻插回
        total = 0;
        for (int i = 0; i < n; ++i) {
            long long mv = 0;
            L.remove(i, mv);
            total += mv;
            long long back = 0;
            L.insert(i, i, back);
        }
        double delMeasured = double(total) / n;

        std::printf("%8d %14.1f %14.1f %12.1f %12.1f\n",
                    n, n / 2.0, insertMeasured, (n - 1) / 2.0, delMeasured);
    }
    std::printf("\n");
}

// ---------- 三对角矩阵 ----------
static void experimentTridiagonal() {
    std::printf("=== 实验 2：三对角矩阵 k = 2i + j 的双向验证（n = 6）===\n");
    const int n = 6;
    int bad = 0, count = 0;
    std::printf("%3s %3s %4s %8s %8s\n", "i", "j", "k", "i'(k)", "j'(k)");
    for (int i = 0; i < n; ++i) {
        for (int j = i - 1; j <= i + 1; ++j) {
            if (j < 0 || j >= n) continue;      // 跳过越界
            int k = 2 * i + j;
            int i2 = (k + 1) / 3;               // 反解：3i-1 <= k < 3(i+1)-1
            int j2 = k - 2 * i2;
            std::printf("%3d %3d %4d %8d %8d%s\n", i, j, k, i2, j2,
                        (i2 == i && j2 == j) ? "" : "  <-- mismatch");
            if (i2 != i || j2 != j) ++bad;
            ++count;
        }
    }
    std::printf("共 %d 个非零元素（理论 3n-2 = %d），不一致 %d 个\n\n", count, 3 * n - 2, bad);
}

// ---------- 稀疏矩阵 ----------
struct Triple {
    int row, col, val;
};

// 朴素转置：对原矩阵每一列扫描整张三元组表，返回比较次数
static int naiveTranspose(const Triple *a, int terms, int cols, Triple *b) {
    int pos = 0, cmp = 0;
    for (int k = 0; k < cols; ++k) {
        for (int i = 0; i < terms; ++i) {
            ++cmp;                              // 一次列号比较
            if (a[i].col == k) {
                b[pos].row = a[i].col;
                b[pos].col = a[i].row;
                b[pos].val = a[i].val;
                ++pos;
            }
        }
    }
    return cmp;
}

// 快速转置：先统计每列非零元个数，再算各行起始位置，最后一趟放置
static void fastTranspose(const Triple *a, int terms, int cols, Triple *b) {
    int *rowSize = new int[cols]();
    int *rowStart = new int[cols]();
    for (int i = 0; i < terms; ++i) rowSize[a[i].col]++;      // O(Terms)
    for (int c = 1; c < cols; ++c)                            // O(Cols)
        rowStart[c] = rowStart[c - 1] + rowSize[c - 1];
    for (int i = 0; i < terms; ++i) {                         // O(Terms)
        int j = rowStart[a[i].col]++;
        b[j].row = a[i].col;
        b[j].col = a[i].row;
        b[j].val = a[i].val;
    }
    delete[] rowSize;
    delete[] rowStart;
}

static void experimentSparse() {
    std::printf("=== 实验 3：稀疏矩阵转置（课件中的 8 个三元组）===\n");
    const int cols = 7, terms = 8;
    Triple a[8] = {{0, 3, 22}, {0, 6, 15}, {1, 1, 11}, {1, 5, 17},
                   {2, 3, -6}, {3, 5, 39}, {4, 0, 91}, {5, 2, 28}};
    Triple b1[8], b2[8];

    int cmp1 = naiveTranspose(a, terms, cols, b1);
    fastTranspose(a, terms, cols, b2);

    std::printf("朴素转置结果：\n");
    for (int i = 0; i < terms; ++i)
        std::printf("  (%d, %d, %d)\n", b1[i].row, b1[i].col, b1[i].val);

    bool same = true;
    for (int i = 0; i < terms; ++i)
        if (b1[i].row != b2[i].row || b1[i].col != b2[i].col || b1[i].val != b2[i].val)
            same = false;

    std::printf("两种算法结果%s\n", same ? "一致" : "不一致");
    std::printf("朴素转置：比较 %d 次（Cols×Terms = %d），写入 %d 次\n",
                cmp1, cols * terms, terms);
    std::printf("快速转置：比较 0 次，写入 %d 次，另需 O(Cols+Terms) 的计数工作\n", terms);
}

int main() {
    experimentMoveCost();
    experimentTridiagonal();
    experimentSparse();
    return 0;
}
