// Problem: https://leetcode.com/problems/integer-replacement/

#include "utils.h"

namespace integer_replacement {

class Solution1 {
public:
    // Note: n must be > 0. If n <= 0, returns 0.
    //
    // Time: O(log(n)), Space: O(1)
    //
    // Note: log(n) is the number of divisions required to get 1 from n == 2^i.
    // If n != 2^i, the worst case is when we need to subtract 1 on every two
    // divisions, e.g. for binary "0101...0101", requiring < 2*log(n) steps
    // (as tests show, this binary is not the worst, but it's close). Also,
    // since log(n) is limited by the bit size of int, the time complexity
    // can be estimated as O(1) (while the number of steps is ~log(n)).
    //
    int run(int n)
    {
        // Idea:
        // If n == 2^i, then we need exactly log(n)=i divisions by 2 to get 1.
        // Otherwise, after zero or more divisions by 2 we will get an odd
        // value, so will spend an extra step to add or subtract 1 (producing
        // an even value again). As every odd value requires an extra step, and
        // we need to minimize the number of steps, we need to minimize the
        // number of odd values produced by division. Division by 2 means
        // shifting 1 bit right; an odd value has the first bit set to 1.
        // Giving these facts, we can see when to add or subtract 1 to minimize
        // the number of odd values:
        //
        // 1. If we have the odd binary "xx...x01", _do not_ add 1, because it
        //    will set the 2nd bit to 1, and the next division will produce the
        //    odd value again. So we should subtract 1 in that case.
        //
        // 2. If we have the odd binary "xx...x11", _do not_ subtract 1,
        //    because we already know that the next division will produce the
        //    odd value again. So we should add 1, which will be carried
        //    through the 2nd and probably more bits, setting them to 0.
        //
        // But our goal - the value 1 - is also odd number, so the rule 2
        // should have the exception: if the value is "00...011", i.e. 3, then
        // subtract 1. Otherwise, adding 1 to 3 produces 4 which requires one
        // extra division to get 1.

        if (n <= 1)
            return 0;

        unsigned int v     = n; // Use the larger type to avoid overflow on n+1 when n is max
        int          steps = 0;

        while (v > 1) {
            if (v & 1) {
                if ((v & 2) && (v != 3)) {
                    v += 1;
                } else {
                    v -= 1;
                }
            } else {
                v >>= 1;
            }
            steps++;
        }
        
        return steps;
    }
};


// Straightforward recursive solution for tests

class Solution2 {
public:
    int run(int n)
    {
        if (n < 0)
            return 0;

        return getMinimimumNumberOfOperations(n);
    }

private:
    // This helper uses the larger type to avoid overflow when n is max
    int getMinimimumNumberOfOperations(unsigned int n) const
    {
        if (n <= 1)
            return 0;

        if (n & 1) {
            const int count1 = getMinimimumNumberOfOperations(n - 1);
            const int count2 = getMinimimumNumberOfOperations(n + 1);
            return 1 + std::min(count1, count2);
        } else {
            return 1 + getMinimimumNumberOfOperations(n / 2);
        }
    }
};


void test(int n)
{
    const int result   = Solution1().run(n);
    const int expected = Solution2().run(n);

    ASSERT_EX( result == expected,
        to_string(n) + " -> " + to_string(expected) );
}

int main()
{
    test(-1);
    test(0);

    for (int i = 1; i <= 32; i++)
        test(i);
    
    if (std::numeric_limits<int>::digits >= 31) {
        test(0b01010101010101010101010101010101); // 45 steps
        test(0b01101010101010101010101010101011); // 46 steps, probably the worst case
    }

    test(std::numeric_limits<int>::max() / 2);
    test(std::numeric_limits<int>::max() - 1);
    test(std::numeric_limits<int>::max());

    return 0;
}

}
