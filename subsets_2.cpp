// Problem: https://leetcode.com/problems/subsets-ii/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace subsets_2 {

class Solution {
public:
    // Time: O(2^n * n), Space: O(2^n * n), Max recursion depth: n + 2, n - number of elements
    //
    // Note: If there are no duplicated elements, there are C(n, 1) subsets of
    // length 1, C(n, 2) of length 2, etc, where C(n, k) = n! / (k! * (n-k)!).
    // So the result will occupy sum(i * C(n, i), i = 1..n) = 2^(n-1) * n
    // memory. Each subset of length i is built by i copyings (if not count
    // the initial empty vector {}). So the total number of copyings is also
    // 2^(n-1) * n. If there are duplicates, the number of subsets is lesser
    // and the same with the required time, space and recursion depth.
    //
    std::vector<std::vector<int>> run(std::vector<int>& nums)
    {
        std::sort(nums.begin(), nums.end());

        std::vector<std::vector<int>> result;
        generateSubsets(result, nums, 0);
        return result;
    }

private:
    // Note: The nums must be sorted
    void generateSubsets(std::vector<std::vector<int>>& result, const std::vector<int>& nums, int nums_begin) const
    {
        result.push_back({});

        for (int i1 = nums_begin; i1 < nums.size();) {
            // Find the interval of duplicates [i1, i2]
            int i2 = i1;
            for (; i2 < nums.size() - 1; i2++) {
                if (nums[i2] != nums[i2 + 1])
                    break;
            }

            // Append each unique subset of [i1, i2] to each unique subset of [i2 + 1, nums.size() - 1]
            for (; i1 <= i2; i1++) {
                const int prev_size = result.size();
                generateSubsets(result, nums, i2 + 1);
                for (int r = prev_size; r < result.size(); r++) {
                    result[r].insert(result[r].end(), nums.begin() + i1, nums.begin() + i2 + 1);
                }
            }

            // Now i1 == i2 + 1 and we generated all subsets containing nums[i1],
            // so do the same for the next unique value nums[i2 + 1]
        }
    }
};


int main()
{
    std::vector<int> nums;

    ASSERT( compare_sets(Solution().run(nums = {}), {{}}) );
    ASSERT( compare_sets(Solution().run(nums = {1}), {{}, {1}}) );
    ASSERT( compare_sets(Solution().run(nums = {1, 2}), {{}, {1}, {2}, {1, 2}}) );
    ASSERT( compare_sets(Solution().run(nums = {1, 2, 3}), {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}) );

    ASSERT( compare_sets(Solution().run(nums = {1, 1}), {{}, {1}, {1, 1}}) );
    ASSERT( compare_sets(Solution().run(nums = {1, 1, 1}), {{}, {1}, {1, 1}, {1, 1, 1}}) );
    ASSERT( compare_sets(Solution().run(nums = {1, 1, 2}), {{}, {1}, {2}, {1, 1}, {1, 2}, {1, 1, 2}}) );
    ASSERT( compare_sets(Solution().run(nums = {1, 1, 2, 2}), {{}, {1}, {2}, {1, 1}, {2, 2}, {1, 2}, {1, 1, 2}, {1, 2, 2}, {1, 1, 2, 2}}) );

    return 0;
}

}
