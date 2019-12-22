// Problem: https://leetcode.com/problems/convert-sorted-list-to-binary-search-tree/

#include <vector>
#include "utils.h"

namespace convert_sorted_list_to_binary_search_tree {

struct ListNode {
    int val;
    ListNode *next;
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};


class Solution {
public:
    // Time: O(n), Space: O(n), Recursion depth <= floor(log(n)) + 3, n - number of nodes
    TreeNode* run(ListNode* head)
    {
        if (!head)
            return nullptr;

        // Convert list to vector
        std::vector<ListNode*> nodes;
        ListNode* node = head;
        while (node) {
            nodes.push_back(node);
            node = node->next;
        }

        // Create tree
        return createSearchTree(nodes.begin(), nodes.end());
    }

private:
    TreeNode* createSearchTree(const std::vector<ListNode*>::const_iterator& nodes_begin,
                               const std::vector<ListNode*>::const_iterator& nodes_end) const
    {
        if (nodes_begin == nodes_end)
            return nullptr;

        auto root_node = nodes_begin + (nodes_end - nodes_begin) / 2;

        TreeNode* root = new TreeNode{(*root_node)->val};
        root->left  = createSearchTree(nodes_begin, root_node);
        root->right = createSearchTree(++root_node, nodes_end);

        return root;
    }
};


void test(std::initializer_list<int> values)
{
    ListNode* list   = create_list<ListNode>(values);
    TreeNode* result = Solution().run(list);

    ASSERT_EX( is_balanced_search_tree(result, true), to_string(values) );

    destroy_list(list);
    destroy_tree(result);
}

int main()
{
    ASSERT( Solution().run(nullptr) == nullptr );

    test({1});
    test({-1});
    test({1, 2});
    test({-1, 2});
    test({1, 2, 3});
    test({1, 2, 3, 4});
    test({1, 2, 3, 4, 5});
    test({1, 3, 5, 6, 8});

    test({1, 1});
    test({1, 1, 1});
    test({1, 1, 1, 1});
    test({1, 1, 2, 2});

    return 0;
}

}
