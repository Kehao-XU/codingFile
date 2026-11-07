/**
 * 给定一个二叉树的 根节点
 * root，想象自己站在它的右侧，按照从顶部到底部的顺序，返回从右侧所能看到的节点值。
 */

#include <iostream>
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
};

std::vector<int> solution(const TreeNode *root) {
    if (root == nullptr)
        return {};

    std::vector<int> ans;
    dfs(root, 0, ans);
    return ans;
}

// 深度优先搜索，先右后左，每一层第一次访问到的数就是最右侧的节点
void dfs(const TreeNode *node, int depth, std::vector<int> &ans) {
    if (!node)
        return;
    if (depth == (int)ans.size())
        ans.push_back(node->val);
    dfs(node->right, depth + 1, ans);
    dfs(node->left, depth + 1, ans);
}