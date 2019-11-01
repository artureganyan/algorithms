// Problem: https://leetcode.com/problems/permutations/

#include <vector>
#include <list>
#include <algorithm>
#include <numeric>
#include "utils.h"

namespace permutations {

// Uses the solution of the next_permutation problem

class Solution1 {
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


// Recursively generates all possible permutations, supposing that
// they can be returned in any order.

class Solution2 {
public:
    // Time: O(n^2 * n!), Space: O(n * n!), recursion depth = n
    std::vector<std::vector<int>> run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return {};

        std::list<int> nums_list(nums.begin(), nums.end());
        return generatePermutations(nums_list);
    }

private:
    // Time: O(n^2 * n!), Space: O(n * n!), n = nums.size(), recursion depth = n
    //
    // Note: Time includes n^2 because each of n! permutation is created by
    // by copying the current sequence (intially {}), adding the next number
    // and repeating the same for each of n recursion steps:
    // {} -> {1} -> {1, 2} -> {1, 2, 3} -> ...
    // So we have 0 + 1 + 2 + ... + n copies, giving O(n^2) time.
    //
    // Note: This solution may frequently use dynamic memory because it
    // removes/inserts an element in the list.
    //
    std::vector<std::vector<int>> generatePermutations(std::list<int>& nums)
    {
        if (!nums.size())
            return {{}}; // Non-empty result

        std::vector<std::vector<int>> result;

        for (size_t i = 0; i < nums.size(); i++) {
            const int value = nums.front();
            nums.pop_front();

            std::vector<std::vector<int>> permutations = generatePermutations(nums);
            for (size_t r = 0; r < permutations.size(); r++) {
                permutations[r].push_back(value);
            }
            result.insert(result.end(), permutations.begin(), permutations.end());

            nums.push_back(value);
        }

        return result;
    }
};


// The same as Solution2, but optimized to avoid copying of intermediate
// results and avoid removing/inserting a number on each recursive call.

class Solution3 {
public:
    // Time: O(n * n!), Space: O(n * n!), n = nums.size(), recursion depth = n
    std::vector<std::vector<int>> run(const std::vector<int>& nums_)
    {
        if (!nums_.size())
            return {};

        std::vector<int> nums = nums_;

        // Preallocate the result array
        int result_size = 1;
        for (int i = 2; i <= nums.size(); i++)
            result_size *= i;

        std::vector<std::vector<int>> result(result_size);

        // Generate permutations
        std::vector<std::vector<int>>::iterator result_pos = result.begin();
        generatePermutations(nums, 0, result_pos);
        return result;
    }

private:
    // Generates all possible permutations for nums[nums_begin, nums.size() - 1]
    // and puts them into [result, result + count], where count is the number of
    // permutations, i.e. (nums.size() - nums_begin)!.
    //
    // Time: O(n * n!), Space: O(n * n!), n = nums.size(), recursion depth = n
    //
    void generatePermutations(std::vector<int>& nums, int nums_begin, std::vector<std::vector<int>>::iterator& result) const
    {
        if (nums_begin == nums.size()) {
            *result = std::vector<int>(nums.size(), 0);
            result++;
            return;
        }

        for (int i = nums_begin; i < nums.size(); i++) {
            auto result_prev = result;
            std::swap(nums[i], nums[nums_begin]);

            generatePermutations(nums, nums_begin + 1, result);
            for (auto r = result_prev; r < result; r++)
                (*r)[nums_begin] = nums[nums_begin];
        }
        for (int i = nums_begin + 1; i < nums.size(); i++)
            std::swap(nums[i], nums[i - 1]);
    }
};


template <typename Solution>
void test()
{
    ASSERT(compare_sets(Solution().run({}), {}));
    ASSERT(compare_sets(Solution().run({1}), {{1}}));
    ASSERT(compare_sets(Solution().run({1, 2}), {{1, 2}, {2, 1}}));
    ASSERT(compare_sets(Solution().run({1, 2, 3}), {{1, 2, 3}, {1, 3, 2}, {2, 3, 1}, {2, 1, 3}, {3, 1, 2}, {3, 2, 1}}));

    // Test performance
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    std::cout << "Run for " << nums.size() << " numbers" << std::endl;
    clock_t start = clock();

    Solution().run(nums);

    double elapsed_secs = double(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Elapsed: " << elapsed_secs << " seconds" << std::endl;
}

int main()
{
    test<Solution1>();
    test<Solution2>();
    test<Solution3>();

    return 0;
}

}
