// Desription: https://leetcode.com/problems/3sum-closest/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace three_sum_closest {

class Solution {
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

    // Time: O(n^2 * logn), Space: O(1) if nums can be modified
    int run_n2logn(std::vector<int>& nums, int target)
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

                size_t ni = std::lower_bound(nums.begin(), nums.end(), n3) - nums.begin();
                if (ni == nums.size())
                    ni--;

                // Check number >= n3
                while (ni < nums.size() && (ni == i1 || ni == i2))
                    ni++;

                if (!(ni == i1 || ni == i2)) {
                    int sum  = n1 + n2 + nums[ni];
                    int diff = std::abs(sum - target);
                    if (diff < sum_diff) {
                        sum_diff = diff;
                        sum_closest = sum;
                    }
                }

                // Check number < n3
                while (ni >= 0 && (ni == i1 || ni == i2))
                    ni--;

                if (!(ni == i1 || ni == i2)) {
                    int sum  = n1 + n2 + nums[ni];
                    int diff = std::abs(sum - target);
                    if (diff < sum_diff) {
                        sum_diff = diff;
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

int main()
{
    auto methods = {&Solution::run, &Solution::run_n2logn};
    std::vector<int> v;

    for (auto method : methods) {
        ASSERT((Solution().*method)(v = {-1, 2, 1, -4}, 1) == 2);
        ASSERT((Solution().*method)(v = {}, 1) == 0);
        ASSERT((Solution().*method)(v = {1, 1}, 1) == 0);
        ASSERT((Solution().*method)(v = {1, 1, 1}, 1) == 3);
        ASSERT((Solution().*method)(v = {1, 2, 1, 2}, 5) == 5);
        ASSERT((Solution().*method)(v = {1, 2, 1, 1}, 5) == 4);
    }

    return 0;
}

}
