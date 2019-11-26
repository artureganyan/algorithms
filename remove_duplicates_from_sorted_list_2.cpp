// Problem: https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/

#include <vector>
#include "utils.h"

namespace remove_duplicates_from_sorted_list_2 {

struct ListNode {
    int val;
    ListNode *next;
};


class Solution {
public:
    // Time: O(n), Space: O(1), n - number of nodes
    //
    // Note: The list must be sorted.
    //
    ListNode* run(ListNode* head)
    {
        if (!head)
            return nullptr;

        int duplicates_count = 0;
        int prev_value = head->val;

        ListNode* before_duplicate = nullptr;
        ListNode* node = head;
        do {
            node = node->next;

            if (node) {
                if (node->val != prev_value) {
                    prev_value = node->val;
                } else {
                    duplicates_count++;
                    continue;
                }
            }

            if (duplicates_count == 0) {
                if (before_duplicate)
                    before_duplicate = before_duplicate->next; else
                    before_duplicate = head;
                continue;
            }

            ListNode* duplicate = nullptr;
            if (before_duplicate) {
                duplicate = before_duplicate->next;
                before_duplicate->next = node;
            } else {
                duplicate = head;
                head = node;
            }

            while (duplicates_count) {
                ListNode* next = duplicate->next;
                delete duplicate;
                duplicate = next;
                duplicates_count--;
            }
        } while (node);

        return head;
    }
};


void test(std::initializer_list<int> list, std::initializer_list<int> expected)
{
    ListNode* head = create_list<ListNode>(list);

    ASSERT_EX( compare_lists(Solution().run(head), expected),
               to_string(list) + " -> " + to_string(expected) );
}

int main()
{
    ASSERT( Solution().run(nullptr) == nullptr );

    test({1}, {1});
    test({1, 2}, {1, 2});
    test({1, 2, 3}, {1, 2, 3});

    test({1, 1}, {});
    test({1, 1, 1}, {});
    test({1, 1, 2}, {2});
    test({1, 1, 1, 2}, {2});
    test({1, 1, 2, 2}, {});
    test({1, 1, 2, 3, 3}, {2});
    test({1, 1, 2, 2, 3}, {3});
    test({1, 2, 2, 3, 3}, {1});
    test({1, 2, 2, 3, 4, 4}, {1, 3});
    test({1, 1, 2, 3, 3, 4}, {2, 4});
    test({1, 2, 2, 3, 3, 4}, {1, 4});
    test({1, 1, 2, 3, 4, 4}, {2, 3});

    return 0;
}

}
