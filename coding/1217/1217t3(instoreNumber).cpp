/*定义两个链表，表示两个非负的整数。它们每位数字都是按照?逆序?的方式存储的，
并且每个节点只能存储一位数字。
请你将两个数相加，并以相同形式返回一个表示和的链表。
你可以假设除了数字 0 之外，这两个数都不会以 0?开头。
*/
#include<stdio.h>
#include<malloc.h>
#include"SimpleNode.h"
Node* addNumber(Node *num1,Node *num2){
    dummy(0);
}

int main(){
    int n1,n2;
    printf("Enter the length of your two numbers.\n");
    scanf("%d%d",&n1,&n2);

    Node *num1=createList(n1);
    Node *num2=createList(n2);

    printf("Your two numbers are:\n");
    printNum(num1->next);
    printNum(num2->next);

    printf("Adding your two numbers....\nIt's ");
    Node *add=addNumber(num1,num2);
    printNum(add);

    freeList(num1);
    freeList(num2);
    freeList(add);
    return 0;
}