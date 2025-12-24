#include<stdio.h>
#include<math.h>
int isPrime(int n){
    // 处理边界条件：1和负数不是素数
    if(n <= 1) {
        return 0;
    }
    // 2是最小的素数
    if(n == 2) {
        return 1;
    }
    // 偶数不是素数
    if(n % 2 == 0) {
        return 0;
    }
    // 只需要检查到sqrt(n)，且只检查奇数
    int k = sqrt(n);
    for(int i=3; i<=k; i+=2){
        if(n % i == 0) {
            return 0;  // 能被整除，不是素数
        }
    }
    return 1;  // 是素数
}

int main(){
    int n;
    printf("Enter the value of n.\n");
    scanf("%d",&n);
    if(isPrime(n)) {
        printf("It is Prime.");
    } else {
        printf("It isn't Prime.");
    }
    return 0;
}