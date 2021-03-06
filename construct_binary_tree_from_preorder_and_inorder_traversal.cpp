// Problem: https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/

#include <vector>
#include <unordered_map>
#include "utils.h"

namespace construct_binary_tree_from_preorder_and_inorder_traversal {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Note: There must be no duplicated values in the traversals.
    //
    // Time: O(n), Space: O(n), Recursion depth <= n + 2, n - number of nodes
    //
    TreeNode* run(const std::vector<int>& preorder, const std::vector<int>& inorder)
    {
        if (!preorder.size() || !inorder.size())
            return nullptr;

        if (preorder.size() != inorder.size())
            throw std::runtime_error("Preorder and inorder traversals do not match");

        inorder_map_.clear();
        for (auto i = inorder.cbegin(); i != inorder.cend(); i++)
            inorder_map_[*i] = i;

        auto preorder_begin = preorder.cbegin();
        return restoreTree(preorder_begin, preorder.end(), inorder.begin(), inorder.end());
    }

private:
    TreeNode* restoreTree(
              std::vector<int>::const_iterator& preorder_begin,
        const std::vector<int>::const_iterator& preorder_end,
        const std::vector<int>::const_iterator& inorder_begin,
        const std::vector<int>::const_iterator& inorder_end) const
    {
        // Idea:
        // The preorder, postorder and inorder traversals look as follows:
        //
        // Preorder:  te = {root, left subtree, right subtree}
        // Postorder: to = {left subtree, right subtree, root}
        // Inorder:   ti = {left subtree, root, right subtree}
        //
        // (where the left/right subtrees are also traversals)
        //
        // Given the preorder or postorder traversal, we know the root but
        // don't know sizes of its subtrees. Given the inorder traversal, we
        // don't know the root, but, if we would know it, we would also know
        // sizes of the subtrees. So we can combine knowledge of the roots
        // and of the subtrees' sizes to restore the tree:
        //
        // 1. Create the tree root (which is te[i=1] or to[i=n]).
        // 2. Find the root in ti (which is ti[r]).
        // 3. For te: Recursively restore the left subtree with i=i+1, ti=ti[1, r-1],
        //    and then the right subtree with i=i+1, ti=ti[r+1, end).
        //    For to: Recursively restore the right subtree with i=i-1, ti=ti[r+1, end),
        //    and then the left subtree with i=i-1 and ti[1, r-1].

        if (preorder_begin == preorder_end || inorder_begin == inorder_end)
            return nullptr;

        const int root_value = *(preorder_begin++);
        TreeNode* root       = new TreeNode{root_value, nullptr, nullptr};

        auto inorder_root = inorder_map_.at(root_value);

        root->left  = restoreTree(preorder_begin, preorder_end, inorder_begin, inorder_root);
        root->right = restoreTree(preorder_begin, preorder_end, ++inorder_root, inorder_end);

        return root;
    }

    typedef std::unordered_map<int /*value*/, std::vector<int>::const_iterator> TraversalMap;
    TraversalMap inorder_map_;
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

    test({3, 2, 1}, {3, 2, 1},
        new TreeNode{3,
            nullptr,
            new TreeNode{2,
                nullptr,
                new TreeNode{1}
            }
        }
    );

    test({1, 2, 3}, {2, 1, 3},
        new TreeNode{1,
            new TreeNode{2},
            new TreeNode{3}
        }
    );

    test({1, 2, 3, 4, 5}, {2, 3, 1, 5, 4},
        new TreeNode{1,
            new TreeNode{2,
                nullptr,
                new TreeNode{3}},
            new TreeNode{4,
                new TreeNode{5},
                nullptr}
        }
    );

    test({1, 2, 3, 4, 5, 6, 7}, {3, 2, 4, 1, 6, 5, 7},
        new TreeNode{1,
            new TreeNode{2,
                new TreeNode{3},
                new TreeNode{4}},
            new TreeNode{5,
                new TreeNode{6},
                new TreeNode{7}}
        }
    );

    return 0;
}

}
