/**
 * 某系统从多个服务器收集日志，每台服务器的日志均按照时间戳升序排列。同一事件可能被多个服务器重复记录。现在需要合并所有日志，保留每个事件时间戳最新的一条记录。
请使用堆排序，最终结果按照时间戳升序输出。若不同事件时间戳相同，输出顺序不限
示例：
输入：k = 3
S1 = [(2,A),(8,C),(12,F)]
S2 = [(1,B),(5,C),(10,E)]
S3 = [(3,D),(7,A),(11,G)]
输出：
[(1,B),(3,D),(7,A),(8,C),(10,E),(11,G),(12,F)]

 */

#include <cstdio>
#include <map>
#include <utility>
#include <vector>

struct Record {
    int ts;
    char ev;
};

// 大顶堆下沉：按时间戳比较
static void siftDown(std::vector<Record> &a, int i, int n) {
    Record tmp = a[i];
    int j = 2 * i + 1;
    while (j < n) {
        if (j + 1 < n && a[j + 1].ts > a[j].ts)
            ++j;
        if (tmp.ts >= a[j].ts)
            break;
        a[i] = a[j];
        i = j;
        j = 2 * j + 1;
    }
    a[i] = tmp;
}

// 堆排序：按时间戳升序
static void heapSort(std::vector<Record> &a) {
    int n = (int)a.size();
    for (int i = (n - 2) / 2; i >= 0; --i)
        siftDown(a, i, n);
    for (int i = n - 1; i > 0; --i) {
        std::swap(a[0], a[i]);
        siftDown(a, 0, i);
    }
}

// 合并 k 条已按时间戳升序的日志
static std::vector<Record>
mergeLogs(const std::vector<std::vector<Record>> &logs) {
    std::vector<Record> all;
    for (const auto &v : logs)
        all.insert(all.end(), v.begin(), v.end());

    heapSort(all); // ① 先把所有记录按时戳升序

    // ② 同一事件保留时间戳最大的一条，即排序后最后一次出现的位置
    std::map<char, int> lastIdx;
    for (int i = 0; i < (int)all.size(); ++i)
        lastIdx[all[i].ev] = i;

    // ③ 只保留每个事件的最后一次出现（子序列仍然升序）
    std::vector<Record> res;
    for (int i = 0; i < (int)all.size(); ++i)
        if (lastIdx[all[i].ev] == i)
            res.push_back(all[i]);
    return res;
}

int main() {
    std::vector<std::vector<Record>> logs = {
        {{2, 'A'}, {8, 'C'}, {12, 'F'}},
        {{1, 'B'}, {5, 'C'}, {10, 'E'}},
        {{3, 'D'}, {7, 'A'}, {11, 'G'}},
    };

    std::vector<Record> res = mergeLogs(logs);
    std::printf("[");
    for (int i = 0; i < (int)res.size(); ++i)
        std::printf("%s(%d,%c)", i ? "," : "", res[i].ts, res[i].ev);
    std::printf("]\n");
    return 0;
}