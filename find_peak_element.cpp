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
