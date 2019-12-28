// Problem: https://leetcode.com/problems/sum-root-to-leaf-numbers/

#include "utils.h"

namespace sum_root_to_leaf_numbers {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Note: Each node value must be within [0, 9]. Overflow of the sum is
    // not detected.
    //
    // Time: O(n), Space: O(h), Recursion depth <= h + 2
    // n - number of nodes, h - height of the tree
    //
    int run(TreeNode* root)
    {
        return calculateSum(root).value;
    }

private:
    struct Sum
    {
        int value;
        int multiplier;
    };

    Sum calculateSum(TreeNode* root) const
    {
        if (!root)
            return {0, 0};

        if (root->val < 0 || root->val > 9)
            throw std::runtime_error("Node value must be within [0, 9]");

        Sum sum_left  = calculateSum(root->left);
        Sum sum_right = calculateSum(root->right);

        Sum result;
        result.multiplier = std::max(sum_left.multiplier * 10 + sum_right.multiplier * 10, 1);
        result.value      = sum_left.value + sum_right.value + root->val * result.multiplier;

        return result;
    }
};

int main()
{
    ASSERT( Solution().run(nullptr) == 0 );

    ASSERT( Solution().run(TemporaryTree<TreeNode>(new TreeNode{0})) == 0 );
    ASSERT( Solution().run(TemporaryTree<TreeNode>(new TreeNode{1})) == 1 );
    ASSERT( Solution().run(TemporaryTree<TreeNode>(new TreeNode{9})) == 9 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{0,
                new TreeNode{1},
                nullptr
            }
        )) == 1 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{0,
                new TreeNode{0,
                    new TreeNode{1},
                    nullptr
                },
                nullptr
            }
        )) == 1 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{0},
                nullptr
            }
        )) == 10 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                nullptr,
                new TreeNode{0}
            }
        )) == 10 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{9},
                nullptr
            }
        )) == 19 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{9},
                new TreeNode{1}
            }
        )) == 30 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{9},
                new TreeNode{9}
            }
        )) == 38 );

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
                },
            }
        )) == 522 );

    ASSERT( Solution().run(
        TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4,
                        new TreeNode{1,
                            nullptr,
                            new TreeNode{0,
                                new TreeNode{1},
                                nullptr
                            }
                        },
                        nullptr
                    },
                    new TreeNode{5,
                        nullptr,
                        new TreeNode{9}
                    }
                },
                new TreeNode{3,
                    new TreeNode{6,
                        new TreeNode{0},
                        new TreeNode{1}
                    },
                    new TreeNode{7,
                        new TreeNode{2},
                        nullptr
                    }
                },
            }
        )) == 129453 );

    return 0;
}

}
