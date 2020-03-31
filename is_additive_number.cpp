// Problem: https://leetcode.com/problems/additive-number/

#include <string>
#include "utils.h"

namespace is_additive_number {

// Note: It's not clear if '0' should be treated as a valid number in this
// problem. It is valid in the current implementation.

// Straightforward recursive solution

class Solution {
public:
    typedef int Number;

    // Note: The string must contain only the digits '0'-'9'. Any substring
    // starting with '0', except the single '0', is not treated as a number.
    // Numbers and sums greater than Number are not handled.
    //
    bool run(const std::string& num)
    {
        return isAdditiveNumber(num, 0, 0, 0, 0);
    }

private:
    bool isAdditiveNumber(const std::string& s, Number n1, Number n2, int n3_start, int n_count) const
    {
        if (n3_start > s.size())
            throw std::runtime_error("Incorrect start position");

        if (n3_start == s.size())
            return n_count >= 3;

        const Number Number_max = std::numeric_limits<Number>::max();

        // n1 + n2 does not fit into Number
        if (Number_max - n1 < n2)
            return false;

        const Number sum12 = n1 + n2;

        for (int i = n3_start, n3 = 0; i < s.size(); i++) {
            // n3 has leading "0", don't continue
            if (n3 == 0 && i > n3_start)
                return false;

            // The current digit does not fit into Number
            if (n3 > 0 && Number_max / n3 < 10)
                return false;

            // Add the current digit
            n3 = n3 * 10 + (s[i] - '0');

            // n3 is n1 + n2 or just the 1st/2nd number, check the next number
            if (n3 == sum12 || n_count < 2) {
                if (isAdditiveNumber(s, n2, n3, i + 1, n_count + 1))
                    return true;
                // All next n3 will be greater, don't check them
                if (n_count >= 2)
                    return false;
            }
        }
        return false;
    }
};

int main()
{
    ASSERT( Solution().run(std::string()) == false );

    ASSERT( Solution().run("0") == false );
    ASSERT( Solution().run("1") == false );

    ASSERT( Solution().run("00") == false );
    ASSERT( Solution().run("01") == false );
    ASSERT( Solution().run("10") == false );
    ASSERT( Solution().run("11") == false );

    ASSERT( Solution().run("000") == true );
    ASSERT( Solution().run("001") == false );
    ASSERT( Solution().run("010") == false );
    ASSERT( Solution().run("011") == true );
    ASSERT( Solution().run("100") == false );
    ASSERT( Solution().run("101") == true );
    ASSERT( Solution().run("110") == false );
    ASSERT( Solution().run("111") == false );

    ASSERT( Solution().run("112") == true );
    ASSERT( Solution().run("122") == false );
    ASSERT( Solution().run("123") == true );
    ASSERT( Solution().run("189") == true );

    ASSERT( Solution().run("1010") == false );
    ASSERT( Solution().run("1910") == true );   // 1 9 10
    ASSERT( Solution().run("9918") == true );   // 9 9 18

    ASSERT( Solution().run("0112") == true );   // 0 1 1 2
    ASSERT( Solution().run("1012") == false );  // not 1 01 2
    ASSERT( Solution().run("1102") == false );  // not 1 1 02

    ASSERT( Solution().run("11011") == true );  // 1 10 11
    ASSERT( Solution().run("10111") == true );  // 10 1 11
    ASSERT( Solution().run("10112") == true );  // 1 0 1 1 2

    ASSERT( Solution().run("101020") == true ); // 10 10 20
    ASSERT( Solution().run("112233") == true ); // 11 22 33

    const int MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run("0" + to_string(MAX) + to_string(MAX)) == true );
    ASSERT( Solution().run("1" + to_string(MAX - 1) + to_string(MAX)) == true );
    ASSERT( Solution().run(to_string(MAX / 2) + to_string(MAX / 2) + to_string(MAX / 2 + MAX / 2)) == true );

    return 0;
}

}
