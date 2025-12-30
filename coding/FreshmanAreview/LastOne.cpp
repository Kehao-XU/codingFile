#include<stdio.h>
#define BasicGroup
/*题目：编写程序，输入两个整数 a 和 b，计算并输出：

a / b 的整数结果

a / b 的浮点数结果（保留3位小数）

a % b 的结果

要求：考虑 b=0 的情况，给出错误提示。

查漏点：

整数除法 vs 浮点数除法

类型转换（隐式/显式）

除零错误处理*/
#ifdef BasicCalc
int main(){
    int a,b;
    printf("Please enter the value of a&b.\n");
    scanf("%d%d",&a,&b);
    if(!b){
        printf("Cannot divide a by b!\n");
    }
    else{
        printf("a/b=%d\t%.3f\n",a/b,(float)(a/b));
        printf("a%b=%d",a%b);
    }
    return 0;
}
#endif

#ifdef BasicGroup
int max(int a[],int n){
    int max=a[0];
    for(int i=0;i<n;i++){
        if(a[i]>max)max=a[i];
    }
    return max;
}

int min(int a[],int n){
    int min=a[0];
    for(int i=0;i<n;i++){
        if(a[i]<min)min=a[i];
    }
    return min;
}

double average(int a[],int n){
    int sum=0;
    for(int i=0;i<n;i++){
        sum+=a[i];
    }
    return (double)sum/(double)n;
}

void reverseGroup(int a[],int n){
    int *head=&a[0],*tail=&a[n-1];
    for(int i=0;i<n/2;i++){
        int temp;
        temp=*head;*head=*tail;*tail=temp;
        head++;
        tail--;
    }
}

int main(){
    int a[5]={1,2,3,6,5};
    int n=5;
    printf("Max=%d,Min=%d,Average=%lf\n",max(a,n),min(a,n),average(a,n));
    reverseGroup(a,n);
    for(int i=0;i<n;i++){
        printf("%d\t",a[i]);
    }
    return 0;
}
#endif