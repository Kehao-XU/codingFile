// Lesson4/src/match.cpp
// BF / KMP（含 next 与优化 next）/ BM（坏字符规则）三者的比较次数对比。
// 对应学案 04 第七节。
// 构建：g++ -O2 -std=c++17 src/match.cpp -o output/match.exe

#include <cstdio>
#include <cstring>

// ---------------- BF ----------------
int bf(const char *t, const char *p, long long &cmp) {
    int n = (int)std::strlen(t), m = (int)std::strlen(p);
    cmp = 0;
    for (int s = 0; s + m <= n; ++s) {          // 窗口起点
        int j = 0;
        while (j < m && (++cmp, t[s + j] == p[j])) ++j;
        if (j == m) return s;                   // 匹配成功
    }
    return -1;
}

// ---------------- KMP ----------------
// next[j] = p[0..j-1] 的最长相等真前缀与真后缀长度，next[0] = -1
void buildNext(const char *p, int *next) {
    int m = (int)std::strlen(p);
    next[0] = -1;
    int j = 0, k = -1;
    while (j < m - 1) {
        if (k == -1 || p[j] == p[k]) {
            ++j;
            ++k;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
}

// 优化版：若 p[j] == p[next[j]]，退过去也必然失配，直接再退一格
void buildNextOpt(const char *p, int *next) {
    int m = (int)std::strlen(p);
    next[0] = -1;
    int j = 0, k = -1;
    while (j < m - 1) {
        if (k == -1 || p[j] == p[k]) {
            ++j;
            ++k;
            next[j] = (p[j] == p[k]) ? next[k] : k;
        } else {
            k = next[k];
        }
    }
}

int kmp(const char *t, const char *p, const int *next, long long &cmp) {
    int n = (int)std::strlen(t), m = (int)std::strlen(p);
    cmp = 0;
    int i = 0, j = 0;                           // i 扫 text，永不回退
    while (i < n && j < m) {
        ++cmp;
        if (j == -1 || t[i] == p[j]) {
            ++i;
            ++j;
        } else {
            j = next[j];                        // text 不动，pattern 回退
        }
    }
    return (j == m) ? i - m : -1;
}

// ---------------- BM（简化：只用坏字符规则）----------------
int bmBadChar(const char *t, const char *p, long long &cmp, long long &shifts) {
    int n = (int)std::strlen(t), m = (int)std::strlen(p);
    int last[256];
    for (int c = 0; c < 256; ++c) last[c] = -1;
    for (int j = 0; j < m; ++j) last[(unsigned char)p[j]] = j;   // 字符在 p 中最后出现的位置

    cmp = 0;
    shifts = 0;
    int i = 0;
    while (i + m <= n) {
        int j = m - 1;
        while (j >= 0 && (++cmp, t[i + j] == p[j])) --j;          // 从右往左比
        if (j < 0) return i;
        int shift = j - last[(unsigned char)t[i + j]];            // 坏字符规则
        i += (shift > 0 ? shift : 1);                             // 至少右移 1 位
        ++shifts;
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
    for (int j = 0; j < m; ++j) std::printf("%4d", j);
    std::printf("\n  p[j]  :");
    for (int j = 0; j < m; ++j) std::printf("%4c", p[j]);
    std::printf("\n  next  :");
    for (int j = 0; j < m; ++j) std::printf("%4d", next[j]);
    std::printf("\n  nextOpt:");
    for (int j = 0; j < m; ++j) std::printf("%4d", nextOpt[j]);
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
    std::printf("text(前 30 字符) = %.30s%s\n", t, std::strlen(t) > 30 ? "..." : "");
    std::printf("pattern = %s, 匹配位置：BF %d, KMP %d, BM %d\n", p, r1, r2, r3);
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
    for (int i = 0; i < 40; ++i) hard[i] = 'a';
    hard[40] = 'c';
    hard[41] = '\0';
    compareOn("最坏输入（BF 的退化案例）", hard, "aaaab");

    // 3. 普通文本
    const char *text = "the quick brown fox jumps over the lazy dog, then searches again";
    compareOn("普通文本", text, "searches");

    return 0;
}
