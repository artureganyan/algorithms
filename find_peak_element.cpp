// Problem: https://leetcode.com/problems/find-peak-element/

#include <vector>
#include "utils.h"

namespace find_peak_element {

class Solution {
public:
    // Note: nums[i] must differ from nums[i+1] for each i. nums[-1] and
    // nums[n] are assumed to be -infinity. If nums is empty, returns -1.
    //
    // Time: O(log(n)), Space: O(1), n - number of integers
    //
    int run(const std::vector<int>& nums)
    {
        // Idea:
        // Since nums[i] != nums[i+1] for each i, the numbers increase and/or
        // decrease from the first to the last one. Therefore, for any interval
        // [i1, i2], if it contains a number that is greater than nums[i1] and
        // nums[i2], there must be at least one peak.
        //
        // Since we assume that nums[-1] and nums[n] are -infinity, any number
        // is greater than the borders, so the peak is guaranteed. Let's take
        // some intermediate number, e.g. the middle one, nums[h]. If it's not
        // a peak, continue searching within [-1, h] if nums[h-1] > nums[h],
        // otherwise within [h, n] (because then nums[h+1] > nums[h]). Doing
        // so, we always search within [i1, i2] where nums[i1] < nums[i1+1]
        // and nums[i2-1] > nums[i2], i.e. where there is at least one number
        // greater than both borders. Depending on the borders, this looks
        // like one of the following cases:
        //
        //                     *                        *
        //                    *                          *
        //    *       *                *        *
        //   *         *                *      *
        //
        // --|---------|--  --|---------|--  --|---------|--
        //   i1        i2     i1        i2     i1        i2
        //
        // Finally, if we will reach the interval of size 3, its middle number
        // will be the peak.

        if (!nums.size())
            return -1;

        int i1 = 0;
        int i2 = nums.size() - 1;

        while (i2 - i1 > 1) {
            const int ih = i1 + (i2 - i1) / 2;
            const int nh = nums[ih];

            if (nh < nums[ih - 1]) {
                i2 = ih;
            } else if (nh < nums[ih + 1]) {
                i1 = ih;
            } else {
                return ih;
            }
        }

        // If we have not found the peak within (0, n-1), we stop with either
        // [0, 1] or [n-2, n-1] (for n=2, this is also [0, 1]). Then, given
        // nums[-1] == nums[n] == -infinity, the peak is the largest number.
        if (nums[i1] > nums[i2])
            return i1;
        return i2;
    }
};

int main()
{
    ASSERT( Solution().run({}) == -1 );

    ASSERT( Solution().run({0}) == 0 );
    ASSERT( Solution().run({-1}) == 0 );
    ASSERT( Solution().run({1}) == 0 );

    ASSERT( Solution().run({1, 3}) == 1 );
    ASSERT( Solution().run({3, 1}) == 0 );

    ASSERT( Solution().run({1, 3, 2}) == 1 );
    ASSERT( Solution().run({1, 3, 2, 1}) == 1 );
    ASSERT( Solution().run({1, 3, 4, 2}) == 2 );

    ASSERT( is_any_of(Solution().run({1, 3, 2, 4, 1}), {1, 3}) );

    return 0;
}

}
