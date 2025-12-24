#include<stdio.h>
#include<string.h>
void reverseString(char *str){
    int n=strlen(str);
    int mid=n/2;
    for(int i=0;i<mid;i++){
        char temp;
        temp=str[i];
        str[i]=str[n-i-1];
        str[n-i-1]=temp;
    }
}

int main(){
    char str[20];
    printf("Enter a string.\n");
    fgets(str,sizeof(str),stdin);
    reverseString(str);
    printf("%s",str);
    return 0;
}