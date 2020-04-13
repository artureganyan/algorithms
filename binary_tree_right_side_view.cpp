// Problem: https://leetcode.com/problems/binary-tree-right-side-view/

#include <vector>
#include <deque>
#include "utils.h"

namespace binary_tree_right_side_view {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Time: O(n), Space: O(w + h)
    // n - number of nodes, w - maximum number of nodes in the tree level, h - height of the tree
    //
    std::vector<int> run(TreeNode* root)
    {
        if (!root)
            return {};

        std::vector<int>            result;
        std::deque<const TreeNode*> nodes = {root};

        while (nodes.size()) {
            const TreeNode* rightmost_node = nodes.back();
            result.push_back(rightmost_node->val);

            const int level_size = nodes.size();
            for (int i = 0; i < level_size; i++) {
                const TreeNode* node = nodes.front();
                if (node->left)
                    nodes.push_back(node->left);
                if (node->right)
                    nodes.push_back(node->right);
                nodes.pop_front();
            }
        }

        return result;
    }
};

int main()
{
    typedef std::vector<int> Result;

    ASSERT(( Solution().run(nullptr) == Result{} ));

    ASSERT(( Solution().run(TemporaryTree<TreeNode>(new TreeNode{1})) == Result{1} ));

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2},
                nullptr
            }
        )) == Result{1, 2} ));

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                nullptr,
                new TreeNode{2}
            }
        )) == Result{1, 2} ));

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2},
                new TreeNode{3}
            }
        )) == Result{1, 3} ));

    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4},
                    nullptr
                },
                new TreeNode{3,
                    nullptr,
                    new TreeNode{5}
                }
            }
        )) == Result{1, 3, 5} ));


    ASSERT(( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    nullptr,
                    new TreeNode{4,
                        nullptr,
                        new TreeNode{6,
                            new TreeNode{7},
                            nullptr
                        }
                    }
                },
                new TreeNode{3,
                    new TreeNode{5},
                    nullptr}
            }
        )) == Result{1, 3, 5, 6, 7} ));

    return 0;
}

}
