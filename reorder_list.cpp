// Problem: https://leetcode.com/problems/reorder-list/

#include <vector>
#include "utils.h"

namespace reorder_list {

struct ListNode {
    int val;
    ListNode *next;
};


// Using O(n) extra space

class Solution1 {
public:
    // Time: O(n), Space: O(n), n - number of nodes
    void run(ListNode* head)
    {
        if (!head)
            return;

        std::vector<ListNode*> nodes;

        ListNode* node = head;
        while (node) {
            nodes.push_back(node);
            node = node->next;
        }

        for (int i1 = 0, i2 = nodes.size() - 1; i1 < i2; i1++, i2--) {
            ListNode* n1 = nodes[i1];
            ListNode* n2 = nodes[i2];
            n2->next = n1->next;
            n1->next = n2;
        }

        ListNode* last_node = nodes[nodes.size() / 2];
        last_node->next = nullptr;
    }
};


// Using O(1) extra space

class Solution2 {
public:
    // Time: O(n^2), Space: O(1), n - number of nodes
    void run(ListNode* head)
    {
        while (head) {
            // Find the current end and the next end
            ListNode* next_end = nullptr;
            ListNode* end      = head;
            while (end->next) {
                next_end = end;
                end      = end->next;
            }

            // Connect the next end to null (if there are > 2 nodes left)
            if (end != head && end != head->next) {
                next_end->next = nullptr;
            } else {
                break;
            }

            // Put the end after the head
            ListNode* next_head = head->next;
            end->next  = head->next;
            head->next = end;

            // Continue with the next head
            head = next_head;
        }
    }
};


template <typename Solution>
void test()
{
    auto test = [](std::initializer_list<int> list, std::initializer_list<int> expected)
    {
        ListNode* head = create_list<ListNode>(list);

        Solution().run(head);
        ASSERT_EX( compare_lists(head, expected), to_string(list) + " -> " + to_string(expected) );

        destroy_list(head);
    };

    ASSERT(( Solution().run(nullptr), true ));

    test({1}, {1});
    test({1, 2}, {1, 2});
    test({1, 2, 3}, {1, 3, 2});
    test({1, 2, 3, 4}, {1, 4, 2, 3});
    test({1, 2, 3, 4, 5}, {1, 5, 2, 4, 3});
    test({0, -1, 2, -3, 4}, {0, 4, -1, -3, 2});
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
