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
    // Time: O(n), Space: O(1), n - number of integers
    //
    int run(const std::vector<int>& values)
    {
        // Idea:
        // We need to find the maximum sum
        //
        // s(v)   = 0*v[0] + 1*v[1] + 2*v[2] + ... + (n-1)*v[n-1]
        //
        // among all rotations of v. Calculating such a sum takes O(n) time, so
        // it takes O(n^2) for the brute-force solution. But we may notice that,
        // given the current rotation of v, the sum for the next rotation can
        // be calculated recurrently:
        //
        // s(v,0) = 0*v[0] + 1*v[1] + 2*v[2] + ... + (n-1)*v[n-1]
        // s(v,1) =          0*v[1] + 1*v[2] + ... + (n-2)*v[n-1] + (n-1)*v[0] =
        //          s(v,0) - ( v[1] +   v[2] + ... +       v[n-1])+ (n-1)*v[0]
        //
        // i.e. as the current sum minus all values except the first one, plus
        // this first value multiplied by n-1. So we could prepare these
        // partial sums ("all values except i-th") in O(n) time and O(n) space,
        // and then find the maximum in O(n) time. But since each partial sum
        // is the total sum minus the i-th value, this can be simplified:
        //
        // s(v,1) = s(v,0) - ( v[1] +   v[2] + ... +       v[n-1] + v[0]) + n*v[0]
        //
        // So we always subtract all values and just change the miltiplier from
        // (n-1) to n. Now, after precalculating the sum of all values in O(n)
        // time and O(1) space, we can find the maximum in O(n) time.

        if (!values.size())
            return 0;

        // Calculate sum of all values
        int total_sum = 0;
        for (int i = 0; i < values.size(); i++)
            total_sum += values[i];

        // Calculate initial sum
        int sum = 0;
        for (int i = 0; i < values.size(); i++)
            sum += values[i] * i;

        // Find maximum sum among all rotations of the values
        int max_sum = sum;
        for (int i = 1; i < values.size(); i++) {
            sum -= total_sum;
            sum += values[i - 1] * values.size();
            max_sum = std::max(max_sum, sum);
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

    // Large random input
    std::vector<int> large_input;
    for (int i = 0; i < 10000; i++)
        large_input.push_back(rand() % 10 - rand() % 10);
    test(large_input);

    return 0;
}

}
