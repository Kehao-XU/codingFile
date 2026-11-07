/**
 * 给定两个整数数组 inorder 和 postorder ，其中 inorder 是二叉树的中序遍历，
 * postorder 是同一棵树的后序遍历，请你构造并返回这颗 二叉树
 */

#include "TreeNode.h"
#include <unordered_map>
#include <vector>

// 在中序区间 [inL, inR] 和后序区间 [postL, postR] 上递归建树：
//   postorder[postR] 是当前子树的根；
//   在 inorder 中找到根的位置 k：[inL, k-1] 是左子树，[k+1, inR] 是右子树；
//   左子树结点数 leftSize = k - inL，用它切分后序区间。
static TreeNode *build(const std::vector<int> &inorder, int inL, int inR,
                       const std::vector<int> &postorder, int postL, int postR,
                       const std::unordered_map<int, int> &pos) {
    if (inL > inR)
        return nullptr;

    int rootValue = postorder[postR];
    int k = pos.at(rootValue); // 根在中序中的下标
    int leftSize = k - inL;    // 左子树结点个数

    TreeNode *root = new TreeNode{rootValue, nullptr, nullptr};
    root->left =
        build(inorder, inL, k - 1, postorder, postL, postL + leftSize - 1, pos);
    root->right =
        build(inorder, k + 1, inR, postorder, postL + leftSize, postR - 1, pos);
    return root;
}

TreeNode *solution(std::vector<int> inorder, std::vector<int> postorder) {
    if (inorder.empty() || inorder.size() != postorder.size())
        return nullptr;

    // 值不重复，预处理“值 -> 中序下标”，避免每层 find 导致 O(n^2)
    std::unordered_map<int, int> pos;
    for (int i = 0; i < (int)inorder.size(); ++i)
        pos[inorder[i]] = i;

    return build(inorder, 0, (int)inorder.size() - 1, postorder, 0,
                 (int)postorder.size() - 1, pos);
}