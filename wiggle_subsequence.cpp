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
        // Idea:
        // We need to find the longest wiggle subsequence of the nums. The
        // sequence is wiggle if it switches between increasing/decreasing on
        // every element, i.e. n1 < n2 > n3 < ... or n1 > n2 < n3 > ... . This
        // can be checked by just comparing each n[i] with n[i-1].
        //
        // If we have checked that {..., n[i-1], n[i]} is wiggle but {...,
        // n[i-1], n[i], n[i+1]} is not, this means one of the following:
        // 1. n[i-1] < n[i] <= n[i+1], or
        // 2. n[i-1] > n[i] >= n[i+1]
        // In both cases, we can remove n[i], because any n[i+2] that continues
        // the wiggle for {n[i-1], n[i+1]}, also continues it for {n[i-1], n[i]},
        // but not vice versa: e.g. for nums = {..., 0, 2, 3, x} remove 2,
        // because x == 2 continues {..., 0, 3} only, and x < 2 continues
        // both {..., 0, 2} and {..., 0, 3}.
        //
        // Also, if there is a wiggle sequence {n[i], n[i+1], ..., n[i+s]},
        // none of its subsequences except {n[i], n[i+1], ..., n[i+s-1]} can
        // have the longer wiggle continuation on n[i+s+1, end): any
        // continuation of such a subsequence either fits the initial sequence
        // or fits it after removing n[i+s] (as shown above). This means any
        // wiggle sequence, probably without the last element, is a part of
        // the longest sequence we search.
        //
        // Therefore, we can iterate through the nums, compare nums[i] with
        // nums[i-1] and increase the wiggle subsequence's length every time we
        // have a switch between increasing/decreasing. If there is no switch,
        // nums[i-1] is considered "removed", i.e. it just does not effect the
        // result.

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
