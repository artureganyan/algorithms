// Problem: https://leetcode.com/problems/path-sum-ii/

#include <vector>
#include "utils.h"

namespace path_sum_2 {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Time: O(avg_leaf_level * leaf_count), Space: O(avg_leaf_level * leaf_count)
    std::vector<std::vector<int>> run(TreeNode* root, int target_sum)
    {
        if (!root)
            return {};

        std::vector<std::vector<int>> result;

        std::vector<int> current_path;
        findPaths(result, root, target_sum, current_path, 0);

        return result;
    }

private:
    void findPaths(std::vector<std::vector<int>>& result, const TreeNode* root, int target_sum,
        std::vector<int>& current_path, int current_sum) const
    {
        if (!root)
            return;

        const int root_val = root->val;
        current_sum += root_val;
        current_path.push_back(root_val);

        if (root->left || root->right) {
            std::vector<int>& current_path_left  = current_path;
            std::vector<int>  current_path_right = current_path;

            findPaths(result, root->left,  target_sum, current_path_left,  current_sum);
            findPaths(result, root->right, target_sum, current_path_right, current_sum);

        } else if (current_sum == target_sum) {
            result.push_back(current_path);
        }
    }
};

int main()
{
    ASSERT( Solution().run(nullptr, 0) == std::vector<std::vector<int>>{} );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1}
            ), 1), {{1}}) );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1}
            ), 0), {}) );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{2},
                    nullptr
                }
            ), 3), {{1, 2}}) );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{2},
                    nullptr
                }
            ), 1), {}) );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{2},
                    nullptr
                }
            ), 2), {}) );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{1},
                    new TreeNode{2}
                }
            ), 3), {{1, 2}}) );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{2},
                    new TreeNode{2}
                }
            ), 3), {{1, 2}, {1, 2}}) );

    ASSERT( compare_sets(Solution().run(TemporaryTree<TreeNode>(
                new TreeNode{1,
                    new TreeNode{2,
                        new TreeNode{-1,
                            nullptr,
                            new TreeNode{3}
                        },
                        nullptr
                    },
                    new TreeNode{2,
                        new TreeNode{2,
                            new TreeNode{0},
                            new TreeNode{0,
                                new TreeNode{0},
                                nullptr
                            }
                        },
                        new TreeNode{2}
                    }
                }
            ), 5), {{1, 2, -1, 3}, {1, 2, 2, 0}, {1, 2, 2, 0, 0}, {1, 2, 2}}) );

    return 0;
}

}
