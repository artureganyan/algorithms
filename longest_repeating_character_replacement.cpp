// Problem: https://leetcode.com/problems/longest-repeating-character-replacement/

#include "utils.h"
#include <string>

namespace longest_repeating_character_replacement {

class Solution {
public:
    // Note: The string must contain only the following uppercase characters:
    // A-Z. If max_replacements < 0, sets it to 0.
    //
    // Time: O(n), Space: O(1), n - number of characters
    //
    int run(const std::string& s, int max_replacements)
    {
        if (!s.size())
            return 0;

        if (max_replacements >= s.size() - 1)
            return s.size();

        if (max_replacements < 0)
            max_replacements = 0;

        // Represents [start, end) interval, with zero or more replacements
        struct Sequence
        {
            int start        = 0;
            int end          = 0;
            int replacements = 0;
        };

        // Go through the string and update the sequence for each character
        std::vector<Sequence> sequences(CHAR_COUNT);
        int                   max_sequence_length = 0;

        for (int i = 0; i < s.size(); i++) {
            const char c  = s[i];
            auto&      cs = sequences[c - CHAR_FIRST];

            // Use available replacements to extend the sequence to the current
            // character. If this is not possible, advance the sequence start 
            // until this become possible.
            const int replacements_required  = i - cs.end;
            const int replacements_available = max_replacements - cs.replacements;
            int       max_current_length     = 0;

            if (replacements_required <= replacements_available) {
                cs.end              = i + 1;
                cs.replacements    += replacements_required;
                max_current_length  = cs.end - cs.start;
            } else {
                cs.end             += replacements_available;
                cs.replacements    += replacements_available;
                max_current_length  = cs.end - cs.start;

                for (int r = replacements_available; r < replacements_required;) {
                    if (s[cs.start++] != c)
                        r++;
                }
                cs.end = i + 1;
            }

            max_sequence_length = std::max(max_sequence_length, max_current_length);
        }

        // Apply unused replacements
        for (const auto& cs : sequences) {
            const int replacements_available = max_replacements - cs.replacements;
            if (replacements_available) {
                const int cs_end    = std::min(cs.end + replacements_available, (int) s.size());
                max_sequence_length = std::max(max_sequence_length, cs_end - cs.start);
            }
        }

        return max_sequence_length;
    }

    static const char CHAR_FIRST = 'A';
    static const char CHAR_LAST  = 'Z';
    static const int  CHAR_COUNT = CHAR_LAST - CHAR_FIRST;
};


int main()
{
    const auto test = [](const std::string& s, int max_replacements, int expected)
    {
        ASSERT_EX( Solution().run(s, max_replacements) == expected,
            s + ", " + to_string(max_replacements) + " -> " + to_string(expected) );
    };

    test("", 0, 0);
    test("", 1, 0);

    test("A", 0, 1);
    test("A", 1, 1);

    test("AA", 0, 2);
    test("AA", 1, 2);
    test("AB", 0, 1);
    test("AB", 1, 2);

    test("AAA", 0, 3);
    test("BAA", 0, 2);
    test("ABA", 0, 1);
    test("AAB", 0, 2);
    test("BAA", 1, 3);
    test("ABA", 1, 3);
    test("AAB", 1, 3);
    test("BAA", 2, 3);
    test("ABA", 2, 3);
    test("AAB", 2, 3);

    test("ABC", 0, 1);
    test("ABC", 1, 2);
    test("ABC", 2, 3);
    test("ABC", 3, 3);

    test("AABB", 0, 2);
    test("ABAB", 0, 1);
    test("BAAB", 0, 2);
    test("AABB", 1, 3);
    test("ABAB", 1, 3);
    test("BAAB", 1, 3);
    test("AABB", 2, 4);
    test("ABAB", 2, 4);
    test("BAAB", 2, 4);
    test("AABB", 3, 4);

    test("ABCAAC", 1, 3);
    test("ABCAAC", 2, 5);
    test("ABCAAC", 3, 6);

    //       BBBB
    test("ABBCACABBA", 4, 8);

    std::string all_chars;
    for (char c = Solution::CHAR_FIRST; c <= Solution::CHAR_LAST; c++)
        all_chars += c;

    test(all_chars, 0, 1);
    test(all_chars, 1, 2);

    const int MAX = std::numeric_limits<int>::max();

    test("", MAX, 0);
    test("A", MAX, 1);
    test(all_chars, MAX, all_chars.size());

    return 0;
}

}
