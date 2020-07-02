// Problem: https://leetcode.com/problems/longest-substring-with-at-least-k-repeating-characters/

#include <string>
#include <unordered_map>
#include "utils.h"

namespace longest_substring_with_at_least_k_repeating_characters {

// Straightforward solution

class Solution {
public:
    // Note: If k <= 0, it's set to 1. The string can contain any characters,
    // although the original problem requires lowercase letters only.
    //
    // Time: O(n^2), Space: O(d)
    // n - length of the string, d - number of different characters in the string
    //
    int run(const std::string& s, int k)
    {
        if (!s.size())
            return 0;

        if (k <= 0)
            k = 1;

        typedef std::unordered_map<char, int /*count*/> CountMap;

        int max_length = 0;

        for (int i1 = 0; i1 < s.size(); i1++) {
            CountMap count;
            int      count_complete = 0;

            for (int i2 = i1; i2 < s.size(); i2++) {
                const char c = s[i2];

                if (++count[c] == k)
                    ++count_complete;

                if (count_complete == count.size())
                    max_length = std::max(max_length, i2 - i1 + 1);
            }
        }

        return max_length;
    }
};

int main()
{
    ASSERT( Solution().run("", -1) == 0 );
    ASSERT( Solution().run("", 0) == 0 );
    ASSERT( Solution().run("", 1) == 0 );

    ASSERT( Solution().run("a", -1) == 1 );
    ASSERT( Solution().run("a", 0) == 1 );
    ASSERT( Solution().run("a", 1) == 1 );
    ASSERT( Solution().run("a", 2) == 0 );

    ASSERT( Solution().run("aa", 1) == 2 );
    ASSERT( Solution().run("aa", 2) == 2 );
    ASSERT( Solution().run("aa", 3) == 0 );

    ASSERT( Solution().run("ab", 1) == 2 );
    ASSERT( Solution().run("ab", 2) == 0 );

    ASSERT( Solution().run("aab", 1) == 3 );
    ASSERT( Solution().run("aba", 1) == 3 );
    ASSERT( Solution().run("aab", 1) == 3 );
    ASSERT( Solution().run("aab", 2) == 2 );
    ASSERT( Solution().run("aba", 2) == 0 );
    ASSERT( Solution().run("baa", 2) == 2 );
    ASSERT( Solution().run("aab", 3) == 0 );

    ASSERT( Solution().run("aabb", 1) == 4 );
    ASSERT( Solution().run("aabb", 2) == 4 );
    ASSERT( Solution().run("abab", 2) == 4 );
    ASSERT( Solution().run("abba", 2) == 4 );
    ASSERT( Solution().run("baba", 2) == 4 );
    ASSERT( Solution().run("bbaa", 2) == 4 );
    ASSERT( Solution().run("aabb", 3) == 0 );

    ASSERT( Solution().run("aaabbc", 1) == 6 );
    ASSERT( Solution().run("aaabbc", 2) == 5 );
    ASSERT( Solution().run("aaabcb", 2) == 3 );
    ASSERT( Solution().run("aaacbb", 2) == 3 );
    ASSERT( Solution().run("aacabb", 2) == 2 );
    ASSERT( Solution().run("acaabb", 2) == 4 );
    ASSERT( Solution().run("caaabb", 2) == 5 );
    ASSERT( Solution().run("aaabbc", 3) == 3 );
    ASSERT( Solution().run("baaabc", 3) == 3 );
    ASSERT( Solution().run("bbaaac", 3) == 3 );
    ASSERT( Solution().run("cbbaaa", 3) == 3 );

    ASSERT( Solution().run("acdadaddecec", 2) == 12 );
    ASSERT( Solution().run("acdadaddecec", 3) == 0 );
    ASSERT( Solution().run("acdadadeecec", 3) == 12 );

    ASSERT( Solution().run("abcdefghijklmnopqrstuvwxyz", 1) == 26 );

    return 0;
}

}
