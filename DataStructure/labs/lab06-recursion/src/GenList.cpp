/**
 * 给定一个无环、不存在自引用的广义表，广义表采用课件定义的存储结构：
广义表类GenList的成员first为表头结点指针；first->tlink指向该表第一层第一个元素结点。
约定：直接放在最外层大表中的原子，权重为 1；每进入一层子表，权重 + 1。
功能：计算广义表加权总和：每个原子的值 ×
自身所在层级权重，把所有结果累加返回总和。
遇到子表结点，需要进入子表内部计算，子表内权重加 1；
同层tlink横向遍历，权重保持不变。
示例 1： 广义表 L = ((1,1),2,(1,1)) 原子(1,1)权重 = 2；原子 2 权重 =
1；原子(1,1)权重 = 2 加权总和 = 1*2+1*2+2*1+1*2+1*2=10 示例 2： 广义表
L=(1,(4,(6))) 总和 = 1*1+4*2+6*3 = 1+8+18 =27
 */

#include "GenList.h"

template <class Type>
int GenList<Type>::weightedsum(GenListNode<Type> *ls, int depth) {
    int sum = 0;
    if (ls == nullptr)
        return 0;
    switch (ls->utype) {
    case (0): {
        sum += weightedsum(ls->tlink, depth);
        break;
    }
    case (1): {
        sum += ls->info.value * depth + weightedsum(ls->tlink, depth);
        break;
    }
    case (2): {
        sum += weightedsum(ls->info.hlink, depth + 1) +
               weightedsum(ls->tlink, depth);
        break;
    }
    default:
        break;
    }
    return sum;
}

template <class Type> int GenList<Type>::weightedsum() {
    return weightedsum(first, 1);
}