// Problem: https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/

#include <vector>
#include "utils.h"

namespace lowest_common_ancestor_of_a_binary_tree {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Note: If one of the target nodes is null or not present in the tree,
    // then the rest one is searched for. If none of the nodes is present,
    // or if the root is null, returns null.
    //
    // Note: In the original problem, the node values must be unique and exist
    // in the tree, but since we search for and return the pointers, this is
    // not necessary. If it's supposed that we should use not the pointers but
    // the pointed values, then the solution should be corrected.
    //
    // Time: O(n), Space: O(h), Recursion depth <= h + 3
    // n - number of nodes, h - height of the tree
    //
    TreeNode* run(TreeNode* root, TreeNode* node1, TreeNode* node2)
    {
        return findParent(root, node1, node2);
    }

private:
    TreeNode* findParent(TreeNode* root, TreeNode* node1, TreeNode* node2) const
    {
        if (!root)
            return nullptr;

        if (root == node1 || root == node2)
            return root;

        const auto parent1 = findParent(root->left,  node1, node2);
        const auto parent2 = findParent(root->right, node1, node2);

        if (parent1 && parent2)
            return root;

        if (parent1)
            return parent1;

        return parent2;
    }
};


void test(TreeNode* root, const std::vector<TreeNode*> nodes, TreeNode* expected)
{
    const auto result = Solution().run(root, nodes[0], nodes[1]);

    const auto node_to_string = [](TreeNode* node)
    {
        return node ? to_string(node->val) : "null";
    };

    ASSERT_EX( result == expected,
        tree_to_string(root) + ", find: " +
        node_to_string(nodes[0]) + ", " + node_to_string(nodes[1]) + " -> " +
        node_to_string(expected)
    );
}

int main()
{
    {
        TemporaryTree<TreeNode> n(new TreeNode{1});
        test(nullptr, {nullptr, nullptr}, nullptr);
        test(nullptr, {n, n}, nullptr);
        test(n, {nullptr, nullptr}, nullptr);
        test(n, {nullptr, n}, n);
        test(n, {n, nullptr}, n);
    }

    {
        TemporaryTree<TreeNode> nn(new TreeNode{-1});
        test(nn, {nn, nn}, nn);

        TemporaryTree<TreeNode> nz(new TreeNode{0});
        test(nz, {nz, nz}, nz);

        TemporaryTree<TreeNode> np(new TreeNode{1});
        test(np, {np, np}, np);
    }

    {
        TreeNode* n[] = {new TreeNode{0}, new TreeNode{1}};
        n[0]->left = n[1];

        TemporaryTree<TreeNode> tree(n[0]);
        test(n[0], {n[0], n[1]}, n[0]);
    }

    {
        TreeNode* n[] = {new TreeNode{0}, new TreeNode{1}};
        n[0]->right = n[1];

        TemporaryTree<TreeNode> tree(n[0]);
        test(n[0], {n[0], n[1]}, n[0]);
    }

    {
        TreeNode* n[] = {new TreeNode{0}, new TreeNode{1}, new TreeNode{2}};
        n[0]->left  = n[1];
        n[0]->right = n[2];

        TemporaryTree<TreeNode> tree(n[0]);
        test(n[0], {n[1], n[2]}, n[0]);
    }

    {
        TreeNode* n[] = {new TreeNode{0}, new TreeNode{1}, new TreeNode{2}, new TreeNode{3}, new TreeNode{4}, new TreeNode{5}, new TreeNode{6}};
        n[0]->left  = n[1];
        n[0]->right = n[2];

        n[1]->left  = n[3];
        n[1]->right = n[4];

        n[2]->right = n[5];

        n[4]->left  = n[6];

        TemporaryTree<TreeNode> tree(n[0]);
        test(n[0], {n[3], n[2]}, n[0]);
        test(n[0], {n[3], n[5]}, n[0]);
        test(n[0], {n[3], n[4]}, n[1]);
        test(n[0], {n[1], n[3]}, n[1]);
        test(n[0], {n[1], n[6]}, n[1]);
    }

    return 0;
}

}
