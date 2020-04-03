// Problem: https://leetcode.com/problems/additive-number/

#include <string>
#include "utils.h"

namespace is_additive_number {

// Note: It's not clear if '0' should be treated as a valid number in this
// problem. It is valid in the current implementations.


class Solution1 {
public:
    // Note: The string must contain only the digits '0'-'9'. Any substring
    // starting with '0', except the single '0', is not treated as a number.
    // Numbers and sums greater than int are not handled.
    //
    // Time: O(1), Space: O(1)
    //
    // Note:
    // The time complexity is estimated as follows:
    // 1. Each number is limited by d decimal digits, where d is 10 for 32-bit
    //    signed int.
    // 2. Each sequence is generated from its 1st and 2nd number, so there are
    //    no more than d*d sequences to check.
    // 3. Each next number of the sequence is <= 2 times greater than the
    //    previous one, ~1.5 times in average, so one more decimal digit
    //    appears approximately every ceil(log(10, 1.5)) numbers. Considering
    //    the point 1, this gives the maximum sequence size of about
    //    s_n = ceil(log(10, 1.5))*d numbers and s_d = s_n*d/2 digits.
    // 4. Checking the most of the sequences will fail likely after the first few
    //    numbers. The other sequences may take longer check to fail, but < s_d
    //    digits. So the total checked digits is expected to be k*s_d, k <= d*d.
    //    This does not depend on the string length (if it's >= s_d).
    //
    bool run(const std::string& num)
    {
        // Idea:
        // The source string can represent many sequences of numbers, but each
        // additive sequence is generated from its 1st and 2nd number. So, we
        // only need to iterate over all possible (1st, 2nd) pairs, and check
        // the rest part according to them.

        // All 1st numbers
        for (int i1 = 0, n1 = 0; i1 < num.size(); i1++) {
            if (!append_digit(n1, num[i1], i1))
                return false;

            // All 2nd numbers
            for (int i2 = i1 + 1, n2 = 0; i2 < num.size(); i2++) {
                if (!append_digit(n2, num[i2], i2 - i1 - 1))
                    break;

                // Check the rest numbers
                for (int i3 = i2 + 1, i3_0 = i3, v1 = n1, v2 = n2, v3 = 0; i3 < num.size(); i3++) {
                    if (!append_digit(v3, num[i3], i3 - i3_0))
                        break;

                    int v12 = 0;
                    if (!sum(v12, v1, v2))
                        break;

                    if (v3 == v12) {
                        if (i3 == num.size() - 1)
                            return true;
                        v1   = v2;
                        v2   = v3;
                        v3   = 0;
                        i3_0 = i3 + 1;

                    } else if (v3 > v12) {
                        break;
                    }
                }
            }
        }

        return false;
    }

private:
    static bool append_digit(int& n, char digit, int digit_index)
    {
        if (digit < '0' || digit > '9')
            throw std::runtime_error("Non-digit characters are not supported");

        // Don't allow leading zero (except single 0)
        if (n == 0 && digit_index > 0)
            return false;

        if (n > 0 && std::numeric_limits<int>::max() / n < 10)
            return false;

        n = n * 10 + (digit - '0');
        return true;
    }

    static bool sum(int& s, int v1, int v2)
    {
        if (std::numeric_limits<int>::max() - v1 < v2)
            return false;

        s = v1 + v2;
        return true;
    }
};


// Recursive solution

class Solution2 {
public:
    // Note: The string must contain only the digits '0'-'9'. Any substring
    // starting with '0', except the single '0', is not treated as a number.
    // Numbers and sums greater than int are not handled.
    //
    // Time: O(1), Space: O(1), Recursion depth <= n + 2, n - number of digits
    //
    // Note: See the Solution1 for explanation.
    //
    bool run(const std::string& num)
    {
        return isAdditiveNumber(num, 0, 0, 0, 0);
    }

private:
    bool isAdditiveNumber(const std::string& s, int n1, int n2, int n3_start, int n_count) const
    {
        if (n3_start > s.size())
            throw std::runtime_error("Incorrect start position");

        if (n3_start == s.size())
            return n_count >= 3;

        const int Number_max = std::numeric_limits<int>::max();

        // n1 + n2 does not fit into int
        if (Number_max - n1 < n2)
            return false;

        const int sum12 = n1 + n2;

        for (int i = n3_start, n3 = 0; i < s.size(); i++) {
            // n3 has leading "0", don't continue
            if (n3 == 0 && i > n3_start)
                return false;

            // The current digit does not fit into int
            if (n3 > 0 && Number_max / n3 < 10)
                return false;

            // Add the current digit
            n3 = n3 * 10 + (s[i] - '0');

            // n3 is n1 + n2 or just the 1st/2nd number, check the next number
            if (n3 == sum12 || n_count < 2) {
                if (isAdditiveNumber(s, n2, n3, i + 1, n_count + 1))
                    return true;
                // All next n3 will be greater, don't check them
                if (n_count >= 2)
                    return false;
            }
        }
        return false;
    }
};


template <typename Solution>
void test()
{
    ASSERT( Solution().run(std::string()) == false );

    ASSERT( Solution().run("0") == false );
    ASSERT( Solution().run("1") == false );

    ASSERT( Solution().run("00") == false );
    ASSERT( Solution().run("01") == false );
    ASSERT( Solution().run("10") == false );
    ASSERT( Solution().run("11") == false );

    ASSERT( Solution().run("000") == true );
    ASSERT( Solution().run("001") == false );
    ASSERT( Solution().run("010") == false );
    ASSERT( Solution().run("011") == true );
    ASSERT( Solution().run("100") == false );
    ASSERT( Solution().run("101") == true );
    ASSERT( Solution().run("110") == false );
    ASSERT( Solution().run("111") == false );

    ASSERT( Solution().run("112") == true );
    ASSERT( Solution().run("122") == false );
    ASSERT( Solution().run("123") == true );
    ASSERT( Solution().run("189") == true );

    ASSERT( Solution().run("1010") == false );
    ASSERT( Solution().run("1910") == true );   // 1 9 10
    ASSERT( Solution().run("9918") == true );   // 9 9 18

    ASSERT( Solution().run("0112") == true );   // 0 1 1 2
    ASSERT( Solution().run("1012") == false );  // not 1 01 2
    ASSERT( Solution().run("1102") == false );  // not 1 1 02

    ASSERT( Solution().run("11011") == true );  // 1 10 11
    ASSERT( Solution().run("10111") == true );  // 10 1 11
    ASSERT( Solution().run("10112") == true );  // 1 0 1 1 2

    ASSERT( Solution().run("101020") == true ); // 10 10 20
    ASSERT( Solution().run("112233") == true ); // 11 22 33

    const int MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run("0" + to_string(MAX) + to_string(MAX)) == true );
    ASSERT( Solution().run("1" + to_string(MAX - 1) + to_string(MAX)) == true );
    ASSERT( Solution().run(to_string(MAX / 2) + to_string(MAX / 2) + to_string(MAX / 2 + MAX / 2)) == true );

    // Test for overflow: MAX9 + 1 != 0. MAX9 is the maximum integer with "9"
    // appended to the end, e.g. "21474836479" if the solution uses 32-bit
    // signed int. If such a string is converted to the integer without overflow
    // check, it will be -1 (MAX9 is MAX * 10 + 9, which is -10 + 9 = -1), so
    // MAX9 + 1 will be 0.

    ASSERT( Solution().run(to_string(MAX) + "910") == false );
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
