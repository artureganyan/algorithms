// Problem: https://leetcode.com/problems/3sum-closest/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace three_sum_closest {

class Solution1 {
public:
    // Time: O(n^2), Space: O(1) if nums can be modified
    int run(std::vector<int>& nums, int target)
    {
        if (nums.size() < 3)
            return 0;

        std::sort(nums.begin(), nums.end());

        int sum_closest = 0;
        int sum_diff	= std::numeric_limits<int>::max();

        for (size_t i1 = 0; i1 < nums.size(); i1++) {
            for (size_t i2 = i1 + 1, i3 = nums.size() - 1; i2 < i3;) {
                const int sum = nums[i1] + nums[i2] + nums[i3];

                int diff = sum - target;
                if (diff == 0)
                    return target;

                if (diff < 0)
                    i2++; else
                    i3--;

                diff = std::abs(diff);
                if (diff < sum_diff) {
                    sum_diff = diff;
                    sum_closest = sum;
                }
            }
        }

        return sum_closest;
    }
};


class Solution2 {
public:
    // Time: O(n^2 * logn), Space: O(1) if nums can be modified
    int run(std::vector<int>& nums, int target)
    {
        if (nums.size() < 3)
            return 0;

        std::sort(nums.begin(), nums.end());

        int sum_closest = 0;
        int sum_diff	= std::numeric_limits<int>::max();

        for (size_t i1 = 0; i1 < nums.size(); i1++) {
            const int n1 = nums[i1];

            for (size_t i2 = i1 + 1; i2 < nums.size(); i2++) {
                const int n2 = nums[i2];
                const int n3 = target - (n1 + n2);

                int i3_closest = std::lower_bound(nums.begin(), nums.end(), n3) - nums.begin();
                if (i3_closest == nums.size())
                    i3_closest--;

                // Check number >= n3
                int i3 = i3_closest;
                while (i3 < nums.size() && (i3 == i1 || i3 == i2))
                    i3++;

                if (i3 < nums.size()) {
                    const int sum  = n1 + n2 + nums[i3];
                    const int diff = std::abs(sum - target);
                    if (diff < sum_diff) {
                        sum_diff    = diff;
                        sum_closest = sum;
                    }
                }

                // Check number < n3
                i3 = i3_closest;
                while (i3 >= 0 && (i3 == i1 || i3 == i2))
                    i3--;

                if (i3 >= 0) {
                    const int sum  = n1 + n2 + nums[i3];
                    const int diff = std::abs(sum - target);
                    if (diff < sum_diff) {
                        sum_diff    = diff;
                        sum_closest = sum;
                    }
                }

                if (sum_closest == target)
                    return sum_closest;
            }
        }

        return sum_closest;
    }
};


template <typename Solution>
void test()
{
    std::vector<int> v;
    ASSERT(Solution().run(v = {}, 1) == 0);
    ASSERT(Solution().run(v = {1}, 1) == 0);
    ASSERT(Solution().run(v = {1, 1}, 1) == 0);
    ASSERT(Solution().run(v = {1, 1, 1}, 1) == 3);
    ASSERT(Solution().run(v = {1, 1, 1}, 2) == 3);
    ASSERT(Solution().run(v = {1, 1, 1}, 3) == 3);
    ASSERT(Solution().run(v = {1, 1, 1}, 4) == 3);
    ASSERT(Solution().run(v = {0, 1, 1}, 1) == 2);
    ASSERT(Solution().run(v = {0, 1, 1}, 2) == 2);
    ASSERT(Solution().run(v = {0, 0, 1}, 1) == 1);
    ASSERT(Solution().run(v = {0, 0, 0}, 1) == 0);
    ASSERT(Solution().run(v = {0, 0, 0}, 0) == 0);
    ASSERT(Solution().run(v = {0, 1, 0, 1, 0}, 0) == 0);
    ASSERT(Solution().run(v = {0, 1, 0, 1, 0}, 1) == 1);
    ASSERT(Solution().run(v = {0, 1, 0, 1, 0}, 2) == 2);
    ASSERT(Solution().run(v = {0, 1, 0, 1, 0}, 3) == 2);
    ASSERT(Solution().run(v = {1, 4, 3, 2}, 1) == 6);
    ASSERT(Solution().run(v = {1, 4, 3, 2}, 5) == 6);
    ASSERT(Solution().run(v = {1, 4, 3, 2}, 6) == 6);
    ASSERT(Solution().run(v = {1, 4, 3, 2}, 8) == 8);
    ASSERT(Solution().run(v = {1, 4, 3, 2}, 9) == 9);
    ASSERT(Solution().run(v = {1, 4, 3, 2}, 10) == 9);
    ASSERT(Solution().run(v = {1, -2, 1, -1, 1}, -3) == -2);
    ASSERT(Solution().run(v = {1, -2, 1, -1, 1}, 0) == 0);
    ASSERT(Solution().run(v = {1, -2, 1, -1, 1}, 1) == 1);
    ASSERT(Solution().run(v = {1, -2, 1, -1, 1}, 3) == 3);
    ASSERT(Solution().run(v = {1, -2, 1, -1, 1}, 4) == 3);
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
