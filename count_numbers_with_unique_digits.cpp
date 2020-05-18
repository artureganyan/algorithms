// Problem: https://leetcode.com/problems/count-numbers-with-unique-digits/

#include <math.h>
#include "utils.h"

namespace count_numbers_with_unique_digits {

// Note: There are only 9 different results (for n=1..9), so they could be
// precalculated by any suitable solution and returned in constant time.

class Solution {
public:
    // Note: If n <= 0, returns 0.
    //
    // Time: O(1) for arbitrary n; ~k*n^2 arithmetic operations for n=0..9
    // Space: O(1)
    //
    int run(int n)
    {
        // Idea:
        // If the number consists of n unique digits, the first digit can be
        // any of 1-9, the second is any of 0-9 except the first, the third is
        // any of 0-9 except the first and second, and so on. So the total
        // count of the numbers with n unique digits is 9*9*8*7*...*(11-n) if
        // n >= 2, and 10 if n == 1. Since we need to count within [0, 10^n),
        // the result is the sum of the counts for n, n-1, ..., 1 digits.
        // Finally, any number with n > 9 digits contains duplicated digits, so
        // n can be limited by 9.

        if (n <= 0)
            return 0;

        if (n > 9)
            n = 9;

        int result = 0;
        for (int i = n; i >= 2; i--) {
            int count = 9;
            for (int d = i - 1; d >= 1; d--)
                count *= 10 - (i - d);
            result += count;
        }
        result += 10;

        return result;
    }
};


// Brute-force implementation for tests
int countNumbersWithUniqueDigits(int n)
{
    const int total   = std::pow(10, n);
    int       repeats = 0;

    for (int i = 0; i < total; i++) {
        bool digits[10] = {false};
        int  x = i;
        while (x) {
            const int d = x % 10;
            if (digits[d]) {
                repeats++;
                break;
            }
            digits[d] = true;
            x /= 10;
        }
    }

    return total - repeats;
}

int main()
{
    // The expected results were obtained by countNumbersWithUniqueDigits()
    ASSERT( Solution().run(0) == 0 );
    ASSERT( Solution().run(1) == 10 );
    ASSERT( Solution().run(2) == 91 );
    ASSERT( Solution().run(3) == 739 );
    ASSERT( Solution().run(4) == 5275 );
    ASSERT( Solution().run(5) == 32491 );
    ASSERT( Solution().run(6) == 168571 );
    ASSERT( Solution().run(7) == 712891 );
    ASSERT( Solution().run(8) == 2345851 );
    ASSERT( Solution().run(9) == 5611771 );
    ASSERT( Solution().run(10) == Solution().run(9) );

    const int MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run(MAX) == Solution().run(9) );

    return 0;
}

}
