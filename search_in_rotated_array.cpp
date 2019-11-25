// Problem: https://leetcode.com/problems/search-in-rotated-sorted-array/

#include <vector>
#include "utils.h"

namespace search_in_rotated_array {

class Solution {
public:
    // Searches for the target in the nums. The nums must be sorted, may be
    // rotated after the sorting, and must not contain duplicated values.
    //
    // Time: O(log(n)), Space: O(1)
    //
    int run(const std::vector<int>& nums, int target)
    {
        if (!nums.size())
            return -1;

        // Binary search for the maximum value. Since the rotated array was
        // initially sorted, and there are no duplicates, all elements at the
        // left of the maximum value are > all elements at the right.
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

        // Binary search for the target, in the appropriate part of the array
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


void test(const std::vector<int>& nums,
          const std::vector<std::pair<int /*value*/, int /*position*/>>& existing,
          const std::vector<int>& missing)
{
    for (const auto& target : existing) {
        ASSERT_EX( Solution().run(nums, target.first) == target.second,
                   to_string(nums) + ", " + to_string(target.first) + " -> " + to_string(target.second));
    }
    for (int target : missing) {
        ASSERT_EX( Solution().run(nums, target) == -1,
                   to_string(nums) + ", " + to_string(target) + " -> -1");
    }
}

int main()
{
    test({}, {}, {0});

    test({1}, {{1, 0}}, {0, 2});

    test({1, 3}, {{1, 0}, {3, 1}}, {0, 2, 4});
    test({3, 1}, {{1, 1}, {3, 0}}, {0, 2, 4});

    test({1, 3, 5}, {{1, 0}, {3, 1}, {5, 2}}, {0, 4, 6});
    test({5, 1, 3}, {{1, 1}, {3, 2}, {5, 0}}, {0, 4, 6});
    test({3, 5, 1}, {{1, 2}, {3, 0}, {5, 1}}, {0, 4, 6});

    return 0;
}

}
