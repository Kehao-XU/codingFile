/**
 * 给定一个经过编码的字符串，返回它解码后的字符串。
编码规则为：k[encoded_string]，表示其中方括号内部的 encoded_string 正好重复 k
次。注意 k 保证为正整数。
你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。
此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 k，例如不会出现像
3a 或 2[4] 的输入。
 */

#include <iostream>
#include <string>
#include <vector>

std::string solution(std::string encoded_string) {
    std::string result;
    std::vector<int> counts;         // repeat counts waiting for their ']'
    std::vector<std::string> prefix; // result before each '['

    int i = 0;
    int n = (int)encoded_string.size();

    while (i < n) {
        char c = encoded_string[i];

        if (c >= '0' && c <= '9') {
            int times = 0;
            while (i < n && encoded_string[i] >= '0' && encoded_string[i] <= '9') {
                times = times * 10 + (encoded_string[i] - '0');
                ++i;
            }
            counts.push_back(times);
        } else if (c == '[') {
            prefix.push_back(result);
            result.clear();
            ++i;
        } else if (c == ']') {
            std::string repeated;
            for (int t = 0; t < counts.back(); ++t)
                repeated += result;
            counts.pop_back();

            result = prefix.back() + repeated;
            prefix.pop_back();
            ++i;
        } else {
            result += c;
            ++i;
        }
    }

    return result;
}

int main() {
    std::string encoded_string;
    if (!(std::cin >> encoded_string))
        return 0;

    std::cout << solution(encoded_string) << '\n';
    return 0;
}
