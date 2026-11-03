// ======================================================================
// 本地测试文件（仅用于自测，不属于作业提交内容，交作业时不要提交本文件）
//
// 目的：手动搭出广义表，调用 GenList<int>::weightedsum() 验证结果。
//
// 说明：构造结点需要访问 GenListNode 的私有成员（utype / info / tlink），
//       这里用 #define private public 在“本测试文件”内临时放开访问限制，
//       不会影响 GenList.h / GenList.cpp 本身，也不需要改动课件代码。
//
// 编译：g++ -std=c++17 -Wall -Wextra -o ..\output\GenList_test.exe
// GenList_test.cpp
// ======================================================================
#include <cassert>
#include <cstddef>
#include <iostream>

#define private public
#include "GenList.h"
#undef private

#include "GenList.cpp" // 模板成员函数的定义在 .cpp 里，一并包含才能实例化

using Node = GenListNode<int>;

// 原子结点：utype = 1，值放在 info.value
static Node *makeAtom(int value, Node *next = nullptr) {
    Node *node = new Node; // 默认构造：utype = 0, tlink = NULL, info.ref = 0
    node->utype = 1;
    node->info.value = value;
    node->tlink = next;
    return node;
}

// 子表结点：utype = 2，info.hlink 指向子表的表头结点
static Node *makeSublist(Node *subHead, Node *next = nullptr) {
    Node *node = new Node;
    node->utype = 2;
    node->info.hlink = subHead;
    node->tlink = next;
    return node;
}

// 表头结点：utype = 0，tlink 指向本层第一个元素
static Node *makeHead(Node *firstElem = nullptr) {
    Node *node = new Node;
    node->utype = 0;
    node->info.ref = 0; // 与课件约定一致：引用计数从 0 开始
    node->tlink = firstElem;
    return node;
}

// 让 list 接管 listHead 这棵树，调用 weightedsum 并与期望值比较
static bool check(const char *name, Node *listHead, int expect) {
    GenList<int> list;
    delete list.first; // 丢掉构造函数里建的空表头
    list.first = listHead;

    int got = list.weightedsum();
    bool ok = (got == expect);
    std::cout << name << ": weightedsum = " << got << " (expect " << expect
              << ") " << (ok ? "[OK]" : "[FAIL]")
              << ", length = " << list.Length() << ", depth = " << list.Depth()
              << '\n';
    // 析构时 ~GenList() -> Remove(first) 会释放 first->tlink 链上的所有结点；
    // 课件中的 Remove 不删除表头结点本身，测试程序里忽略这一个结点的泄漏。
    return ok;
}

int main() {
    bool ok = true;

    // ((1,1),2,(1,1))：两个子表里的 4 个 1 权重为 2，原子 2 权重为 1
    ok &= check(
        "((1,1),2,(1,1))",
        makeHead(makeSublist(
            makeHead(makeAtom(1, makeAtom(1))),
            makeAtom(2, makeSublist(makeHead(makeAtom(1, makeAtom(1))))))),
        10);

    // (1,(4,(6)))：1*1 + 4*2 + 6*3 = 27
    ok &= check(
        "(1,(4,(6)))",
        makeHead(makeAtom(1, makeSublist(makeHead(makeAtom(
                                 4, makeSublist(makeHead(makeAtom(6)))))))),
        27);

    // 空表
    ok &= check("()", makeHead(), 0);

    // 纯原子表
    ok &= check("(1,2,3)", makeHead(makeAtom(1, makeAtom(2, makeAtom(3)))), 6);

    // 含空子表：(1,(2,()),3) = 1*1 + 2*2 + 3*1 = 8
    ok &=
        check("(1,(2,()),3)",
              makeHead(makeAtom(
                  1, makeSublist(makeHead(makeAtom(2, makeSublist(makeHead()))),
                                 makeAtom(3)))),
              8);

    std::cout << (ok ? "all tests passed" : "some tests failed") << '\n';
    return ok ? 0 : 1;
}
