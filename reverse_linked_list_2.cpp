// Problem: https://leetcode.com/problems/reverse-linked-list-ii/

#include "utils.h"

namespace reverse_linked_list_2 {

struct ListNode {
    int val;
    ListNode *next;
};


class Solution {
public:
    // Note: If from < 1, or to < 1, or from >= to, does nothing and returns
    // the head. If from >= number of nodes, does not reverse but iterate over
    // the list. If to > number of nodes, does reverse to the end of the list.
    //
    // Time: O(n), Space: O(1), n - number of nodes
    //
    ListNode* run(ListNode* head, int from, int to)
    {
        if (!head || from < 1 || to < 1 || from >= to)
            return head;

        ListNode* node             = head;
        ListNode* node_prev        = nullptr;
        ListNode* node_before_from = nullptr;
        ListNode* node_from        = nullptr;
        ListNode* node_to          = nullptr;
        int       node_index       = 1;

        while (node && node_index <= to) {
            ListNode* node_next = node->next;
            if (node_index > from && node_index <= to) {
                node->next = node_prev;
            } else if (node_index == from - 1) {
                node_before_from = node;
            } else if (node_index == from) {
                node_from = node;
            } else if (node_index == to) {
                node_to = node;
            }
            node_prev = node;
            node      = node_next;
            node_index++;
        }

        if (!node_from)
            return head;

        if (!node_to)
            node_to = node_prev;

        node_from->next = node;
        if (node_before_from)
            node_before_from->next = node_to;

        if (node_from == head)
            return node_to;
        return head;
    }
};


void test(std::initializer_list<int> list, int from, int to, std::initializer_list<int> expected)
{
    ListNode* head = create_list<ListNode>(list);

    ASSERT_EX( compare_lists(head = Solution().run(head, from, to), expected),
               to_string(list) + ", " + to_string(from) + "-" + to_string(to) + " -> " + to_string(expected) );

    destroy_list(head);
}

int main()
{
    ListNode* list = create_list<ListNode>({1, 2});
    ASSERT( Solution().run(nullptr, 1, 2) == nullptr );
    ASSERT( Solution().run(list, 0, 2) == list );
    ASSERT( Solution().run(list, 1, 0) == list );
    ASSERT( Solution().run(list, 2, 1) == list );
    ASSERT( Solution().run(list, 1, 1) == list );
    ASSERT( Solution().run(list, 2, 2) == list );
    destroy_list(list);

    test({1}, 1, 1, {1});
    test({1, 2}, 1, 2, {2, 1});
    test({1, 2}, 1, 3, {2, 1});
    test({1, 2, 3}, 1, 2, {2, 1, 3});
    test({1, 2, 3}, 1, 3, {3, 2, 1});
    test({1, 2, 3}, 2, 3, {1, 3, 2});
    test({1, 2, 3, 4, 5}, 2, 3, {1, 3, 2, 4, 5});
    test({1, 2, 3, 4, 5}, 3, 4, {1, 2, 4, 3, 5});
    test({1, 2, 3, 4, 5}, 2, 4, {1, 4, 3, 2, 5});

    return 0;
}

}
