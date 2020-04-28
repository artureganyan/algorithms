// Problem: https://leetcode.com/problems/odd-even-linked-list/

#include "utils.h"

namespace odd_even_linked_list {

struct ListNode {
    int val;
    ListNode *next;
};

class Solution {
public:
    // Time: O(n), Space: O(1), n - number of nodes
    ListNode* run(ListNode* head)
    {
        if (!head)
            return nullptr;

        ListNode* head_odd  = head;
        ListNode* tail_odd  = head_odd;
        ListNode* head_even = head->next;
        ListNode* tail_even = head_even;

        if (!head_even)
            return head_odd;

        ListNode* node       = head_even->next;
        int       node_index = 3;

        while (node) {
            if (node_index % 2) {
                tail_odd->next  = node;
                tail_odd        = node;
            } else {
                tail_even->next = node;
                tail_even       = node;
            }
            node = node->next;
            node_index++;
        }

        tail_odd->next  = head_even;
        tail_even->next = nullptr;

        return head_odd;
    }
};


void test(std::initializer_list<int> source, std::initializer_list<int> expected)
{
    ListNode* source_list = create_list<ListNode>(source);
    ListNode* result      = Solution().run(source_list);

    ASSERT_EX( compare_lists(result, expected),
        to_string(source) + " -> " + to_string(expected) );

    destroy_list(result);
}

int main()
{
    ASSERT( Solution().run(nullptr) == nullptr );

    test({1}, {1});
    test({1, 2}, {1, 2});
    test({1, 2, 3}, {1, 3, 2});
    test({1, 2, 3, 4}, {1, 3, 2, 4});
    test({1, 2, 3, 4, 5}, {1, 3, 5, 2, 4});
    test({1, 2, 3, 4, 5, 6}, {1, 3, 5, 2, 4, 6});

    return 0;
}

}
