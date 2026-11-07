#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    fp=fopen("data/newFile.txt","r");
    if(!fp){
        printf("Open file failed!\n");
        exit(-1);
    }
    
    fseek(fp,0L,SEEK_END);
    printf("%ld",ftell(fp));  // %ld �� long ���͵ĸ�ʽ˵����
    fclose(fp);

    return 0;
}