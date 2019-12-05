// Problem: https://leetcode.com/problems/add-two-numbers/

#include "utils.h"

namespace add_two_numbers {

struct ListNode {
    int val;
    ListNode *next;
};


class Solution {
public:
    // Note: Numbers must be stored in reverse order (e.g. 10 as 0 -> 1).
    // If any of numbers is null, returns null.
    //
    // Time: O(s1 + s2), Space: O(max(s1, s2))
    // s1 - number of elements in n1
    // s2 - number of elements in n2
    //
    ListNode* run(ListNode* n1, ListNode* n2)
    {
        if (!n1 || !n2)
            return nullptr;

        ListNode* result_head = nullptr;
        ListNode* result      = nullptr;
        int       carry       = 0;

        while (n1 || n2) {
            int value = carry;
            if (n1) {
                value += n1->val;
                n1     = n1->next;
            }
            if (n2) {
                value += n2->val;
                n2     = n2->next;
            }
            carry = value / 10;
            value = value % 10;

            ListNode* r = new ListNode{value, nullptr};
            if (result)
                result->next = r; else
                result_head  = r;
            result = r;
        }

        if (carry)
            result->next = new ListNode{carry, nullptr};

        return result_head;
    }
};


void test(std::initializer_list<int> n1, std::initializer_list<int> n2, std::initializer_list<int> expected)
{
    ListNode* n1_list = create_list<ListNode>(n1);
    ListNode* n2_list = create_list<ListNode>(n2);

    ASSERT_EX( compare_lists(Solution().run(n1_list, n2_list), expected),
               to_string(n1) + " + " + to_string(n2) + " -> " + to_string(expected) );

    destroy_list(n1_list);
    destroy_list(n2_list);
}

void test_symmetric(std::initializer_list<int> n1, std::initializer_list<int> n2, std::initializer_list<int> expected)
{
    test(n1, n2, expected);

    if (n1.size() != n2.size() || !std::equal(n1.begin(), n1.end(), n2.begin()))
        return;

    test(n2, n1, expected);
}

int main()
{
    ListNode* list = create_list<ListNode>({1});
    ASSERT( Solution().run(nullptr, list) == nullptr );
    ASSERT( Solution().run(list, nullptr) == nullptr );
    destroy_list(list);

    test_symmetric({0}, {0}, {0});
    test_symmetric({0}, {1}, {1});
    test_symmetric({1}, {1}, {2});
    test_symmetric({1}, {2}, {3});
    test_symmetric({1}, {8}, {9});
    test_symmetric({1}, {9}, {0, 1});
    test_symmetric({2}, {9}, {1, 1});
    test_symmetric({9}, {9}, {8, 1});
    test_symmetric({1}, {0, 1}, {1, 1});
    test_symmetric({1}, {9, 1}, {0, 2});
    test_symmetric({1}, {0, 2}, {1, 2});
    test_symmetric({1}, {9, 9}, {0, 0, 1});
    test_symmetric({2}, {9, 9}, {1, 0, 1});
    test_symmetric({1}, {0, 0, 1}, {1, 0, 1});
    test_symmetric({1}, {9, 0, 1}, {0, 1, 1});
    test_symmetric({1}, {9, 9, 1}, {0, 0, 2});
    test_symmetric({1}, {9, 9, 9}, {0, 0, 0, 1});
    test_symmetric({2}, {9, 9, 9}, {1, 0, 0, 1});
    test_symmetric({0, 1}, {0, 1}, {0, 2});
    test_symmetric({0, 1}, {1, 1}, {1, 2});
    test_symmetric({0, 1}, {9, 1}, {9, 2});
    test_symmetric({1, 1}, {9, 1}, {0, 3});
    test_symmetric({2, 1}, {9, 1}, {1, 3});
    test_symmetric({0, 1}, {0, 9}, {0, 0, 1});
    test_symmetric({1, 1}, {0, 9}, {1, 0, 1});
    test_symmetric({1, 1}, {9, 9}, {0, 1, 1});
    test_symmetric({2, 1}, {9, 9}, {1, 1, 1});
    test_symmetric({2, 2}, {9, 9}, {1, 2, 1});
    test_symmetric({9, 9}, {9, 9}, {8, 9, 1});
    test_symmetric({0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
                   {0, 2, 4, 6, 8, 0, 3, 5, 7, 9, 1});

    return 0;
}

}
