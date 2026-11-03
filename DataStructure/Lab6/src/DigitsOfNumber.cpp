/**
 * 电话数字按键映射：2→abc，3→def，4→ghi，5→jkl，6→mno，7→pqrs，8→tuv，9→wxyz。
 *  给定仅由数字2?9组成的字符串digits，返回该数字串可以表示的全部字母组合。
先写出原始递归回溯版本；
将递归程序转换为非递归；必须手动构造栈，模拟递归工作记录（栈帧）；
分析递归版本的时间、递归栈空间复杂度。
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const std::vector<std::string> letters = {"abc", "def",  "ghi", "jkl",
                                          "mno", "pqrs", "tuv", "wxyz"};

static void dfs(const std::string &digits, int i, std::string &path,
                std::vector<std::string> &ans) {
    if (i == (int)digits.size()) {
        ans.push_back(path);
        return;
    }

    for (char c : letters[digits[i] - '2']) {
        path.push_back(c);
        dfs(digits, i + 1, path, ans);
        path.pop_back();
    }
}

std::vector<std::string> solution(const std::string &digits) {
    std::vector<std::string> ans;
    std::string path;
    dfs(digits, 0, path, ans);
    return ans;
}

// ——————————————————— 递归版本复杂度分析 ———————————————————
// 设数字串长度为 n，第 j 个数字对应 m_j 个字母（3 ≤ m_j ≤ 4）。
//
// 时间：叶子结点数 = 结果个数 = Π m_j ≤ 4^n；每到达一个叶子要把长度 n 的
//       path 拷贝进 ans（O(n)），故时间上界为 O(n · 4^n)；
//       由于 m_j 最小为 3，下界为 Ω(n · 3^n)，即介于 O(n·3^n) 与 O(n·4^n)
//       之间，通常记作 O(n · 4^n)。
//       另外非叶子的内部结点总数为 O(4^n)，不影响量级。
//
// 递归工作栈空间：递归深度恰为 n（每层决策一个数字），每层帧只需保存 i 与
//       循环变量等 O(1) 个变量，故递归栈空间为 O(n)；
//       外加 path 的 O(n)。存放结果的 ans 需要 O(n · 4^n)，一般不计入辅助空间。

// ——————————————— 非递归版本：手动构造栈模拟栈帧 ———————————————
// 每个 Frame 精确对应递归中的一个调用 dfs(i)：
//   i —— 递归参数 i，即当前正在决策第 i 个数字；
//   k —— 递归里那个 for 循环的循环变量，本层已经试到第 k 个候选字母；
//   本层“当前选中的字母”保存在 path[i] 中，相当于该栈帧的局部变量。
// 对应关系：
//   st.push_back(...)  == 递归调用 dfs(i + 1)（压入新栈帧）；
//   st.pop_back()      == 该层 return / 本层分支枚举完，返回到上一层；
//   path[i] = c 覆盖本层旧字母 == “撤销选择 + 选择下一个字母”。
struct Frame {
    int i; // 当前决策的数字下标
    int k; // 本层下一个待尝试的字母下标（0 ~ letters[digit].size()）
};

std::vector<std::string> solutionIterative(const std::string &digits) {
    std::vector<std::string> ans;
    const int n = (int)digits.size();
    std::string path((size_t)n, ' '); // path[i] 存第 i 层当前选中的字母

    std::vector<Frame> st; // 手动构造的递归工作栈
    st.push_back({0, 0});  // 相当于调用 dfs(0)

    while (!st.empty()) {
        Frame &frame = st.back();

        if (frame.i == n) {      // 等价于递归终止条件：每位都选好了
            ans.push_back(path); // n 为 0 时这里得到空串，与递归版一致
            st.pop_back();       // 该层返回
            continue;
        }

        const std::string &opts = letters[digits[frame.i] - '2'];
        if (frame.k == (int)opts.size()) { // for 循环走完：本层所有分支都试过
            st.pop_back();                 // 该层返回，回到上一层
            continue;
        }

        const char c = opts[frame.k];
        const int nextI = frame.i + 1;
        ++frame.k;                 // 循环变量前进（for 的 ++k）
        path[(size_t)frame.i] = c; // “选择”：本层选中字母 c
        st.push_back({nextI, 0});  // “递归调用”：压入下一层的栈帧
    }
    return ans;
}

// 测试：逐行读入数字串，比较递归版与非递归版的输出是否一致
int main() {
    std::string digits;
    std::cout << "Please input digits (2-9, empty line to quit)" << '\n';
    while (std::getline(std::cin, digits)) {
        if (digits.empty())
            break;

        std::vector<std::string> rec = solution(digits);
        std::vector<std::string> it = solutionIterative(digits);

        std::cout << digits << " -> " << it.size() << " combinations:";
        for (size_t i = 0; i < it.size(); ++i)
            std::cout << ' ' << it[i];
        std::cout << '\n';

        // 两版输出顺序不保证相同，排序后再比较集合内容
        std::sort(rec.begin(), rec.end());
        std::sort(it.begin(), it.end());
        std::cout << (rec == it ? "[OK] iterative == recursive"
                                : "[FAIL] iterative != recursive")
                  << '\n';
    }
    return 0;
}