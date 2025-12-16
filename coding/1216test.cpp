#define _CRT_SEECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct ListNode {
    int data;
    struct ListNode* input_next;
	struct ListNode* ordered_next;
} ListNode;

struct ListNode* createList(int arr[], int size)
{
    struct ListNode* input_head = NULL;
    struct ListNode* input_tail = NULL;
	struct ListNode* ordered_head = NULL;
    struct ListNode* ordered_tail = NULL;
    
	int temp;
	int *arr2= (int*)malloc(size * sizeof(int));
	memcpy(arr2, arr, size * sizeof(int));  // 修复：参数顺序错误，应该是目标在前，源在后
    for (int i = 0; i < size; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            if(arr2[i] > arr2[j])
            {
                temp = arr2[i];
                arr2[i] = arr2[j];
                arr2[j] = temp;
            }
		}
    }
    for (int i = 0; i < size; i++) {
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->data = arr[i];
        newNode->input_next = NULL;
        if (input_head == NULL) {
            input_head = newNode;
            input_tail = newNode;
        }
        else {
            input_tail->input_next = newNode;
            input_tail = newNode;
        }

    }
    //!!这里为什么又创建了一个新的链表
    for (int i = 0; i < size; i++) {
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->data = arr2[i];
        newNode->input_next = NULL;
        newNode->ordered_next=NULL;
        if (ordered_head == NULL) {
            ordered_head = newNode;
            ordered_tail = newNode;
        }
        else {
            ordered_tail->ordered_next = newNode;
            ordered_tail = newNode;
        }
    }
    
	input_head->ordered_next = ordered_head;
    return input_head;
}

void travelNodeLink1(struct ListNode* head)
{
    struct ListNode* p = head;
    if (head == NULL)
    {
        printf("链表为空。\n");
        return;
    }
    printf("\n数据：\n");
    while (p != NULL)
    {
        printf("%d\n", p->data);
        p = p->input_next;
    }
}

void travelNodeLink2(struct ListNode* head)
{
    struct ListNode* p = head->ordered_next;
    if (head == NULL)
    {
        printf("链表为空。\n");
        return;
    }
    printf("\n数据：\n");
    while (p != NULL)
    {
        printf("%d\n", p->data);
        p = p->ordered_next;
    }
}

void freeList(struct ListNode* head){
    struct ListNode *temp=head->input_next,*HEAD=head->input_next;
    while(HEAD){
        HEAD=HEAD->input_next;
        free(temp);
        temp=HEAD;
    }

    temp=HEAD=head->ordered_next;
    while(HEAD){
        HEAD=HEAD->ordered_next;
        free(temp);
        temp=HEAD;
    }

    free(head);
}

int main()
{
    int arr[] = { 1,3,2,4,5,6 };
    int size = sizeof(arr) / sizeof(arr[0]);
    struct ListNode* head = createList(arr, size);
    printf("原始链表：");
    travelNodeLink1(head);
	printf("排序后链表：");  
	travelNodeLink2(head);
    freeList(head);
    return 0;
}