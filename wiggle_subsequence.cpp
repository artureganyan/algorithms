// Problem: https://leetcode.com/problems/wiggle-subsequence/

#include <vector>
#include "utils.h"

namespace wiggle_subsequence {

class Solution {
public:
    // Note: If the nums is empty, returns 0. Otherwise returns >= 1,
    // considering the subsequence of 1 element as valid.
    //
    // Time: O(n), Space: O(1), n - number of integers
    //
    int run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return 0;

        int length     = 1;
        int difference = 0;

        for (int i = 1; i < nums.size(); i++) {
            const int n1 = nums[i - 1];
            const int n2 = nums[i];
            const int d  = (n1 > n2 ? -1 : n1 < n2 ? 1 : 0);

            if (d && (d == difference || difference == 0)) {
                difference = -d;
                length++;
            }
        }

        return length;
    }
};

int main()
{
    ASSERT( Solution().run({}) == 0 );

    ASSERT( Solution().run({0}) == 1 );
    ASSERT( Solution().run({1}) == 1 );
    ASSERT( Solution().run({-1}) == 1 );

    ASSERT( Solution().run({0, 1}) == 2 );
    ASSERT( Solution().run({1, 0}) == 2 );
    ASSERT( Solution().run({1, 1}) == 1 );

    ASSERT( Solution().run({0, 1, 0}) == 3 );
    ASSERT( Solution().run({1, 0, 1}) == 3 );
    ASSERT( Solution().run({1, 0, 0}) == 2 );
    ASSERT( Solution().run({1, 1, 0}) == 2 );
    ASSERT( Solution().run({0, 1, 1}) == 2 );
    ASSERT( Solution().run({0, 0, 1}) == 2 );
    ASSERT( Solution().run({1, 1, 1}) == 1 );

    ASSERT( Solution().run({0, 1, 2}) == 2 );
    ASSERT( Solution().run({2, 1, 0}) == 2 );

    ASSERT( Solution().run({0, 1, 2, 1}) == 3 );
    ASSERT( Solution().run({0, 1, 1, 2}) == 2 );
    ASSERT( Solution().run({0, 2, 1, 1}) == 3 );
    ASSERT( Solution().run({1, 0, 1, 2}) == 3 );
    ASSERT( Solution().run({1, 0, 2, 1}) == 4 );
    ASSERT( Solution().run({1, 1, 0, 2}) == 3 );
    ASSERT( Solution().run({1, 1, 2, 0}) == 3 );
    ASSERT( Solution().run({2, 0, 1, 1}) == 3 );
    ASSERT( Solution().run({2, 1, 0, 1}) == 3 );
    ASSERT( Solution().run({2, 1, 1, 0}) == 2 );

    //                         *           *   *   *      *  *   *     *
    ASSERT( Solution().run({1, 1, -1, -1, -3, -2, -3, 3, 10, 9, 11, 0, 0}) == 8 );

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    ASSERT( Solution().run({MAX}) == 1 );
    ASSERT( Solution().run({MIN}) == 1 );
    ASSERT( Solution().run({MAX, MIN, MAX}) == 3 );
    ASSERT( Solution().run({MIN, MAX, MIN}) == 3 );
    ASSERT( Solution().run({MAX, MAX, MIN}) == 2 );
    ASSERT( Solution().run({MIN, MAX, MAX}) == 2 );
    ASSERT( Solution().run({MIN, MIN, MAX}) == 2 );
    ASSERT( Solution().run({MAX, MIN, MIN}) == 2 );
    ASSERT( Solution().run({MAX, MAX, MAX}) == 1 );
    ASSERT( Solution().run({MIN, MIN, MIN}) == 1 );

    return 0;
}

}
