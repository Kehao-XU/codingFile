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

Polynomial subtract_polynomial(Polynomial p1, Polynomial p2){
    Polynomial SubPoly=(Term*)malloc(sizeof(Term));
    if(!SubPoly){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    SubPoly->coef=-1;
    SubPoly->exp=-1;
    SubPoly->next=NULL;

    Polynomial cur1=p1->next,cur2=p2->next;
    Polynomial tail=SubPoly,p;
    while(cur1&&cur2){
        if(cur1->exp>cur2->exp){
            p=(Term*)malloc(sizeof(Term));
            if(!p){
                printf("Memory allocation failed!\n");
                destroy_polynomial(SubPoly);
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
                destroy_polynomial(SubPoly);
                return NULL;
            }

            // 减法操作：p2的项取相反数
            p->coef=-cur2->coef;
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
                destroy_polynomial(SubPoly);
                return NULL;
            }

            // 减法操作：p1的系数减去p2的系数
            p->coef=cur1->coef-cur2->coef;
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
            destroy_polynomial(SubPoly);
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
            destroy_polynomial(SubPoly);
            return NULL;
        }

        p->coef=-cur2->coef;
        p->exp=cur2->exp;
        p->next=NULL;

        tail->next=p;
        tail=tail->next;
        cur2=cur2->next;
    }

    return SubPoly;
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

Polynomial SimplyMultiply(Polynomial p1, Polynomial p2){
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

void oxidize_polynomial(Polynomial p,int n){    
    Polynomial cur=p->next,temp;
    if(!cur){
        // 如果多项式为空，直接添加一个指数为n的零项
        temp=(Polynomial)malloc(sizeof(Term));
        if(!temp){
            printf("Memory allocation failed!\n");
            return;
        }
        temp->coef=0;
        temp->exp=n;
        temp->next=NULL;
        p->next=temp;
        cur=temp;
    }

    if(cur->exp<n){
        temp=(Polynomial)malloc(sizeof(Term));
        if(!temp){
            printf("Memory allocation failed!\n");
            return;
        }

        temp->coef=0;
        temp->exp=n;
        temp->next=cur;
        p->next=temp;
        p=p->next;

        while(p->exp-cur->exp>1){
            temp=(Polynomial)malloc(sizeof(Term));
            if(!temp){
                printf("Memory allocation failed!\n");
                return;
            }

            temp->coef=0;
            temp->exp=p->exp-1;
            temp->next=cur;
            p->next=temp;
            p=p->next;
        }
    }

    while(cur->next){
        if(cur->exp-cur->next->exp==1)continue;
        else{
            temp=(Polynomial)malloc(sizeof(Term));
            if(!temp){
                printf("Memory allocation failed!\n");
                return;
            }

            temp->coef=0;
            temp->exp=cur->exp-1;
            temp->next=cur->next;
            cur->next=temp;
            cur=temp;
        }
    }
    
    if(cur->exp==0)return;
    else{
        while(cur->exp){
            temp=(Polynomial)malloc(sizeof(Term));
            if(!temp){
                printf("Memory allocation failed!\n");
                return;
            }

            temp->coef=0;
            temp->exp=cur->exp-1;
            temp->next=NULL;
            cur->next=temp;
            cur=temp;
        }
    }

    return;
}

void reduce_polynomial(Polynomial p){
    Polynomial cur=p->next,prev=p;
    while(cur){
        // 移除系数为0的项（考虑浮点数精度问题）
        if(cur->coef < 1e-6 && cur->coef > -1e-6){
            prev->next=cur->next;
            free(cur);
            cur=prev->next;
        }
        else{
            cur=cur->next;
            prev=prev->next;
        }
    }
}

int prepareKaratsuba(Polynomial p1,Polynomial p2){
    Polynomial cur1=p1->next,cur2=p2->next;
    int length1=0,length2=0;
    
    // 计算两个多项式的长度
    while(cur1){
        cur1=cur1->next;
        length1++;
    }
    while(cur2){
        cur2=cur2->next;
        length2++;
    }
    
    int n=(length1>length2?length1:length2);
    
    // 找到大于等于n的最小2的幂
    int powerOfTwo = 1;
    while(powerOfTwo < n) {
        powerOfTwo *= 2;
    }
    n = powerOfTwo;
    
    // 补零操作
    oxidize_polynomial(p1, n - 1); // 最高指数为n-1，因此多项式有n项
    oxidize_polynomial(p2, n - 1);
    
    return n;
}

Polynomial KaratsubaMultiply(Polynomial p1,Polynomial p2,int n){
    Polynomial cur1=p1->next,cur2=p2->next,p,tail;

    if(n==1){
        p=(Polynomial)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            return NULL;
        }

        tail=(Polynomial)malloc(sizeof(Term));
        if(!tail){
            printf("Memory allocation failed!\n");
            free(p);
            return NULL;
        }

        p->coef=-1;
        p->exp=-1;
        p->next=tail;
        tail->coef=cur1->coef*cur2->coef;
        tail->exp=0;
        tail->next=NULL;

        return p;
    }

    int half = n / 2;
    Polynomial First1=(Term*)malloc(sizeof(Term)),First0=(Term*)malloc(sizeof(Term)),\
    Second1=(Term*)malloc(sizeof(Term)),Second0=(Term*)malloc(sizeof(Term));
    if(!First1){
        printf("Memory allocation failed!\n");
        return NULL;
    }
    if(!First0){
        printf("Memory allocation failed!\n");
        free(First1);
        return NULL;
    }
    if(!Second1){
        printf("Memory allocation failed!\n");
        free(First1);
        free(First0);
        return NULL;
    }
    if(!Second0){
        printf("Memory allocation failed!\n");
        free(First1);
        free(First0);
        free(Second1);
        return NULL;
    }

    First1->coef=First0->coef=Second1->coef=Second0->coef=-1;
    First1->exp=First0->exp=Second1->exp=Second0->exp=-1;
    First1->next=NULL;
    First0->next=NULL;
    Second1->next=NULL;
    Second0->next=NULL;

    // 将p1拆分为First1（高次项）和First0（低次项）
    tail=First1;
    int i;
    for(i=0;i<half;i++){
        p=(Polynomial)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            destroy_polynomial(First1);
            destroy_polynomial(First0);
            destroy_polynomial(Second1);
            destroy_polynomial(Second0);
            return NULL;
        }

        p->coef=cur1->coef;
        p->exp=cur1->exp - half;
        p->next=NULL;
        cur1=cur1->next;
        tail->next=p;
        tail=tail->next;
    }
    
    tail=First0;
    for(;i<n;i++){
        p=(Polynomial)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            destroy_polynomial(First1);
            destroy_polynomial(First0);
            destroy_polynomial(Second1);
            destroy_polynomial(Second0);
            return NULL;
        }

        p->coef=cur1->coef;
        p->exp=cur1->exp;
        p->next=NULL;
        cur1=cur1->next;
        tail->next=p;
        tail=tail->next;
    }

    // 将p2拆分为Second1（高次项）和Second0（低次项）
    tail=Second1;
    for(i=0;i<half;i++){
        p=(Polynomial)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            destroy_polynomial(First1);
            destroy_polynomial(First0);
            destroy_polynomial(Second1);
            destroy_polynomial(Second0);
            return NULL;
        }

        p->coef=cur2->coef;
        p->exp=cur2->exp - half;
        p->next=NULL;
        cur2=cur2->next;
        tail->next=p;
        tail=tail->next;
    }
    
    tail=Second0;
    for(;i<n;i++){
        p=(Polynomial)malloc(sizeof(Term));
        if(!p){
            printf("Memory allocation failed!\n");
            destroy_polynomial(First1);
            destroy_polynomial(First0);
            destroy_polynomial(Second1);
            destroy_polynomial(Second0);
            return NULL;
        }

        p->coef=cur2->coef;
        p->exp=cur2->exp;
        p->next=NULL;
        cur2=cur2->next;
        tail->next=p;
        tail=tail->next;
    }

    // 计算A1*B1和A0*B0
    Polynomial multi1=KaratsubaMultiply(First1,Second1,half);
    Polynomial multi2=KaratsubaMultiply(First0,Second0,half);
    
    // 计算(A1+A0)*(B1+B0)
    Polynomial sumA=add_polynomial(First1,First0);
    Polynomial sumB=add_polynomial(Second1,Second0);
    Polynomial midMulti=KaratsubaMultiply(sumA,sumB,half);
    
    // 计算(A1+A0)*(B1+B0) - A1*B1
    Polynomial temp=subtract_polynomial(midMulti,multi1);
    
    // 计算(A1+A0)*(B1+B0) - A1*B1 - A0*B0
    Polynomial mid=subtract_polynomial(temp,multi2);
    
    // 调整指数：将A1*B1的结果乘以x^(2*half)
    cur1=multi1->next;
    while(cur1){
        cur1->exp+=2*half;
        cur1=cur1->next;
    }
    
    // 调整指数：将mid的结果乘以x^half
    cur2=mid->next;
    while(cur2){
        cur2->exp+=half;
        cur2=cur2->next;
    }
    
    // 组合结果：A1*B1*x^(2*half) + mid*x^half + A0*B0
    Polynomial MultiPolyHalf=add_polynomial(multi1,mid);
    Polynomial MultiPoly=add_polynomial(MultiPolyHalf,multi2);
    
    // 释放临时内存
    destroy_polynomial(multi1);
    destroy_polynomial(multi2);
    destroy_polynomial(sumA);
    destroy_polynomial(sumB);
    destroy_polynomial(midMulti);
    destroy_polynomial(temp);
    destroy_polynomial(mid);
    destroy_polynomial(MultiPolyHalf);
    destroy_polynomial(First1);
    destroy_polynomial(First0);
    destroy_polynomial(Second1);
    destroy_polynomial(Second0);

    return MultiPoly;
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

    int n=prepareKaratsuba(Poly1,Poly2);
    Polynomial multiPoly=KaratsubaMultiply(Poly1,Poly2,n);
    reduce_polynomial(multiPoly);
    print_polynomial(multiPoly);

    destroy_polynomial(Poly1);
    destroy_polynomial(Poly2);
    destroy_polynomial(SumPoly);
    destroy_polynomial(multiPoly);
    return 0;
}