// Problem: https://leetcode.com/problems/rotate-function/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace rotate_function {

class Solution {
public:
    // Note: If the values is empty, returns 0. The result and the sum of all
    // the values must fit into int.
    //
    // Time: O(n), Space: O(n), n - number of integers
    //
    int run(const std::vector<int>& values)
    {
        if (!values.size())
            return 0;

        // Prepare partial sums
        struct PartialSum
        {
            int left;
            int right;
        };

        std::vector<PartialSum> sums(values.size(), {0, 0});
        PartialSum              sum = {0, 0};

        for (int i = 1; i < values.size(); i++) {
            sum.left += values[i - 1];
            sums[i].left = sum.left;
        }
        for (int i = values.size() - 1; i >= 0; i--) {
            sum.right += values[i];
            sums[i].right = sum.right;
        }

        // Prepare initial left/right sums
        sum = {0, 0};
        for (int i = 1; i < values.size(); i++)
            sum.right += values[i] * i;

        // Find maximum rotated sum
        int max_sum = sum.left + sum.right;
        for (int i = 1; i < values.size(); i++) {
            sum.left  += -sums[i].left + values[i - 1] * values.size();
            sum.right -= sums[i].right;
            max_sum    = std::max(max_sum, sum.left + sum.right);
        }
        return max_sum;
    }
};

// Test by comparing with the brute-force result
void test(const std::vector<int>& values)
{
    int expected = 0;

    auto values_rotated = values;
    for (int r = 0; r < values_rotated.size(); r++) {
        std::rotate(values_rotated.begin(), values_rotated.end() - 1, values_rotated.end());

        int sum = 0;
        for (int i = 1; i < values_rotated.size(); i++)
            sum += values_rotated[i] * i;

        expected = std::max(expected, sum);
    }

    const int result = Solution().run(values);

    ASSERT_EX( result == expected,
        to_string(values) + " -> " + to_string(expected) );
}

int main()
{
    test({});
    
    test({0});
    test({1});
    test({-1});

    test({1, 2});
    test({2, 1});

    test({1, 2, 3});
    test({1, 3, 2});
    test({2, 1, 3});
    test({2, 3, 1});
    test({3, 1, 2});
    test({3, 2, 1});

    test({0, 101, 0, -1024, 8, 123, -10, 1, 0});

    // Min/max values
    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    test({MAX});
    test({0, MAX});
    test({MIN});
    test({0, MIN});

    // Large input
    std::vector<int> large_input;
    for (int i = 0; i < 10000; i++)
        large_input.push_back(rand() % 10 - rand() % 10);
    test(large_input);

    return 0;
}

}
