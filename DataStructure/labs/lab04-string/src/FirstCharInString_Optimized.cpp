#include <cstdio>
#include <string>

/**
 * 题目：给定一个字符串
 * s，找到它的第一个不重复的字符，返回它的索引；不存在则返回 -1。
 *
 * 本文件是 FirstCharInString.cpp（std::map
 * 写法）的更优版本，单独存放，不覆盖原做法。
 *
 * ── 原写法（std::map<char,int>）的问题 ──────────────────────────
 *   1. 时间 O(n log n)：std::map 是红黑树，每次查找/插入都是 O(log n)；
 *   2. 为了同时记录"是否出现过"和"首次下标"，容易用 0 当哨兵值，
 *      与合法下标 0 冲突 —— 原代码就踩了这个坑（见文末备注）。
 *
 * ── 关键认识：字符的取值空间只有 256 种 ────────────────────────
 *   键域既然极小，就不必用树或哈希表，直接用定长数组当"直址表"：
 *   O(1) 访问、连续内存、零堆分配。这是"键域很小"场景最常用的优化。
 *
 * ── 方案：两遍扫描计数法 ──────────────────────────────────────
 *   第一遍：统计每个字符出现的次数；
 *   第二遍：从左往右找第一个次数为 1 的字符。
 *   时间复杂度 O(n)，空间 O(1)（固定 256 个 int）。
 *   只存"次数"，判断条件就是干净的 == 1，不会再有哨兵值歧义。
 */

int solution(const std::string &s) {
    int count[256] = {0}; // 值域小 -> 定长数组当直址表
    for (unsigned char c : s)
        ++count[c]; // 第一遍：计数

    for (int i = 0; i < (int)s.size(); ++i) // 第二遍：找第一个只出现一次的
        if (count[(unsigned char)s[i]] == 1)
            return i;

    return -1;
}

// ------------------------------------------------------------------
// 下面只是自测，提交到判题系统时删掉即可
// ------------------------------------------------------------------

// 暴力参考实现，仅用于校验正确性
int bruteForce(const std::string &s) {
    for (int i = 0; i < (int)s.size(); ++i) {
        int k = 0;
        for (char c : s)
            if (c == s[i])
                ++k;
        if (k == 1)
            return i;
    }
    return -1;
}

int main() {
    // 其中的 "abc"、"aabb" 正是能触发原 map 版哨兵值 bug 的用例
    const char *cases[] = {"abc", "aabb", "leetcode", "loveleetcode",
                           "",    "z",    "aab"};
    bool allOk = true;
    for (const char *cs : cases) {
        std::string s = cs;
        int got = solution(s);
        int want = bruteForce(s);
        bool ok = (got == want);
        allOk = allOk && ok;
        std::printf("input=%-14s got=%2d  expect=%2d  %s\n", cs, got, want,
                    ok ? "OK" : "WRONG");
    }
    std::printf("%s\n", allOk ? "ALL PASSED" : "SOME FAILED");
    return allOk ? 0 : 1;
}

/**
 * 备注：原 map 版为什么错
 *   它用 record[c] != 0 判断"之前是否出现过"，但 map 的默认值就是 0，
 *   而索引 0 也是合法值，两者被混为一谈：
 *     - 输入 "abc"（答案 0）：'a' 存成 0，第二遍误判为"出现过"而跳过，返回 1；
 *     - 输入 "aabb"（答案 -1）：第二个 'a' 时
 * record['a']==0，被误当成首次出现， 'a' 因此没被标记为重复，最终错误地返回 0。
 */
