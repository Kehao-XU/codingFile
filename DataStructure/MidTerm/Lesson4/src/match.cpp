// Lesson4/src/match.cpp
// BF / KMP（含 next 与优化 next）/ BM（坏字符规则）三者的比较次数对比。
// 对应学案 04 第七节。
// 构建：g++ -O2 -std=c++17 src/match.cpp -o output/match.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/match.cpp。

#include <cstdio>
#include <cstring>

// ---------------- BF ----------------
// TODO: 朴素匹配。对每个起点 s（0 <= s <= n-m）逐字符比较，返回首次匹配的下标，
//       找不到返回 -1。cmp 累加字符比较次数（每比较一次字符就 ++cmp）。
//       自测：text 为 40 个 'a' 加一个 'c'、pattern 为 "aaaab"
//       时，比较次数应远大于
//             KMP 和 BM。
int bf(const char *t, const char *p, long long &cmp) {
    // TODO
    cmp = 0;
    int cur = 0;
    int PATTERN_SIZE = (int)std::strlen(p), TEXT_SIZE = (int)std::strlen(t);
    while (cur + PATTERN_SIZE < TEXT_SIZE) {
        int i = 0;
        for (; i < PATTERN_SIZE; ++i) {
            ++cmp;
            if (t[cur + i] != p[i])
                break;
        }
        if (i == PATTERN_SIZE)
            return cur;
        else
            ++cur;
    }
    return -1;
}

// ---------------- KMP ----------------
// TODO: 构造 next 数组。next[j] = p[0..j-1] 的最长相等真前缀与真后缀长度，
//       约定 next[0] = -1。用递推（j 前进，失配时 k = next[k]）实现，O(m)。
//       自测：p = "abaabcac" 时 next = -1 0 0 1 1 2 0 1。
void buildNext(const char *p, int *next) {
    // TODO
    int SIZE = std::strlen(p);
    next[0] = -1;

    int j = 0, k = -1;
    while (j < SIZE - 1) {
        if (k == -1 || p[j] == p[k]) {
            ++j;
            ++k;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
}

// TODO: 优化版 next。在 next[j] = k 之后再看 p[j] 是否等于 p[k]，
//       相等就令 next[j] = next[k]，跳过必然失配的那一步。
//       自测：p = "aaaa" 时，普通 next = -1 0 1 2 3，优化 next = -1 -1 -1 -1。
void buildNextOpt(const char *p, int *next) {
    // TODO
    int SIZE = std::strlen(p);
    next[0] = -1;
    int j = 0, k = -1;
    while (j < SIZE - 1) {
        if (k == -1 || p[j] == p[k]) {
            ++j;
            ++k;
            next[j] = (p[j] == p[k]) ? next[k] : k;
        } else {
            k = next[k];
        }
    }
}

// TODO: KMP 匹配。i 扫 text 且永不回退，j 扫 pattern，失配时 j = next[j]。
//       cmp 累加比较次数（注意 j == -1 那一轮也算一次）。
//       返回首次匹配的下标，找不到返回 -1。
//       自测：最坏输入上比较次数应明显低于 BF。
int kmp(const char *t, const char *p, const int *next, long long &cmp) {
    // TODO
    cmp = 0;
    int i = 0, j = 0;
    int TEXT_SIZE = std::strlen(t), PATTERN_SIZE = std::strlen(p);

    while (i < TEXT_SIZE && j < PATTERN_SIZE) {
        ++cmp;
        if (j == -1 || t[i] == p[j]) {
            ++i;
            ++j;
        } else {
            j = next[j];
        }
    }

    return (j == PATTERN_SIZE) ? i - PATTERN_SIZE : -1;
}

// ---------------- BM（简化：只用坏字符规则）----------------
// TODO: BM 的坏字符规则。先预处理 last[c] = 字符 c 在 p
// 中最后出现的位置（没有则 -1），
//       然后从右往左比较：失配时按 shift = j - last[t[i+j]] 右移，至少移动 1
//       位。 cmp 累加比较次数，shifts 累加移位次数。
//       返回首次匹配的下标，找不到返回 -1。
//       自测：普通英文文本上，比较次数应远少于 BF 和 KMP。
int bmBadChar(const char *t, const char *p, long long &cmp, long long &shifts) {
    // TODO
    int TEXT_SIZE = std::strlen(t), PATTERN_SIZE = std::strlen(p);
    int last[256];
    for (int i = 0; i < 256; ++i)
        last[i] = -1;
    for (int j = 0; j < PATTERN_SIZE; ++j)
        last[(unsigned char)p[j]] = j;

    cmp = 0;
    shifts = 0;
    int i = 0;
    while (i + PATTERN_SIZE < TEXT_SIZE) {
        int j = PATTERN_SIZE - 1;
        while (j >= 0 && (++cmp, t[i + j] == p[j]))
            --j;
        if (j < 0)
            return i;
        else {
            int shift = j - last[(unsigned char)t[i + j]];
            i += (shift > 0 ? shift : 1);
            ++shifts;
        }
    }
    return -1;
}

// ---------------- 测试 ----------------
static void printNext(const char *p) {
    int m = (int)std::strlen(p);
    int next[64], nextOpt[64];
    buildNext(p, next);
    buildNextOpt(p, nextOpt);
    std::printf("pattern = %s\n", p);
    std::printf("  j     :");
    for (int j = 0; j < m; ++j)
        std::printf("%4d", j);
    std::printf("\n  p[j]  :");
    for (int j = 0; j < m; ++j)
        std::printf("%4c", p[j]);
    std::printf("\n  next  :");
    for (int j = 0; j < m; ++j)
        std::printf("%4d", next[j]);
    std::printf("\n  nextOpt:");
    for (int j = 0; j < m; ++j)
        std::printf("%4d", nextOpt[j]);
    std::printf("\n\n");
}

static void compareOn(const char *tag, const char *t, const char *p) {
    long long c1, c2, c3, sh;
    int next[64];
    buildNext(p, next);

    int r1 = bf(t, p, c1);
    int r2 = kmp(t, p, next, c2);
    int r3 = bmBadChar(t, p, c3, sh);

    std::printf("--- %s ---\n", tag);
    std::printf("text(前 30 字符) = %.30s%s\n", t,
                std::strlen(t) > 30 ? "..." : "");
    std::printf("pattern = %s, 匹配位置：BF %d, KMP %d, BM %d\n", p, r1, r2,
                r3);
    std::printf("%-6s %12s %12s\n", "算法", "比较次数", "其他");
    std::printf("%-6s %12lld %12s\n", "BF", c1, "-");
    std::printf("%-6s %12lld %12s\n", "KMP", c2, "text 指针不回退");
    std::printf("%-6s %12lld %12lld 次位移\n", "BM", c3, sh);
    std::printf("\n");
}

int main() {
    // 1. next 数组手算对照
    printNext("abaabcac");
    printNext("aaaa");

    // 2. 最坏输入：40 个 'a' 加一个 'c'，pattern 前四位全是 'a'
    char hard[64];
    for (int i = 0; i < 40; ++i)
        hard[i] = 'a';
    hard[40] = 'c';
    hard[41] = '\0';
    compareOn("最坏输入（BF 的退化案例）", hard, "aaaab");

    // 3. 普通文本
    const char *text =
        "the quick brown fox jumps over the lazy dog, then searches again";
    compareOn("普通文本", text, "searches");

    return 0;
}
