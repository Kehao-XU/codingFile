/*定义两个链表，表示两个非负的整数。它们每位数字都是按照?逆序?的方式存储的，
并且每个节点只能存储一位数字。
请你将两个数相加，并以相同形式返回一个表示和的链表。
你可以假设除了数字 0 之外，这两个数都不会以 0?开头。
*/
#include<stdio.h>
#include<malloc.h>
#include"SimpleNode.h"
#include<math.h>
Node* addNumber(Node *num1,Node *num2){
    Node* add=(Node*)malloc(sizeof(Node));
    if(!add){
        printf("Memory allocation failed!\n");
        return NULL;
    }
    add->value=-1;
    add->next=NULL;

    Node *tail=NULL,*current1=num1->next,*current2=num2->next;
    Node *p;
    int upgrade=0;

    while(current1&&current2){
        p=(Node*)malloc(sizeof(Node));
        if(!p){
            printf("Memory allocation failed!\n");
            freeList(add);
            return NULL;
        }

        p->value=current1->value+current2->value+upgrade;
        upgrade=0;
        if(p->value>10)upgrade=1;
        p->value%=10;
        p->next=NULL;

        if(!add->next){
            add->next=tail=p;
        }
        else{
            tail->next=p;
            tail=tail->next;
        }
        current1=current1->next;
        current2=current2->next;
    }

    if(current1==NULL&&current2==NULL&&upgrade){
        p=(Node*)malloc(sizeof(Node));
        if(!p){
            printf("Memory allocation failed!\n");
            freeList(add);
            return NULL;
        }

        p->value=1;
        p->next=NULL;
        tail->next=p;
        tail=tail->next;

        return add;
    }

    while(current1){
        p=(Node*)malloc(sizeof(Node));
        if(!p){
            printf("Memory allocation failed!\n");
            freeList(add);
            return NULL;
        }

        p->value=current1->value+upgrade;
        upgrade=0;
        if(p->value==10){
            p->value=0;
            upgrade=1;
        }
        p->next=NULL;

        tail->next=p;
        tail=tail->next;
        current1=current1->next;
    }

    while(current2){
        p=(Node*)malloc(sizeof(Node));
        if(!p){
            printf("Memory allocation failed!\n");
            freeList(add);
            return NULL;
        }

        p->value=current2->value+upgrade;
        upgrade=0;
        if(p->value==10){
            p->value=0;
            upgrade=1;
        }
        p->next=NULL;

        tail->next=p;
        tail=tail->next;
        current2=current2->next;
    }

    return add;
}

void printNum(Node *num){
    Node *temp=num;
    int number=0,i=0;
    while(num){
        num=num->next;
        number+=temp->value*pow(10,i);
        temp=num;
        i++;
    }
    printf("%d\n",number);
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
    printNum(add->next);

    freeList(num1);
    freeList(num2);
    freeList(add);
    return 0;
}