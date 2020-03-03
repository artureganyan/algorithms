// Problem: https://leetcode.com/problems/kth-smallest-element-in-a-bst/

#include <vector>
#include "utils.h"

namespace kth_smallest_element_in_a_bst {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Note: If the node does not exist, returns 0. The first node has index 1.
    //
    // Time: O(n), Space: O(h), Recursion depth <= h + 3
    // n - number of nodes, h - height of the tree
    //
    int run(TreeNode* root, int index)
    {
        int             node_index = 0;
        const TreeNode* node       = findNode(root, node_index, index);
        if (!node)
            return 0;

        return node->val;
    }

private:
    TreeNode* findNode(TreeNode* root, int& current_index, int target_index) const
    {
        if (!root || target_index < 0)
            return nullptr;

        if (auto node = findNode(root->left, current_index, target_index))
            return node;

        current_index++;
        if (current_index == target_index)
            return root;

        if (auto node = findNode(root->right, current_index, target_index))
            return node;

        return nullptr;
    }
};


void test(TreeNode* root, const std::vector<int>& targets, const std::vector<int>& results)
{
    if (targets.size() != results.size())
        throw std::runtime_error("Targets and results must have the same size");

    for (int i = 0; i < targets.size(); i++) {
        const int target   = targets[i];
        const int expected = results[i];
        const int result   = Solution().run(root, target);

        ASSERT_EX( result == expected,
            tree_to_string(root) + ", find " + to_string(target) + " -> " + to_string(result) );
    }
}

int main()
{
    ASSERT( Solution().run(nullptr, 0) == 0 );

    test(TemporaryTree<TreeNode>(new TreeNode{0}), {1}, {0});
    test(TemporaryTree<TreeNode>(new TreeNode{1}), {1}, {1});
    test(TemporaryTree<TreeNode>(new TreeNode{-1}), {1}, {-1});

    test(TemporaryTree<TreeNode>(new TreeNode{1}), {2}, {0});
    test(TemporaryTree<TreeNode>(new TreeNode{1}), {0}, {0});
    test(TemporaryTree<TreeNode>(new TreeNode{1}), {-1}, {0});

    test(TemporaryTree<TreeNode>(
            new TreeNode{2,
                new TreeNode{1},
                nullptr
            }),
        {1, 2}, {1, 2});

    test(TemporaryTree<TreeNode>(
            new TreeNode{1,
                nullptr,
                new TreeNode{2}
            }),
        {1, 2}, {1, 2});

    test(TemporaryTree<TreeNode>(
            new TreeNode{2,
                new TreeNode{1},
                new TreeNode{3}
            }),
        {1, 2, 3}, {1, 2, 3});

    test(TemporaryTree<TreeNode>(
            new TreeNode{3,
                new TreeNode{1,
                    nullptr,
                    new TreeNode{2}
                },
                new TreeNode{5,
                    new TreeNode{4},
                    nullptr
                }
            }),
        {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5});

    return 0;
}

}
