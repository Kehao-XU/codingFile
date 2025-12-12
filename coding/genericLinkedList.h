#ifndef GENERIC_LINKED_LIST_H
#define GENERIC_LINKED_LIST_H

#include <stdlib.h>

/**
 * @brief 通用链表释放函数
 * 
 * @param head 链表头节点指针
 * @param get_next 函数指针，用于获取节点的下一个节点
 * @param free_data 函数指针，用于释放节点中的数据（可选，NULL表示不释放数据）
 */
void freeList(void* head, void* (*get_next)(void* node), void (*free_data)(void* data)) {
    void* current = head;
    while (current != NULL) {
        void* next = get_next(current);
        if (free_data != NULL) {
            free_data(current);
        }
        free(current);
        current = next;
    }
}

#endif // GENERIC_LINKED_LIST_H
