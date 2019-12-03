// Problem: https://leetcode.com/problems/decode-ways/

#include <string>
#include "utils.h"

namespace decode_ways {

class Solution {
public:
    // Returns the number of possible decodings for the given string,
    // assuming the following map: '1' -> 'A', '2' -> 'B', ..., '26' -> 'Z'.
    // The string must contain only characters from '0' to '9'. If it's
    // empty, returns 1. If it contains code '0' (e.g. "01", "100", "1001"),
    // returns 0.
    //
    // Time: O(2^n), Space: O(n), Recursion depth: n
    //
    // Note:
    // The best case is when there are no 2-digit codes, resulting in O(n)
    // time. The worst case is when each character can be treated as both
    // 1 and 2-digit code (e.g. "12121..."), resulting in the Fibonacci
    // number result(n) = result(n - 1) + result(n - 2), recursively calculated
    // in O(2^n) time.
    //
    int run(const std::string& encoded)
    {
        return getNumberOfDecodings(encoded, 0);
    }

private:
    int getNumberOfDecodings(const std::string& s, int start) const
    {
        // Substring is empty
        if (start >= s.size())
            return 1;

        const char c1 = s[start];
        // Substring starts with code '0' and can't be decoded
        if (c1 == '0')
            return 0;

        // Count decodings for 1-digit code c1
        int result = getNumberOfDecodings(s, start + 1);

        // Count decodings for 2-digit code c1 c2, if possible
        if (start < s.size() - 1) {
            const char c2   = s[start + 1];
            const int  code = (c1 - '0') * 10 + (c2 - '0');
            if (code <= 26)
                result += getNumberOfDecodings(s, start + 2);
        }

        return result;
    }
};

int main()
{
    ASSERT( Solution().run("") == 1 );
    ASSERT( Solution().run("0") == 0 );
    ASSERT( Solution().run("00") == 0 );
    ASSERT( Solution().run("01") == 0 );
    ASSERT( Solution().run("010") == 0 );
    ASSERT( Solution().run("1") == 1 );
    ASSERT( Solution().run("2") == 1 );
    ASSERT( Solution().run("9") == 1 );
    ASSERT( Solution().run("10") == 1 );
    ASSERT( Solution().run("11") == 2 );
    ASSERT( Solution().run("12") == 2 );
    ASSERT( Solution().run("20") == 1 );
    ASSERT( Solution().run("21") == 2 );
    ASSERT( Solution().run("26") == 2 );
    ASSERT( Solution().run("27") == 1 );
    ASSERT( Solution().run("36") == 1 );
    ASSERT( Solution().run("99") == 1 );
    ASSERT( Solution().run("100") == 0 );
    ASSERT( Solution().run("101") == 1 );
    ASSERT( Solution().run("111") == 3 );
    ASSERT( Solution().run("112") == 3 );
    ASSERT( Solution().run("121") == 3 );
    ASSERT( Solution().run("211") == 3 );
    ASSERT( Solution().run("119") == 3 );
    ASSERT( Solution().run("191") == 2 );
    ASSERT( Solution().run("911") == 2 );
    ASSERT( Solution().run("199") == 2 );
    ASSERT( Solution().run("919") == 2 );
    ASSERT( Solution().run("991") == 1 );
    ASSERT( Solution().run("999") == 1 );
    ASSERT( Solution().run("1000") == 0 );
    ASSERT( Solution().run("1001") == 0 );
    ASSERT( Solution().run("1010") == 1 );
    ASSERT( Solution().run("1100") == 0 );
    ASSERT( Solution().run("1110") == 2 );
    ASSERT( Solution().run("1111") == 5 );

    return 0;
}

}
