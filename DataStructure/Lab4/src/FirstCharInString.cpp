/***
 * 给定一个字符串 s ，找到 它的第一个不重复的字符，并返回它的索引 。如果不存在，则返回 -1 。
 */

#include <iostream>
#include <map>
#include <string>
#include <vector>

int solution(const std::string &s) {
    std::map<char, int> record;
    for (int i = 0; i < (int)s.size(); ++i) {
        char c = s[i];
        if (record.find(c) != record.end())
            record[c] = -1;
        else
            record[c] = i;
    }

    for (int i = 0; i < (int)s.size(); ++i) {
        char c = s[i];
        if (record[c] != -1)
            return i;
    }

    return -1;
}

int main() {
    std::string s;
    std::cout << "Please input your string" << '\n';
    std::cin >> s;
    std::cout << solution(s) << '\n';
    return 0;
}