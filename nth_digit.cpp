// Problem: https://leetcode.com/problems/nth-digit/

#include "utils.h"
#include <math.h>

namespace nth_digit {

class Solution {
public:
    // Note: If n <= 0, returns 0.
    //
    // Time: O(lg(n)), Space: O(1), n - digit index, lg - decimal logarithm
    //
    int run(int n)
    {
        // Idea:
        // To find the n-th digit, first find the number it belongs to. There
        // are 9 1-digit numbers, 90 2-digits, 900 3-digits, etc. So,
        // n = 1 * 9 + 2 * 90 + ... + (d-1) * 9 * 10^(d-2) + r, where r is an
        // index relative to d-digit numbers. E.g. if n points at some 3-digit
        // number:
        //
        // 1 2 ... 9   1 0   1 1   1 2  ...   9 9     1  0  0   ...  x1  x2  x3
        // 1 2 ... 9  10 11 12 13 14 15 ... 188 189 190 191 192     n-1  n
        // <-- 9 -->  <---------- 2 * 90 ---------> <-------- r --------->
        //
        // Therefore we can subtract each count(i) = i * 9 * 10^(i-1) from n
        // until we get n <= count(d), where d is the digits count of the
        // target number (d = 3 in the example above). Then, this number is
        // 10^(d-1) + floor((n - 1) / d), and the target digit is (n - 1) mod d
        // (considering 0 as the highest digit).
        //
        // This solution has an issue: for the large n, count(i) may not fit
        // into int. But this can be avoided by either using larger data type,
        // or checking count(i) for overflow (if it overflows, then d == i).

        if (n <= 0)
            return 0;

        int numbers_count     = 9;
        int digits_per_number = 1;
        int digits_count      = 9;

        while (n > digits_count) {
            n -= digits_count;

            if (!multiply(numbers_count, 10))
                break;

            digits_count = ++digits_per_number;
            if (!multiply(digits_count, numbers_count))
                break;
        }
        
        const int number = std::pow(10, digits_per_number - 1) + (n - 1) / digits_per_number;
        const int digit  = (n - 1) % digits_per_number;

        return (number / (int) std::pow(10, digits_per_number - digit - 1)) % 10;
    }

private:
    // Returns true if there was no overflow. Otherwise returns false and does
    // not change a.
    //
    static inline bool multiply(int& a, int b)
    {
        if (a > std::numeric_limits<int>::max() / b)
            return false;

        a *= b;
        return true;
    }
};

int main()
{
    const auto test = [](int digit, int expected)
    {
        ASSERT_EX( Solution().run(digit) == expected, to_string(digit) + " -> " + to_string(expected) );
    };

    for (int i = 1; i <= 9; i++)
        test(i, i);

    test(10, 1);    // 10
    test(11, 0);    // 

    test(188, 9);   // 99
    test(189, 9);   //
    
    test(190, 1);   // 100
    test(191, 0);   //
    test(192, 0);   //

    test(193, 1);   // 101
    test(194, 0);   //
    test(195, 1);   //

    test(259, 1);   // 123
    test(260, 2);   //
    test(261, 3);   //

    test(1258, 4);  // 456
    test(1259, 5);  //
    test(1260, 6);  //

    test(2257, 7);  // 789
    test(2258, 8);  //
    test(2259, 9);  //

    const int MAX = std::numeric_limits<int>::max();
    if (sizeof(int) == 4) {
        test(MAX-2, 7); // 250954972, points at digit 7
        test(MAX-1, 2); //
        test(MAX,   2); // 250954973, points at digit 2
    } else {
        // Check that it does not fail
        const int result = Solution().run(MAX);
        ASSERT( 0 <= result && result <= 9 );
    }

    return 0;
}

}
