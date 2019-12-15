// Problem: https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/

#include <vector>
#include <list>
#include "utils.h"

namespace binary_tree_zigzag_level_order_traversal {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Time: O(n), Space: O(n), n - number of nodes
    std::vector<std::vector<int>> run(TreeNode* root)
    {
        if (!root)
            return {};

        typedef std::vector<std::vector<int>> Result;
        typedef std::list<const TreeNode*>    NodeList;

        enum Direction {
            Left  = -1,
            Right = +1
        };

        NodeList                 nodes     = {root};
        NodeList::const_iterator next_node = nodes.cbegin();
        Direction                direction = Right;
        Result                   result    = {{}};

        while (true) {
            // Go through the current level and build the next one as follows:
            // nodes = {nodes of the next level, nodes of the current level}
            const TreeNode* node = *next_node;
            result.back().push_back(node->val);

            if (direction == Right) {
                if (node->left)
                    nodes.push_front(node->left);

                if (node->right)
                    nodes.push_front(node->right);
            } else {
                if (node->right)
                    nodes.push_front(node->right);

                if (node->left)
                    nodes.push_front(node->left);
            }

            // Go to the next node of the current level
            next_node = nodes.erase(next_node);
            if (next_node != nodes.end())
                continue;

            // Go to the first node of the next level
            next_node = nodes.begin();
            if (next_node == nodes.end())
                break;

            if (direction == Left)
                direction = Right; else
                direction = Left;

            result.push_back({});
        }

        return result;
    }
};

int main()
{
    typedef std::vector<std::vector<int>> Result;

    ASSERT( Solution().run(nullptr) == Result{} );

    ASSERT( Solution().run(TemporaryTree<TreeNode>(new TreeNode{1})) == Result{{1}} );

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{3},
                    nullptr
                },
                nullptr
            })
        ) == Result{{1}, {2}, {3}} ));

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                nullptr,
                new TreeNode{2,
                    nullptr,
                    new TreeNode{3}
                }
            })
        ) == Result{{1}, {2}, {3}} ));

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{3,
                    new TreeNode{4,
                            new TreeNode{8},
                            nullptr
                        },
                    new TreeNode{5,
                            nullptr,
                            new TreeNode{7}
                        }
                },
                new TreeNode{2,
                    nullptr,
                    new TreeNode{6}
                }
            })
        ) == Result{{1}, {2, 3}, {4, 5, 6}, {7, 8}} ));

    return 0;
}

}
