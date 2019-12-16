// Problem: https://leetcode.com/problems/4sum/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace four_sum {

class Solution {
public:
    // Time: O(n^3), Space: O(1)
    std::vector<std::vector<int>> run(std::vector<int>& nums, int target)
    {
        std::sort(nums.begin(), nums.end());

        std::vector<std::vector<int>> result;

        for (size_t i1 = 0; i1 < nums.size();) {
            for (size_t i2 = i1 + 1; i2 < nums.size();) {
                for (size_t i3 = i2 + 1, i4 = nums.size() - 1; i3 < i4;) {
                    int sum = nums[i1] + nums[i2] + nums[i3] + nums[i4];
                    if (sum == target) {
                        result.push_back({nums[i1], nums[i2], nums[i3], nums[i4]});
                        do i3++; while (i3 < i4 && nums[i3] == nums[i3 - 1]);
                        do i4--; while (i3 < i4 && nums[i4] == nums[i4 + 1]);
                    } else if (sum < target) {
                        i3++;
                    } else {
                        i4--;
                    }
                }
                do i2++; while (i2 < nums.size() && nums[i2] == nums[i2 - 1]);
            }
            do i1++; while (i1 < nums.size() && nums[i1] == nums[i1 - 1]);
        }

        return result;
    }
};

int main()
{
    std::vector<int> v;
    ASSERT(compare_sets(Solution().run(v = {}, 0), {}));
    ASSERT(compare_sets(Solution().run(v = {1}, 1), {}));
    ASSERT(compare_sets(Solution().run(v = {1, 0}, 1), {}));
    ASSERT(compare_sets(Solution().run(v = {1, 0, 0}, 1), {}));
    ASSERT(compare_sets(Solution().run(v = {1, 0, 0, 0}, 1), {{1, 0, 0, 0}}));
    ASSERT(compare_sets(Solution().run(v = {0, 1, 0, 0}, 1), {{1, 0, 0, 0}}));
    ASSERT(compare_sets(Solution().run(v = {0, 0, 1, 0}, 1), {{1, 0, 0, 0}}));
    ASSERT(compare_sets(Solution().run(v = {0, 0, 0, 1}, 1), {{1, 0, 0, 0}}));
    ASSERT(compare_sets(Solution().run(v = {1, 0, 0, 1}, 1), {}));
    ASSERT(compare_sets(Solution().run(v = {1, -1, 0, 1}, 1), {{1, -1, 0, 1}}));
    ASSERT(compare_sets(Solution().run(v = {1, 1, 1, 1}, 4), {{1, 1, 1, 1}}));
    ASSERT(compare_sets(Solution().run(v = {1, 1, 1, 1, 1, 1, 1}, 4), {{1, 1, 1, 1}}));
    ASSERT(compare_sets(Solution().run(v = {1, 2, 3, 4}, 10), {{1, 2, 3, 4}}));
    ASSERT(compare_sets(Solution().run(v = {1, 2, 3, 4, 1}, 10), {{1, 2, 3, 4}}));
    ASSERT(compare_sets(Solution().run(v = {1, 2, 3, 4, 1, 2}, 10), {{1, 2, 3, 4}}));
    ASSERT(compare_sets(Solution().run(v = {1, 2, 3, 4, 1, 2, 3}, 10), {{1, 2, 3, 4}, {2, 2, 3, 3}}));

    return 0;
}

}
