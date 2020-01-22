// Problem: https://leetcode.com/problems/binary-search-tree-iterator/

#include <vector>
#include "utils.h"

namespace binary_search_tree_iterator {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// Note: Actually, the iterator does not need the tree to be a binary-search
// tree (BST), it only does the inorder traversal. So it could be renamed to
// e.g. InorderTreeIterator.
//
// Time:  O(1) in average and O(h) in the worst case for next(),
//        O(1) for hasNext(),
//        O(h) for the constructor
// Space: O(h) during the iterator lifetime
// h - height of the tree
//
// Note: We visit each leaf node 1 time and each non-leaf node 3 times (when
// going to its leftmost leaf and when returning to the node from its subtrees).
// So, if iterate over the whole tree, we do < 3*n visits for n iterations,
// i.e. O(1) time per iteration in average. In the worst case we visit h+1
// nodes on a single iteration, to reach the leftmost leaf or to return to the
// tree root, and this requires O(h) time.

class BSTIterator {
public:
    BSTIterator(TreeNode* root)
    {
        next_node_ = root;
        goToSmallestNode();
    }

    int next()
    {
        const auto value = next_node_->val;
        goToLargerNode();
        return value;
    }

    bool hasNext() const
    {
        return next_node_ != nullptr;
    }

private:
    void goToSmallestNode()
    {
        if (!next_node_)
            return;

        while (const TreeNode* left = next_node_->left) {
            parent_nodes_.push_back(next_node_);
            next_node_ = left;
        }
    }

    void goToLargerNode()
    {
        if (!next_node_)
            return;

        // Go to the smallest node of the right subtree
        if (const TreeNode* right_child = next_node_->right) {
            parent_nodes_.push_back(next_node_);
            next_node_ = right_child;
            goToSmallestNode();
            return;
        }

        // Go to the first larger parent node
        while (parent_nodes_.size()) {
            const TreeNode* parent    = parent_nodes_.back();
            const bool      is_larger = (parent->left == next_node_);

            parent_nodes_.pop_back();
            next_node_ = parent;

            if (is_larger)
                return;
        }

        // There is no larger node
        next_node_ = nullptr;
    }

    std::vector<const TreeNode*> parent_nodes_;
    const TreeNode*              next_node_;
};


void test(TreeNode* root, const std::vector<int>& expected)
{
    std::vector<int> result;

    BSTIterator it = root;
    while (it.hasNext())
        result.push_back(it.next());

    ASSERT_EX( result == expected,
        tree_to_string(root) + " -> " + to_string(expected) );
}

int main()
{
    test(nullptr, {});

    test(TemporaryTree<TreeNode>(
             new TreeNode{1}
        ), {1});

    test(TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{-1},
                nullptr
            }
        ), {-1, 1});

    test(TemporaryTree<TreeNode>(
            new TreeNode{1,
                nullptr,
                new TreeNode{2}
            }
        ), {1, 2});

    test(TemporaryTree<TreeNode>(
            new TreeNode{1,
                new TreeNode{-1},
                new TreeNode{2}
            }
        ), {-1, 1, 2});

    test(TemporaryTree<TreeNode>(
             new TreeNode{3,
                new TreeNode{2,
                    new TreeNode{1},
                    nullptr
                },
                new TreeNode{4}
            }
        ), {1, 2, 3, 4});

    test(TemporaryTree<TreeNode>(
             new TreeNode{4,
                new TreeNode{3,
                    new TreeNode{1,
                        new TreeNode{0},
                        new TreeNode{2}
                    },
                    nullptr
                },
                new TreeNode{5,
                    nullptr,
                    new TreeNode{6}}
            }
        ), {0, 1, 2, 3, 4, 5, 6});

    return 0;
}

}
