// Problem: https://leetcode.com/problems/unique-binary-search-trees-ii/

#include <vector>
#include "utils.h"

namespace unique_binary_search_trees_2 {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

}

template <>
std::string to_string(unique_binary_search_trees_2::TreeNode* const& tree)
{
    return tree_to_string(tree);
}


namespace unique_binary_search_trees_2 {

class Solution {
public:
    // Note: Returned trees contain shared nodes, i.e. one node may be
    // referenced by multiple trees.
    //
    std::vector<TreeNode*> run(int n)
    {
        if (n < 1)
            return {};

        std::vector<TreeNode*> result;
        generate(result, 1, n);
        return result;
    }

private:
    void generate(std::vector<TreeNode*>& result, int min, int max) const
    {
        if (min > max) {
            result.push_back(nullptr);
            return;
        }

        for (int i = min; i <= max; i++) {
            std::vector<TreeNode*> subtree_left;
            generate(subtree_left, min, i - 1);

            std::vector<TreeNode*> subtree_right;
            generate(subtree_right, i + 1, max);

            for (TreeNode* left : subtree_left) {
                for (TreeNode* right : subtree_right) {
                    result.push_back(new TreeNode{i, left, right});
                }
            }
        }
    }
};


void test(int n, const std::vector<TreeNode*>& expected)
{
    auto compare =
        static_cast<bool (*)(const std::vector<TreeNode*>&, const std::vector<TreeNode*>&)>(
            compare_sets<std::vector<TreeNode*>, compare_trees_functor<TreeNode>>);

    std::vector<TreeNode*> result = Solution().run(n);
    ASSERT_EX( compare(result, expected), to_string(n) + " -> " + to_string(result) );

    destroy_trees(result);
    destroy_trees(expected);
}

int main()
{
    test(0, {});

    test(1, {new TreeNode{1}});

    test(2, {
        new TreeNode{1,
            nullptr,
            new TreeNode{2}
        },
        new TreeNode{2,
            new TreeNode{1},
            nullptr
        }
    });

    test(3, {
        new TreeNode{1,
            nullptr,
            new TreeNode{2,
                nullptr,
                new TreeNode{3}
            }
        },
        new TreeNode{1,
            nullptr,
            new TreeNode{3,
                new TreeNode{2},
                nullptr
            }
        },
        new TreeNode{2,
            new TreeNode{1},
            new TreeNode{3}
        },
        new TreeNode{3,
            new TreeNode{1,
                nullptr,
                new TreeNode{2}
            },
            nullptr
        },
        new TreeNode{3,
            new TreeNode{2,
                new TreeNode{1},
                nullptr
            },
            nullptr
        }
    });

    return 0;
}

}
