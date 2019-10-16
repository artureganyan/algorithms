// Problem: https://leetcode.com/problems/letter-combinations-of-a-phone-number/

#include <string>
#include <math.h>
#include "utils.h"

namespace letter_combinations_of_phone_number {

class Solution {
public:
    // Time: O(n * C), Space: S(n * C) for result, C = 3^n1 * 4^n2
    // n  - number of digits,
    // n1 - number of digits associated with 3 letters,
    // n2 - number of digits associated with 4 letters
    //
    // Idea:
    // For digits = [2, 3, 4]:
    // 1. result = [a, b, c]
    // 2. result = [[a, b, c] + d, [a, b, c] + e, [a, b, c] + f] = [result + d, result + e, result + f]
    // 3. result = [result + g, result + h, result + i]
    //
    // Note: If there are only 0 or 1 digits, returns {""}.
    //
    std::vector<std::string> run(std::string digits)
    {
        if (!digits.size())
            return std::vector<std::string>();

        static const char letters[][5] =
        {
            {0, 0, 0, 0, 0},            // 0
            {0, 0, 0, 0, 0},            // 1
            {3, 'a', 'b', 'c', 0},      // 2
            {3, 'd', 'e', 'f', 0},      // 3
            {3, 'g', 'h', 'i', 0},      // 4
            {3, 'j', 'k', 'l', 0},      // 5
            {3, 'm', 'n', 'o', 0},      // 6
            {4, 'p', 'q', 'r', 's'},    // 7
            {3, 't', 'u', 'v', 0},      // 8
            {4, 'w', 'x', 'y', 'z'}     // 9
        };

        std::vector<std::string> result;
        result.push_back(std::string());

        for (size_t i = 0; i < digits.size(); i++) {
            const int digit = (digits[i] - '0');
            if (digit < 0 || digit > 9)
                return std::vector<std::string>();

            const int letter_count = letters[digit][0];

            // Add letter_count-1 duplicates of result to its end
            const size_t result_size = result.size();
            for (int l = 0; l < letter_count - 1; l++) {
                for (size_t r = 0; r < result_size; r++)
                    result.push_back(result[r]);
            }

            // Add each letter to appropriate duplicate in result
            size_t r = 0;
            for (int l = 0; l < letter_count; l++) {
                const char letter = letters[digit][l + 1];
                for (size_t c = 0; c < result_size; c++)
                    result[r++].push_back(letter);
            }
        }

        return result;
    }
};

int main()
{
    ASSERT(compare_sets(Solution().run("23"), std::vector<std::string>({"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"})));
    ASSERT(compare_sets(Solution().run("2"), std::vector<std::string>({"a", "b", "c"})));
    ASSERT(compare_sets(Solution().run(""), std::vector<std::string>()));
    ASSERT(compare_sets(Solution().run("0"), std::vector<std::string>({""})));
    ASSERT(compare_sets(Solution().run("01"), std::vector<std::string>({""})));
    ASSERT(compare_sets(Solution().run("1x2"), std::vector<std::string>()));
    return 0;
}

}
