// Problem: https://leetcode.com/problems/insertion-sort-list/

#include "utils.h"

namespace insertion_sort_list {

struct Node {
    int val;
    Node* next;
};

class Solution {
public:
    // Time: O(n^2), Space: O(1), n - number of nodes
    Node* run(Node* head)
    {
        if (!head)
            return nullptr;

        Node* head_sorted = head;
        Node* last_sorted = head;

        while (last_sorted && last_sorted->next) {
            // Find the insertion pos
            Node* node         = head_sorted;
            Node* insert_node  = last_sorted->next;
            Node* insert_after = nullptr;

            while (node != insert_node) {
                if (node->val >= insert_node->val)
                    break;
                insert_after = node;
                node         = node->next;
            }

            // Insert the node
            if (insert_after) {
                if (insert_after != last_sorted) {
                    last_sorted->next  = insert_node->next;
                    insert_node->next  = insert_after->next;
                    insert_after->next = insert_node;
                } else {
                    last_sorted        = insert_node;
                }
            } else {
                last_sorted->next = insert_node->next;
                insert_node->next = head_sorted;
                head_sorted       = insert_node;
            }
        }

        return head_sorted;
    }
};


void test(std::initializer_list<int> list, std::initializer_list<int> expected)
{
    ASSERT_EX( compare_lists(Solution().run(create_temp_list<Node>(list)), expected),
        to_string(list) + " -> " + to_string(expected) );
}

int main()
{
    ASSERT( Solution().run(nullptr) == nullptr );

    test({ 0}, { 0});
    test({ 1}, { 1});
    test({-1}, {-1});

    test({0, 1}, {0, 1});
    test({1, 0}, {0, 1});
    test({1, 1}, {1, 1});

    test({1, 1, 1}, {1, 1, 1});

    test({1, 0, 2}, {0, 1, 2});
    test({1, 2, 0}, {0, 1, 2});
    test({0, 1, 2}, {0, 1, 2});
    test({0, 2, 1}, {0, 1, 2});
    test({2, 0, 1}, {0, 1, 2});
    test({2, 1, 0}, {0, 1, 2});

    test({1, -1, 2, 0, 4, 3, 1, 3}, {-1, 0, 1, 1, 2, 3, 3, 4});

    return 0;
}

}
