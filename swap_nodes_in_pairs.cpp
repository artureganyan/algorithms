// Problem: https://leetcode.com/problems/swap-nodes-in-pairs/

#include "utils.h"

namespace swap_nodes_in_pairs {

struct ListNode {
    int val;
    ListNode *next;
};

class Solution
{
public:
    // Time: O(n), Space: O(1)
    ListNode* run(ListNode* head)
    {
        if (!head)
            return nullptr;

        if (!head->next)
            return head;

        ListNode* result = head->next;
        ListNode* n1     = head;
        ListNode* n2     = head->next;

        while (n2) {
            ListNode* n3 = n2->next;
            ListNode* n4 = n3 ? n3->next : nullptr;

            n2->next = n1;
            n1->next = n4 ? n4 : n3;

            n1 = n3;
            n2 = n4;
        }

        return result;
    }

    // Probably more straightforward way.
    //
    // Time: O(n), Space: O(1)
    //
    ListNode* run2(ListNode* head)
    {
        if (!head)
           return nullptr;

        if (!head->next)
            return head;

        ListNode* result    = head->next;
        ListNode* n0        = nullptr;
        ListNode* n1        = nullptr;
        ListNode* n2        = head;

        int i = 1;
        while (n2) {
            i++;

            n0 = n1;
            n1 = n2;
            n2 = n2->next;

            if (i % 2)
                continue;

            if (!n2)
                break;

            n1->next = n2->next;
            n2->next = n1;
            std::swap(n1, n2);

            if (n0)
                n0->next = n1;
        }

        return result;
    }
};

int main()
{
    auto methods = {&Solution::run, &Solution::run2};

    for (auto method : methods) {
        ASSERT(compare_lists((Solution().*method)(create_temp_list<ListNode>({1, 2, 3, 4})), {2, 1, 4, 3}));
        ASSERT(compare_lists((Solution().*method)(create_temp_list<ListNode>({1})), {1}));
        ASSERT(compare_lists((Solution().*method)(create_temp_list<ListNode>({1, 2})), {2, 1}));
        ASSERT(compare_lists((Solution().*method)(create_temp_list<ListNode>({1, 2, 3})), {2, 1, 3}));
        ASSERT(compare_lists((Solution().*method)(nullptr), {}));
    }
    return 0;
}

}
