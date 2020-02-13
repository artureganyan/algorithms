// Problem: https://leetcode.com/problems/count-complete-tree-nodes/

#include <cmath>
#include "utils.h"

namespace count_complete_tree_nodes {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Note: The tree must be complete.
    //
    // Time: O(h^2), Space: O(h), Recursion depth <= h + 3, h - height of the tree
    //
    int run(TreeNode* root)
    {
        const int levels = countLevelsInCompleteTree(root);
        return countNodesInCompleteTree(root, levels);
    }

private:
    int countLevelsInCompleteTree(const TreeNode* root) const
    {
        int count = 0;

        const TreeNode* node = root;
        while (node) {
            node = node->left;
            count++;
        }

        return count;
    }

    int countNodesInCompleteTree(const TreeNode* root, int levels) const
    {
        if (!root)
            return 0;

        if (levels <= 0)
            throw std::runtime_error("Tree levels must be > 0");

        int count_left  = 0;
        int count_right = 0;

        const int levels_left  = levels - 1;
        const int levels_right = countLevelsInCompleteTree(root->right);
        assert(levels_right < levels);

        // The last level of the left subtree is completely filled (so the left
        // subtree is perfect)
        if (levels_right == levels - 1) {
            count_left  = countNodesInPerfectTree(levels_left);
            count_right = countNodesInCompleteTree(root->right, levels_right);

        // The last level of the right subtree is empty (so the right subtree
        // is perfect)
        } else {
            count_left  = countNodesInCompleteTree(root->left, levels - 1);
            count_right = countNodesInPerfectTree(levels_right);
        }

        return 1 + count_left + count_right;
    }

    int countNodesInPerfectTree(int levels) const
    {
        if (levels < 0)
            return 0;

        return std::pow(2, levels) - 1;
    }
};


int main()
{
    ASSERT( Solution().run(nullptr) == 0 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{0}
        )) == 1 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2},
                nullptr
            }
        )) == 2 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2},
                new TreeNode{3}
            }
        )) == 3 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4},
                    nullptr
                },
                new TreeNode{3}
            }
        )) == 4 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4},
                    new TreeNode{5}
                },
                new TreeNode{3}
            }
        )) == 5 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4},
                    new TreeNode{5}
                },
                new TreeNode{3,
                    new TreeNode{6},
                    nullptr
                }
            }
        )) == 6 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4},
                    new TreeNode{5}
                },
                new TreeNode{3,
                    new TreeNode{6},
                    new TreeNode{7}
                }
            }
        )) == 7 );

    return 0;
}

}
