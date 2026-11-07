/***
 * 给定一个非空的字符串 s ，检查是否可以通过由它的一个子串重复多次构成。
 */

#include <iostream>
#include <string>

bool solution(const std::string &s) {
    int SIZE = (int)s.size();

    if (SIZE == 0)
        return false;
    if (SIZE == 1)
        return true;

    for (int i = 1; i <= SIZE / 2; ++i) {
        if (s[i] == s[0]) {
            if (SIZE % i == 0) {
                int j = i;
                while (j < SIZE) {
                    if (s[j] != s[j % i])
                        break;
                    ++j;
                }
                if (j == SIZE)
                    return true;
            }
        }
    }

    return false;
}

int main() {
    std::string s;
    std::cout << "Please input your string" << '\n';
    std::cin >> s;
    std::cout << (solution(s) ? "YES" : "NO") << '\n';
    return 0;
}