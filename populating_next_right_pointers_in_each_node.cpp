// Problem: https://leetcode.com/problems/populating-next-right-pointers-in-each-node/

#include <vector>
#include "utils.h"

namespace populating_next_right_pointers_in_each_node {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode* next;
};


// Non-recursive solution

class Solution1 {
public:
    // Sets the next pointer of each node to the next right node of the
    // same level.
    //
    // Time: O(n), Space: O(n), n - number of nodes
    //
    void run(TreeNode* root)
    {
        if (!root)
            return;

        std::vector<TreeNode*> nodes = {root};
        int level_end = nodes.size();

        for (int i = 0; i < nodes.size(); i++) {
            TreeNode* node = nodes[i];

            if (node->left)
                nodes.push_back(node->left);

            if (node->right)
                nodes.push_back(node->right);

            if (i != level_end - 1) {
                node->next = nodes[i + 1];
            } else {
                level_end = nodes.size();
            }
        }
    }
};


// Recursive solution, requires O(log(n)) memory but O(n * log(n)) time

class Solution2 {
public:
    // Sets the next pointer of each node to the next right node of the
    // same level.
    //
    // Note: The tree must be perfect, i.e. each level must have the maximum
    // possible number of nodes.
    //
    // Time: O(n * log(n)), Space: O(log(n)), Recursion depth <= ceil(log(n)) + 3
    // n - number of nodes
    //
    void run(TreeNode* root)
    {
        if (!root)
            return;

        std::vector<bool> path;
        updateTree(root, root, path);
    }

private:
    // Returns a node found by moving down from the root by the path. If
    // path[i] == false, moves left, otherwise right. If there is no such node,
    // returns null.
    //
    TreeNode* findNode(TreeNode* root, const std::vector<bool>& path) const
    {
        if (!root)
            return nullptr;

        TreeNode* result = root;
        for (int i = 0; i < path.size() && result; i++) {
            if (path[i])
                result = result->right; else
                result = result->left;
        }
        return result;
    }

    void updateTree(TreeNode* tree_root, TreeNode* node, std::vector<bool>& node_path) const
    {
        if (!tree_root || !node)
            return;

        // Find the next right node.
        // Suppose that the node_path is: [  ... , move left,  <the rest is moving right>].
        // Then the right node path is:   [<same>, move right, <the rest is moving left>].
        // So reverse the tail of the current path up to the first left move.
        std::vector<bool> right_node_path = node_path;
        for (int i = right_node_path.size() - 1; i >= 0; i--) {
            std::vector<bool>::reference bit = right_node_path[i];
            bit = !bit;
            if (!bit)
                continue;

            node->next = findNode(tree_root, right_node_path);
            if (!node->next)
                throw std::runtime_error("Missing the next right node");
            break;
        }

        // Update left and right subtrees
        const size_t prev_path_size = node_path.size();
        node_path.push_back(false);
        updateTree(tree_root, node->left, node_path);

        node_path.resize(prev_path_size);
        node_path.push_back(true);
        updateTree(tree_root, node->right, node_path);
    }
};


// Recursive solution, requires O(n) time and O(h) stack memory

#define SOLUTION3_SUPPORT_NON_PERFECT_TREES

class Solution3 {
public:
    // Sets the next pointer of each node to the next right node of the
    // same level.
    //
    // Time: O(n), Space: O(h) on stack, Recursion depth <= h + 3
    // n - number of nodes, h - height of the tree
    //
    void run(TreeNode* root)
    {
        if (!root)
            return;

        updateTree(root, nullptr);
    }

private:
    void updateTree(TreeNode* node, TreeNode* parent) const
    {
        if (!node)
            return;

        // Idea:
        // To find the next right node of the current level, we can use the
        // next pointers of the previous (parent) level.

#ifdef SOLUTION3_SUPPORT_NON_PERFECT_TREES
        if (parent) {
            if (parent->left == node && parent->right) {
                node->next = parent->right;
            } else {
                parent = parent->next;
            }
        }
        while (!node->next && parent) {
            if (parent->left) {
                node->next = parent->left;
            } else if (parent->right) {
                node->next = parent->right;
            } else {
                parent = parent->next;
            }
        }
#else
        if (parent) {
            if (parent->left == node) {
                node->next = parent->right;
            } else if (parent->next) {
                node->next = parent->next->left;
            } else {
                // This is the rightmost node
            }
            if (!node->next && parent->next)
                throw std::runtime_error("Missing the next right node");
        }
#endif

        updateTree(node->left,  node);
        updateTree(node->right, node);
    }
};


template <typename Solution>
void test(bool perfect_trees_only = true)
{
    auto test_tree = [](TreeNode* root, const std::vector<int>& expected_level_order_traversal)
    {
        Solution().run(root);

        std::vector<int> traversal;

        const TreeNode* level = root;
        while (level) {
            const TreeNode* node = level;
            level = nullptr;

            while (node) {
                if (!level) {
                    if (node->left) {
                        level = node->left;
                    } else if (node->right) {
                        level = node->right;
                    }
                }
                traversal.push_back(node->val);
                node = node->next;
            }
        }

        ASSERT_EX( traversal == expected_level_order_traversal,
            tree_to_string(root) + " -> " + to_string(expected_level_order_traversal) );

        destroy_tree(root);
    };

    test_tree(nullptr, {});

    test_tree(new TreeNode{1}, {1});

    if (!perfect_trees_only) {
        test_tree(
            new TreeNode{1,
                new TreeNode{2},
                nullptr
            },
            {1, 2}
        );

        test_tree(
            new TreeNode{1,
                nullptr,
                new TreeNode{2},
            },
            {1, 2}
        );
    }

    test_tree(
        new TreeNode{1,
            new TreeNode{2},
            new TreeNode{3},
        },
        {1, 2, 3}
    );

    if (!perfect_trees_only) {
        test_tree(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4},
                    nullptr
                },
                new TreeNode{3,
                    new TreeNode{5},
                    nullptr
                },
            },
            {1, 2, 3, 4, 5}
        );

        test_tree(
            new TreeNode{1,
                new TreeNode{2,
                    new TreeNode{4},
                    nullptr
                },
                new TreeNode{3,
                    nullptr,
                    new TreeNode{5}
                },
            },
            {1, 2, 3, 4, 5}
        );
    }

    test_tree(
        new TreeNode{1,
            new TreeNode{2,
                new TreeNode{4},
                new TreeNode{5}
            },
            new TreeNode{3,
                new TreeNode{6},
                new TreeNode{7}
            },
        },
        {1, 2, 3, 4, 5, 6, 7}
    );
}

int main()
{
    test<Solution1>(false);
    test<Solution2>(true);
#ifdef SOLUTION3_SUPPORT_NON_PERFECT_TREES
    test<Solution3>(false);
#else
    test<Solution3>(true);
#endif

    return 0;
}

}
