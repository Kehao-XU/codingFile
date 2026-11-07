// Lesson8/src/huffman_codec.cpp
// 补充练习：任意 n 个权值的霍夫曼树、编码、解码、前缀码校验。
// 对应学案 08 第四节（自测第 4 题的一组权值）。
// 构建：g++ -O2 -std=c++17 src/huffman_codec.cpp -o output/huffman_codec.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/huffman_codec.cpp。

#include <cstdio>
#include <cstring>

const int MAXN = 32;

// 霍夫曼树的数组表示：叶子 0..n-1，内部结点 n..2n-2。
struct HNode {
    int weight;
    int parent, left, right;
};

// 在 0..k-1 中找 parent == -1 的权值最小和次小的两个结点。
// 这个函数已经给好，专注合并逻辑本身。
void findTwoMin(const HNode *t, int k, int &m1, int &m2) {
    m1 = m2 = -1;
    for (int i = 0; i < k; ++i) {
        if (t[i].parent != -1) continue;
        if (m1 == -1 || t[i].weight < t[m1].weight) {
            m2 = m1;
            m1 = i;
        } else if (m2 == -1 || t[i].weight < t[m2].weight) {
            m2 = i;
        }
    }
}

// ---------- 任务 1：构造霍夫曼树 ----------
// 合并 n-1 轮；每轮取两个最小根，新结点权值为二者之和。
// 返回 WPL（= 所有内部结点权值之和），并打印 "  合并 %d 与 %d -> %d\n"。
// TODO 1
int buildHuffman(HNode *t, int n) {
    // TODO
    return 0;
}

// ---------- 任务 2：生成编码 ----------
// 从每个叶子往根走：是父结点的左子女记 '0'，右子女记 '1'；
// 收集完是反的，要逆序写进 code[i]。len[i] 存编码长度。
// TODO 2
void buildCodes(const HNode *t, int n, char code[][MAXN], int *len) {
    // TODO
}

// ---------- 任务 3：解码 ----------
// 从根出发：'0' 走左、'1' 走右；到叶子就输出 ch[叶子]，再回到根。
// TODO 3
void decode(const HNode *t, int root, const char *bits, const char *ch) {
    // TODO
}

// ---------- 任务 4：前缀码校验 ----------
// 两两检查：短编码是否是长编码的前缀。全部满足返回 true。
// TODO 4
bool isPrefixFree(char code[][MAXN], int n) {
    // TODO
    return false;
}

int main() {
    const char ch[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    const int w[] = {15, 3, 14, 2, 6, 9, 16, 17};
    const int n = 8, total = 2 * n - 1;

    HNode t[MAXN];
    for (int i = 0; i < n; ++i) t[i] = {w[i], -1, -1, -1};
    for (int i = n; i < total; ++i) t[i] = {0, -1, -1, -1};

    // 任务 1
    std::printf("=== 任务 1：构造霍夫曼树 ===\n");
    int wpl = buildHuffman(t, n);
    std::printf("\n");

    // 任务 2
    std::printf("=== 任务 2：编码表 ===\n");
    char code[MAXN][MAXN];
    int len[MAXN];
    buildCodes(t, n, code, len);
    for (int i = 0; i < n; ++i)
        std::printf("  %c : %-6s (权值 %d，长度 %d)\n", ch[i], code[i], w[i],
                    len[i]);
    std::printf("\n");

    // 任务 3
    std::printf("=== 任务 3：WPL ===\n");
    int sumWL = 0;
    for (int i = 0; i < n; ++i) sumWL += w[i] * len[i];
    std::printf("内部结点权值和 = %d，Σ 权值×深度 = %d\n\n", wpl, sumWL);

    // 任务 4：编码 + 解码
    std::printf("=== 任务 4：编码与解码 ===\n");
    const char *text = "ABCDEFGH";
    char bits[1024] = {0};
    for (int i = 0; text[i]; ++i) {
        int k = text[i] - 'A';
        std::strcat(bits, code[k]);
    }
    std::printf("原文：%s\n", text);
    std::printf("编码：%s（%d 位）\n", bits, (int)std::strlen(bits));
    std::printf("解码：");
    decode(t, total - 1, bits, ch);
    std::printf("\n\n");

    // 任务 5
    std::printf("=== 任务 5：前缀码校验 ===\n");
    std::printf("%s\n", isPrefixFree(code, n) ? "没有编码是另一个编码的前缀"
                                              : "存在前缀冲突");
    return 0;
}
