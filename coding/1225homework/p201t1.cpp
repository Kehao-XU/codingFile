#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp=fopen("../test.txt","r");
    if(fp==NULL){
        printf("File open failed!\n");
        return 0;
    }

    int c=0,cnt=0;
    while(c!=EOF){
        c=fgetc(fp);
        if(c>='A'&&c<='z')cnt++;
    }

    printf("Letters:%d",cnt);
    int dir=fclose(fp);
    if(dir==EOF){
        printf("File close failed!\n");
        return 0;
    }
    return 0;
}