# DFS算法问题集（C语言实现）

## 一、DFS算法简介

**深度优先搜索（Depth-First Search，DFS）** 是一种用于遍历或搜索树或图的算法。它会尽可能深地搜索树的分支，直到找到目标或者无法继续深入时，才会回溯到上一个节点继续搜索。

### 基本思想
1. 访问起始节点
2. 对起始节点的未访问邻接节点，递归地进行深度优先搜索

### 适用场景
- 树的遍历（前序、中序、后序）
- 图的遍历（连通性问题、路径查找）
- 组合问题（子集、排列、组合总和）
- 回溯问题（N皇后、数独）
- 迷宫求解

---

## 二、问题列表（按难度升序）

### 1. 二叉树的前序遍历

#### 问题描述
给定一个二叉树，返回它的前序遍历结果。前序遍历顺序为：根节点 → 左子树 → 右子树。

#### 算法思路
使用递归实现，按照前序遍历的顺序访问每个节点：
1. 访问根节点
2. 递归遍历左子树
3. 递归遍历右子树

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>

// 二叉树节点结构
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点
TreeNode* createNode(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 前序遍历
void preorderTraversal(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->val);  // 访问根节点
    preorderTraversal(root->left);  // 递归左子树
    preorderTraversal(root->right);  // 递归右子树
}

// 测试代码
int main() {
    // 构建一个二叉树
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    
    printf("前序遍历结果：");
    preorderTraversal(root);
    printf("\n");
    
    // 释放内存
    free(root->left->right);
    free(root->left->left);
    free(root->right);
    free(root->left);
    free(root);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(n)，其中n是二叉树的节点数，每个节点只需访问一次
- 空间复杂度：O(h)，其中h是二叉树的高度，递归调用栈的深度不会超过树的高度

---

### 2. 二叉树的中序遍历

#### 问题描述
给定一个二叉树，返回它的中序遍历结果。中序遍历顺序为：左子树 → 根节点 → 右子树。

#### 算法思路
使用递归实现，按照中序遍历的顺序访问每个节点：
1. 递归遍历左子树
2. 访问根节点
3. 递归遍历右子树

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>

// 二叉树节点结构
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点
TreeNode* createNode(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 中序遍历
void inorderTraversal(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    inorderTraversal(root->left);  // 递归左子树
    printf("%d ", root->val);  // 访问根节点
    inorderTraversal(root->right);  // 递归右子树
}

// 测试代码
int main() {
    // 构建一个二叉树
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    
    printf("中序遍历结果：");
    inorderTraversal(root);
    printf("\n");
    
    // 释放内存
    free(root->left->right);
    free(root->left->left);
    free(root->right);
    free(root->left);
    free(root);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(n)，其中n是二叉树的节点数
- 空间复杂度：O(h)，其中h是二叉树的高度

---

### 3. 二叉树的后序遍历

#### 问题描述
给定一个二叉树，返回它的后序遍历结果。后序遍历顺序为：左子树 → 右子树 → 根节点。

#### 算法思路
使用递归实现，按照后序遍历的顺序访问每个节点：
1. 递归遍历左子树
2. 递归遍历右子树
3. 访问根节点

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>

// 二叉树节点结构
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点
TreeNode* createNode(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 后序遍历
void postorderTraversal(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    postorderTraversal(root->left);  // 递归左子树
    postorderTraversal(root->right);  // 递归右子树
    printf("%d ", root->val);  // 访问根节点
}

// 测试代码
int main() {
    // 构建一个二叉树
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    
    printf("后序遍历结果：");
    postorderTraversal(root);
    printf("\n");
    
    // 释放内存
    free(root->left->right);
    free(root->left->left);
    free(root->right);
    free(root->left);
    free(root);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(n)，其中n是二叉树的节点数
- 空间复杂度：O(h)，其中h是二叉树的高度

---

### 4. 二叉树的最大深度

#### 问题描述
给定一个二叉树，找出其最大深度。二叉树的深度为根节点到最远叶子节点的最长路径上的节点数。

#### 算法思路
使用递归实现：
1. 若树为空，深度为0
2. 否则，深度为左子树深度和右子树深度的最大值加1

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 二叉树节点结构
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点
TreeNode* createNode(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 计算最大深度
int maxDepth(TreeNode* root) {
    if (root == NULL) {
        return 0;
    }
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return fmax(leftDepth, rightDepth) + 1;
}

// 测试代码
int main() {
    // 构建一个二叉树
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->left = createNode(6);
    
    int depth = maxDepth(root);
    printf("二叉树的最大深度：%d\n", depth);
    
    // 释放内存
    free(root->left->right);
    free(root->left->left);
    free(root->right->left);
    free(root->right);
    free(root->left);
    free(root);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(n)，其中n是二叉树的节点数，每个节点只需访问一次
- 空间复杂度：O(h)，其中h是二叉树的高度，递归调用栈的深度不会超过树的高度

---

### 5. 二叉树的路径和

#### 问题描述
给定一个二叉树和一个目标和，判断该树中是否存在根节点到叶子节点的路径，这条路径上所有节点值相加等于目标和。

#### 算法思路
使用递归实现：
1. 若树为空，返回false
2. 若当前节点是叶子节点，检查当前和是否等于目标和
3. 否则，递归检查左子树和右子树，更新当前和

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>

// 二叉树节点结构
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点
TreeNode* createNode(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 判断路径和是否存在
bool hasPathSum(TreeNode* root, int targetSum, int currentSum) {
    if (root == NULL) {
        return false;
    }
    
    currentSum += root->val;
    
    // 如果是叶子节点
    if (root->left == NULL && root->right == NULL) {
        return currentSum == targetSum;
    }
    
    // 递归检查左子树和右子树
    return hasPathSum(root->left, targetSum, currentSum) || 
           hasPathSum(root->right, targetSum, currentSum);
}

// 接口函数
bool hasPathSumInterface(TreeNode* root, int targetSum) {
    return hasPathSum(root, targetSum, 0);
}

// 测试代码
int main() {
    // 构建一个二叉树
    TreeNode* root = createNode(5);
    root->left = createNode(4);
    root->right = createNode(8);
    root->left->left = createNode(11);
    root->right->left = createNode(13);
    root->right->right = createNode(4);
    root->left->left->left = createNode(7);
    root->left->left->right = createNode(2);
    root->right->right->right = createNode(1);
    
    int targetSum = 22;
    if (hasPathSumInterface(root, targetSum)) {
        printf("存在路径和为 %d 的路径\n", targetSum);
    } else {
        printf("不存在路径和为 %d 的路径\n", targetSum);
    }
    
    // 释放内存
    free(root->left->left->right);
    free(root->left->left->left);
    free(root->left->left);
    free(root->left);
    free(root->right->right->right);
    free(root->right->right);
    free(root->right->left);
    free(root->right);
    free(root);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(n)，其中n是二叉树的节点数，每个节点只需访问一次
- 空间复杂度：O(h)，其中h是二叉树的高度，递归调用栈的深度不会超过树的高度

---

### 6. 岛屿数量

#### 问题描述
给定一个由 '1'（陆地）和 '0'（水）组成的二维网格，计算岛屿的数量。岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。

#### 算法思路
使用DFS实现：
1. 遍历网格中的每个单元格
2. 若当前单元格是陆地，则岛屿数量加1，并使用DFS将所有相邻的陆地标记为已访问

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>

// DFS函数，将与(i,j)相邻的所有陆地标记为已访问
void dfs(char** grid, int gridSize, int* gridColSize, int i, int j) {
    // 检查边界条件
    if (i < 0 || i >= gridSize || j < 0 || j >= gridColSize[i] || grid[i][j] == '0') {
        return;
    }
    
    // 标记当前单元格为已访问
    grid[i][j] = '0';
    
    // 访问四个相邻方向
    dfs(grid, gridSize, gridColSize, i-1, j);  // 上
    dfs(grid, gridSize, gridColSize, i+1, j);  // 下
    dfs(grid, gridSize, gridColSize, i, j-1);  // 左
    dfs(grid, gridSize, gridColSize, i, j+1);  // 右
}

// 计算岛屿数量
int numIslands(char** grid, int gridSize, int* gridColSize) {
    if (gridSize == 0) {
        return 0;
    }
    
    int count = 0;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridColSize[i]; j++) {
            if (grid[i][j] == '1') {
                count++;
                dfs(grid, gridSize, gridColSize, i, j);
            }
        }
    }
    
    return count;
}

// 测试代码
int main() {
    // 创建测试网格
    int gridSize = 4;
    int gridColSize[] = {5, 5, 5, 5};
    char** grid = (char**)malloc(gridSize * sizeof(char*));
    for (int i = 0; i < gridSize; i++) {
        grid[i] = (char*)malloc(gridColSize[i] * sizeof(char));
    }
    
    // 初始化网格
    char* row0 = "11110";
    char* row1 = "11010";
    char* row2 = "11000";
    char* row3 = "00000";
    
    for (int i = 0; i < gridColSize[0]; i++) {
        grid[0][i] = row0[i];
        grid[1][i] = row1[i];
        grid[2][i] = row2[i];
        grid[3][i] = row3[i];
    }
    
    int islands = numIslands(grid, gridSize, gridColSize);
    printf("岛屿数量：%d\n", islands);
    
    // 释放内存
    for (int i = 0; i < gridSize; i++) {
        free(grid[i]);
    }
    free(grid);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(m*n)，其中m是网格的行数，n是网格的列数，每个单元格只需访问一次
- 空间复杂度：O(m*n)，最坏情况下，整个网格都是陆地，递归调用栈的深度达到m*n

---

### 7. 组合总和

#### 问题描述
给定一个无重复元素的数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。candidates 中的数字可以无限制重复被选取。

#### 算法思路
使用回溯算法（DFS的一种应用）实现：
1. 对数组进行排序（方便剪枝）
2. 从数组的第一个元素开始，递归地构建组合
3. 若当前和等于target，则将当前组合加入结果集
4. 若当前和大于target，则剪枝，不再继续递归

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 比较函数，用于qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// DFS回溯函数
void dfs(int* candidates, int candidatesSize, int target, int start, int* path, int pathSize, 
         int** result, int* resultSize, int* returnColumnSizes) {
    // 找到一个有效组合
    if (target == 0) {
        result[*resultSize] = (int*)malloc(pathSize * sizeof(int));
        memcpy(result[*resultSize], path, pathSize * sizeof(int));
        returnColumnSizes[*resultSize] = pathSize;
        (*resultSize)++;
        return;
    }
    
    // 尝试所有可能的元素
    for (int i = start; i < candidatesSize; i++) {
        // 剪枝：如果当前元素大于剩余target，后面的元素也会更大，直接跳过
        if (candidates[i] > target) {
            break;
        }
        
        // 将当前元素加入路径
        path[pathSize] = candidates[i];
        
        // 递归搜索，允许重复使用当前元素
        dfs(candidates, candidatesSize, target - candidates[i], i, path, pathSize + 1, 
            result, resultSize, returnColumnSizes);
    }
}

// 主函数
int** combinationSum(int* candidates, int candidatesSize, int target, int* returnSize, int** returnColumnSizes) {
    // 初始化结果集
    int** result = (int**)malloc(1000 * sizeof(int*));
    *returnSize = 0;
    *returnColumnSizes = (int*)malloc(1000 * sizeof(int));
    
    // 排序数组，方便剪枝
    qsort(candidates, candidatesSize, sizeof(int), compare);
    
    // 用于存储当前路径
    int* path = (int*)malloc(100 * sizeof(int));
    
    // 开始DFS
    dfs(candidates, candidatesSize, target, 0, path, 0, result, returnSize, *returnColumnSizes);
    
    free(path);
    return result;
}

// 测试代码
int main() {
    int candidates[] = {2, 3, 6, 7};
    int candidatesSize = sizeof(candidates) / sizeof(candidates[0]);
    int target = 7;
    int returnSize;
    int* returnColumnSizes;
    
    int** result = combinationSum(candidates, candidatesSize, target, &returnSize, &returnColumnSizes);
    
    // 打印结果
    printf("组合总和为 %d 的组合有：\n", target);
    for (int i = 0; i < returnSize; i++) {
        printf("[");
        for (int j = 0; j < returnColumnSizes[i]; j++) {
            printf("%d", result[i][j]);
            if (j < returnColumnSizes[i] - 1) {
                printf(", ");
            }
        }
        printf("]\n");
        free(result[i]);
    }
    
    // 释放内存
    free(result);
    free(returnColumnSizes);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(2^n)，其中n是candidates数组的长度，最坏情况下需要考虑所有可能的组合
- 空间复杂度：O(target)，递归调用栈的深度不会超过target

---

### 8. N皇后问题

#### 问题描述
n 皇后问题研究的是如何将 n 个皇后放置在 n×n 的棋盘上，并且使皇后彼此之间不能相互攻击（即任意两个皇后都不能处于同一行、同一列或同一斜线上）。

#### 算法思路
使用回溯算法（DFS的一种应用）实现：
1. 从第一行开始，逐行放置皇后
2. 对于每一行，尝试在每一列放置皇后
3. 检查当前位置是否合法（即与已放置的皇后不冲突）
4. 若合法，则放置皇后，并递归处理下一行
5. 若所有行都成功放置皇后，则找到一个解

#### C语言实现
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 检查在第row行第col列放置皇后是否合法
bool isSafe(int** board, int n, int row, int col) {
    int i, j;
    
    // 检查同一列
    for (i = 0; i < row; i++) {
        if (board[i][col] == 1) {
            return false;
        }
    }
    
    // 检查左上对角线
    for (i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    // 检查右上对角线
    for (i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 1) {
            return false;
        }
    }
    
    return true;
}

// DFS回溯函数
bool solveNQueensUtil(int** board, int n, int row, int** solutions, int* solutionCount) {
    // 所有行都成功放置皇后，找到一个解
    if (row == n) {
        // 保存当前解
        solutions[*solutionCount] = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 1) {
                    solutions[*solutionCount][i] = j;
                    break;
                }
            }
        }
        (*solutionCount)++;
        return true;
    }
    
    bool found = false;
    // 尝试在当前行的每一列放置皇后
    for (int col = 0; col < n; col++) {
        // 检查当前位置是否合法
        if (isSafe(board, n, row, col)) {
            // 放置皇后
            board[row][col] = 1;
            
            // 递归处理下一行
            found = solveNQueensUtil(board, n, row + 1, solutions, solutionCount) || found;
            
            // 回溯：移除皇后
            board[row][col] = 0;
        }
    }
    
    return found;
}

// 主函数
int** solveNQueens(int n, int* returnSize) {
    // 初始化棋盘
    int** board = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        board[i] = (int*)calloc(n, sizeof(int));
    }
    
    // 初始化结果集
    int** solutions = (int**)malloc(1000 * sizeof(int*));
    *returnSize = 0;
    
    // 开始DFS
    solveNQueensUtil(board, n, 0, solutions, returnSize);
    
    // 释放棋盘内存
    for (int i = 0; i < n; i++) {
        free(board[i]);
    }
    free(board);
    
    return solutions;
}

// 打印棋盘
void printBoard(int* solution, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (solution[i] == j) {
                printf("Q ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// 测试代码
int main() {
    int n = 4;
    int returnSize;
    
    int** solutions = solveNQueens(n, &returnSize);
    
    printf("%d皇后问题共有 %d 种解法：\n\n", n, returnSize);
    for (int i = 0; i < returnSize; i++) {
        printf("解法 %d：\n", i + 1);
        printBoard(solutions[i], n);
        free(solutions[i]);
    }
    
    free(solutions);
    
    return 0;
}
```

#### 复杂度分析
- 时间复杂度：O(n!)，其中n是皇后的数量，每一行有n种选择，但是由于剪枝，实际运行时间会比n!小很多
- 空间复杂度：O(n)，递归调用栈的深度为n，棋盘的大小为n×n

---

## 三、总结

DFS算法是一种非常重要的搜索算法，它在树、图的遍历以及组合、回溯问题中有着广泛的应用。本文通过8个不同难度的问题，展示了DFS算法在C语言中的实现，从简单的二叉树遍历到复杂的回溯问题，覆盖了DFS的主要应用场景。

通过学习这些问题，你可以掌握DFS算法的基本思想、实现方法以及优化技巧，为解决更复杂的算法问题打下基础。

### 学习建议
1. 先理解DFS的基本思想和递归实现
2. 掌握二叉树的三种遍历方式
3. 学习DFS在图遍历中的应用（如岛屿数量问题）
4. 深入学习回溯算法（组合总和、N皇后问题）
5. 尝试使用非递归方式实现DFS（使用栈）

希望这些问题对你学习DFS算法有所帮助！