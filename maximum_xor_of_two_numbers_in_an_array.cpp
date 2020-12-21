// Problem: https://leetcode.com/problems/maximum-xor-of-two-numbers-in-an-array/

#include "utils.h"
#include <vector>

namespace maximum_xor_of_two_numbers_in_an_array {

// Straightforward solution with O(n^2) time

class Solution1 {
public:
    // Note: The nums must contain non-negative integers only. If the nums 
    // contains 1 or none integers, returns 0.
    //
    // Time: O(n^2), Space: O(1), n - number of integers
    //
    int run(const std::vector<int>& nums)
    {
        if (nums.size() <= 1)
            return 0;

        int result = 0;

        for (int i1 = 0; i1 < nums.size(); i1++) {
            for (int i2 = i1 + 1; i2 < nums.size(); i2++)
                result = std::max(result, nums[i1] ^ nums[i2]);
        }
        
        return result;
    }
};


// Solution with O(n) time, using the prefix tree

class Solution2 {
public:
    // Note: The nums must contain non-negative integers only. If the nums 
    // contains 1 or none integers, returns 0.
    //
    // Time: O(n), Space: O(1), n - number of integers
    //
    // Note: The actual space for 32-bit int is about 2^32 bits = 2^29 bytes
    // = 512 mb, to store the prefix tree.
    //
    int run(const std::vector<int>& nums)
    {
        if (nums.size() <= 1)
            return 0;

        BinaryPrefixTree<int> nums_tree;
        for (const int n : nums)
            nums_tree.add(n);

        int result = 0;
        for (const int n : nums)
            result = std::max(result, n ^ nums_tree.findLongestPrefix(~n));
        return result;
    }

private:
    // Represents the prefix tree of the bit patterns of BitCount bits. Each
    // pattern is a value of the integer type T. Provides constant-time
    // insertion and search.
    //
    template <typename T, int BitCount = std::numeric_limits<int>::digits>
    class BinaryPrefixTree {
    public:
        BinaryPrefixTree() : nodes_(NODE_COUNT, false) {}

        void add(T number)
        {
            size_t level = 1;
            size_t i     = 1;

            for (T mask = HIGHEST_BIT; mask != 0; mask >>= 1) {
                const bool bit = number & mask;
                i += bit;

                nodes_[i] = true;

                moveToLeftChild(i, level);
            }
        }

        T findLongestPrefix(T number) const
        {
            size_t level  = 1;
            size_t i      = 1;
            T      result = 0;

            for (size_t mask = HIGHEST_BIT; mask != 0; mask >>= 1) {
                bool bit = number & mask;
                if (!nodes_[i + bit])
                    bit = !bit;
                i += bit;
                
                assert(nodes_[i]);
                result |= bit ? mask : 0;

                moveToLeftChild(i, level);
            }

            return result;
        }
    
    private:
        void moveToLeftChild(size_t& i, size_t& level) const
        {
            const size_t first_node_1 = (1 << level) - 1;
            const size_t first_node_2 = (1 << (level + 1)) - 1;

            i = first_node_2 + (i - first_node_1) * 2;
            level++;
        }

        static const int    BIT_COUNT   = BitCount;
        static const int    HIGHEST_BIT = 1 << (BIT_COUNT - 1);
        static const size_t NODE_COUNT  = (size_t(1) << (BIT_COUNT + 1)) - 1;

        std::vector<bool> nodes_;
    };
};


template <typename Solution>
void test()
{
    const auto test = [](const std::vector<int>& nums, int expected)
    {
        ASSERT_EX( Solution().run(nums) == expected,
            to_string(nums) + " -> " + to_string(expected) );
    };

    test({}, 0);

    test({0}, 0);
    test({1}, 0);

    test({0, 0}, 0);
    test({0, 1}, 1);
    test({1, 0}, 1);
    test({1, 1}, 0);
    test({1, 2}, 3);

    test({0, 0, 0}, 0);
    test({1, 0, 0}, 1);
    test({0, 1, 0}, 1);
    test({0, 0, 1}, 1);
    test({1, 1, 0}, 1);
    test({1, 0, 1}, 1);
    test({1, 1, 1}, 0);

    test({0, 1, 2}, 3);
    test({1, 0, 2}, 3);
    test({2, 0, 1}, 3);
    test({2, 1, 0}, 3);

    test({15, 10, 0, 5}, 15);
    test({15, 10, 1, 5}, 15);
    test({15, 14, 12, 5}, 11);

    const int MAX = std::numeric_limits<int>::max();

    test({MAX}, 0);
    test({MAX, MAX}, 0);
    test({MAX, 0}, MAX);
    test({MAX, MAX-1}, 1);
    test({MAX, MAX, 0}, MAX);

    // The maximum number of random integers allowed by the original problem
    {
        const int MAX_COUNT = 20000;
        
        std::vector<int> nums(MAX_COUNT);
        for (int i = 0; i < MAX_COUNT; i++)
            nums[i] = rand();

        int expected = 0;
        for (int i1 = 0; i1 < nums.size(); i1++) {
            for (int i2 = i1 + 1; i2 < nums.size(); i2++) {
                expected = std::max(expected, nums[i1] ^ nums[i2]);
            }
        }

        test(nums, expected);
    }
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
