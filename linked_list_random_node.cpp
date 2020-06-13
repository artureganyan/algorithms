// Problem: https://leetcode.com/problems/linked-list-random-node/

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "utils.h"

namespace linked_list_random_node {

struct ListNode {
    int val;
    ListNode *next;
};


// Straightforward solution with linear time and constant space

// Note: The nodes count must remain unchanged during the object lifetime.
//
// Time:  O(n) for constructor and getRandom()
// Space: O(1) for constructor, getRandom() and the object lifetime
// n - number of nodes

class Solution1 {
public:
    Solution1(ListNode* head)
    {
        head_ = head;

        const ListNode* node = head;
        while (node) {
            nodes_count_++;
            node = node->next;
        }
    }

    int getRandom()
    {
        if (!head_)
            throw std::out_of_range("getRandom(): The list is empty");

        const int index = rand() % nodes_count_;

        const ListNode* node = head_;
        for (int i = 0; i < index; i++)
            node = node->next;
        return node->val;
    }

private:
    const ListNode* head_        = nullptr;
    int             nodes_count_ = 0;
};


// Straightforward solution with constant/linear time and linear/constant space

// Note: The nodes count and addresses must remain unchanged during the object
// lifetime.
//
// Time:  O(n) for constructor
//        O(n/c) for getRandom()
// Space: O(c) for constructor and the object lifetime
//        O(1) for getRandom()
// n - number of nodes
// c - capacity, by default c = n

class Solution2 {
public:
    // If the capacity is <= 0, it's set to the nodes count
    Solution2(ListNode* head, int capacity = -1)
    {
        // Calculate the block size
        const ListNode* node = head;

        if (capacity > 0) {
            nodes_count_ = 0;
            while (node) {
                nodes_count_++;
                node = node->next;
            }
            block_size_ = std::max(nodes_count_ / capacity, 1);
        } else {
            block_size_ = 1;
        }

        // Convert the list to the vector
        int node_index = 0;
        nodes_count_   = 0;
        node           = head;

        while (node) {
            if (node_index % block_size_ == 0)
                nodes_.push_back(node);
            nodes_count_++;
            node_index++;
            node = node->next;
        }
    }

    int getRandom()
    {
        if (!nodes_.size())
            throw std::out_of_range("getRandom(): The list is empty");

        const int index = rand() % nodes_count_;

        const ListNode* node = nodes_[index / block_size_];
        for (int i = index % block_size_; i > 0; i--)
            node = node->next;
        return node->val;
    }

private:
    std::vector<const ListNode*> nodes_;
    int                          nodes_count_;
    int                          block_size_;
};


// Solution with constant/linear time and constant space

// Note: The nodes addresses must remain unchanged during the object lifetime.
//
// Warning: Each call to getRandom() randomly selects one of the next
// selection_size nodes, starting from the node n=head. Then, depending on the
// implementation, it cyclically advances n to either a) the node after the
// selection_size nodes, or b) the node after the selected one. If the
// selection_size is constant, this gets the constant time complexity, but
// also the following issues:
// 1. The node can be selected again not earlier than after
//    ceil(nodes_count/selection_size)-1 calls.
// 2. For a), the nodes are never selected in their original order, but for b) 
//    this is possible.
// 3. On a single call, the probability of each node within the selection_size
//    frame is the same, but for other nodes it's 0.
// 4. On k*nodes_count/selection_size calls, where k is such that k*nodes_count
//    divides by selection_size, the probability of each node:
//    for a): is the same; each node can be selected at most k times
//    for b): is supposed to approach the same value if k is large enough, but
//            this SHOULD BE CHECKED
//    
// Time:  O(s) for constructor
//        O(s) for getRandom()
// Space: O(1) for constructor, getRandom() and the object lifetime
// s - selection size

// If defined, advances the selection frame by selection_size nodes on each
// getRandom(). Otherwise, advances the frame right after the selected node.
#define SOLUTION3_FIXED_SELECTION_FRAME

class Solution3 {
public:
    // If the selection_size <= 0 or > nodes count, it's set to the nodes count
    Solution3(ListNode* head, int selection_size)
    {
        head_ = head;
        node_ = head;

        selection_size_ = 0;
        for (auto node = head; node; node = node->next) {
            selection_size_++;
            if (selection_size_ == selection_size)
                break;
        }
    }

    int getRandom()
    {
        if (!node_)
            throw std::out_of_range("getRandom(): The list is empty");

        const int index  = rand() % selection_size_;
        const int result = advance(node_, index)->val;
#ifdef SOLUTION3_FIXED_SELECTION_FRAME
        advance(node_, selection_size_ - index);
#else
        advance(node_, 1);
#endif
        return result;
    }

private:
    inline const ListNode* advance(const ListNode*& node, int steps = 1) const
    {
        for (int i = 0; i < steps; i++) {
            node = node->next;
            if (!node)
                node = head_;
        }
        return node;
    }

    const ListNode* head_           = nullptr;
    const ListNode* node_           = nullptr;
    int             selection_size_ = 0;
};


template <typename Solution, auto... args>
void test(const std::string& name = {})
{
    auto test = [](const std::vector<int>& values)
    {
        std::cout << to_string(values) << std::endl;

        TemporaryList<ListNode> list = create_temp_list<ListNode>(values);

        Solution s(list, args...);

        // Get random values
        std::cout << "Getting random values..." << std::endl;

        std::unordered_set<int>                          values_set(values.begin(), values.end());
        std::unordered_map<int /*value*/, int /*count*/> values_count;

        const int runs_count = values.size() * 5000;
        for (int i = 0; i < runs_count; i++) {
            const int v = s.getRandom();
            if (values_set.find(v) == values_set.end())
                ASSERT_EX( false, to_string(values) + " missing " + to_string(v) );
            values_count[v]++;
        }

        // Check the probability of the randomized values
        std::cout << "Check probability:" << std::endl;

        for (const int v : values) {
            const double frequency  = values_count[v] / double(runs_count);
            const double expected   = 1.0 / values.size();
            const double expected_e = expected / 10.0;
            ASSERT_EX( expected - expected_e <= frequency && frequency <= expected + expected_e,
                to_string(v) + ", " + to_string(frequency) + " ~ " + to_string(expected) );
        }
    };

    std::cout << name << std::endl;

    // Test for null
    {
        Solution s(nullptr, args...);
    }

    test({0});
    test({1});
    test({-1});

    test({1, 2});
    test({1, 2, 3});

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();
    test( {-1, MAX, 0, 1000, 1, 123, -2, 100000, 10, MIN});

    // Test for long list
    {
        std::vector<int> values;
        for (int i = 0; i < 1000; i++)
            values.push_back(i);
        test(values);
    }
}

int main()
{
    test<Solution1>("Solution1");

    test<Solution2>("Solution2, capacity=default");
    test<Solution2, 0>("Solution2, capacity=0");
    test<Solution2, 1>("Solution2, capacity=1");
    test<Solution2, 2>("Solution2, capacity=2");
    test<Solution2, 3>("Solution2, capacity=3");
    test<Solution2, 101>("Solution2, capacity=101");
    test<Solution2, std::numeric_limits<int>::max()>("Solution2, capacity=max");

    test<Solution3, 0>("Solution3, selection_size=0");
    test<Solution3, 1>("Solution3, selection_size=1");
    test<Solution3, 2>("Solution3, selection_size=2");
    test<Solution3, 3>("Solution3, selection_size=3");
    test<Solution3, 101>("Solution3, selection_size=101");
    test<Solution3, std::numeric_limits<int>::max()>("Solution3, selection_size=max");

    return 0;
}

}
