// Problem: https://leetcode.com/problems/reconstruct-original-digits-from-english/

#include "utils.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace reconstruct_original_digits_from_english {

class Solution {
public:
    // Note: The s must contain only the lowercase letters which must be
    // possible to reorder into the names of the digits 0-9. If the s is empty
    // or if not all the letters compose digit names (e.g. "oent" is "one"
    // with unused "t"), returns empty string.
    //
    // Time: O(n), Space: O(n) for result, n - number of characters
    //
    std::string run(const std::string& s)
    {
        if (!s.size())
            return {};

        // Count the string chars
        CharCount s_chars;
        for (const auto c : s)
            s_chars[c]++;

        // Count the digits presented in the string
        static const int digit_order[] = {0, 2, 6, 8, 7, 5, 4, 1, 3, 9};
        std::vector<int> digit_count(10, 0);

        for (const int d : digit_order) {
            int word_count = std::numeric_limits<int>::max();

            for (const auto i : digit_chars[d]) {
                word_count = std::min(word_count, s_chars[i.first] / i.second);
                if (!word_count)
                    break;
            }

            for (const auto i : digit_chars[d])
                s_chars[i.first] -= i.second * word_count;

            digit_count[d] += word_count;
        }

        // Check that all the chars are used
        for (const auto i : s_chars) {
            if (i.second > 0)
                return {};
        }

        // Construct the result
        std::string result;
        for (int d = 0; d < digit_count.size(); d++) {
            const int count = digit_count[d];
            if (count)
                result += std::string(count, '0' + d);
        }
        return result;
    }

    static std::vector<std::string> digit_names;

private:
    typedef std::unordered_map<char, int /*count*/> CharCount;
    typedef std::vector<CharCount> DigitCharCount;

    static DigitCharCount initDigitCharCount()
    {
        DigitCharCount char_count(10);
        for (int d = 0; d < digit_names.size(); d++) {
            const auto& name  = digit_names[d];
            auto&       count = char_count[d];
            for (const auto c : name)
                count[c]++;
        }
        return char_count;
    }

    static DigitCharCount digit_chars;
};

std::vector<std::string> Solution::digit_names = {
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

Solution::DigitCharCount Solution::digit_chars = initDigitCharCount();


int main()
{
    const auto test = [](const std::string& s, const std::string& expected)
    {
        ASSERT_EX( Solution().run(s) == expected, s + " -> " + expected );
    };

    const auto test_shuffled = [](const std::vector<int>& digits)
    {
        std::string s;
        for (int d : digits)
            s += Solution::digit_names[d];

        std::random_shuffle(s.begin(), s.end());

        std::unordered_map<int /*digit*/, int /*count*/> digits_count;
        for (int d : digits)
            digits_count[d]++;

        std::string expected;
        for (int d = 0; d < digits_count.size(); d++) {
            const int count = digits_count[d];
            if (count)
                expected += std::string(count, '0' + d);
        }

        ASSERT_EX( Solution().run(s) == expected, s + " -> " + expected );
    };

    test("", "");

    test("zero",  "0");
    test("one",   "1");
    test("two",   "2");
    test("three", "3");
    test("four",  "4");
    test("five",  "5");
    test("six",   "6");
    test("seven", "7");
    test("eight", "8");
    test("nine",  "9");

    test("zeroone", "01");
    test("onezero", "01");
    test("oeoernz", "01");

    test("zeroonetwo", "012");
    test("otonozerew", "012");

    test("zeroonetwothreefourfivesixseveneightnine", "0123456789");

    // Test for wrong decoding into "three"
    //    |     |    ||    |
    test("twoeightfoureighteight", "24888");

    // Test shuffled
    test_shuffled({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

    // Test shuffled large random input
    {
        const int count = 1000;

        std::vector<int> digits;
        for (int i = 0; i < count; i++)
            digits.push_back(rand() % 10);

        test_shuffled(digits);
    }

    return 0;
}

}
