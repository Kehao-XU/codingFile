# Lab5

## 题目与目标
单调栈、滑动窗口与字符串解码练习，训练线性扫描、双端队列和栈的嵌套处理；统一标准输入输出。

## 题目清单
| 文件 | 考察点 | 核心算法 | 时间 | 空间 |
|---|---|---|---|---|
| DailyTemperature.cpp | 下一个更高温的间隔 | 单调递减栈存下标，遇更高温弹栈结算 | O(n) | O(n) |
| MaxInWindow.cpp | 滑动窗口最大值（基础版） | list 保存窗口值并保持非递增，线性删除/插入 | O(nk) | O(k) |
| MaxInWindow_Optimized.cpp | 滑动窗口最大值（优化版） | deque 存下标，维护单调递减队列 | O(n) | O(k) |
| StringCode.cpp | 解码 k[encoded_string] | 次数栈 + 前缀串栈，迭代处理数字/[/] | O(L·d)，最坏 O(L²)；L 为输出长、d 为嵌套深度 | O(L) |

## 优化版对比
| 维度 | MaxInWindow | Optimized |
|---|---|---|
| 存储 | list 存值 | deque 存下标 |
| 移出窗口 | 线性找同值，O(k) | 队首下标过期即弹，O(1) 摊还 |
| 插入 | 线性找位置，O(k) | 弹掉队尾较小/相等值，O(1) 摊还 |
| 总时间 | O(nk) | O(n) |
| 稳健性 | 依赖“值必在表中”；重复值按值删恰好等价 | 按下标过期，重复最大值处理明确 |

## 实现要点
- DailyTemperature 栈内温度递减；`temperatures[i] > temperatures[stack.back()]` 时结算 `i-prev`，余者保持 0。
- 基础版在 `i>=k` 时删除 `nums[i-k]`，再按非递增插入，窗口满后取 `front()`。
- 优化版先弹 `front() <= i-k`，再弹 `back()` 且值 `<= nums[i]`，最后压入 i。
- StringCode 支持多位次数：`[` 保存前缀并清空结果，`]` 重复后与 `prefix.back()` 拼接。
- 两版窗口函数均对 `k<=0` 或 `k>n` 返回空。

## 观察与问题
- 基础版删除循环无 `end()` 保护，靠不变式保证能找到值；正确但脆弱。其本质是窗口有序副本，非严格单调队列，`k≈n` 退化为 O(n²)。
- StringCode 未校验格式：孤立 `]` 或数字后无 `[` 会对空栈 `.back()`，属未定义行为；次数用 int，极大 k 可能溢出。题目保证输入合法。
- 两版窗口代码重复，可抽公共 I/O；list 的节点开销大于 deque。

---

# Lab6

## 题目与目标
递归/回溯、递归转显式栈、泛型广义表；重点在终止条件、路径恢复、引用计数与复杂度。

## 题目清单
| 文件 | 考察点 | 核心算法 | 时间 | 空间 |
|---|---|---|---|---|
| DigitsOfNumber.cpp | 电话号码字母组合 | 递归 DFS；另用 Frame 栈模拟 | O(n·4^n) 上界 | O(n) 栈 + 输出 |
| Subset.cpp | 无重复数组全部子集 | 递归“不选/选”决策 | O(n·2^n) | O(n) 栈 + 输出 |
| GenList.h | 泛型广义表 | 表头 + 孩子-兄弟链，utype/union；递归操作 | O(N)（Head/First/Next O(1)） | O(N) |
| GenList.cpp | 广义表加权和 | 原子 value×depth，子表 depth+1 | O(N) | O(深度) |
| GenList_test.cpp | weightedsum 验证 | 手工构造 5 个表 | 固定用例 | 固定用例 |

## GenList 模板设计
- 节点用 `utype` 区分 0 表头/1 原子/2 子表，`tlink` 指同层后继，匿名 `union info` 复用 `ref/value/hlink`；`GenList` 仅存表头 `first`，私有递归助手，公有 Head/Tail/First/Next/Copy/Length/Depth/weightedsum/DelValue。
- `weightedsum`：表头沿 `tlink` 同层走；原子加 `value*depth`；子表先以 `depth+1` 递归 `hlink`，再走 `tlink`。入口 depth=1，故示例 10、27、8 正确；Length 空表 0，Depth 空表 1。
- 局限：union 直接放 `Type`，非平凡类型（如 `std::string`）会因 union 特殊成员被删除而无法实例化；加权和要求 `Type` 可与 int 相乘，泛型性有限。

## 测试覆盖
- 5 例：`((1,1),2,(1,1))`=10、`(1,(4,(6)))`=27、`()`=0、`(1,2,3)`=6、`(1,(2,()),3)`=8，覆盖嵌套、深嵌套、空表、纯原子、空子表。
- 仅校验 weightedsum，Length/Depth 只打印；未验证 Copy、Head/Tail、Equal、DelValue、析构/引用计数和非 int 类型。测试用 `#define private public` 并包含 `GenList.cpp`。

## 观察与问题
- 严重：未自定义拷贝构造/赋值，默认浅拷贝 `first`，赋值还丢弃目标旧链；`b=a` 后两对象共享同一链，任一方析构会清空另一方内容并重复递减 ref，取过的节点指针会悬空。`Copy` 覆盖 `first` 不释放旧链，`Tail` 不清理 `lt`，均泄漏。
- `Remove` 递减 ref，ref<=0 时才释放子链，且始终不删除表头（测试注释承认表头泄漏）；`Copy` 不增计数，ref 共享未实现。
- `Equal` 私有且 `int x` 在 utype=0 分支未初始化（UB）；`DelValue` 需表头指针但 `first` 私有，且直接 delete 未调计数。`weightedsum` 的模板定义在 `.cpp`，其他 TU 仅含头会链接失败。
- DigitsOfNumber 对非 2–9 字符索引 letters 会越界（题目已限定）；Subset 遇重复元素会产重复子集（题目已限定）。
- 边界：Digits 空串返回一个空组合，但 main 遇空行直接退出；Subset 空输入返回一个空子集（2^0=1）。
