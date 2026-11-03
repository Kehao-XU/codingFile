/***
 * DNA序列?由一系列核苷酸组成，缩写为?'A',?'C',?'G'?和?'T'.。
例如，"ACGAATTCCG"?是一个?DNA序列?。
在研究?DNA?时，识别 DNA 中的重复序列非常有用。
给定一个表示?DNA序列?的字符串?s?，返回所有在 DNA
分子中出现不止一次的?长度为?10?的序列(子字符串)。你可以按?任意顺序?返回答案。
 */

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<std::string> solution(const std::string &s) {
    std::vector<std::string> ans;
    if ((int)s.size() < 10)
        return ans;

    std::unordered_set<std::string> seen;   // 出现过的 10-mer
    std::unordered_set<std::string> added;  // 已加入答案的 10-mer（去重）

    for (int i = 0; i + 10 <= (int)s.size(); ++i) {
        std::string sub = s.substr(i, 10);
        // insert 返回 pair<迭代器, bool>，second == false 表示集合里已存在
        if (!seen.insert(sub).second)          // 之前出现过 -> 是重复序列
            if (added.insert(sub).second)      // 且还没加进答案
                ans.push_back(sub);
    }

    return ans;
}

int main() {
    std::string s;
    std::cout << "Please input your DNA string" << '\n';
    std::cin >> s;

    std::vector<std::string> ans = solution(s);
    std::cout << "count = " << ans.size() << '\n';
    for (const std::string &t : ans)
        std::cout << t << '\n';

    return 0;
}