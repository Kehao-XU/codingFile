# NOTES · 数据结构期中复习

> 给下一个会话用。读完这一页就够，不要再去读课件原文和学案全文。
> 最后更新：2026-11-03

## 一句话现状

复旦《数据结构》期中复习，已完成 8 讲学案（对应课件 chapter-1 到 chapter-6-2），每讲配一份可编译代码和真实运行输出。期中考 11-09。

## 产物清单

| 目录 | 学案 | 主题 | 代码 |
|---|---|---|---|
| `MidTerm/Lesson1/` | 学案01_数据结构与算法复杂度.md | 概念、ADT、程序步、大 O | `src/timing.cpp` |
| `MidTerm/Lesson2/` | 学案02_线性表数组与顺序表.md | 地址计算、矩阵压缩、顺序表 AMN | `src/seqlist.cpp` |
| `MidTerm/Lesson3/` | 学案03_链表.md | 单/循环/双向/静态链表 | `src/linkedlist.cpp` |
| `MidTerm/Lesson4/` | 学案04_串与模式匹配.md | BF / KR / KMP next / BM | `src/match.cpp` |
| `MidTerm/Lesson5/` | 学案05_栈和队列.md | 两栈共享、isp/icp、循环队列 | `src/stack_queue.cpp` |
| `MidTerm/Lesson6/` | 学案06_递归和广义表.md | 递归转非递归、n 皇后、广义表 | `src/recursion.cpp` |
| `MidTerm/Lesson7/` | 学案07_树与二叉树.md | 术语、性质、遍历、建树 | `src/binarytree.cpp` |
| `MidTerm/Lesson8/` | 学案08_二叉树续.md | Catalan、线索、堆、Huffman | `src/heap_huffman.cpp` |

每个 lesson 下都是 `学案*.md` + `src/` + `solution/` + `output/` + `log/`：`src/` 是练习骨架（函数体只剩 `// TODO` + 任务描述），`solution/` 是参考答案，`output/` 放 `.exe`（已忽略），`log/` 放实测日志 `.txt`（进库）。

另外三份横向材料：`MidTerm/期中总复习.md`（八讲汇总 + 30 条易错点 + 往年卷链接），`MidTerm/期中冲刺.md`（精简版：速查 20 条 + 15 道填空 + 8 道手算 + 8 道算法默写 + 5 道简答），`MidTerm/viz/`（22 个自包含 HTML 动画，`index.html` 是入口，双击即可看，无外部依赖；左边图形、右边代码行高亮和变量表，可单步）。

改动动画后先跑 `node tools/viz_check.js`：它把全部帧画一遍，检查元素越界、文字重叠、文字压图形。只能抓几何问题，抓不到字形细节。

工具：`tools/extract_pptx.py` 抽课件正文，用法见文末。

课件在 `课件/chapter-*.pptx`，共 8 个，覆盖到期中（树）为止。搜索与索引、图、排序没有课件。

## 已定的约定，改之前先读

1. **学案结构**：概念骨架 → 重点小节（只详写考试密度高的）→ 代价表 → 动手 → 易错点 → 自测 → 一句话带走 → 延伸阅读。不写"很重要""要注意"这类空话，同一件事只说一遍。
2. **符号先定义后使用**：不能出现没定义的变量。例如求和里要写明"m = ⌊log₂n⌋，r = 0..m 表示外层第 r+1 次执行"。
3. **数学记号**：文档内全部 KaTeX。行内 `$...$`，独立 `$$...$$`。禁止 Unicode 上下标（`n₀`、`2ⁿ`）；裸下划线必须进数学并补花括号（`a_ij` → `$a_{ij}$`）；``` 代码块和 `行内代码` 不动；代码标识符、文件路径、链接 URL 里保留 `_`。（这条只管写进文件的内容；对话里一律纯文本，终端不渲染 LaTeX。）
4. **旁注体例**：`> 旁注｜……` 放可跳过的展开，`延伸阅读` 放外链。每讲 1-3 处，不要泛滥。
5. **代码**：C++17，C 风格，不依赖第三方库；`g++ -O2 -std=c++17 -Wall` 必须零警告；输出写进 `output/*.txt`，学案里的数字必须与实测一致。

## 已完成的关键数字（可直接引用，不必重跑）

- 顺序表平均移动：插入 $n/2$、删除 $(n-1)/2$，实测精确吻合
- 最坏输入上 BF 185 次比较、KMP 82 次、BM 37 次；普通文本上 BF 59、KMP 108、BM 16
- Hanoi 移动 $2^n-1$；Fib(30) 递归调用 2,692,537 次；8 皇后 92 解
- 堆排序结果正确；Huffman 权值 {7,5,4,2} 的 WPL = 35（两种算法互相验证）
- Catalan $b_0..b_8$ = 1,1,2,5,14,42,132,429,1430

## 上下文开销：context-report.py

路径：`C:\Users\29606\.config\opencode\scripts\context-report.py`

```powershell
python C:\Users\29606\.config\opencode\scripts\context-report.py
python ... --session ses_xxx     # 指定会话
python ... --all                 # 列出所有会话，按输入量排序
python ... --top 10              # 最大的 10 个工具输出
python ... --dir <path>          # 指定项目目录（默认当前目录）
```

脚本只读 SQLite（`~/.local/share/opencode/opencode.db`），不改数据。统计的是**字符数**，中文约 1 字 ≈ 0.7 token，代码约 1 字 ≈ 0.3 token；本项目的实测换算率约 1.8 字符/token。

**输出只看两处：**

1. `合计` —— 每轮重发的字符数，除以 1.8 约等于当前上下文 token。决定"还装不装得下"。
2. `最大的工具输出` —— 扫一遍，挑出"已经用完、后面不会再回看"的条目，字符数加起来除以 `合计`，就是能卸掉的比例。

其余三块不用看：`累计输入/缓存读/费用` 是钱不是决策；`工具返回明细` 只用于诊断哪类工具吃预算（read 占比高说明该用 grep 而不是整文件读）；`死重（只算可判定的）` 是**下限**，只统计"被同参数覆盖"和"读后又改过"，测不出"用完了但没被覆盖"的部分，别拿它当结论。

基线（2026-11-03，本会话）：每轮重发 39.8 万字符 ≈ 22 万 token，其中约 19 万是 8 个课件的提取文本，属于功能性死重（注：用 `tools/extract_pptx.py` 重抽实测为 14.0 万字符 ≈ 7.8 万 token，口径不同，引用时以实测为准）；可判定死重仅 7.5%。累计 $0.29，缓存读 4800 万 token。

## 待办

- 第 9-11 讲（搜索与索引、图、排序）等课件；拿到后按同样的结构续写 `MidTerm/Lesson9/` 起。
- `MidTerm/` 已提交；后续改动见 git log。
- 若要用新版上下文：把本文件的路径直接丢给新会话，说"读 notes.md，接着做 X"。

## 附：抽取课件文本（`tools/extract_pptx.py`）

```powershell
python tools/extract_pptx.py 课件/*.pptx --brief      # 全部重抽，只回报路径/页数/行数
python tools/extract_pptx.py 课件/chapter-9-full.pptx # 单份，回报带行号的目录
```

正文写进 `material/<stem>.txt`（已 gitignore，可随时重建），stdout 只留目录。要精确引用时按目录给的行号 grep，不要整文件读：

```powershell
grep -n "isp" material/chapter-4-full.txt
```

8 份全部重抽实测：583 页，14.0 万字符，合计约 7.8 万 token。表格按行输出，单元格用 ` | ` 分隔；幻灯片顺序取自 `presentation.xml` 的 `sldIdLst`，不靠文件名。

派给子会话时就一句话：「跑 `tools/extract_pptx.py` 抽某章，把回执发我」。它读多少都不进主上下文。

坑：不要在 PowerShell 里管道接它——`| Select-Object`、`| Out-File` 会按 936 解码，中文变乱码。直接看输出，或让它写文件再用 grep。

课件文件名与学案编号错开一位：chapter-1→L1、2-1→L2、2-2→L3、**chapter-3→L4 串**、**chapter-4→L5 栈和队列**、chapter-5→L6 递归、6-1→L7、6-2→L8。

