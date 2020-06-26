// Problem: https://leetcode.com/problems/elimination-game/

#include "utils.h"

namespace elimination_game {

class Solution {
public:
    // Note: If n <= 0, returns 0.
    //
    // Time: O(log(n)), Space: O(1), n - number of integers
    //
    int run(int n)
    {
        // Idea:
        // Each pass keeps every second number, i.e. the half of numbers, so
        // that n=floor(n/2), giving floor(log(n)) total passes. Eliminating
        // the half of numbers in this way doubles the distance between
        // consecutive numbers: e.g. 1 2 3 4 5 6 7 8 9 -> 2 4 6 8 -> 4 8.
        // Finally, the left-to-right pass through the range [i1, i2] always
        // eliminates the first number, increasing i1 by the current distance,
        // and eliminates the last number if the range size is odd, decreasing
        // i2 by the same distance. The right-to-left pass works symmetrically:
        // always decreases i2 and increases i1 if the size is odd. So we can
        // iteratively narrow [1, n] range to the single number, dividing the
        // amount of numbers by 2, doubling the distance and shifting the range
        // borders:
        //
        // 1 2 3 4 5 6 7 8 9 - i1=1, i2=9, n=9, distance is 1
        // |---|---|---|-->|
        //
        //   2   4   6   8   - i1=2, i2=8, n=4, distance is 2
        //   <---|-------|
        //
        //   2       6       - i1=2, i2=6, n=2, distance is 4
        //   |------->
        //
        //           6       - i1=6, i2=6, n=1
        //

        if (n <= 0)
            return 0;

        int i1 = 1;
        int i2 = n;

        for (int distance = 1, direction = 1; i1 < i2; distance *= 2, direction *= -1, n /= 2) {
            const bool n_odd = n % 2;
            if (direction > 0 || n_odd)
                i1 += distance;
            if (direction < 0 || n_odd)
                i2 -= distance;
        }

        return i1;
    }
};

int main()
{
    ASSERT( Solution().run(-1) == 0 );
    ASSERT( Solution().run(0) == 0 );
    ASSERT( Solution().run(1) == 1 );
    ASSERT( Solution().run(2) == 2 );
    ASSERT( Solution().run(3) == 2 );
    ASSERT( Solution().run(4) == 2 );
    ASSERT( Solution().run(5) == 2 );
    ASSERT( Solution().run(6) == 4 );
    ASSERT( Solution().run(7) == 4 );
    ASSERT( Solution().run(8) == 6 );
    ASSERT( Solution().run(9) == 6 );
    ASSERT( Solution().run(10) == 8 );
    ASSERT( Solution().run(11) == 8 );
    ASSERT( Solution().run(12) == 6 );
    ASSERT( Solution().run(1024) == 342 );
    ASSERT( Solution().run(1025) == 342 );

    // Make sure it's done in reasonable time
    const int MAX = std::numeric_limits<int>::max();
    ASSERT(( Solution().run(MAX), true ));

    return 0;
}

}
