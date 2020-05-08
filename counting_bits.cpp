// Problem: https://leetcode.com/problems/counting-bits/

#include <vector>
#include <unordered_map>
#include <bitset>
#include "utils.h"

namespace counting_bits {

class Solution {
public:
    // Note: If n < 0, returns empty result.
    //
    // Time: O(n), Space: O(n) for result
    //
    std::vector<int> run(int n)
    {
        // Idea:
        // If we know the count of 1 bits for the number x, we can use it to
        // calculate the count for x+1 as follows: if add 1 to x, it's carried
        // through the first c bits, toggling them to 0, and placed at bit c+1,
        // toggling it to 1, so that x loses first c ones and get 1 new (c may
        // be 0 if there is no carry). So 1) the count of 1 bits for x+1 is the
        // count for x plus 1 and minus c, and 2) the difference between x+1
        // and x is 00..011..1, where the first c+1 bits are 1. The point 2)
        // means that there are only d possible differences, where d is the bit
        // size of int (more precisely, d-1 because int is signed). We can
        // precalculate the count of 1 bits for these differences, and use it
        // in constant time like ones_count[(x+1)^x].

        if (n < 0)
            return {};

        // Prepare the count of 1 bits for x^(x-1) masks
        std::unordered_map<int /*x^(x-1)*/, int /*bit count*/> ones_count;
        for (int mask = 0; mask < std::numeric_limits<int>::max();) {
            mask = (mask << 1) | 1;
            ones_count[mask] = ones_count.size() + 1;
        }

        // Calculate the count of 1 bits in [0, n]
        std::vector<int> result(n + 1);
        result[0] = 0;

        for (int x = 1; x <= n; x++) {
            const int bits_diff = x ^ (x - 1);
            const int bits_lost = ones_count[bits_diff] - 1;
            result[x] = result[x - 1] + 1 - bits_lost;
        }

        return result;
    }
};


void test(int n)
{
    const int BIT_COUNT = std::numeric_limits<int>::digits;

    std::vector<int> expected;
    for (int i = 0; i <= n; i++) {
        const int bit_count = std::bitset<BIT_COUNT>(i).count();
        expected.push_back(bit_count);
    }

    const std::vector<int> result = Solution().run(n);
    ASSERT_EX( result == expected, to_string(n) + " -> " + to_string(expected) );
}

int main()
{
    test(0);
    test(1);
    test(2);
    test(3);
    test(0xFFFF);

    return 0;
}

}
