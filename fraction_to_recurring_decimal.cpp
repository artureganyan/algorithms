// Problem: https://leetcode.com/problems/fraction-to-recurring-decimal/

#include <string>
#include <unordered_map>
#include <cmath>
#include "utils.h"

namespace fraction_to_recurring_decimal {

class Solution {
public:
    // Time: O(n), Space: O(n)
    // n - number of digits in the decimal representation of v = numerator / denominator,
    //     supposing that the repeating fractional part is written only once.
    //
    // Note: If not count the result string, the space complexity is O(f) where
    // f is the number of digits in the fractional part of the representation
    // described above.
    //
    std::string run(int numerator, int denominator)
    {
        if (denominator == 0)
            throw std::runtime_error("Integer division by zero");

        if (numerator == 0)
            return "0";

        // Calculate integral part
        const int integral_part = numerator / denominator;
        std::string result = std::to_string(integral_part);

        // Calculate fraction part
        int fraction_part = numerator - integral_part * denominator;
        if (fraction_part == 0)
            return result;

        const bool is_negative = std::signbit(numerator) ^ std::signbit(denominator);
        if (integral_part == 0 && is_negative)
            result.insert(0, "-");
        result += ".";

        std::unordered_map<int /*fraction*/, size_t /*pos*/> fractions;
        while (fraction_part != 0) {
            fraction_part *= 10;

            auto& fraction_pos = fractions[fraction_part];
            if (fraction_pos != 0) {
                result.insert(fraction_pos, "(");
                result += ")";
                break;
            }
            fraction_pos = result.size();

            const int fraction_digit = fraction_part / denominator;
            if (fraction_digit >= 0)
                result += '0' + fraction_digit; else
                result += '0' - fraction_digit;

            fraction_part -= fraction_digit * denominator;
        }

        return result;
    }
};

int main()
{
    ASSERT( Solution().run(0, 1) == "0" );
    ASSERT( Solution().run(1, 1) == "1" );
    ASSERT( Solution().run(-1, 1) == "-1" );
    ASSERT( Solution().run(1, -1) == "-1" );
    ASSERT( Solution().run(-1, -1) == "1" );

    ASSERT( Solution().run(1, 2) == "0.5" );
    ASSERT( Solution().run(1, -2) == "-0.5" );
    ASSERT( Solution().run(-1, 2) == "-0.5" );
    ASSERT( Solution().run(-1, -2) == "0.5" );

    ASSERT( Solution().run(1, 3) == "0.(3)" );
    ASSERT( Solution().run(1, 4) == "0.25" );
    ASSERT( Solution().run(1, 5) == "0.2" );
    ASSERT( Solution().run(1, 6) == "0.1(6)" );
    ASSERT( Solution().run(1, 7) == "0.(142857)" );
    ASSERT( Solution().run(1, 8) == "0.125" );
    ASSERT( Solution().run(1, 9) == "0.(1)" );
    ASSERT( Solution().run(1, 10) == "0.1" );
    ASSERT( Solution().run(2, 10) == "0.2" );
    ASSERT( Solution().run(3, 10) == "0.3" );
    ASSERT( Solution().run(9, 10) == "0.9" );
    ASSERT( Solution().run(1, 60) == "0.01(6)" );
    ASSERT( Solution().run(1, 100) == "0.01" );
    ASSERT( Solution().run(1, 1000) == "0.001" );
    ASSERT( Solution().run(1, 3000) == "0.000(3)" );

    ASSERT( Solution().run(10, 1) == "10" );
    ASSERT( Solution().run(10, 2) == "5" );
    ASSERT( Solution().run(10, 3) == "3.(3)" );
    ASSERT( Solution().run(10, 9) == "1.(1)" );
    ASSERT( Solution().run(10, 10) == "1" );
    ASSERT( Solution().run(10, 20) == "0.5" );
    ASSERT( Solution().run(10, 30) == "0.(3)" );
    ASSERT( Solution().run(10, 100) == "0.1" );

    ASSERT( Solution().run(std::numeric_limits<int>::max(), 1) == std::to_string(std::numeric_limits<int>::max()) );
    ASSERT( Solution().run(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()) == "1" );

    ASSERT( Solution().run(std::numeric_limits<int>::min(), 1) == std::to_string(std::numeric_limits<int>::min()) );
    ASSERT( Solution().run(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()) == "1" );

    return 0;
}

}
