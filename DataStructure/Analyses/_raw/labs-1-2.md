# Lab1

## 题目与目标
练数组/字符串上的线性扫描与哈希表：最长连续递增段、首个不重复字符、两数之和。

## 题目清单
| 文件 | 考察点 | 核心算法 | 时间 | 空间 |
|---|---|---|---|---|
| LongestConsecutiveList.cpp | 最长连续递增子段及其起止 | 游标扫描，r 单调前进 + 剩余长度剪枝 | O(n) | O(1) |
| OnlyChar.cpp | 第一个只出现一次的字符下标 | `record[256]` 直接寻址，两趟扫描 | O(n) | O(1) |
| OnlyChar_Optimized.cpp | 同上，另给单趟版与基准测试 | int[256] 计数；`firstIdx[256]` + queue 惰性筛除；暴力对照 | O(n) | O(1) |
| SumOfTwoNums.cpp | 两数之和返回下标 | `unordered_map` 边查边存（先查后存） | 平均 O(n) | O(n) |

## 实现要点
- LongestConsecutiveList：外层 `while (l < SIZE - maxlen)` 借"剩余长度 ≤ maxlen"提前退出；内层推进 r，段长 `r-l`，严格更大才更新 `maxL/maxR`；`l = r++` 保证 r 不回头。
- OnlyChar：`unsigned char` 做下标避免负值，先统计再找最小 `cnt==1`；空串自然返回 -1。
- OnlyChar_Optimized：`solution` 与原签名一致可直接替换；`solutionSinglePass` 中 `firstIdx` 三态（-2 未出现 / -1 已重复 / 下标），queue 只存首次出现位置，末尾统一弹出失效候选。
- SumOfTwoNums：`need = target - nums[i]`，先 `find` 后 `seen[nums[i]] = i`，确保同一元素不会被用两次；重复值覆盖下标不影响答案。

## 观察与问题
- LongestConsecutiveList：空数组时 `SIZE - maxlen = -1` 不进循环，却仍访问 `nums[maxR-1]`（即 `nums[0]`），越界 UB；`int SIZE = nums.size()` 有窄化风险。
- OnlyChar：逻辑正确；`<queue>/<unordered_map>/<vector>` 未使用。两文件中文字节为 GBK，按 UTF-8 读是乱码（不影响编译）。
- OnlyChar_Optimized 基准不公平：`bench(solution, ...)` 传给按值收 `std::string` 的版本，每次迭代复制 1e6 字符；单趟版用 `const&`，注释里 6.40ms vs 0.50ms 的 12.8 倍含复制成本。注释称重复字符会"出队"，实际是末尾统一 pop，队列最多 256 项。
- SumOfTwoNums：`main` 直接取 `ans[0]/ans[1]`，无解时返回空 vector 会越界；形参按值传 vector 多一次拷贝，宜改 `const&`。

---

# Lab2

## 题目与目标
练有序数组上的归并、快慢指针与双端双指针：两等长有序数组的中位数、原地删值、有序数组平方。

## 题目清单
| 文件 | 考察点 | 核心算法 | 时间 | 空间 |
|---|---|---|---|---|
| MidOfConsecutiveArray.cpp | 两等长升序数组按"第 ⌈L/2⌉ 位"定义的中位数 | 归并计数到第 n 个；另附等长窗口二分 | O(n) / 二分 O(log n) | O(1) |
| RemoveValueInArray.cpp | 原地删除等于 val 的元素并返回新长度 | 读写双指针（快慢指针），保持相对顺序 | O(n) | O(1) |
| SquareOfConsecutiveArray.cpp | 非递减数组逐元素平方后仍非递减 | 左右双指针取较大平方，从后往前填 | O(n) | O(n)（输出数组） |

## 实现要点
- 中位数取题面"第 ⌈L/2⌉ 个位置"：S1 长 5 取第 3 个 15，合并后长 10 取第 5 个即 11；代码 `cnt < SIZE`（SIZE = nums1.size()）返回第 n 小，与样例 11 吻合。
- `solutionBinarySearch` 两窗口等长收缩：偶数窗口丢 mid，奇数窗口保留 mid（`(hi1-lo1)%2` 判奇偶）；mid 值相等即返回，最后取两窗口较小者。
- RemoveValue：`j` 读、`i` 写，仅非 val 才写入并 `++count`；被删位置由后续元素覆盖，稳定保序。
- Square：`l/r` 两端向中间比较 `*l**l` 与 `*r**r`，大者填 `ans[i--]`，相等走左分支；空数组先返回 `{}`。

## 观察与问题
- MidOfConsecutiveArray：两版都默认两数组等长且非空，无断言；不等长时"第 n 小"语义失效，二分版窗口失配后可能原地循环。空数组时 `hi=-1`，二分版会读 `nums1[0]` 越界。`cnt < SIZE` 为 int/size_t 混比，有警告。
- 二分版在等长前提下窗口每次同量收缩、奇偶分支与"相等即返回"自洽，样例与手算用例均正确。
- RemoveValue：实现正确；`count` 与 `i-nums.begin()` 等价，属冗余。
- Square：`int` 平方有溢出风险（|nums[i]| > 46340 时），题面只约束长度未约束值域；`<stack>` 未使用。
- 以上两文件注释同为 GBK 编码，UTF-8 下显示乱码。
