// Problem: https://leetcode.com/problems/linked-list-cycle-ii/

#include <unordered_set>
#include "utils.h"

namespace linked_list_cycle_2 {

struct ListNode {
    int val;
    ListNode *next;
};

class Solution {
public:
    // Time: O(n), Space: O(n), n - number of nodes
    ListNode* run(ListNode* head)
    {
        if (!head)
            return nullptr;

        std::unordered_set<const ListNode*> visited_nodes;

        ListNode* node = head;
        while (node) {
            const bool visited = (visited_nodes.insert(node).second == false);
            if (visited)
                return node;
            node = node->next;
        }

        return nullptr;
    }
};


void test(ListNode* head, ListNode* expected)
{
    ASSERT_EX( Solution().run(head) == expected,
        list_to_string(head) + " -> " + (expected ? to_string(expected->val) : "nullptr") );

    destroy_list(head);
}

int main()
{
    ASSERT( Solution().run(nullptr) == nullptr );

    ASSERT( Solution().run(TemporaryList<ListNode>(new ListNode{1})) == nullptr );
    ASSERT( Solution().run(TemporaryList<ListNode>(new ListNode{1, new ListNode{-2}})) == nullptr );
    ASSERT( Solution().run(TemporaryList<ListNode>(new ListNode{1, new ListNode{-2, new ListNode{3}}})) == nullptr );

    {
        ListNode* n1 = new ListNode{1};
        n1->next = n1;
        test(n1, n1);
    }

    {
        ListNode* n1 = new ListNode{1};
        ListNode* n2 = new ListNode{2, n1};
        n1->next = n2;
        test(n1, n1);
    }

    {
        ListNode* n1 = new ListNode{1};
        ListNode* n2 = new ListNode{2};
        ListNode* n3 = new ListNode{3, n1};
        n1->next = n2;
        n2->next = n3;
        test(n1, n1);
    }

    {
        ListNode* n1 = new ListNode{1};
        ListNode* n2 = new ListNode{2};
        ListNode* n3 = new ListNode{3, n2};
        n1->next = n2;
        n2->next = n3;
        test(n1, n2);
    }

    return 0;
}

}
