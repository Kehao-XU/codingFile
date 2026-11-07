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
        free(matrix[i]);
    }
    free(matrix);
}

int check_broken_diagonals(int **matrix,int n,int magic_sum){
    int sum[n]={0};

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[(i+j)%n]+=matrix[i][j];
        }
    }

    for(int i=0;i<n;i++){
        if(sum[i]!=magic_sum)return 0;
    }

    for(int i=0;i<n;i++)sum[i]=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[abs(i-j)%n]+=matrix[i][j];
        }
    }

    for(int i=0;i<n;i++){
        if(sum[i]!=magic_sum)return 0;
    }

    return 1;
}

int is_demonic_magic(int **matrix,int n,int *magic_sum){
    printf("=== Result ===\n");
    *magic_sum=n*(pow(n,2)+1)/2;
    int sum[n]={0};
    int col=1,row=1,diag=1;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[i]+=matrix[i][j];
        }
        if(sum[i]!=*magic_sum){
            col=0;
            break;
        }
    }
    
    for(int i=0;i<n;i++)sum[i]=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            sum[j]+=matrix[i][j];
        }
    }

    for(int i=0;i<n;i++){
        if(sum[i]!=*magic_sum){
            row=0;
            break;
        }
    }

    if(!check_broken_diagonals(matrix,n,*magic_sum))diag=0;
    
    if(row&&col&&diag){
        printf("Yes,it is magic demonic\nRow sum:Equal\nCol sum:Equal\nDiagonals:Equal\n");
        return 1;
    }
    else{
        printf("No,it isnt magic demonic\n");
        if(row) printf("Row sum:Equal\n");
        else printf("Row sum:Not equal\n");
        if(col) printf("Col sum:Equal\n");
        else printf("Col sum:Not equal\n");
        if(diag) printf("Diagonals: Equal\n");
        else printf("Diagonals:Not equal\n");
        return 0;
    }
}

int dfs(int i,int j,int **matrix,int n,int **memo,int **path){
    dummy(0);
}

int find_longest_path(int **matrix,int n,int **result_path){
    int **memo=(int**)malloc(n*sizeof(int*));
    if(!memo){
        printf("Memory allocation failed!\n");
        return -1;
    }

    for(int i=0;i<n;i++){
        memo[i]=(int*)malloc(n*sizeof(int));
        if(!memo[i]){
            printf("Memory allocation failed!\n");
            return -1;
        }
        for(int j=0;j<n;j++)memo[i][j]=0;
    }

    int **path=(int**)malloc(n*sizeof(int*));
    if(!path){
        printf("Memory allocation failed!\n");
        return -1;
    }

    for(int i=0;i<n;i++){
        path[i]=(int*)malloc(n*sizeof(int));
        if(!path[i]){
            printf("Memory allocation failed!\n");
            return -1;
        }
        for(int j=0;j<n;j++)path[i][j]=0;
    }

    int **count=(int**)malloc(n*sizeof(int*));
    if(!count){
        printf("Memory allocation failed!\n");
        return -1;
    }

    for(int i=0;i<n;i++){
        count[i]=(int*)malloc(n*sizeof(int));
        if(!count[i]){
            printf("Memory allocation failed!\n");
            return -1;
        }
        for(int j=0;j<n;j++)count[i][j]=0;
    }

    for(int i=0,max=0,curi=0,curj=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(memo[i][j]>max){
                max=memo[i][j];
                curi=i;
                curj=j;
            }
        }
    }

    int length=0,curi=0,curj=0;
    while(memo[curi][curj]>0){
        if(curi-1>=0&&memo[curi-1][curj]==memo[curi][curj]-1){
            result_path[curi][curj]=++length;
            curi--;
            continue;
        }
        if(curj+1<=n&&memo[curi][curj+1]==memo[curi][curj]-1){
            result_path[curi][curj]=++length;
            curj++;
            continue;
        }
        if(curi+1<=n&&memo[curi+1][curj]==memo[curi][curj]-1){
            result_path[curi][curj]=++length;
            curi++;
            continue;
        }
        if(curj-1>=0&&memo[curi][curj-1]==memo[curi][curj]-1){
            result_path[curi][curj]=++length;
            curj--;
            continue;
        }
    }
    result_path[curi][curj]=++length;

    return length;
}

void print_longest_path(int **matrix,int n,int **result_path){
    int i=0,j=0;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++) if(result_path[i][j]==1) break;
        if(result_path[i][j]==1)break;
    }
}

int main(){
    int n;
    scanf("%d",&n);
    int **matrix=create_matrix(n);
    int magic_sum=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)printf("%d ",matrix[i][j]);
        printf("\n");
    }
    int status=is_demonic_magic(matrix,n,&magic_sum);
    free_matrix(matrix,n);
    return 0;
}