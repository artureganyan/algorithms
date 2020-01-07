// Problem: https://leetcode.com/problems/word-break/

#include <vector>
#include <string>
#include "utils.h"

namespace word_break {

class Solution {
public:
    // Time: O(n^2 * d), Space: O(n), Recursion depth <= n + 2
    // n - length of the string, d - length of the dictionary
    //
    // Note: The worst case looks like s = "aaaaa", dict = {"aaaab", "aaab",
    // "aab", "ab", "a"}, when we can split s only by the last word ("a"), but
    // need to compare s with the whole dictionary on each step. More
    // precisely, for the example above we compare s with d = dictionary.size()
    // words on the first step, s[2, s.size()] with d-1 words on the second
    // step, s[3, s.size()] with d-2 words on the third step, etc. This results
    // in comparing (n + n-1 + n-2 + ... + 1) characters on the first step,
    // (n-1 + n-2 + ... + 1) on the second one, and so on, giving O(n^2 * n)
    // comparisons. If d > n, the same worst case is when the dictionary
    // contain multiple words of the same length like {"aaaab", "aaaac",
    // "aaab", "aaac", ...}. Then, if d = n * k, the comparisons above are
    // repeated k times, giving O(n^2 * d) operations.
    //
    bool run(const std::string& s, const std::vector<std::string>& dictionary)
    {
        return canSplitByWords(s, 0, dictionary);
    }

private:
    bool canSplitByWords(const std::string& s, int s_start, const std::vector<std::string>& dictionary) const
    {
        if (!s.size() || !dictionary.size())
            return false;

        if (s_start == s.size())
            return true;

        for (int i = 0; i < dictionary.size(); i++) {
            const std::string& word = dictionary[i];
            if (s.compare(s_start, word.size(), word) != 0)
                continue;

            if (canSplitByWords(s, s_start + word.size(), dictionary))
                return true;
        }

        return false;
    }
};


int main()
{
    ASSERT( Solution().run(std::string(), {}) == false );
    ASSERT( Solution().run(std::string(), {std::string()}) == false );
    ASSERT( Solution().run(std::string(), {"a"}) == false );
    ASSERT( Solution().run("a", {}) == false );

    ASSERT( Solution().run("a", {"a"}) == true );
    ASSERT( Solution().run("a", {"b"}) == false );
    ASSERT( Solution().run("a", {"aa"}) == false );
    ASSERT( Solution().run("a", {"aa", "a"}) == true );

    ASSERT( Solution().run("aa", {"a"}) == true );
    ASSERT( Solution().run("aa", {"aa"}) == true );
    ASSERT( Solution().run("aa", {"a", "aa"}) == true );
    ASSERT( Solution().run("aa", {"a", "a"}) == true );
    ASSERT( Solution().run("aa", {"a", "b"}) == true );
    ASSERT( Solution().run("aa", {"b", "a"}) == true );
    ASSERT( Solution().run("aa", {"aaa"}) == false );

    ASSERT( Solution().run("aba", {"a", "b"}) == true );
    ASSERT( Solution().run("abc", {"a", "b", "c"}) == true );
    ASSERT( Solution().run("abc", {"ab", "c"}) == true );
    ASSERT( Solution().run("abc", {"a", "bc"}) == true );
    ASSERT( Solution().run("abc", {"abc"}) == true );
    ASSERT( Solution().run("abc", {"ab", "bc"}) == false );

    ASSERT( Solution().run("abacbc", {"a", "b", "c"}) == true );
    ASSERT( Solution().run("abacbc", {"c", "acb", "ab"}) == true );
    ASSERT( Solution().run("abacbc", {"bc", "ac", "ab"}) == true );
    ASSERT( Solution().run("abacbc", {"cbc", "aba"}) == true );
    ASSERT( Solution().run("abacbc", {"acbc", "abac"}) == false );
    ASSERT( Solution().run("abacbc", {"acb", "ab"}) == false );
    ASSERT( Solution().run("abacbc", {"ab", "cb", "c"}) == false );

    return 0;
}

}
