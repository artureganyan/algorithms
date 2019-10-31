// Problem: https://leetcode.com/problems/permutations/

#include <vector>
#include "utils.h"

namespace permutations {

// Uses the solution of the next_permutation problem

class Solution {
public:
    // Time: O(n * n!), Space: O(n * n!), n = nums.size()
    //
    // Note: nums must contain unique values, otherwise the result will
    // contain duplicates.
    //
    std::vector<std::vector<int>> run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return {};

        int result_size = 1;
        for (int i = 2; i <= nums.size(); i++)
            result_size *= i;

        std::vector<std::vector<int>> result(result_size);
        result[0] = nums;

        for (int i = 1; i < result_size; i++) {
            result[i] = result[i - 1];
            nextPermutation(result[i]);
        }

        return result;
    }

private:
    // Time: O(n), Space: O(1)
    void nextPermutation(std::vector<int>& nums) const
    {
        if (nums.size() <= 1)
            return;

        // Find the tail sorted in decreasing order in nums:
        // nums[0] ... nums[i] < nums[i+1] >= nums[i+2] >= ... >= nums[n-1]
        int i = nums.size() - 2;
        while (i >= 0 && nums[i] >= nums[i + 1])
            i--;

        // The tail nums[i+1, n] is lexicographically the largest possible
        // sequence of elements {nums[i+1], ..., nums[n-1]}. So, the next
        // lexicographical nums must differ from the original one at position i.
        // Therefore, nums[i] must be replaced with an element from the tail,
        // and this element must be the lowest one but still greater nums[i]:
        // nums[0] ... nums[i] < nums[i+1] >= ... >= nums[x] >= nums[x+1] >= ... >= nums[n-1],
        // nums[i] < nums[x] and nums[i] >= nums[x+1]
        if (i >= 0) {
            int x = i + 2;
            while (x < nums.size() && nums[i] < nums[x])
                x++;

            std::swap(nums[x - 1], nums[i]);
        }

        // The new tail is also sorted in descending order, i.e. the largest
        // lexicographical sequence among such tails. So reverse it to
        // get the lowest one, so that new nums become the next permutation.
        for (int i1 = i + 1, i2 = nums.size() - 1; i1 < i2; i1++, i2--)
            std::swap(nums[i1], nums[i2]);
    }
};

int main()
{
    ASSERT(compare_sets(Solution().run({}), {}));
    ASSERT(compare_sets(Solution().run({1}), {{1}}));
    ASSERT(compare_sets(Solution().run({1, 2}), {{1, 2}, {2, 1}}));
    ASSERT(compare_sets(Solution().run({2, 1}), {{1, 2}, {2, 1}}));
    ASSERT(compare_sets(Solution().run({1, 2, 3}), {{1, 2, 3}, {1, 3, 2}, {2, 3, 1}, {2, 1, 3}, {3, 1, 2}, {3, 2, 1}}));
    ASSERT(compare_sets(Solution().run({3, 1, 2}), {{1, 2, 3}, {1, 3, 2}, {2, 3, 1}, {2, 1, 3}, {3, 1, 2}, {3, 2, 1}}));

    return 0;
}

}
