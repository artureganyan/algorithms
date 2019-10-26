// Problem: https://leetcode.com/problems/search-in-rotated-sorted-array/

#include <vector>
#include "utils.h"

namespace search_in_rotated_array {

class Solution
{
public:
    // Time: O(log(n)), Space: O(1)
    int run(std::vector<int>& nums, int target)
    {
        if (!nums.size())
            return -1;

        // Binary search for the maximum value (it divides the rotated array on two parts)
        //
        // Note: It's important that there are no duplicated values, i.e. any value
        // in one part of array is less/bigger than any value in the other, so that
        // we easily know where to find the required part.
        int i1 = 0;
        int i2 = nums.size() - 1;
        while (i2 - i1 > 1) {
            const int i = (i1 + i2) / 2;
            if (nums[i1] < nums[i]) {
                i1 = i;
            } else {
                i2 = i;
            }
        }
        const int i_max = nums[i2] > nums[i1] ? i2 : i1;

        // Binary search for the target, in the appropriate part of array
        if (nums[0] <= target && target <= nums[i_max]) {
            i1 = 0;
            i2 = i_max;
        } else {
            i1 = i_max + 1;
            i2 = nums.size() - 1;
        }

        while (i1 <= i2) {
            const int i = (i1 + i2) / 2;
            const int n = nums[i];
            if (target < n) {
                i2 = i - 1;
            } else if (target > n) {
                i1 = i + 1;
            } else {
                return i;
            }
        }
        return -1;
    }
};

int main()
{
    std::vector<int> v;
    ASSERT(Solution().run(v = {}, 1) == -1);
    ASSERT(Solution().run(v = {1}, 1) == 0);
    ASSERT(Solution().run(v = {1, 2, 3}, 1) == 0);
    ASSERT(Solution().run(v = {1, 2, 3}, 2) == 1);
    ASSERT(Solution().run(v = {1, 2, 3}, 3) == 2);
    ASSERT(Solution().run(v = {1, 2, 3}, 4) == -1);
    ASSERT(Solution().run(v = {3, 1, 2}, 3) == 0);
    ASSERT(Solution().run(v = {2, 3, 1}, 1) == 2);
    ASSERT(Solution().run(v = {2, 3, 1}, 2) == 0);
    ASSERT(Solution().run(v = {2, 3, 1}, 3) == 1);
    ASSERT(Solution().run(v = {2, 3, 1}, 4) == -1);
    return 0;
}

}
