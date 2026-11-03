#include <cstdio>
#include <string>
#include <vector>

bool solutionConcat(const std::string &s) {
    int n = (int)s.size();
    if (n < 2)
        return false;
    return (s + s).find(s, 1) != (std::string::size_type)n;
}

int main() {
    const char *cases[] = {
        "abcabc", "abab", "aaaa", "ababab", "abcdabcdabcd", "aba",    "abcab",
        "a",      "",     "aa",   "abcabd", "zzzzz",        "xyzxyzx"};
    bool allOk = true;
    for (const char *cs : cases) {
        std::string s = cs;
        bool b = solutionConcat(s);
        bool ok = (a == want) && (b == want);
        allOk = allOk && ok;
        std::printf("input=%-14s kmp=%-3s concat=%-3s expect=%-3s  %s\n", cs,
                    a ? "YES" : "NO", b ? "YES" : "NO", want ? "YES" : "NO",
                    ok ? "OK" : "WRONG");
    }
    std::printf("%s\n", allOk ? "ALL PASSED" : "SOME FAILED");
    return allOk ? 0 : 1;
}
