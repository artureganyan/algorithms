// Problem: https://leetcode.com/problems/sort-list/

#include "utils.h"

namespace sort_list {

struct ListNode {
    int val;
    ListNode *next;
};


// Quicksort
//
// Note: The original problem requires O(n * log(n)) time and O(1) space, so
// this solution does not fit for both time (in the worst case) and space (as
// it uses stack).

class Solution1 {
public:
    // Time: O(n * log(n)) in average, O(n^2) in the worst case (e.g. {2, 1, 4, 3, 6, 5, ...})
    // Space: O(log(n)) in average, O(n) in the worst case, on stack
    // Recursion depth <= n + 2
    // n - number of nodes
    //
    ListNode* run(ListNode* head)
    {
        return sort(head, nullptr);
    }

private:
    ListNode* sort(ListNode* begin, ListNode* end) const
    {
        if (!begin)
            return nullptr;

        if (begin == end)
            return begin;

        const auto pivot_value  = begin->val;
        ListNode*  new_begin    = begin;
        ListNode*  node         = begin;
        ListNode*  node_prev    = nullptr;

        while (node != end) {
            ListNode* node_next = node->next;
            if (node->val < pivot_value) {
                if (node_prev) {
                    node_prev->next = node->next;
                }
                node->next = new_begin;
                new_begin  = node;
            } else {
                node_prev = node;
            }
            node = node_next;
        }

        new_begin   = sort(new_begin, begin);
        begin->next = sort(begin->next, end);

        return new_begin;
    }
};


// Merge sort

class Solution2 {
public:
    // Time: O(n * log(n)), Space: O(1), n - number of nodes
    ListNode* run(ListNode* head)
    {
        if (!head)
            return nullptr;

        // Get the list size
        int       list_size = 0;
        ListNode* node      = head;
        while (node) {
            node = node->next;
            list_size++;
        }

        // Do the merge sort
        ListNode* new_head = head;

        for (int block_size = 1; block_size < list_size; block_size *= 2) {
            // Merge blocks {n1 -> ...} -> {n2 -> ...}, each of the block_size
            ListNode* n1      = new_head;
            ListNode* n1_prev = nullptr;

            while (n1) {
                // Get the block {n2 -> ...}
                ListNode* n2      = n1;
                ListNode* n2_prev = n1_prev;

                int s1 = block_size;
                int s2 = 0;
                while (n2 && s2 < block_size) {
                    n2_prev = n2;
                    n2      = n2->next;
                    s2++;
                }
                if (s2 != block_size)
                    break;

                // Merge blocks
                while (n1 && n2 && s1 && s2) {
                    if (n1->val > n2->val) {
                        ListNode* n2_next = n2->next;

                        if (n2_prev)
                            n2_prev->next = n2->next;

                        if (n1_prev) {
                            n1_prev->next = n2;
                        } else {
                            new_head = n2;
                        }

                        n2->next = n1;
                        n2       = n2_next;
                        s2--;
                    } else {
                        n1_prev = n1;
                        n1      = n1->next;
                        s1--;
                    }
                }

                // Get the next block {n1 -> ...}
                while (n2 && s2--) {
                    n2_prev = n2;
                    n2      = n2->next;
                }
                n1      = n2;
                n1_prev = n2_prev;
            }
        }

        return new_head;
    }
};


template <typename Solution>
void test()
{
    const auto test = [](std::initializer_list<int> list, std::initializer_list<int> expected)
    {
        TemporaryList<ListNode> temp_list = create_temp_list<ListNode>(list);

        ASSERT_EX( compare_lists(Solution().run(temp_list), expected),
            to_string(list) + " ->" + to_string(expected) );
    };

    ASSERT( Solution().run(nullptr) == nullptr );

    test({ 0}, { 0});
    test({ 1}, { 1});
    test({-1}, {-1});

    test({1, 1}, {1, 1});

    test({1, 2}, {1, 2});
    test({2, 1}, {1, 2});

    test({1, 2, 3}, {1, 2, 3});
    test({1, 3, 2}, {1, 2, 3});
    test({2, 1, 3}, {1, 2, 3});
    test({2, 3, 1}, {1, 2, 3});
    test({3, 1, 2}, {1, 2, 3});
    test({3, 2, 1}, {1, 2, 3});

    test({2, -1, 3, 0, 4, 1}, {-1, 0, 1, 2, 3, 4});
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
