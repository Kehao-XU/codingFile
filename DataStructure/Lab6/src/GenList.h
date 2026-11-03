#pragma once
#include <cassert>
#include <cstddef>

template <class Type> class GenList; // 前置声明，供下面的 friend 使用

template <class Type> class GenListNode { // 广义表结点类定义
    friend class GenList<Type>;

  private:
    int utype;                    // =0/1/2
    GenListNode<Type> *tlink;     // 指向同一层下一结点的指针
    union {                       // 联合
        int ref;                  // utype=0，存放引用计数
        Type value;               // utype=1，存放数值
        GenListNode<Type> *hlink; // utype=2，存放指向子表的指针
    } info;

  public:
    GenListNode() : utype(0), tlink(NULL) { info.ref = 0; }
    GenListNode(GenListNode<Type> &RL) {
        utype = RL.utype;
        tlink = RL.tlink;
        info = RL.info;
    }
};

template <class Type> class GenList { // 广义表类定义
  private:
    GenListNode<Type> *first; // 广义表头指针
    // 复制一个ls指示的无共享非递归表
    GenListNode<Type> *Copy(GenListNode<Type> *ls);
    // 计算由ls指示的非递归表的深度
    int Depth(GenListNode<Type> *ls);
    // 计算由ls指示的非递归表的长度
    int Length(GenListNode<Type> *ls);
    // 比较以s和t为表头的两个表是否相等
    bool Equal(GenListNode<Type> *s, GenListNode<Type> *t);
    // 释放以ls为表头结点的广义表
    void Remove(GenListNode<Type> *ls);
    // 计算有ls指示的广义表的总权重
    int weightedsum(GenListNode<Type> *ls, int depth);

  public:
    // 构造函数
    GenList() {
        first = new GenListNode<Type>;
        assert(first != NULL);
    }
    ~GenList();                      // 析构函数
    bool Head(GenListNode<Type> &x); // 返回表头元素
    bool Tail(GenList<Type> &lt);    // 返回表尾
    GenListNode<Type> *First();      // 返回第一个元素
    // 返回表元素elem的直接后继元素
    GenListNode<Type> *Next(GenListNode<Type> *elem);
    void Copy(const GenList<Type> &R);            // 复制广义表
    int Length();                                 // 计算广义表的长度
    int Depth();                                  // 计算广义表的深度
    int weightedsum();                            // 计算广义表的总权重
    void DelValue(GenListNode<Type> *ls, Type x); // 删除值为x的结点
};

template <class Type> bool GenList<Type>::Head(GenListNode<Type> &x) {
    if (first->tlink == NULL)
        return false;
    else { // 非空表
        x.utype = first->tlink->utype;
        x.info = first->tlink->info;
        return true;
    }
}
template <class Type> bool GenList<Type>::Tail(GenList<Type> &lt) {
    if (first->tlink == NULL)
        return false;
    else { // 非空表
        lt.first->utype = 0;
        lt.first->info.ref = 0;
        lt.first->tlink = Copy(first->tlink->tlink);
        return true;
    }
}

template <class Type> GenListNode<Type> *GenList<Type>::First() {
    if (first->tlink == NULL)
        return NULL;
    else
        return first->tlink;
}

// 返回表元素elem的直接后继元素
template <class Type>
GenListNode<Type> *GenList<Type>::Next(GenListNode<Type> *elem) {
    if (elem->tlink == NULL)
        return NULL;
    else
        return elem->tlink;
}

template <class Type>
void GenList<Type>::Copy(const GenList<Type> &R) { // 公有函数
    first = Copy(R.first);
}
template <class Type>
GenListNode<Type> *GenList<Type>::Copy(GenListNode<Type> *ls) { // 私有函数
    GenListNode<Type> *q = NULL;
    if (ls != NULL) {
        q = new GenListNode<Type>;
        q->utype = ls->utype;
        switch (ls->utype) {
        case 0:
            q->info.ref = ls->info.ref;
            break;
        case 1:
            q->info.value = ls->info.value;
            break;
        case 2:
            q->info.hlink = Copy(ls->info.hlink);
            break;
        }
        q->tlink = Copy(ls->tlink);
    }
    return q;
}

template <class Type> int GenList<Type>::Length() { // 公有函数
    return Length(first->tlink);
}
template <class Type>
int GenList<Type>::Length(GenListNode<Type> *ls) { // 私有函数
    if (ls != NULL)
        return 1 + Length(ls->tlink);
    else
        return 0;
}

template <class Type> int GenList<Type>::Depth() { return Depth(first); }
template <class Type> int GenList<Type>::Depth(GenListNode<Type> *ls) {
    if (ls->tlink == NULL)
        return 1; // 空表
    GenListNode<Type> *temp = ls->tlink;
    int m = 0, n;
    while (temp != NULL) {      // 在表顶层横扫
        if (temp->utype == 2) { // 结点为子表
            n = Depth(temp->info.hlink);
            if (m < n)
                m = n; // m为最大深度
        }
        temp = temp->tlink;
    }
    return m + 1;
}

template <class Type>
void GenList<Type>::DelValue(GenListNode<Type> *ls, Type x) {
    if (ls->tlink != NULL) { // 非空表
        GenListNode<Type> *p = ls->tlink;
        while (p != NULL && (p->utype == 1 && p->info.value == x)) {
            ls->tlink = p->tlink;
            delete p;      // 删除
            p = ls->tlink; // 指向同一层后继结点
        }
        if (p != NULL) {
            if (p->utype == 2) // 在子表中递归删除
                DelValue(p->info.hlink, x);
            DelValue(p, x); // 在以p为表头的链表中递归删除
        }
    }
}

template <class Type> GenList<Type>::~GenList() { Remove(first); }
template <class Type> void GenList<Type>::Remove(GenListNode<Type> *ls) {
    ls->info.ref--;          // 引用计数减一
    if (ls->info.ref <= 0) { // 引用计数减至0才能删除
        GenListNode<Type> *q;
        while (ls->tlink != NULL) { // 横扫链表
            q = ls->tlink;
            if (q->utype == 2) { // 递归删除子表
                Remove(q->info.hlink);
                if (q->info.hlink->info.ref <= 0)
                    delete q->info.hlink; // 删除子表附加头结点
            }
            ls->tlink = q->tlink;
            delete q;
        }
    }
}

template <class Type>
bool GenList<Type>::Equal(GenListNode<Type> *s, GenListNode<Type> *t) {
    int x;
    if (s->tlink == NULL && t->tlink == NULL)
        return true; // 表s与表t都是空表或者所有结点都比较完
    if (s->tlink != NULL && t->tlink != NULL &&
        s->tlink->utype == t->tlink->utype) { // 两表都非空且结点标志相同
        if (s->tlink->utype == 1)             // 原子结点，比较对应数据
            x = s->tlink->info.value == t->tlink->info.value ? 1 : 0;
        else if (s->tlink->utype == 2) // 子表结点，递归比较子表
            x = Equal(s->tlink->info.hlink, t->tlink->info.hlink);
        if (x == 1)
            return Equal(s->tlink, t->tlink); // 递归比较同一层下一结点
    }
    return false;
}
