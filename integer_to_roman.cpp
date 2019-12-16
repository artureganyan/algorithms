// Problem: https://leetcode.com/problems/integer-to-roman/

#include <string>
#include "utils.h"

namespace integer_to_roman {

class Solution1 {
public:
    // Time: O(n), Space: O(n), n - number of digits
    std::string run(int num)
    {
        if (num <= 0 || num > 3999)
            return std::string();

        const char symbols[] = {'I', 'V', 'X', 'L', 'C', 'D', 'M', '?', '?'};

        std::string result;
        int i = sizeof(symbols) / sizeof(symbols[0]) - 1;
        int d = 1000;

        while (num) {
            int digit = num / d;
            num -= digit * d;
            d	/= 10;
            i	-= 2;

            if (!digit)
                continue;

            const char one	= symbols[i];
            const char five	= symbols[i + 1];
            const char ten	= symbols[i + 2];

            if (digit <= 3) {
                result += std::string(digit, one);
            } else if (digit == 4) {
                result += one;
                result += five;
            } else if (digit <= 8) {
                result += five;
                result += std::string(digit - 5, one);
            } else {
                result += one;
                result += ten;
            }
        }

        return result;
    }
};


// A bit more verbose version

class Solution2 {
public:
    // Time: O(n), Space: O(n), n - number of digits
    std::string run(int num)
    {
        if (num <= 0 || num > 3999)
            return std::string();

        struct Symbol
        {
            int		multiplier;
            char	one;
            char	five;
        };

        const Symbol symbols[] = {{1, 'I', 'V'}, {10, 'X', 'L'}, {100, 'C', 'D'}, {1000, 'M', '?'}, {10000, '?', '?'}};
        const size_t symbols_count = sizeof(symbols) / sizeof(symbols[0]);

        std::string result;

        for (int i = symbols_count - 1; i >= 0; i--) {
            const Symbol &s = symbols[i];
            if (num < s.multiplier)
                continue;

            int digit = num / s.multiplier;
            num -= digit * s.multiplier;

            if (digit <= 3) {
                result += std::string(digit, s.one);
            } else if (digit == 4) {
                result += s.one;
                result += s.five;
            } else if (digit <= 8) {
                result += s.five;
                result += std::string(digit - 5, s.one);
            } else {
                result += s.one;
                result += symbols[i + 1].one;
            }
        }

        return result;
    }
};


template <typename Solution>
void test()
{
    ASSERT(Solution().run(1) == "I");
    ASSERT(Solution().run(3) == "III");
    ASSERT(Solution().run(4) == "IV");
    ASSERT(Solution().run(5) == "V");
    ASSERT(Solution().run(8) == "VIII");
    ASSERT(Solution().run(9) == "IX");
    ASSERT(Solution().run(10) == "X");
    ASSERT(Solution().run(49) == "XLIX");
    ASSERT(Solution().run(50) == "L");
    ASSERT(Solution().run(58) == "LVIII");
    ASSERT(Solution().run(1994) == "MCMXCIV");
    ASSERT(Solution().run(2001) == "MMI");
    ASSERT(Solution().run(3999) == "MMMCMXCIX");
    ASSERT(Solution().run(-1) == "");
    ASSERT(Solution().run(0) == "");
    ASSERT(Solution().run(4000) == "");
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
