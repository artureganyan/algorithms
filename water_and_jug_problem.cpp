// Problem: https://leetcode.com/problems/water-and-jug-problem/

#include "utils.h"

namespace water_and_jug_problem {

class Solution {
public:
    // Note: If x < 0, or y < 0, or z < 0, returns false. If z == 0, returns
    // true because 2 empty jugs represent "no water". If one jug is absent
    // (x == 0 or y == 0), returns true if the other is equal to z, otherwise
    // returns false.
    //
    // Time: O(n), Space: O(1), n = max(x, y)
    //
    int run(int x, int y, int z)
    {
        // Idea:
        // If z is not equal to x, y or x+y, we have 2 possible ways to get z
        // from x and y: pour x into y multiple times, or vice versa. Each way
        // is to pour water in one particular direction, because pouring in
        // both directions (like x -> y and then y -> x) has no sense.
        //
        // Let's say x is the largest jug. Then these 2 ways look as follows:
        // 1. Fill x, and then pour x into y and empty y, until x becomes empty.
        //    This is x -= y, giving ceil(x/y) different amounts of water. Also,
        //    the last amount (which is <= y) can be combined with the full x,
        //    giving one more different amount x + y.
        // 2. Fill y, pour y into x and, if x becomes full, empty x and pour
        //    the remainder of y into x. This is x = (x + y) mod x, or, which
        //    is the same, x = i*y mod x, i >= 1. This gives at most x different
        //    amounts of water, because x*y mod x == 0. Also, every time x
        //    becomes full, we have one more different amount x + y.
        //
        // Finally, we may notice that any amount produced by 1 is also
        // produced by 2: since the last step of 1 produces x mod y, then, if
        // there is i >= 0 such that x mod y == i*y mod x, 2 produces the same
        // as 1 at i-th step and later. One can check that i = (x - floor(x/y))
        // satisfies the equation. Therefore we can calculate all the different
        // amounts produced by 2 to find the answer.

        if (x < 0 || y < 0 || z < 0)
            return false;

        if (z == 0)
            return true;

        if (x == 0)
            return z == y;

        if (y == 0)
            return z == x;

        if (z == x || z == y || z == x + y)
            return true;

        if (z > x + y)
            return false;

        // Make sure that x >= y
        if (x < y)
            std::swap(x, y);

        const int x_full = x;
        const int y_full = y; // Unused

        // This is not necessary (see the explanation above)
#if 0
        // Way 1, x -> y: fill x, pour x into y and empty y, i.e. do x -= y,
        // until x > y.
        for (x = x_full; x > y; x -= y) {
            if (x == z)
                return true;
        }
        // Now we have x <= y, so can pour it into y and combine with full x
        if (x == z || x + x_full == z)
            return true;
#endif
        // Way 2, x <- y: fill y, pour it into x and, if x becomes full, empty
        // x and pour the remainder of y into x. So we do x = (x + y) mod x,
        // or, which is the same, x = i*y mod x, i >= 1. Repeat this until we
        // get z, or until y poured into x makes it full (this will happen at
        // least on x-th step because x*y mod x == 0).
        for (x = y; x != x_full; x += y) {
            if (x > x_full)
                x -= x_full;

            if (x == z || (x <= y && x + x_full == z))
                return true;
        }

        return false;
    }
};


void test(int x, int y, int z, bool expected)
{
    const auto run = [](int x, int y, int z, bool expected)
    {
        ASSERT_EX( Solution().run(x, y, z) == expected,
            to_string(x) + ", " + to_string(y) + ", " + to_string(z) + " -> " + to_string(expected) );
    };

    run(x, y, z, expected);

    if (x == y)
        return;

    run(y, x, z, expected);
}

int main()
{
    test(-1, 0, 0, false);
    test(0, 0, -1, false);
    test(-1, 0, -1, false);
    test(-1, -1, -1, false);

    test(0, 0, 0, true);
    test(1, 0, 0, true);
    test(1, 1, 0, true);

    test(0, 0, 1, false);
    test(1, 0, 1, true);
    test(1, 1, 1, true);
    test(1, 2, 1, true);
    test(2, 0, 1, false);
    test(2, 2, 1, false);

    test(1, 2, 3, true);
    test(1, 1, 3, false);
    test(2, 2, 3, false);
    test(2, 3, 3, true);

    test(1, 3, 1, true);
    test(1, 3, 2, true);
    test(1, 3, 3, true);
    test(1, 3, 4, true);
    test(1, 3, 5, false);
    test(1, 3, 6, false);

    test(7, 2, 5, true);
    test(7, 2, 3, true);
    test(7, 2, 1, true);
    test(7, 2, 8, true);
    test(7, 2, 10, false);

    test(7, 3, 1, true);
    test(7, 3, 2, true);
    test(7, 3, 5, true);
    test(7, 3, 8, true);
    test(7, 3, 9, true);

    return 0;
}

}
