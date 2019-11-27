// Problem: https://leetcode.com/problems/partition-list/

#include "utils.h"

namespace partition_list {

struct ListNode {
    int val;
    ListNode *next;
};


class Solution {
public:
    // Time: O(n), Space: O(1), n - number of nodes
    ListNode* run(ListNode* head, int value)
    {
        if (!head)
            return nullptr;

        // Idea:
        // Rearrange nodes as follows:
        // node_lt1 -> ... -> node_lt2 -> node_ge1 -> ... -> node_ge2
        // < value            < value     >= value           >= value

        ListNode* node_lt1 = nullptr;
        ListNode* node_lt2 = nullptr;
        ListNode* node_ge1 = nullptr;
        ListNode* node_ge2 = nullptr;
        ListNode* node     = head;

        while (node) {
            if (node->val < value) {
                if (node_lt2) {
                    node_lt2->next = node;
                    node_lt2 = node;
                } else {
                    node_lt1 = node;
                    node_lt2 = node;
                }
            } else {
                if (node_ge2) {
                    node_ge2->next = node;
                    node_ge2 = node;
                } else {
                    node_ge1 = node;
                    node_ge2 = node;
                }
            }
            node = node->next;
        }

        if (node_lt2)
            node_lt2->next = node_ge1;

        if (node_ge2)
            node_ge2->next = nullptr;

        if (node_lt1)
            return node_lt1; else
            return node_ge1;
    }
};


void test(std::initializer_list<int> list, int value, std::initializer_list<int> expected)
{
    ListNode* head = create_list<ListNode>(list);

    ASSERT_EX( compare_lists(Solution().run(head, value), expected),
               to_string(list) + ", " + to_string(value) + " -> " + to_string(expected) );
}

int main()
{
    ASSERT( Solution().run(nullptr, 1) == nullptr );

    test({1}, 1, {1});
    test({1, 1}, 1, {1, 1});
    test({1, 1, 1}, 1, {1, 1, 1});

    test({1, 3}, 1, {1, 3});
    test({3, 1}, 1, {3, 1});
    test({1, 3}, 2, {1, 3});
    test({3, 1}, 2, {1, 3});
    test({1, 3}, 3, {1, 3});
    test({3, 1}, 3, {1, 3});

    test({1, 1, 3}, 2, {1, 1, 3});
    test({3, 1, 1}, 2, {1, 1, 3});
    test({1, 3, 1}, 2, {1, 1, 3});

    test({1, 1, 3, 3}, 2, {1, 1, 3, 3});
    test({3, 3, 1, 1}, 2, {1, 1, 3, 3});
    test({1, 3, 1, 3}, 2, {1, 1, 3, 3});
    test({3, 1, 3, 1}, 2, {1, 1, 3, 3});
    test({3, 1, 1, 3}, 2, {1, 1, 3, 3});
    test({1, 3, 3, 1}, 2, {1, 1, 3, 3});

    test({1, 2, 3}, 2, {1, 2, 3});
    test({1, 3, 2}, 2, {1, 3, 2});
    test({2, 1, 3}, 2, {1, 2, 3});
    test({2, 3, 1}, 2, {1, 2, 3});
    test({3, 1, 2}, 2, {1, 3, 2});
    test({3, 2, 1}, 2, {1, 3, 2});

    test({1, 2, 3, 4}, 3, {1, 2, 3, 4});
    test({4, 3, 2, 1}, 3, {2, 1, 4, 3});
    test({1, 3, 4, 2}, 3, {1, 2, 3, 4});
    test({4, 2, 1, 3}, 3, {2, 1, 4, 3});
    test({3, 1, 4, 2}, 3, {1, 2, 3, 4});
    test({2, 4, 1, 3}, 3, {2, 1, 4, 3});

    test({3, 1, 4, 3, 4, 1, 5, 3, 2}, 3, {1, 1, 2, 3, 4, 3, 4, 5, 3});

    return 0;
}

}
