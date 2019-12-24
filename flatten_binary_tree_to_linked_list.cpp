// Problem: https://leetcode.com/problems/flatten-binary-tree-to-linked-list/

#include <vector>
#include "utils.h"

namespace flatten_binary_tree_to_linked_list {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

class Solution {
public:
    // Transforms the tree as follows:
    //
    //      root             root
    //     /    \           /    \
    //  left    right     null  flatten(left subtree)
    // subtree subtree             \
    //                            flatten(right subtree)
    //
    // Time: O(n), Space: O(h), Recursion depth <= h + 3
    // n - number of nodes, h - height of the tree
    //
    void run(TreeNode* root)
    {
        flatten(root);
    }

private:
    // Returns the last (the innermost) node of the flattened tree
    TreeNode* flatten(TreeNode* root) const
    {
        if (!root)
            return nullptr;

        // Flatten the subtrees
        TreeNode* last_left  = flatten(root->left);
        TreeNode* last_right = flatten(root->right);

        // Flatten the tree
        if (last_left) {
            last_left->right = root->right;
            root->right      = root->left;
            root->left       = nullptr;
        } else {
            // Already flattened
        }

        // Return the last node
        if (last_right)
            return last_right;

        if (last_left)
            return last_left;

        return root;
    }
};


void test(TreeNode* source, TreeNode* expected)
{
    const std::string source_str = tree_to_string(source);

    Solution().run(source);

    ASSERT_EX( compare_trees(source, expected),
        source_str + " -> " + tree_to_string(expected) );

    destroy_tree(source);
    destroy_tree(expected);
}

int main()
{
    test(nullptr, nullptr);

    test(new TreeNode{1}, new TreeNode{1});

    test(
        new TreeNode{1,
            new TreeNode{1},
            nullptr
        },
        new TreeNode{1,
            nullptr,
            new TreeNode{1}
        }
    );

    test(
        new TreeNode{1,
            new TreeNode{-2},
            nullptr
        },
        new TreeNode{1,
            nullptr,
            new TreeNode{-2}
        }
    );

    test(
        new TreeNode{1,
            nullptr,
            new TreeNode{-2}
        },
        new TreeNode{1,
            nullptr,
            new TreeNode{-2}
        }
    );

    test(
        new TreeNode{1,
            new TreeNode{2},
            new TreeNode{3}
        },
        new TreeNode{1,
            nullptr,
            new TreeNode{2,
                nullptr,
                new TreeNode{3}
            }
        }
    );

    test(
        new TreeNode{1,
            new TreeNode{2,
                new TreeNode{3},
                new TreeNode{4}
            },
            new TreeNode{5,
                new TreeNode{6,
                    new TreeNode{7},
                    nullptr
                },
                new TreeNode{8,
                    nullptr,
                    new TreeNode{9}
                }
            }
        },
        new TreeNode{1,
            nullptr,
            new TreeNode{2,
                nullptr,
                new TreeNode{3,
                    nullptr,
                    new TreeNode{4,
                        nullptr,
                        new TreeNode{5,
                            nullptr,
                            new TreeNode{6,
                                nullptr,
                                new TreeNode{7,
                                    nullptr,
                                    new TreeNode{8,
                                        nullptr,
                                        new TreeNode{9}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    );

    return 0;
}

}
