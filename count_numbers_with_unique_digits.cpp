// Problem: https://leetcode.com/problems/count-numbers-with-unique-digits/

#include <math.h>
#include "utils.h"

namespace count_numbers_with_unique_digits {

// Note: There are only 10 different results (for n=1..10), so they could be
// precalculated with any suitable solution and returned in constant time.

// Non-recursive solution, directly counting numbers with unique digits

class Solution1 {
public:
    // Note: If n == 0, returns 1, because we count within [0, 1). If n < 0,
    // returns 0.
    //
    // Time: O(1) for arbitrary n; k*n arithmetic operations for n=0..10
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
        // Finally, any number with n > 10 digits contains duplicated digits,
        // so n can be limited by 10.

        if (n < 0)
            return 0;

        if (n == 0)
            return 1;

        if (n > 10)
            n = 10;
#if 1
        // Sum for 1, 2, ..., n digits, in O(n)
        int result = 10;
        for (int i = 2, result_i = 9; i <= n; i++) {
            result_i *= 11 - i;
            result   += result_i;
        }
#else
        // Sum for n, n-1, ..., 1 digits, in O(n^2)
        int result = 0;
        for (int i = n; i >= 2; i--) {
            int count = 9;
            for (int d = i - 1; d >= 1; d--)
                count *= 10 - (i - d);
            result += count;
        }
        result += 10;
#endif
        return result;
    }
};


// Recursive solution via counting numbers with non-unique digits

class Solution2 {
public:
    // Note: If n == 0, returns 1, because we count within [0, 1). If n < 0,
    // returns 0.
    //
    // Time: O(1) for arbitrary n; n*(n+1)/2 calls for n=0..10
    // Space: O(1)
    // Recursion depth <= n + 1
    //
    int run(int n)
    {
        if (n < 0)
            return 0;

        if (n == 0)
            return 1;

        if (n > 10)
            n = 10;

        const long long total   = std::pow(10, n);
        const long long repeats = countRepeats(n, false);

        return (int) (total - repeats);
    }

private:
    long long countRepeats(int n, bool can_start_with_zero) const
    {
        // Idea:
        // We want to count all the numbers containing duplicated digits,
        // within [0, 10^n). The first digit d of such a number can be 1-9.
        // Each of the rest digits can be 0-9, and all the possible
        // combinations of the rest digits can be divided into 2 disjoint sets:
        // s1: digits which contain one or more duplicates
        // s2: digits which contain no duplicates and have one digit equal to d:
        //     {dxx...x, xdx...x, xxd...x, ..., xxx...d}
        // So, the count of the numbers with n non-unique digits is 9*(|s1|+|s2|),
        // where |s1| and |s2| is the sets' sizes. The total count of the
        // numbers with n, n-1, ..., 1 non-unique digits is calculated
        // recursively as count(n) = 9*(|s1|+|s2|) + count(n-1). Similarly,
        // |s1| is calculated recursively as s1(n) = 10*(s1(n-1)+s2(n-1)) (its
        // digits can start with 0). |s2| is just s2(n) = (n-1)*9*8*7*...*(11-n),
        // because the digit d can be placed at any of n-1 positions, and the
        // rest n-2 digits must be unique: the first one is any of 0-10 except d,
        // the second is any of 0-10 except d and the first one, etc. Finally,
        // any number with n > 10 digits contains duplicated digits.

        if (n <= 1)
            return 0;

        long long result = 0;
        if (n > 10) {
            result = std::pow(10, n) - std::pow(10, n-1);
            n = 10;
        }

        long long unique = n - 1;
        for (int i = 0; i < n - 2; i++)
            unique *= (9 - i);

        long long repeats = countRepeats(n - 1, true);

        result += (can_start_with_zero ? 10 : 9) * (repeats + unique) +
                  (can_start_with_zero ? 0 : countRepeats(n - 1, false));

        return result;
    }
};


// Non-recursive version of Solution2, with linear time

class Solution3 {
public:
    // Note: If n == 0, returns 1, because we count within [0, 1). If n < 0,
    // returns 0.
    //
    // Time: O(1) for arbitrary n, k*n arithmetic operations for n=0..10
    // Space: O(1)
    //
    int run(int n)
    {
        // Idea: See Solution2 for details

        if (n < 0)
            return 0;

        if (n == 0)
            return 1;

        if (n > 10)
            n = 10;

        long long repeats_total = 0;
        long long repeats_i     = 0;
        long long unique_i      = 1;

        for (int i = 2; i <= n; i++) {
            repeats_total = 9  * (repeats_i + unique_i) + repeats_total;
            repeats_i     = 10 * (repeats_i + unique_i);
            unique_i      = i  * (unique_i / (i - 1)) * (11 - i);
        }

        return (int) (std::pow(10, n) - repeats_total);
    }
};


// Brute-force implementation for tests
long long countNumbersWithUniqueDigits(int n)
{
    const long long total  = std::pow(10, n);
    long long       result = 0;

    for (long long i = 0; i < total; i++) {
        bool digits[10] = {false};
        long long x = i;
        while (x) {
            const int d = x % 10;
            if (digits[d])
                break;
            digits[d] = true;
            x /= 10;
        }
        if (!x)
            result++;
    }

    return result;
}

template <typename Solution>
void test()
{
    // The expected results were obtained by countNumbersWithUniqueDigits()
    ASSERT( Solution().run(0) == 1 );
    ASSERT( Solution().run(1) == 10 );
    ASSERT( Solution().run(2) == 91 );
    ASSERT( Solution().run(3) == 739 );
    ASSERT( Solution().run(4) == 5275 );
    ASSERT( Solution().run(5) == 32491 );
    ASSERT( Solution().run(6) == 168571 );
    ASSERT( Solution().run(7) == 712891 );
    ASSERT( Solution().run(8) == 2345851 );
    ASSERT( Solution().run(9) == 5611771 );
    ASSERT( Solution().run(10) == 8877691 );
    ASSERT( Solution().run(11) == Solution().run(10) );

    const int MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run(MAX) == Solution().run(10) );
}

int main()
{
    test<Solution1>();
    test<Solution2>();
    test<Solution3>();

    return 0;
}

}
