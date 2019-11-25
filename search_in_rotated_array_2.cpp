// Problem: https://leetcode.com/problems/search-in-rotated-sorted-array-ii/

#include <vector>
#include "utils.h"

namespace search_in_rotated_array_2 {

class Solution {
public:
    // Searches for the target in the nums. The nums must be sorted and may be
    // rotated after the sorting.
    //
    // Time: O(n1 + log(n2)), Space: O(1)
    // n1 - number of elements equal at the beginning and at the end,
    // n2 - number of the rest elements + 1
    //
    // Note: E.g. for {1, 1, 2, 2, 3, 3, 0, 1, 1} there are n1 = 4 elements
    // at both ends which are the same and will be handled in linear time,
    // and n2 = 6 elements which will be searched in logarithmic time.
    //
    bool run(const std::vector<int>& nums, int target)
    {
        if (!nums.size())
            return false;

        // Skip duplicates at the beginning and at the end of the array:
        // 1 1 1 2 2 3 3 0 1 1
        // ----|         |----
        //     i1        i2
        int i1 = 0;
        int i2 = nums.size() - 1;

        const int n0 = nums[0];
        while (i1 < i2 && nums[i1 + 1] == n0) i1++;
        while (i1 < i2 && nums[i2] == n0)     i2--;

        // Binary search for the maximum value. Since the rotated array was
        // initially sorted, all elements at the left of the maximum value are
        // > all elements at the right, except 1) the possible duplicates at
        // the beginning and at the end, which were already skipped,
        // 2) duplicated maximum values, so choose the rightmost one.
        while (i2 - i1 > 1) {
            const int i = (i1 + i2) / 2;
            if (nums[i1] <= nums[i]) {
                i1 = i;
            } else {
                i2 = i;
            }
        }
        const int i_max = nums[i2] >= nums[i1] ? i2 : i1;

        // Assure that we have choosen the rightmost maximum
        assert(i_max == nums.size() - 1 || nums[i_max] > nums[i_max + 1]);

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
                return true;
            }
        }
        return false;
    }
};


void test(const std::vector<int>& nums, const std::vector<int>& existing, const std::vector<int>& missing)
{
    for (int target : existing) {
        ASSERT_EX( Solution().run(nums, target) == true,
                   to_string(nums) + ", " + to_string(target) + " -> true");
    }
    for (int target : missing) {
        ASSERT_EX( Solution().run(nums, target) == false,
                   to_string(nums) + ", " + to_string(target) + " -> false");
    }
}

int main()
{
    test({}, {}, {0});

    test({1}, {1}, {0, 2});

    test({1, 3}, {1, 3}, {0, 2, 4});
    test({3, 1}, {1, 3}, {0, 2, 4});

    test({1, 3, 5}, {1, 3, 5}, {0, 4, 6});
    test({5, 1, 3}, {1, 3, 5}, {0, 4, 6});
    test({3, 5, 1}, {1, 3, 5}, {0, 4, 6});

    test({1, 1, 3}, {1, 3}, {0, 2, 4});
    test({3, 1, 1}, {1, 3}, {0, 2, 4});
    test({1, 3, 1}, {1, 3}, {0, 2, 4});

    test({1, 1, 1, 3}, {1, 3}, {0, 2, 4});
    test({3, 1, 1, 1}, {1, 3}, {0, 2, 4});
    test({1, 3, 1, 1}, {1, 3}, {0, 2, 4});
    test({1, 1, 3, 1}, {1, 3}, {0, 2, 4});

    test({1, 3, 3, 3}, {1, 3}, {0, 2, 4});
    test({3, 1, 3, 3}, {1, 3}, {0, 2, 4});
    test({3, 3, 1, 3}, {1, 3}, {0, 2, 4});
    test({3, 3, 3, 1}, {1, 3}, {0, 2, 4});

    test({1, 1, 3, 3, 5, 5}, {1, 3, 5}, {0, 2, 4});
    test({1, 3, 3, 5, 5, 1}, {1, 3, 5}, {0, 2, 4});
    test({1, 1, 3, 3, 5, 5, 1}, {1, 3, 5}, {0, 2, 4});
    test({1, 3, 3, 5, 5, 1, 1}, {1, 3, 5}, {0, 2, 4});
    test({1, 1, 3, 3, 5, 5, 1, 1}, {1, 3, 5}, {0, 2, 4});

    // Check for handling the rightmost maximum
    test({3, 3, 5, 5, 1}, {1}, {});
    test({3, 3, 5, 5, 5, 1}, {1}, {});

    return 0;
}

}
