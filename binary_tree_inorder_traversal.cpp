// Problem: https://leetcode.com/problems/binary-tree-inorder-traversal/

#include <vector>
#include "utils.h"

namespace binary_tree_inorder_traversal {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Time: O(n), Space: O(n + h), n - number of nodes, h - height of the tree
    std::vector<int> run(TreeNode* root)
    {
        if (!root)
            return {};

        std::vector<int>             result;
        std::vector<const TreeNode*> nodes     = {root};
        const TreeNode*              node_prev = nullptr;

        while (nodes.size()) {
            const TreeNode* node = nodes.back();

            // The right subtree is handled => the left subtree and the node are handled => go up
            if (node->right && node->right == node_prev) {
                nodes.pop_back();

            // The left subtree is not handled => go left
            } else if (node->left && node->left != node_prev) {
                nodes.push_back(node->left);

            // The left subtree is handled => handle the node and go right (if possible) or up
            } else {
                result.push_back(node->val);
                if (node->right) {
                    nodes.push_back(node->right);
                } else {
                    nodes.pop_back();
                }
            }

            node_prev = node;
        }

        return result;
    }
};

int main()
{
    ASSERT(( Solution().run(nullptr) == std::vector<int>{} ));

    ASSERT(( Solution().run(TemporaryTree<TreeNode>(new TreeNode{1})) == std::vector<int>{1} ));

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{6,
                new TreeNode{3,
                    new TreeNode{2,
                            new TreeNode{1},
                            nullptr
                        },
                    new TreeNode{4,
                            nullptr,
                            new TreeNode{5}
                        }
                },
                new TreeNode{8,
                    new TreeNode{7},
                    new TreeNode{9}
                }
            })
        ) == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9} ));

    return 0;
}

}
