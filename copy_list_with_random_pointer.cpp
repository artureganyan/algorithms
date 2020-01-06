// Problem: https://leetcode.com/problems/copy-list-with-random-pointer/

#include <unordered_map>
#include "utils.h"

namespace copy_list_with_random_pointer {

struct Node {
    int val;
    Node* next;
    Node* random;
};

class Solution {
public:
    // Time: O(n), Space: O(n), n - number of nodes
    Node* run(Node* head)
    {
        if (!head)
            return nullptr;

        // Copy nodes
        std::unordered_map<const Node* /*original*/, Node* /*copy*/> copies;
        copies[nullptr] = nullptr;

        const Node* node = head;
        while (node) {
            Node* node_copy = new Node;
            node_copy->val  = node->val;
            copies[node]    = node_copy;
            node            = node->next;
        }

        // Set next/random pointers
        node = head;
        while (node) {
            Node* node_copy   = copies[node];
            node_copy->next   = copies[node->next];
            node_copy->random = copies[node->random];

            if (!node_copy->random && node->random)
                throw std::runtime_error("Incorrect random pointer");

            node = node->next;
        }

        return copies[head];
    }
};


bool compare_lists(const Node* head1, const Node* head2)
{
    if (!head1 || !head2)
        return head1 == head2;

    std::unordered_map<const Node* /*node1*/, const Node* /*node2*/> copies;

    // Check node values
    const Node* node1 = head1;
    const Node* node2 = head2;
    while (node1 && node2) {
        if (node1->val != node2->val)
            return false;

        copies[node1] = node2;
        node1 = node1->next;
        node2 = node2->next;
    }

    if (node1 || node2)
        return false;

    // Check random pointers
    node1 = head1;
    node2 = head2;
    while (node1 && node2) {
        if (node2->random != copies[node1->random])
            return false;

        node1 = node1->next;
        node2 = node2->next;
    }

    return true;
}

void test(Node* head)
{
    const Node* result = Solution().run(head);

    ASSERT_EX( compare_lists(head, result), list_to_string(head) );

    destroy_list(head);
    destroy_list(result);
}

int main()
{
    ASSERT( Solution().run(nullptr) == nullptr );

    test(new Node{0});
    test(new Node{1, new Node{1}});
    test(new Node{1, new Node{-2, new Node{3}}});

    {
        Node* n1 = new Node{1};
        n1->random = n1;
        test(n1);
    }

    {
        Node* n1 = new Node{1};
        Node* n2 = new Node{2};
        n1->next   = n2;
        n1->random = n2;
        test(n1);
    }

    {
        Node* n1 = new Node{1};
        Node* n2 = new Node{2};
        n1->next   = n2;
        n1->random = n2;
        n2->random = n1;
        test(n1);
    }

    {
        Node* n1 = new Node{1};
        Node* n2 = new Node{2};
        Node* n3 = new Node{3};
        n1->next   = n2;
        n1->random = n3;
        n2->next   = n3;
        n2->random = n3;
        n3->random = n1;
        test(n1);
    }

    return 0;
}

}
