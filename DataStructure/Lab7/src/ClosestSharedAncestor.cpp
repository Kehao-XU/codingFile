/**
 * 给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。
百度百科中最近公共祖先的定义为：“对于有根树 T 的两个节点
p、q，最近公共祖先表示为一个节点 x，满足 x 是 p、q 的祖先且 x
的深度尽可能大（一个节点也可以是它自己的祖先）。”
 */
#include "TreeNode.h"
TreeNode *solution(TreeNode *root, TreeNode *p, TreeNode *q) {
    if (root == nullptr || root == p || root == q)
        return root; // 空，或找到了 p/q，把结果往上传

    TreeNode *left = solution(root->left, p, q);
    TreeNode *right = solution(root->right, p, q);

    if (left != nullptr && right != nullptr)
        return root; // p、q 分居两侧 → 当前节点就是最近公共祖先
    return left != nullptr ? left : right; // 只在一边找到，继续往上带
}