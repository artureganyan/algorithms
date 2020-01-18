// Problem: https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/

#include <vector>
#include "utils.h"

namespace find_minimum_in_rotated_sorted_array {

class Solution {
public:
    // Note: The nums must not contain duplicates.
    //
    // Time: O(log(n)), Space: O(1), n - number of elements
    //
    int run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return 0;

        int i1 = 0;
        int i2 = nums.size() - 1;

        while (i1 < i2) {
            const int i_half = i1 + (i2 - i1) / 2;
            if (nums[i_half] < nums[i2]) {
                i2 = i_half;
            } else {
                i1 = i_half + 1;
            }
        }

        return nums[i1];
    }
};

int main()
{
    ASSERT( Solution().run({}) == 0 );

    ASSERT( Solution().run({ 0}) ==  0 );
    ASSERT( Solution().run({ 1}) ==  1 );
    ASSERT( Solution().run({-1}) == -1 );

    ASSERT( Solution().run({1, 3}) == 1 );
    ASSERT( Solution().run({3, 1}) == 1 );

    ASSERT( Solution().run({1, 3, 5}) == 1 );
    ASSERT( Solution().run({5, 1, 3}) == 1 );
    ASSERT( Solution().run({3, 5, 1}) == 1 );

    ASSERT( Solution().run({-3, -1, 2, 5}) == -3 );
    ASSERT( Solution().run({5, -3, -1, 2}) == -3 );
    ASSERT( Solution().run({2, 5, -3, -1}) == -3 );
    ASSERT( Solution().run({-1, 2, 5, -3}) == -3 );

    return 0;
}

}
