// Problem: https://leetcode.com/problems/integer-replacement/

#include "utils.h"

namespace integer_replacement {

class Solution1 {
public:
    // Note: n must be > 0. If n <= 0, returns 0.
    //
    // Time: O(log(n)), Space: O(1)
    //
    // Note: Since n is limited by the size of int, the time complexity can be
    // estimated as O(1).
    //
    int run(int n)
    {
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
    for (int i = 0; i <= 32; i++)
        test(i);
    
    test(1024);
    test(std::numeric_limits<int>::max() / 2);
    test(std::numeric_limits<int>::max());

    return 0;
}

}
