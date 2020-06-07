// Problem: https://leetcode.com/problems/linked-list-random-node/

#include <vector>
#include <unordered_set>
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


template <typename Solution, auto... args>
void test(const std::string& name = {})
{
    auto test = [](const std::vector<int>& values)
    {
        TemporaryList<ListNode> list = create_temp_list<ListNode>(values);

        Solution s(list, args...);

        // Get random values
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
        std::cout << to_string(values) << std::endl;

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
        Solution s(nullptr);
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

    return 0;
}

}
