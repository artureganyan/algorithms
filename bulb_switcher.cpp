// Problem: https://leetcode.com/problems/bulb-switcher/

#include <cmath>
#include "utils.h"

namespace bulb_switcher {

class Solution {
public:
    // Note: If n <= 0, returns 0.
    //
    // Time: O(1), Space: O(1)
    //
    int run(int n)
    {
        // Idea:
        // The pass number k switches the bulb number i only if i divides by k.
        // Initially, all the n bulbs are turned off. Therefore, after n passes,
        // the i-th bulb will be turned on if there are odd number of passes
        // that switch i, i.e. if i has the odd number of positive integer
        // divisors. Any positive integer divides by 1 and by itself, and, if
        // it's not a square, any other divisor d1 gives the quotient d2 such
        // that d1 != d2: i = 1 * i, d1 * d2, d3 * d4, ..., d[k-1] * d[k], the
        // number of divisors is even. If i is x^2, then d1 = d2 = x, so the
        // number of divisors is odd. So the total number of bulbs which will
        // be turned on after n passes is the number of squares within [1, n],
        // which is floor(sqrt(n)).

        if (n < 0)
            return 0;

        return (int) std::sqrt(n);
    }
};

int main()
{
    ASSERT( Solution().run(0) == 0 );

    ASSERT( Solution().run(1) == 1 );
    ASSERT( Solution().run(2) == 1 );
    ASSERT( Solution().run(3) == 1 );
    ASSERT( Solution().run(4) == 2 );
    ASSERT( Solution().run(5) == 2 );
    ASSERT( Solution().run(10) == 3 );
    ASSERT( Solution().run(100) == 10 );
    ASSERT( Solution().run(1000) == 31 );

    const int MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run(MAX) == (int) std::sqrt(MAX) );

    return 0;
}

}
