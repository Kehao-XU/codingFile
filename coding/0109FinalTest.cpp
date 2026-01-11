#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int** create_matrix(int n){
    int **matrix=(int**)malloc(n*sizeof(int*));
    if(!matrix){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    for(int i=0;i<n;i++){
        matrix[i]=(int*)malloc(n*sizeof(int));
        if(!matrix[i]){
            printf("Memory allocation failed!\n");
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

void free_matrix(int **matrix,int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            free(matrix[i]+j);
        }
    }
}

int check_broken_diagonals(int **matrix,int n,int magic_sum){
    int sum[n]={0};

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[(i+j)%n]+=matrix[i][j];
        }
    }

    for(int i=0;i<n-1;i++){
        if(sum[i]!=magic_sum)return 0;
    }

    sum[n]={0};
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[(i-j)%n]+=matrix[i][j];
        }
    }

    for(int i=0;i<n-1;i++){
        if(sum[i]!=magic_sum)return 0;
    }

    return 1;
}

int is_demonic_magic(int **matrix,int n,int *magic_sum){
    printf("=== 幻方验证结果 ===");
    *magic_sum=n*(pow(n,2)+1)/2;
    int sum[n]={0};
    int col=1,row=1,diag=1;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[i]+=matrix[i][j];
        }
    }

    for(int i=0;i<n-1;i++){
        if(sum[i]!=*magic_sum){
            col=0;
            break;
        }
    }
    
    sum[n]={0};
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[j]+=matrix[i][j];
        }
    }

    for(int i=0;i<n-1;i++){
        if(sum[i]!=*magic_sum){
            row=0;
            break;
        }
    }

    if(!check_broken_diagonals(matrix,n,*magic_sum))diag=0;
    
    if(row&&col&&diag){
        printf("是恶魔幻方\n行和：相等\n列和：相等\n泛化对角线和：相等")；
        return 1;
    }
    else{
        printf("不是恶魔幻方\n");
        if(row) printf("行和：相等\n");
        else printf("行和：不相等\n");
        if(col) printf("列和：相等\n");
        else printf("列和：不相等\n");
        if(diag) printf("泛化对角线和：相等\n");
        else printf("泛化对角线和：不相等\n");
        return 0;
    }
}

int dfs(int i,int j,int **matrix,int n,int **memo,int **path){
    dummy(0);
}
int find_longest_path(int **matrix,int n,int **result_path){
    dummy(0);
}

double calculate_variance(int **matrix, int n){
    dummy(0);
}
void optimize_magic(int **matrix, int n, int max_swaps){
    dummy(0);
}

int main(){
    int n=scanf("%d",&n);
    int **matrix=create_matrix(n);
    int magic_sum=0;
    int status=is_demonic_magic(matrix,n,&magic_sum);
    free(matrix,n);
    return 0;
}