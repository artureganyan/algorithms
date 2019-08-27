// Desription: https://leetcode.com/problems/longest-substring-without-repeating-characters/

#include <string>
#include <unordered_map>
#include "utils.h"

namespace longest_substring_without_repeating_characters {

class Solution
{
public:
    // Time: O(n), Space: O(1)
    int run(std::string s)
    {
        typedef std::unordered_map<char, size_t /*pos*/> CharMap;
        CharMap chars;

        size_t max_length = 0;
        size_t seq_start  = 0;

        for (size_t i = 0; i < s.size(); i++) {
            char c = s[i];

            CharMap::const_iterator ci = chars.find(c);
            if (ci != chars.end()) {
                max_length = std::max(max_length, i - seq_start);
                seq_start  = ci->second + 1;
            }
            chars[c] = i;
        }

        max_length = std::max(max_length, s.size() - seq_start);
        return (int) max_length;
    }
};

int main()
{
    ASSERT(Solution().run("") == 0);
    ASSERT(Solution().run("aaa") == 1);
    ASSERT(Solution().run("abcab") == 3);
    ASSERT(Solution().run("abcabxycbbztore") == 6);
    ASSERT(Solution().run("pwwket") == 4);
    return 0;
}

}
