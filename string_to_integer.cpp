// Problem: https://leetcode.com/problems/string-to-integer-atoi/

#include <string>
#include <limits>
#include "utils.h"

namespace string_to_integer {

class Solution
{
public:
    // Time: O(n), Space: O(1)
    //
    // Leading spaces and trailing non-numeric characters are ignored.
    //
    int run(std::string s)
    {
        static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");

        if (!s.size())
            return 0;

        // Detect substring [i1, i2] where number is placed
        int sign = +1;
        int i1 = -1;
        int i2 = -2;

        for (int i = 0; i < s.size(); i++) {
            const char c = s[i];
            if (i1 < 0) {
                if (c == ' ') {
                    continue;
                } else if (c == '-') {
                    sign = -1;
                    i1 = i + 1;
                } else if (c == '+') {
                    sign = +1;
                    i1 = i + 1;
                } else if ('0' <= c && c <= '9') {
                    i1 = i;
                    i2 = i;
                } else {
                    return 0;
                }
            } else if ('0' <= c && c <= '9') {
                i2 = i;
                continue;
            } else {
                break;
            }
        }

        if (i2 < i1)
            return 0;

        // Convert string to integer
        const int INT_MAX = std::numeric_limits<int>::max();
        const int INT_MIN = std::numeric_limits<int>::min();
        const int MAX_DIGITS = std::numeric_limits<int>::digits10;

        int result          = 0;
        int multiplier      = 1;
        int digits_count    = 1;

        for (int i = i2; i >= i1; i--) {
            const int digit = (s[i] - '0');

            if ((digits_count == MAX_DIGITS && digit > INT_MAX / multiplier) ||
                (digits_count >  MAX_DIGITS && digit > 0))
                return sign > 0 ? INT_MAX : INT_MIN;

            const int add = sign * digit * multiplier;
            if (sign > 0 && result > (INT_MAX - add)) return INT_MAX;
            if (sign < 0 && result < (INT_MIN - add)) return INT_MIN;

            result += add;
            multiplier *= 10;
            digits_count++;
        }

        return result;
    }
};

int main()
{
    ASSERT(Solution().run("42") == 42);
    ASSERT(Solution().run("   -42") == -42);
    ASSERT(Solution().run("-42   ") == -42);
    ASSERT(Solution().run("4193 and words") == 4193);
    ASSERT(Solution().run("words and 4193") == 0);
    ASSERT(Solution().run("-91283472332") == -2147483648);
    ASSERT(Solution().run("+91283472332") == +2147483647);
    ASSERT(Solution().run("") == 0);
    ASSERT(Solution().run("x1") == 0);
    ASSERT(Solution().run("1x") == 1);
    ASSERT(Solution().run("+1") == 1);
    ASSERT(Solution().run("++1") == 0);
    ASSERT(Solution().run("--1") == 0);
    ASSERT(Solution().run("+") == 0);
    ASSERT(Solution().run("-") == 0);
    ASSERT(Solution().run(" ") == 0);
    ASSERT(Solution().run("") == 0);
    ASSERT(Solution().run("10000000000") == +2147483647);
    ASSERT(Solution().run("-10000000000") == -2147483648);
    ASSERT(Solution().run("00002147483647") == +2147483647);
    ASSERT(Solution().run("-00002147483648") == -2147483648);
    return 0;
}

}
