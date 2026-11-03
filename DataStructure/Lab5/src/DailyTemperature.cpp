/**
 * 给定一个整数数组 temperatures，表示每天的温度。
 * 返回一个数组 answer，其中 answer[i] 是指对于第 i
 * 天，下一个更高温度出现在几天后。 如果气温在这之后都不会升高，请在该位置用 0
 * 来代替。
 */

#include <iostream>
#include <vector>

std::vector<int> solution(std::vector<int> temperatures) {
    std::vector<int> answer(temperatures.size(), 0);
    std::vector<int> stack;  // indices whose temperatures are decreasing

    for (int i = 0; i < (int)temperatures.size(); ++i) {
        while (!stack.empty() && temperatures[i] > temperatures[stack.back()]) {
            int prev = stack.back();
            stack.pop_back();
            answer[prev] = i - prev;
        }
        stack.push_back(i);
    }

    return answer;
}

int main() {
    int n;
    if (!(std::cin >> n))
        return 0;

    std::vector<int> temperatures(n);
    for (int i = 0; i < n; ++i)
        std::cin >> temperatures[i];

    std::vector<int> answer = solution(temperatures);
    for (int i = 0; i < (int)answer.size(); ++i)
        std::cout << answer[i] << (i + 1 == (int)answer.size() ? '\n' : ' ');

    return 0;
}
