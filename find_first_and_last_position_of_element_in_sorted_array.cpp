// Problem: https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/

#include <vector>
#include "utils.h"

namespace find_first_and_last_position_of_element_in_sorted_array {

class Solution {
private:
    // Returns index of the first element that is greater than value,
    // or nums.size() + 1 if there is no such element
    //
    // Time: O(log(n)), Space: O(1)
    //
    static int upper_bound(const std::vector<int>& nums, int value)
    {
        if (!nums.size())
            return 1;

        int i1 = 0;
        int i2 = nums.size() - 1;
        while (i1 < i2) {
            const int i = (i1 + i2) / 2;
            if (value < nums[i]) {
                i2 = i - 1;
            } else {
                i1 = i + 1;
            }
        }

        if (nums[i1] <= value)
            return i1 + 1;
        return i1;
    }

public:
    // Time: O(log(n)), Space: O(1)
    std::vector<int> run(const std::vector<int>& nums, int target)
    {
        if (!nums.size())
            return {-1, -1};

        int i1 = upper_bound(nums, target - 1);
        int i2 = upper_bound(nums, target) - 1;
        if (i1 > nums.size() || nums[i1] != target || nums[i2] != target)
            return {-1, -1};

        return {i1, i2};
    }
};

int main()
{
    ASSERT(( Solution().run({}, 1) == std::vector<int>{-1, -1} ));
    ASSERT(( Solution().run({5}, 5) == std::vector<int>{0, 0} ));
    ASSERT(( Solution().run({5, 5, 5}, 5) == std::vector<int>{0, 2} ));
    ASSERT(( Solution().run({4, 5, 5, 5, 6}, 5) == std::vector<int>{1, 3} ));
    ASSERT(( Solution().run({4, 5, 5, 5, 6}, 7) == std::vector<int>{-1, -1} ));
    ASSERT(( Solution().run({4, 5, 5, 5, 6}, 3) == std::vector<int>{-1, -1} ));
    ASSERT(( Solution().run({1, 5, 5, 5}, 5) == std::vector<int>{1, 3} ));
    ASSERT(( Solution().run({5, 5, 5, 7}, 5) == std::vector<int>{0, 2} ));

    return 0;
}

}
