// Problem: https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/

#include <vector>
#include "utils.h"

namespace construct_binary_tree_from_preorder_and_inorder_traversal {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    TreeNode* run(const std::vector<int>& preorder, const std::vector<int>& inorder)
    {
        if (!preorder.size() || !inorder.size())
            return nullptr;

        if (preorder.size() != inorder.size())
            throw std::runtime_error("Preorder and inorder traversions do not match");

        auto preorder_begin = preorder.cbegin();
        return construct(preorder_begin, preorder.end(), inorder.begin(), inorder.end());
    }

private:
    TreeNode* construct(
              std::vector<int>::const_iterator& preorder_begin,
        const std::vector<int>::const_iterator& preorder_end,
        const std::vector<int>::const_iterator& inorder_begin,
        const std::vector<int>::const_iterator& inorder_end) const
    {
        if (preorder_begin == preorder_end || inorder_begin == inorder_end)
            return nullptr;

        const int root_value = *preorder_begin;
        TreeNode* root       = new TreeNode{root_value, nullptr, nullptr};

        auto inorder_root = inorder_begin;
        while (inorder_root != inorder_end && *inorder_root != root_value)
            inorder_root++;

        if (inorder_root == inorder_end)
            throw std::runtime_error("Preorder and inorder traversions do not match");

        preorder_begin++;

        root->left  = construct(preorder_begin, preorder_end, inorder_begin, inorder_root);
        root->right = construct(preorder_begin, preorder_end, ++inorder_root, inorder_end);

        return root;
    }
};


void test(const std::vector<int>& preorder, const std::vector<int>& inorder, TreeNode* expected)
{
    TreeNode* result = Solution().run(preorder, inorder);

    ASSERT_EX( compare_trees(result, expected),
        to_string(preorder) + ", " + to_string(inorder) + " -> " + tree_to_string(expected) );

    destroy_tree(result);
    destroy_tree(expected);
}

int main()
{
    test({}, {}, nullptr);
    test({}, {1}, nullptr);
    test({1}, {}, nullptr);

    test({1}, {1}, new TreeNode{1});

    test({2, 1}, {1, 2},
        new TreeNode{2,
            new TreeNode{1},
            nullptr
        }
    );

    test({3, 2, 1}, {1, 2, 3},
        new TreeNode{3,
            new TreeNode{2,
                new TreeNode{1},
                nullptr
            },
            nullptr
        }
    );

    return 0;
}

}
