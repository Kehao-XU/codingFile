#include<stdio.h>
#include<stdlib.h>
int** create_matrix(){
    int n=0;
    scanf("%d",&n);

    int **matrix=(int**)malloc(n*sizeof(int*));
    if(!matrix){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    for(int i=0;i<n;i++){
        matrix[i]=(int*)malloc(n*sizeof(int));
        if(!matrix[i]){
            printf("Memory allocation failed!\n");
            for(int j=0;j<=i;j++){
                free(matrix[j]);
            }
            return NULL;
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            scanf("%d",&matrix[i][j]);
        }
    }

    return matrix;
}

int check_broken_diagonals(int **matrix,int n,int magic_sum){
    dummy(0);
}

int is_demonic_magic(int **matrix,int n,int *magic_sum){
    dummy(0);
}