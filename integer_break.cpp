// Problem: https://leetcode.com/problems/integer-break/

#include <vector>
#include "utils.h"

namespace integer_break {

// Recursive solution

class Solution1 {
public:
    // Note: If n <= 1, returns 0. If the result does not fit into int, it's
    // undefined (for 32-bit signed int, n must be <= 58).
    //
    // Time: O(n^2), Space: O(n), Recursion depth <= 4
    //
    int run(int n)
    {
        if (n < 0)
            return 0;

        std::vector<int> results(n + 1, 0);
        return getMaximumProduct(n, 0, results);
    }

private:
    int getMaximumProduct(int n, int count, std::vector<int>& results) const
    {
        assert(0 <= n && n < results.size());

        if (n == 0)
            return count > 1 ? 1 : 0;

        if (results[n] > 0)
            return results[n];

        int result = 0;

        // The recursion depth depends on the order of recursive calls here.
        // If do them for 0, 1, ..., n-1, then the call for i depends on the
        // results for 0, 1, ..., i-1, which are ready at the moment of call,
        // so the depth is 3 (including the initial call). If do for n-1, n-2,
        // ..., 0, then the call for n-1 depends on n-2, which depends on n-3,
        // and so on down to 0, giving the depth of n+1. In both cases, the
        // total number of calls is the same: n*(n+1)/2+1.
#if 1
        for (int i = n; i >= 1; i--)
            result = std::max(result, i * getMaximumProduct(n - i, count + 1, results));
#else
        for (int i = 1; i <= n; i++)
            result = std::max(result, i * getMaximumProduct(n - i, count + 1, results));
#endif
        results[n] = result;
        return result;
    }
};


// Non-recursive solution

class Solution2 {
public:
    // Note: If n <= 1, returns 0. If the result does not fit into int, it's
    // undefined (for 32-bit signed int, n must be <= 58).
    //
    // Time: O(n^2), Space: O(n)
    //
    int run(int n)
    {
        if (n <= 1)
            return 0;

        std::vector<int> results(n + 1, 0);

        // Calculate results for 1, 2, ..., n
        for (int n1 = 1; n1 <= n; n1++) {
            // Here we know results for 1, 2, ..., n1-1, and n1 can be a sum of
            // these integers only. So find the result for n1 among the sequences
            // which start with 1, 2, ..., n1-1.
            int result_n1 = 0;
            for (int n2 = 1; n2 < n1; n2++)
                result_n1 = std::max(result_n1, n2 * results[n1 - n2]);

            // If n1 is < n, it can be used by itself in the sequence which
            // sums to n. So check if n1 alone is larger than any of the
            // previously checked sequences.
            if (n1 < n)
                result_n1 = std::max(result_n1, n1);

            // Save the result
            results[n1] = result_n1;
        }

        return results.back();
    }
};


template <typename Solution>
void test()
{
    ASSERT( Solution().run(0) == 0 );
    ASSERT( Solution().run(1) == 0 );
    ASSERT( Solution().run(2) == 1 );   // 1 * 1
    ASSERT( Solution().run(3) == 2 );   // 2 * 1
    ASSERT( Solution().run(4) == 4 );   // 2 * 2
    ASSERT( Solution().run(5) == 6 );   // 3 * 2
    ASSERT( Solution().run(6) == 9 );   // 3 * 3
    ASSERT( Solution().run(7) == 12 );  // 3 * 4
    ASSERT( Solution().run(8) == 18 );  // 3 * 3 * 2
    ASSERT( Solution().run(9) == 27 );  // 3 * 3 * 3
    ASSERT( Solution().run(10) == 36 ); // 3 * 3 * 4
    ASSERT( Solution().run(11) == 54 ); // 3 * 3 * 3 * 2
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
