// Problem: https://leetcode.com/problems/partition-equal-subset-sum/

#include "utils.h"
#include <vector>

namespace partition_equal_subset_sum {

class Solution {
public:
    // Note: If the nums size <= 1, returns false. The total sum of the nums
    // must fit into int.
    //
    // Note: This implementation does not constrain the nums size and values,
    // but the original problem considers no more than 200 values, each within
    // [1, 100].
    //
    bool run(const std::vector<int>& nums)
    {
        // Idea:
        // If we can divide the numbers into two subsets which have the same
        // sum of elements, this sum is always total_sum / 2 (where total_sum
        // is the sum of all numbers). Therefore, 1) the solution can exist for
        // the even total_sum only, 2) we should find out if total_sum / 2 can
        // be composed from the given numbers.

        if (nums.size() <= 1)
            return false;

        int total_sum = 0;
        for (const int n : nums)
            total_sum += n;

        if (total_sum % 2)
            return false;

        const int target_sum = total_sum / 2;

        auto nums_sorted = nums;
        std::sort(nums_sorted.begin(), nums_sorted.end());

        return canComposeSum(nums_sorted, target_sum, 0);
    }

private:
    // The nums must be sorted in ascending order
    bool canComposeSum(const std::vector<int>& nums, int sum, int start) const
    {
        if (start >= nums.size())
            return false;

        for (int i = start; i < nums.size(); i++) {
            const int n = nums[i];
            // This element can be a part of the sum
            if (n < sum) {
                // Try to find the remaining sum in [i+1, end)
                if (canComposeSum(nums, sum - n, i + 1)) {
                    return true;
                // This element + any combination of [i+1, end) do not compose the sum,
                // therefore it can't be used, so skip all such elements
                } else while (i < nums.size() - 1 && nums[i + 1] == n) {
                    i++;
                }
            // The sum is found
            } else if (n == sum) {
                return true;
            // This and next elements are greater, no reason to continue
            } else {
                return false;
            }
        }

        return false;
    }
};

int main()
{
    const auto test = [](const std::vector<int>& nums, bool expected)
    {
        ASSERT_EX( Solution().run(nums) == expected,
            to_string(nums) + " -> " + to_string(expected) );
    };

    test({}, false);
    test({1}, false);

    test({1, 1}, true);
    test({1, 2}, false);
    test({2, 1}, false);

    test({1, 1, 1}, false);
    test({2, 1, 1}, true);
    test({1, 2, 1}, true);
    test({1, 1, 2}, true);
    test({2, 2, 1}, false);
    test({2, 1, 2}, false);
    test({1, 2, 2}, false);
    test({2, 2, 2}, false);

    test({1, 1, 2}, true);
    test({1, 2, 3}, true);

    const int MAX_COUNT = 200;
    const int MIN_VALUE = 1;
    const int MAX_VALUE = 100;

    test({2, 1, MAX_VALUE, 1, MAX_VALUE-10, 7, 1}, true);
    test({2, 1, MAX_VALUE, 1, MAX_VALUE-9, 7, 1}, false);

    {
        std::vector<int> nums = {};
        for (int i = 0; i < MAX_COUNT; i++)
            nums.push_back(MIN_VALUE);
        test(nums, true);
    }

    {
        std::vector<int> nums = {};
        for (int i = 0; i < MAX_COUNT; i++)
            nums.push_back(MAX_VALUE);
        test(nums, true);
    }

    // {1, 2, 2, ..., 2}, which can be very slow if all possible combinations
    // are brute-forced
    {
        std::vector<int> nums = {1};
        for (int i = 0; i < MAX_COUNT - 1; i++)
            nums.push_back(2);
        test(nums, false);
    }

    return 0;
}

}
