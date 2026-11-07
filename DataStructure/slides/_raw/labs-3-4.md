# Lab3

## 题目与目标
围绕单链表的三道经典题：反转链表、检测环并返回入环点、判断回文链表。目标是练习指针操作、Floyd 快慢指针、原地修改链表并在判定后恢复，追求 O(1) 额外空间。

## 题目清单
| 文件 | 考察点 | 核心算法 | 时间 | 空间 |
|---|---|---|---|---|
| 1_ReverseList.cpp | 原地反转、指针改写 | 迭代三指针 prev/current/next | O(n) | O(1) |
| 2_ListLoopDetect.cpp | 判环、求入环点 | Floyd 快慢指针，相遇后从头同步走 | O(n) | O(1) |
| 3_PalindromeListDetect.cpp | 回文判定、链表复原 | 快慢指针找中点 + 反转后半 + 逐值比较 + 再反转复原 | O(n) | O(1) |

## 实现要点
- 反转：每轮先保存 current->next，再改指向 prev，最后两指针前移；返回 prev。空表、单节点由 while 自然覆盖；测试后 freeList 释放。
- 判环：slow 走 1 步、fast 走 2 步，遇空即无环；相遇后 p 回到 head，与 slow 同速前进，交点即入环点。用例覆盖 pos=1、0、-1 和空表。
- 回文：偶数长度（fast==nullptr）从 slow 起反转后半，奇数长度（fast!=nullptr）跳过中间节点；比较完再将后半反转，用 prevSlow->next 接回。代码先令 slow=prev 再复原，逻辑正确；7 组用例（含 0/1/2 节点）全部通过。

## 观察与问题
- 2_ListLoopDetect 的测试未调用 freeList，链表全部泄漏；makeCycle 未校验 pos 越界，pos≥长度会空指针崩溃（现有用例未触发）。成环链表本身也无法直接逐个 delete。
- 3 的 freeList 检测到环就直接 return，属防御性设计；若复原失败会泄漏，但实测未发生。比较循环中 p2->next==nullptr 的提前 break 是冗余判断。
- Lab3 三个文件的中文注释为 GBK 编码，按 UTF-8 读取呈乱码，影响可读性。
- 1 无功能问题；2、3 的算法与边界处理均正确。

---

# Lab4

## 题目与目标
三道字符串题：找出 DNA 中重复出现的长度 10 子串、找第一个不重复字符的下标、判断字符串能否由某个子串重复多次构成。重点比较哈希表/直接寻址、枚举周期/拼接查找的复杂度差异。

## 题目清单
| 文件 | 考察点 | 核心算法 | 时间 | 空间 |
|---|---|---|---|---|
| DNARepeat.cpp | 固定长度重复子串、去重 | 滑窗 substr(10) + 两个 unordered_set | O(n·L)=O(n)，L=10 | O(n·L)≈O(n) |
| FirstCharInString.cpp | 首不重复字符 | std::map 记录首次下标，重复置 -1，两遍扫描 | O(n log σ)，σ≤256 时实为 O(n) | O(σ) |
| FirstCharInString_Optimized.cpp | 同上，数组直接寻址 | count[256] 计数 + 两遍扫描 | O(n) | O(1)（256 int） |
| LoopString.cpp | 重复子串构成 | 枚举周期 i（n%i==0 且 s[i]==s[0]），按 s[j%i] 校验 | O(n·τ(n))，上界 O(n√n) | O(1) |
| LoopString_Optimized.cpp | 同上，拼接法 | (s+s).find(s,1)!=n | 依 find 实现，平均近线性、最坏 O(n²) | O(n) |

## 优化版对比
| 题目 | 普通版 | 优化版 | 复杂度变化 |
|---|---|---|---|
| 首个不重复字符 | std::map，树查找+结点分配 | 256 桶计数数组，直接下标 | O(n log σ)→O(n)，空间 O(σ)→O(1)，常数显著下降 |
| 重复子串 | 枚举周期并逐位验证 | s+s 后从下标 1 查找 s | 最坏 O(n√n)→平均近线性；代价是 O(n) 空间，最坏仍可能 O(n²) |
| DNA 重复串 | 无优化版 | — | — |

## 实现要点
- DNA：长度 <10 直接返回空；窗口取 10 字符，seen.insert 失败说明出现过，再由 added 去重后入答案，因此按第二次出现顺序输出且无重复。
- 首字符普通版：首次记录下标，重复改为 -1；第二遍返回首个 record[c]!=-1 的位置，无则 -1。实测 "abc"→0、"aabb"→-1，正确。
- 优化版：unsigned char 作下标避免负值；统计后第二遍找 count==1；自带 bruteForce 对拍，7 组用例全过。
- 循环串普通版：n==0 返回 false，n==1 返回 true；只检查能整除 n 且首字符相同的周期。优化版 n<2 返回 false。

## 观察与问题
- 【严重】LoopString_Optimized.cpp 无法编译：main 使用未声明的 a、want（第 20 行），g++ 已复现；文件也没有注释所称的 KMP 实现。solutionConcat 本身在 n≥2 时判定正确。
- LoopString.cpp 对 n==1 返回 true，与“子串重复多次”（至少两次、真子串）的题意不符，单字符应判 false，属语义 bug；空串返回 false 合理。
- FirstCharInString_Optimized.cpp 末尾注释称原版用 record[c]!=0 判断、对 "abc" 返回 1，但原版实际用 find() 判断，源码并无该 bug，注释与事实不符。优化仍成立。
- DNARepeat 的两个哈希集合可合并为计数表；substr 每窗复制，可换 string_view/滚动哈希。
- 三个程序都用 cin>>s，无法读含空格输入；优化版把测试 main 留在源文件，提交时宜拆分。
