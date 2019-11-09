// Problem: https://leetcode.com/problems/rotate-list/

#include <vector>
#include "utils.h"

namespace rotate_list {

struct ListNode {
    int val;
    ListNode *next;
};


class Solution1 {
public:
    // Time: O(n), Space: O(1), n - number of nodes
    //
    // Note: For k in [1, n], it requires 2*n - k iterations. More formally:
    // for k > 0, it requires 2*n - (k mod n) iterations if (k mod n) != 0,
    // otherwise n iterations.
    //
    ListNode* run(ListNode* head, int k)
    {
        if (!head)
            return nullptr;

        if (k <= 0)
            return head;

        // Count nodes and find the end node
        ListNode* end   = head;
        int       count = 1;

        while (end->next) {
            end = end->next;
            count++;
        }

        k = k % count;
        if (k == 0)
            return head;

        // Find new_end and new_head nodes:
        //
        // n1 -> n2 -> ... -> n(count-k) -> n(count-k+1)... -> n(count) -> 0
        // |                  |             |                  |
        // head            new_end       new_head             end
        //
        ListNode* new_end = head;
        int       index   = 0;

        while (index < count - k - 1) {
            new_end = new_end->next;
            index++;
        }
        ListNode* new_head = new_end->next;

        // Rotate list
        new_end->next = end->next;
        end->next     = head;
        return new_head;
    }
};


// Optimized version of the Solution1

class Solution2 {
public:
    // Time: O(n), Space: O(1), n - number of nodes
    //
    // Note: For k in [1, n] or any k = C*n, C - integer > 0,
    // it requires n iterations, otherwise 2*n - (k mod n).
    //
    ListNode* run(ListNode* head, int k)
    {
        if (!head)
            return nullptr;

        if (k <= 0)
            return head;

        // Find the end, new_end and new_head nodes. This can be done in 1 pass
        // if k <= count, otherwise the second pass is required:
        //
        // n1 -> n2 -> ... -> n(count-k) -> n(count-k+1)... -> n(count) -> 0
        // |                  |             |                  |
        // head            new_end       new_head             end
        //
        ListNode* end     = head;
        ListNode* new_end = nullptr;
        int       count   = 1;

        while (end->next) {
            end = end->next;
            count++;

            if (new_end) {
                new_end = new_end->next;
            } else if (count == k + 1) {
                new_end = head;
            }
        }

        k = k % count;
        if (k == 0)
            return head;

        // If new_end was not obtained with the first pass, do the second one
        if (!new_end) {
            new_end = head;
            for (int i = 0; i < count - k - 1; i++)
                new_end = new_end->next;
        }
        ListNode* new_head = new_end->next;

        // Rotate list
        new_end->next = end->next;
        end->next     = head;
        return new_head;
    }
};


template <typename Solution>
void test()
{
    ASSERT( Solution().run(nullptr, 1) == nullptr );

    ListNode* head = create_list<ListNode>({1});
    ASSERT( Solution().run(head, -1) == head );
    ASSERT( Solution().run(head, 0) == head );
    ASSERT( Solution().run(head, 1) == head );
    ASSERT( Solution().run(head, 2) == head );
    destroy_list(head);

    head = create_list<ListNode>({1, 2, 3});
    ASSERT( compare_lists(head = Solution().run(head, 0), {1, 2, 3}) );
    ASSERT( compare_lists(head = Solution().run(head, 1), {3, 1, 2}) );
    ASSERT( compare_lists(head = Solution().run(head, 1), {2, 3, 1}) );
    ASSERT( compare_lists(head = Solution().run(head, 1), {1, 2, 3}) );
    ASSERT( compare_lists(head = Solution().run(head, 3), {1, 2, 3}) );
    ASSERT( compare_lists(head = Solution().run(head, 4), {3, 1, 2}) );
    ASSERT( compare_lists(head = Solution().run(head, 2), {1, 2, 3}) );
    ASSERT( compare_lists(head = Solution().run(head, 6), {1, 2, 3}) );
    destroy_list(head);
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
