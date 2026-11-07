#include<stdio.h>
#include<stdlib.h>
struct Node{
    int val;
    Node *left;
    Node *right;
};

Node* createNode(int val){
    Node *p=(Node*)malloc(sizeof(Node));
    p->val=val;
    p->left=NULL;
    p->right=NULL;

    return p;
}

void deleteTree(Node *root){
    Node *temp=root,*cur=root,*parent=NULL,*parcur=NULL;
    while(cur||parent){
        if(cur->left&&cur->right){
            if(!parent){
                parent=parcur=cur;
                cur=cur->left;
                parent->left=NULL;
            }
            else{
                parcur->left=cur;
                parcur=cur;
                cur=cur->left;
                parcur->left=NULL;
            }
            continue;
        }
        if(cur->left){
            temp=cur;
            cur=cur->left;
            free(temp);
            temp=NULL;
            continue;
        }
        if(cur->right){
            temp=cur;
            cur=cur->right;
            free(temp);
            temp=NULL;
            continue;
        }
        if(cur){
            free(cur);
            cur=NULL;
        }
        if(parent){
            cur=parent;
            parent=NULL;
            parcur=NULL;
        }
    }
}

void deleteTreeRecurve(Node *root){
    if(root == NULL){
        return;
    }
    // 先释放左子树
    deleteTreeRecurve(root->left);
    // 再释放右子树
    deleteTreeRecurve(root->right);
    // 最后释放当前节点
    free(root);
}

void deleteTreeIterative(Node *root){
    if(root == NULL){
        return;
    }
    
    // 使用栈来模拟后序遍历
    Node *stack[100];
    int top = -1;
    Node *cur = root;
    Node *prev = NULL;
    
    do{
        // 先将当前节点及其所有左子节点入栈
        while(cur != NULL){
            stack[++top] = cur;
            cur = cur->left;
        }
        
        while(cur == NULL && top >= 0){
            cur = stack[top];
            
            // 如果右子节点为空或已经被访问过
            if(cur->right == NULL || cur->right == prev){
                // 释放当前节点
                free(cur);
                top--;
                prev = cur;
                cur = NULL;
            } else {
                // 处理右子树
                cur = cur->right;
            }
        }
    } while(top >= 0);
}

void preorderTraversal(Node* root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->val);
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

int main(){
    Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    
    printf("前序遍历结果：");
    preorderTraversal(root);
    printf("\n");

    deleteTree(root);

    return 0;
}