// Problem: https://leetcode.com/problems/multiply-strings/

#include <string>
#include <unordered_map>
#include "utils.h"

namespace multiply_strings {

class Solution {
public:
    // Time: O(n * m), Space: O(n + m), n = num1.size(), m = num2.size()
    std::string run(const std::string& num1, const std::string& num2)
    {
        if (!num1.size() || !num2.size())
            return {};

        // Multiply
        std::string result(num1.size() + num2.size(), '0');

        for (int n1 = num1.size() - 1, r1 = result.size() - 1; n1 >= 0; n1--, r1--) {
            const int d1 = num1[n1] - '0';
            if (d1 == 0)
                continue;

            int carry = 0;
            for (int n2 = num2.size() - 1, r2 = r1; n2 >= 0; n2--, r2--) {
                const int d2 = num2[n2] - '0';
                const int dr = result[r2] - '0';
                const int v  = dr + d1 * d2 + carry;

                result[r2] = '0' + (v % 10);
                carry      = v / 10;
            }

            if (carry) {
                const int r2 = r1 - num2.size();
                const int dr = result[r2] - '0';
                result[r2] = '0' + (dr + carry);
            }
        }

        // Remove trailing zeros
        int trailing_zeros = 0;
        while (result[trailing_zeros] == '0' && trailing_zeros < result.size() - 1)
            trailing_zeros++;

        result.erase(result.begin(), result.begin() + trailing_zeros);

        return result;
    }
};

bool test_symmetrical(const std::string& num1, const std::string& num2, const std::string& expected)
{
    return Solution().run(num1, num2) == expected &&
           Solution().run(num2, num1) == expected;
}

int main()
{
    ASSERT(test_symmetrical("", "", std::string()));
    ASSERT(test_symmetrical("0", "", std::string()));
    ASSERT(test_symmetrical("0", "0", "0"));
    ASSERT(test_symmetrical("1", "0", "0"));
    ASSERT(test_symmetrical("1", "1", "1"));
    ASSERT(test_symmetrical("1", "2", "2"));
    ASSERT(test_symmetrical("1", "9", "9"));
    ASSERT(test_symmetrical("2", "5", "10"));
    ASSERT(test_symmetrical("2", "9", "18"));
    ASSERT(test_symmetrical("1", "10", "10"));
    ASSERT(test_symmetrical("10", "10", "100"));
    ASSERT(test_symmetrical("100", "10", "1000"));
    ASSERT(test_symmetrical("101", "5", "505"));
    ASSERT(test_symmetrical("123", "1", "123"));
    ASSERT(test_symmetrical("123", "100", "12300"));
    ASSERT(test_symmetrical("1234567890", "1234567890", "1524157875019052100"));
    return 0;
}

}
