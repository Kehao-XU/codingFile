#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * 给定一个字符串 s ，找到 它的第一个不重复的字符，并返回它的索引
 * 。如果不存在，则返回 -1 。
 */

int solution(std::string s) {
    int record[256] = {0};
    for (unsigned char c : s) {
        ++record[c];
    }
    for (int i = 0; i < (int)s.size(); ++i) {
        if (record[(unsigned char)s[i]] == 1)
            return i;
    }
    return -1;
}

int main() {
    std::string s = "lovefudan";
    std::cout << solution(s) << '\n';
    return 0;
}