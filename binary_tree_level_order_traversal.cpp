// Problem: https://leetcode.com/problems/binary-tree-level-order-traversal/

#include <vector>
#include "utils.h"

namespace binary_tree_level_order_traversal {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Time: O(n), Space: O(n), n - number of nodes
    std::vector<int> run(TreeNode* root)
    {
        if (!root)
            return {};

        std::vector<int> result;
        std::vector<const TreeNode*> nodes = {root};

        for (int i = 0; i < nodes.size(); i++) {
            const TreeNode* node = nodes[i];

            result.push_back(node->val);

            if (node->left)
                nodes.push_back(node->left);

            if (node->right)
                nodes.push_back(node->right);
        }

        return result;
    }
};

int main()
{
    ASSERT( Solution().run(nullptr) == std::vector<int>{} );

    ASSERT( Solution().run(TemporaryTree<TreeNode>(new TreeNode{1})) == std::vector<int>{1} );

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4,
                            new TreeNode{8},
                            nullptr
                        },
                    new TreeNode{5,
                            nullptr,
                            new TreeNode{9}
                        }
                },
                new TreeNode{3,
                    new TreeNode{6},
                    new TreeNode{7}
                }
            })
        ) == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9} ));

    return 0;
}

}
