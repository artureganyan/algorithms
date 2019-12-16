// Problem: https://leetcode.com/problems/validate-binary-search-tree/

#include <limits>
#include "utils.h"

namespace validate_binary_search_tree {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Time: O(n), Space: O(h), Max recursion depth: h + 3
    // n - number of nodes, h - height of the tree
    //
    // Note: For an empty (null) tree, returns true.
    //
    bool run(TreeNode* root)
    {
        return isValid(root, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    }

private:
    bool isValid(TreeNode* root, int min, int max) const
    {
        if (!root)
            return true;

        if (root->val < min || root->val > max)
            return false;

        return isValid(root->left,  min, root->val - 1) &&
               isValid(root->right, root->val + 1, max);
    }
};


void test(TreeNode* tree, bool expected)
{
    ASSERT_EX( Solution().run(tree) == expected, tree_to_string(tree) + " -> " + to_string(expected) );

    destroy_tree(tree);
}

int main()
{
    test(nullptr, true);

    test(new TreeNode{1}, true);

    test(new TreeNode{1,
            nullptr,
            new TreeNode{2}
        }, true);

    test(new TreeNode{1,
            new TreeNode{2},
            nullptr
        }, false);

    test(new TreeNode{2,
            new TreeNode{1},
            nullptr
        }, true);

    test(new TreeNode{2,
            nullptr,
            new TreeNode{1}
        }, false);

    test(new TreeNode{2,
            new TreeNode{1},
            new TreeNode{3}
        }, true);

    test(new TreeNode{2,
            nullptr,
            new TreeNode{3,
                new TreeNode{1},
                nullptr
            }
        }, false);

    test(new TreeNode{2,
            new TreeNode{1,
                nullptr,
                new TreeNode{3}
            },
            nullptr
        }, false);

    test(new TreeNode{1,
            nullptr,
            new TreeNode{2,
                nullptr,
                new TreeNode{3}
            }
        }, true);

    test(new TreeNode{3,
            new TreeNode{2,
                new TreeNode{1},
                nullptr
            },
            nullptr
        }, true);

    return 0;
}

}
