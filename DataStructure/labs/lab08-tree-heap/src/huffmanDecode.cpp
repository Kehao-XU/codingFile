/**
 * 某文件压缩系统使用哈夫曼编码压缩文本，接收端获得字符编码表和分块传输的二进制数据。由于分块传输，一个字符的编码可能跨越多个数据块。
请设计一个基于哈夫曼编码树的译码程序，恢复原始文本。
示例：
输入：
codes = {
    A: 0, B: 10, C: 110, D: 111
}
chunks = ["101", "1011", "10"]
输出：
BCDA
异常处理：
编码表存在前缀冲突：输出 INVALID_TABLE。
数据无法沿译码树继续：输出 INVALID_STREAM。
数据结束时仍未完成一个字符的译码：输出 INCOMPLETE。

 */

#include <cstdio>
#include <map>
#include <string>
#include <vector>

// ---------- 译码树（0-1 前缀树） ----------
struct TrieNode {
    int next[2] = {-1, -1}; // 左右子结点下标
    char ch = '\0';         // 非 0 表示某条编码在此结束
};

// 把一条编码插入译码树；返回 false 表示违反前缀码规则
static bool insertCode(std::vector<TrieNode> &trie, const std::string &code,
                       char ch) {
    if (code.empty())
        return false;
    int cur = 0;
    for (char c : code) {
        if (c != '0' && c != '1')
            return false;
        if (trie[cur].ch != '\0')
            return false; // 已有一个更短的编码是当前编码的前缀
        int b = c - '0';
        if (trie[cur].next[b] == -1) {
            trie[cur].next[b] = (int)trie.size();
            trie.push_back(TrieNode());
        }
        cur = trie[cur].next[b];
    }
    if (trie[cur].ch != '\0')
        return false; // 同一编码出现两次
    if (trie[cur].next[0] != -1 || trie[cur].next[1] != -1)
        return false; // 当前编码是别人的前缀
    trie[cur].ch = ch;
    return true;
}

// 返回译码结果，或三种异常串之一
static std::string decode(const std::map<char, std::string> &codes,
                          const std::vector<std::string> &chunks) {
    std::vector<TrieNode> trie(1);
    for (const auto &kv : codes)
        if (!insertCode(trie, kv.second, kv.first))
            return "INVALID_TABLE";

    std::string out;
    int cur = 0; // 0 是根，也是每个字符的起点
    for (const std::string &chunk : chunks) {
        for (char c : chunk) {
            if (c != '0' && c != '1')
                return "INVALID_STREAM";
            cur = trie[cur].next[c - '0'];
            if (cur == -1)
                return "INVALID_STREAM"; // 走不下去了
            if (trie[cur].ch != '\0') {
                out.push_back(trie[cur].ch);
                cur = 0; // 回到根，开始下一个字符
            }
        }
    }
    return cur == 0 ? out : "INCOMPLETE"; // 停在半路说明不完整
}

int main() {
    std::map<char, std::string> codes = {
        {'A', "0"}, {'B', "10"}, {'C', "110"}, {'D', "111"}};
    std::vector<std::string> chunks = {"101", "1011", "10"};
    std::printf("%s\n", decode(codes, chunks).c_str());

    // 附加用例
    std::map<char, std::string> bad = {{'A', "0"}, {'B', "01"}};
    std::printf("%s\n",
                decode(bad, {"01"}).c_str()); // INVALID_TABLE：0 是 01 的前缀

    std::map<char, std::string> partial = {{'A', "0"}, {'B', "10"}};
    std::printf(
        "%s\n",
        decode(partial, {"11"}).c_str()); // INVALID_STREAM：没有 11 分支
    std::printf("%s\n",
                decode(codes, {"1101"}).c_str()); // INCOMPLETE：剩下 "1" 没译完
    std::printf("%s\n", decode(codes, {"110"}).c_str()); // C
    return 0;
}