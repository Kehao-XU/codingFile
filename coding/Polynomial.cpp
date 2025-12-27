/*设计一个程序来处理一元多项式的加法和乘法。每个多项式由若干个项组成，每个项包括：

系数（浮点数）

指数（非负整数）

多项式按指数降序排列在链表中。

要求实现以下功能：

创建多项式：从键盘输入，每行输入一个项的系数和指数（以0 0结束）

多项式加法：将两个多项式相加，返回结果多项式

多项式乘法：将两个多项式相乘，返回结果多项式

输出多项式：格式化输出多项式（如 3.5x^2 + 2.1x - 1.0）

销毁多项式：释放所有内存*/
#include<stdio.h>
#include<malloc.h>
struct Term {
    float coef;      // 系数
    int exp;         // 指数
    struct Term *next;
};

typedef struct Term* Polynomial;

Polynomial create_polynomial(){
    Polynomial Poly=(Term*)malloc(sizeof(Term)),Tail,p;
    if(!Poly){
        printf("Memory allocation failed!\n");
        return NULL;
    }
    Poly->exp=-1;
    Poly->coef=-1;
    Poly->next=NULL;
    Tail=Poly;

    printf("\nPlease enter the polynomial.\ne.g. \"4.5 2\"stands for 4.5x^2\n");

    int exponent;
    float coefficient;
    do{
        scanf("%f",&coefficient);
        scanf("%d",&exponent);

        if((coefficient-0)>1e-6||exponent!=0){
            p=(Term*)malloc(sizeof(Term));
            p->coef=coefficient;
            p->exp=exponent;
            p->next=NULL;
            Tail->next=p;
            Tail=p;
        }
        else break;
    }
    while(true);

    printf("---------------\n");

    return Poly;
}

void destroy_polynomial(Polynomial p){
    Polynomial temp;
    while(p){
        temp=p;
        p=p->next;
        free(temp);
    }
    return;
}

Polynomial add_polynomial(Polynomial p1, Polynomial p2){
    Polynomial SumPoly=(Term*)malloc(sizeof(Term));
    if(!SumPoly){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    SumPoly->coef=-1;
    SumPoly->exp=-1;
    SumPoly->next=NULL;

    Polynomial cur1=p1->next,cur2=p2->next;
    Polynomial tail=SumPoly,p;
    while(cur1&&cur2){
        if(cur1->exp>cur2->exp){
            p=(Term*)malloc(sizeof(Term));
            if(!p){
                printf("Memory allocation failed!\n");
                destroy_polynomial(SumPoly);
                return NULL;
            }

            p->coef=cur1->coef;
            p->exp=cur1->exp;
            p->next=NULL;

            cur1=cur1->next;
            tail->next=p;
            tail=tail->next;

            continue;
        }
        if(cur1->exp<cur2->exp){
            p=(Term*)malloc(sizeof(Term));

            if(!p){
                printf("Memory allocation failed!\n");
                destroy_polynomial(SumPoly);
                return NULL;
            }

            p->coef=cur2->coef;
            p->exp=cur2->exp;
            p->next=NULL;

            cur2=cur2->next;
            tail->next=p;
            tail=tail->next;

            continue;
        }
        if(cur1->exp==cur2->exp){
            p=(Term*)malloc(sizeof(Term));
            if(!p){
                printf("Memory allocation failed!\n");
                destroy_polynomial(SumPoly);
                return NULL;
            }

            p->coef=cur1->coef+cur2->coef;
            p->exp=cur1->exp;
            p->next=NULL;

            cur1=cur1->next;
            cur2=cur2->next;
            tail->next=p;
            tail=tail->next;

            continue;
        }
    }
    
    while(cur1){
        p=(Term*)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            destroy_polynomial(SumPoly);
            return NULL;
        }

        p->coef=cur1->coef;
        p->exp=cur1->exp;
        p->next=NULL;

        tail->next=p;
        tail=tail->next;
        cur1=cur1->next;
    }

    while(cur2){
        p=(Term*)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            destroy_polynomial(SumPoly);
            return NULL;
        }

        p->coef=cur2->coef;
        p->exp=cur2->exp;
        p->next=NULL;

        tail->next=p;
        tail=tail->next;
        cur2=cur2->next;
    }

    return SumPoly;
}

Polynomial findPoly(Polynomial p, int NewTermExp, Polynomial* prev){
    Polynomial temp = p;
    Polynomial cur = p->next;
    
    while(cur){
        if(cur->exp > NewTermExp){
            cur = cur->next;
            temp = temp->next;
            continue;
        }
        if(cur->exp == NewTermExp){
            // 找到匹配的指数，返回该节点
            return cur;
        }
        if(cur->exp < NewTermExp){
            // 找到了插入位置，返回NULL并设置prev
            break;
        }
    }
    
    // 设置prev为要插入位置的前一个节点
    *prev = temp;
    return NULL;
}

Polynomial multiply_polynomial(Polynomial p1, Polynomial p2){
    Polynomial multiPoly=(Polynomial)malloc(sizeof(Term));
    if(!multiPoly){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    multiPoly->coef=-1;
    multiPoly->exp=-1;
    multiPoly->next=NULL;

    Polynomial cur1=p1->next,cur2=p2->next,p;
    if(!cur1||!cur2){
        p=(Polynomial)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            free(multiPoly);
            return NULL;
        }

        p->coef=0;
        p->exp=0;
        p->next=NULL;
        multiPoly->next=p;

        return multiPoly;
    }

    Polynomial target=NULL, prev;
    while(cur1){
        while(cur2){
            target=findPoly(multiPoly,cur1->exp+cur2->exp,&prev);
            if(!target){
                p=(Polynomial)malloc(sizeof(Term));
                if(!p){
                    printf("Memory allocation failed!\n");
                    destroy_polynomial(multiPoly);
                    return NULL;
                }

                p->coef=(cur1->coef)*(cur2->coef);
                p->exp=cur1->exp+cur2->exp;
                p->next=NULL;

                // 插入新节点到prev之后
                Polynomial temp = prev->next;
                prev->next = p;
                p->next = temp;

                cur2=cur2->next;
            }
            else{
                // 找到匹配的指数，直接累加系数
                target->coef+=(cur1->coef)*(cur2->coef);
                cur2=cur2->next;
            }
        }
        cur2=p2->next;
        cur1=cur1->next;
    }

    return multiPoly;
}

void print_polynomial(Polynomial p){
    Polynomial current=p->next;
    if(!current){
        printf("This Polynomial doesnt exist!\n");
        return;
    }

    printf("\n");
    while(current->next){
        printf("%.2fx^%d+",current->coef,current->exp);
        current=current->next;
    }

    if(current->exp==0)printf("%.2f\n",current->coef);
    else printf("%.2fx^%d\n",current->coef,current->exp);

    return;
}

int main(){
    Polynomial Poly1=create_polynomial(),Poly2=create_polynomial();
    print_polynomial(Poly1);
    print_polynomial(Poly2);

    Polynomial SumPoly=add_polynomial(Poly1,Poly2);
    print_polynomial(SumPoly);

    Polynomial multiPoly=multiply_polynomial(Poly1,Poly2);
    print_polynomial(multiPoly);

    destroy_polynomial(Poly1);
    destroy_polynomial(Poly2);
    destroy_polynomial(SumPoly);
    destroy_polynomial(multiPoly);
    return 0;
}