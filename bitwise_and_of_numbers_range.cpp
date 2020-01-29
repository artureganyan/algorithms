// Problem: https://leetcode.com/problems/bitwise-and-of-numbers-range/

#include "utils.h"

namespace bitwise_and_of_numbers_range {

class Solution {
public:
    // Note: The following must be true: m >= 0, n >= 0, m <= n.
    //
    // Time: O(1), Space: O(1)
    //
    int run(int m, int n)
    {
        // Idea:
        // After the bitwise "and" of the range [n1, n2], the i-th bit of the
        // result is 0 if:
        // 1) The i-th bit of n1 is 0 (so that the rest numbers do not matter), or
        // 2) (bits i, i-1, ..., 0 of n1) + (n2 - n1) >= 2^(i+1)
        //
        // The case 2 means that, when incrementing from n1 to n2, the i-th bit
        // of n1 will change from 1 to 0, producing 0 for the i-th bit of the
        // result.

        if (m < 0 || n < 0 || m > n)
            return 0;

        if (m == n)
            return m;

        const int    BIT_COUNT = 31;
        unsigned int bit       = 0x1;
        unsigned int bit_block = 0x1;
        const int    delta     = n - m;
        int          result    = m;

        for (int i = 0; i < BIT_COUNT; i++) {
            if ((m & bit_block) + delta >= (bit << 1))
                result &= ~bit;
            bit <<= 1;
            bit_block = (bit_block << 1) | 1;
        }
        return result;
    }
};

int main()
{
    ASSERT( Solution().run(-1, 1) == 0 );
    ASSERT( Solution().run(1, -1) == 0 );
    ASSERT( Solution().run(-1, -1) == 0 );

    ASSERT( Solution().run(0, 0) == 0 );
    ASSERT( Solution().run(0, 1) == 0 );

    ASSERT( Solution().run(1, 1) == 1 );
    ASSERT( Solution().run(1, 2) == 0 );
    ASSERT( Solution().run(1, 3) == 0 );

    ASSERT( Solution().run(2, 1) == 0 );
    ASSERT( Solution().run(2, 2) == 2 );
    ASSERT( Solution().run(2, 3) == 2 );
    ASSERT( Solution().run(2, 4) == 0 );

    ASSERT( Solution().run(3, 3) == 3 );
    ASSERT( Solution().run(3, 4) == 0 );

    ASSERT( Solution().run(5, 6) == 4 );
    ASSERT( Solution().run(5, 7) == 4 );
    ASSERT( Solution().run(5, 8) == 0 );

    ASSERT( Solution().run(6, 7) == 6 );

    ASSERT( Solution().run(0, 0x7FFFFFFF) == 0 );
    ASSERT( Solution().run(1, 0x7FFFFFFF) == 0 );
    ASSERT( Solution().run(0x7FFFFFFF, 0x7FFFFFFF) == 0x7FFFFFFF );

    return 0;
}

}
