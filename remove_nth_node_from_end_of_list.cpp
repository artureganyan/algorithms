// Problem: https://leetcode.com/problems/remove-nth-node-from-end-of-list/

#include "utils.h"

namespace remove_nth_node_from_end_of_list {

struct ListNode {
    int val;
    ListNode *next;
};

class Solution1 {
public:
    // Time: O(n), Space: O(1)
    //
    // Note: n must be > 0 (n = 1 means the last node)
    //
    ListNode* run(ListNode* head, int n)
    {
        if (!head || n <= 0)
            return nullptr;

        ListNode  before_head   = {0, head};
        ListNode* before_target = &before_head;
        ListNode* node          = head;

        int i = 1;
        while (node->next) {
            node = node->next;
            if (++i > n)
                before_target = before_target->next;
        }

        if (i < n)
            return nullptr;

        if (i == n)
            head = head->next;

        ListNode* target = before_target->next;
        before_target->next = target->next;
        delete target;

        return head;
    }
};


// Does 2 passes

class Solution2 {
public:
    // Time: O(n), Space: O(1)
    //
    // Note: n must be > 0 (n = 1 means the last node)
    //
    ListNode* run(ListNode* head, int n)
    {
        if (!head || n <= 0)
            return nullptr;

        // Count all nodes
        ListNode* node = head;

        int count = 0;
        while (node) {
            count++;
            node = node->next;
        }

        const int target_index = count - n;
        if (target_index < 0)
            return nullptr;

        // Remove nth node from the end
        ListNode* node_prev = nullptr;
        node = head;

        int i = 0;
        while (i < target_index) {
            node_prev = node;
            node = node->next;
            i++;
        }

        if (node_prev) {
            node_prev->next = node->next;
        } else {
            head = node->next;
        }
        delete node;

        return head;
    }
};


template <typename Solution>
void test()
{
    ListNode *head = new ListNode{0, new ListNode{1, new ListNode{2, new ListNode{3, 0}}}};
    ASSERT(compare_lists(Solution().run(head, 5), {}));
    ASSERT(compare_lists(Solution().run(head, 0), {}));
    ASSERT(compare_lists(head = Solution().run(head, 4), {1, 2, 3}));
    ASSERT(compare_lists(head = Solution().run(head, 1), {1, 2}));
    ASSERT(compare_lists(head = Solution().run(head, 2), {2}));
    ASSERT(compare_lists(head = Solution().run(head, 1), {}));
    ASSERT(compare_lists(Solution().run(nullptr, 0), {}));
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
