// Problem: https://leetcode.com/problems/reverse-words-in-a-string/

#include <string>
#include "utils.h"

namespace reverse_words_in_a_string {

class Solution {
public:
    // Time: O(n), Space: O(1) (supposing that returned value is moved)
    // n - length of the string
    //
    std::string run(std::string s)
    {
        removeExcessiveSpaces(s);
        reverse(s, 0, s.size());
        reverseWords(s);
        return s;
    }

private:
    // Removes leading, trailing and repeated spaces
    static void removeExcessiveSpaces(std::string& s)
    {
        if (!s.size())
            return;

        int removed_count = 0;

        for (int i = 0; i < s.size(); i++) {
            const char c = s[i];
            if (std::isspace(c)) {
                if (i == 0 || std::isspace(s[i - 1])) {
                    removed_count++;
                    continue;
                }
            }
            s[i - removed_count] = c;
        }

        s.resize(s.size() - removed_count);
        if (s.size() && std::isspace(s.back()))
            s.resize(s.size() - 1);
    }

    static void reverse(std::string& s, int i1, int i2)
    {
        for (i2--; i1 < i2; i1++, i2--)
            std::swap(s[i1], s[i2]);
    }

    static void reverseWords(std::string& s)
    {
        int word_start = 0;
        for (int i = 0; i < s.size(); i++) {
            if (std::isspace(s[i])) {
                reverse(s, word_start, i);
                word_start = i + 1;
            }
        }
        reverse(s, word_start, s.size());
    }
};

int main()
{
    ASSERT( Solution().run(std::string()) == std::string() );
    ASSERT( Solution().run(" ") == std::string() );
    ASSERT( Solution().run("  ") == std::string() );
    ASSERT( Solution().run("   ") == std::string() );
    ASSERT( Solution().run("a") == "a" );
    ASSERT( Solution().run(" a ") == "a" );
    ASSERT( Solution().run(" a  a ") == "a a" );
    ASSERT( Solution().run(" ab  a ") == "a ab" );
    ASSERT( Solution().run("  word1  w2  abc  ") == "abc w2 word1" );

    return 0;
}

}
