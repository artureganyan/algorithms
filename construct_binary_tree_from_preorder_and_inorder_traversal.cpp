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
    // Time: O(n), Space: O(n), Recursion depth <= n + 2, n - number of nodes
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
        // For preorder traversal pt={pt1, pt2, ..., ptn}:
        // 1. pt[1] is the root of the tree.
        // 2. pt[i] is the root of either the left subtree of pt[i-1] or
        //    the right subtree of some previous pt[i-k], k > 0.
        //
        // So we could create the root pt[1], and for each i = 2..n:
        // 1. Create node pt[i].
        // 2. If previous node pt[i-1] should have left subtree, add pt[i] as
        //    its left child. Otherwise add pt[i] as the right child of the
        //    nearest previous node pt[i-k] that should have right subtree.
        //
        // But we don't know if a node should have left/right subtrees. This
        // can be found from inorder traversal it={it1, it2, ..., itn}:
        // 1. If it[r] is the root node, it[1, r) and it(r, n] are inorder
        //    traversals of its left/right subtrees (so if the traversal is
        //    empty, the subtree is also empty).
        // 2. We can recursively apply 1 to check each node.

        if (preorder_begin == preorder_end || inorder_begin == inorder_end)
            return nullptr;

        // Create the root node
        const int root_value = *preorder_begin;
        TreeNode* root       = new TreeNode{root_value, nullptr, nullptr};

        // Find this node in the inorder traversal
        /*
        // Note: This requires O(n) time and results in the overall O(n^2) time,
        // for the worst case when the tree has only the left children.
        auto inorder_root = inorder_begin;
        while (inorder_root != inorder_end && *inorder_root != root_value)
            inorder_root++;

        if (inorder_root == inorder_end)
            throw std::runtime_error("Preorder and inorder traversals do not match");
        */
        auto inorder_root = inorder_map_.at(root_value);

        // Create the left and right subtrees
        preorder_begin++;

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
