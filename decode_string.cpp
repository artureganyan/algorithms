// Problem: https://leetcode.com/problems/decode-string/

#include "utils.h"
#include <string>

namespace decode_string {

class Solution {
public:
    // Note: The string can contain any characters except zero digit '0', and
    // can be empty. Every integer it contains must be followed by "[repeat]",
    // where the repeat string has the same format. Braces '[' and ']' are only
    // allowed after an integer as shown above.
    //
    // Time: O(r), Space: O(r), Recursion depth <= d_max + 1
    // r     - number of characters in the result
    // d_max - maximum nesting level of "[]"
    //
    std::string run(const std::string& s)
    {
        std::string result;
        int         pos = 0;

        decode(s, result, pos);
        if (pos != s.size())
            throwError("Mismatched ]", pos);

        return result;
    }

private:
    void decode(const std::string& s, std::string& result, int& pos) const
    {
        for (int& i = pos; i < s.size(); i++) {
            const char c = s[pos];

            if (std::isdigit(c)) {
                int count = readInteger(s, i);
                if (count <= 0)
                    throwError("Expected number > 0", i);
                if (i == s.size() || s[i] != '[')
                    throwError("Expected [", i);

                const int repeat_start = result.size();
                decode(s, result, ++i);

                const int  repeat_size = result.size() - repeat_start;
                const auto repeat      = result.substr(repeat_start, repeat_size);
                while (--count)
                    result += repeat;
                    
            } else if (c == '[') {
                throwError("Expected number before [", i);

            } else if (c == ']') {
                return;
            
            } else {
                result += c;
            }
        }
    }

    int readInteger(const std::string& s, int& pos) const
    {
        std::string number;
        for (; pos < s.size(); pos++) {
            const char c = s[pos];
            if (!std::isdigit(c))
                break;
            number += c;
        }
        return std::stoi(number);
    }

    void throwError(const std::string& message, int pos = -1) const
    {
        throw std::invalid_argument(message + (pos >= 0 ? " at " + to_string(pos) : ""));
    };
};

int main()
{
    ASSERT( Solution().run("") == "" );

    ASSERT( Solution().run(" ") == " " );
    ASSERT( Solution().run("  ") == "  " );
    ASSERT( Solution().run("a") == "a" );
    ASSERT( Solution().run("ab") == "ab" );

    ASSERT( Solution().run("1[]") == "" );
    ASSERT( Solution().run("1[a]") == "a" );
    ASSERT( Solution().run("1[ab]") == "ab" );
    ASSERT( Solution().run("1[ a]") == " a" );
    ASSERT( Solution().run("1[a ]") == "a " );
    ASSERT( Solution().run("1[ a ]") == " a " );

    ASSERT( Solution().run("2[]") == "" );
    ASSERT( Solution().run("2[a]") == "aa" );
    ASSERT( Solution().run("2[ab]") == "abab" );
    ASSERT( Solution().run("2[ a]") == " a a" );
    ASSERT( Solution().run("2[a ]") == "a a " );
    ASSERT( Solution().run("2[ a ]") == " a  a " );

    ASSERT( Solution().run("1[]1[]") == "" );
    ASSERT( Solution().run("1[1[]]") == "" );
    ASSERT( Solution().run("1[1[]1[]]") == "" );
    ASSERT( Solution().run("1[1[1[]]]") == "" );
    ASSERT( Solution().run("2[]2[]") == "" );
    ASSERT( Solution().run("2[2[]]") == "" );
    ASSERT( Solution().run("2[2[]2[]]") == "" );
    ASSERT( Solution().run("2[2[2[]]]") == "" );

    ASSERT( Solution().run("1[a]1[a]") == "aa" );
    ASSERT( Solution().run("1[1[a]]") == "a" );
    ASSERT( Solution().run("1[1[a]1[a]]") == "aa" );
    ASSERT( Solution().run("1[1[1[a]]]") == "a" );
    ASSERT( Solution().run("2[a]2[a]") == "aaaa" );
    ASSERT( Solution().run("2[2[a]]") == "aaaa" );
    ASSERT( Solution().run("2[2[a]2[a]]") == "aaaaaaaa" );
    ASSERT( Solution().run("2[2[2[a]]]") == "aaaaaaaa" );

    ASSERT( Solution().run("abc") == "abc" );
    ASSERT( Solution().run("a2[b]c3[de]f") == "abbcdededef" );
    ASSERT( Solution().run("a2[bc3[de]]f") == "abcdededebcdededef" );
    ASSERT( Solution().run("2[a3[bc]2[d]1[e]fg]ab3[c]2[d]e") == "abcbcbcddefgabcbcbcddefgabcccdde" );

    ASSERT_EXCEPTION( Solution().run("1"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("1["), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("1]"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("1a"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a1b"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a]"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a["), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("1[a]]"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("1[[a]"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("0"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("0[a]"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("1[0[a]]"), std::invalid_argument );

    return 0;
}

}
