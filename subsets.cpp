// Problem: https://leetcode.com/problems/subsets/

#include <vector>
#include "utils.h"

namespace subsets {

// Straightforward solution, with excessive copying of partial results

class Solution1 {
public:
    // Time: O(2^n * n^2), Space: O(2^n * n), Recursion depth: n + 2, n - number of elements
    //
    // Note: There are C(n, 1) subsets of length 1, C(n, 2) of length 2, etc,
    // where C(n, k) = n! / (k! * (n-k)!). So the result will occupy
    // sum(i * C(n, i), i = 1..n) = 2^(n-1) * n memory. Each subset of length
    // i is built by copying the partial results of length 1, 2, ..., i (if not
    // count the initial empty vector {}). So there are 1 + 2 + ... + i =
    // i * (i+1) / 2 copyings per subset, and the total number of copyings is
    // sum(i * (i+1) / 2 * C(n, i), i=1..n) = 1/2 * (2^(n-2) * (n+n^2) + 2^(n-1) * n).
    // This is approximated by 2^(n-3) * n^2 < 2^n * n^2.
    //
    std::vector<std::vector<int>> run(const std::vector<int>& nums)
    {
        return generateSubsets(nums, 0);
    }

private:
    std::vector<std::vector<int>> generateSubsets(const std::vector<int>& nums, int nums_begin) const
    {
        std::vector<std::vector<int>> result = {{}};

        for (int i = nums_begin; i < nums.size(); i++) {
            std::vector<std::vector<int>> subsets = generateSubsets(nums, i + 1);
            for (std::vector<int>& subset : subsets) {
                subset.push_back(nums[i]);
                result.push_back(subset);
            }
        }

        return result;
    }
};


// Optimized to avoid copying of partial results

class Solution2 {
public:
    // Time: O(2^n * n), Space: O(2^n * n), Recursion depth: n + 2, n - number of elements
    //
    // Note: There are C(n, 1) subsets of length 1, C(n, 2) of length 2, etc,
    // where C(n, k) = n! / (k! * (n-k)!). So the result will occupy
    // sum(i * C(n, i), i = 1..n) = 2^(n-1) * n memory. Each subset of length i
    // is built by i copyings (if not count the initial empty vector {}). So
    // the total number of copyings is also 2^(n-1) * n.
    //
    std::vector<std::vector<int>> run(const std::vector<int>& nums)
    {
        std::vector<std::vector<int>> result;
        generateSubsets(result, nums, 0);
        return result;
    }

private:
    void generateSubsets(std::vector<std::vector<int>>& result, const std::vector<int>& nums, int nums_begin) const
    {
        result.push_back({});

        for (int i = nums_begin; i < nums.size(); i++) {
            const int prev_size = result.size();
            generateSubsets(result, nums, i + 1);
            for (int r = prev_size; r < result.size(); r++) {
                result[r].push_back(nums[i]);
            }
        }
    }
};


template <typename Solution>
void test()
{
    ASSERT( compare_sets_of_sets(Solution().run({}), {{}}) );

    ASSERT( compare_sets_of_sets(Solution().run({-1}), {{}, {-1}}) );
    ASSERT( compare_sets_of_sets(Solution().run({ 0}), {{}, { 0}}) );
    ASSERT( compare_sets_of_sets(Solution().run({ 1}), {{}, { 1}}) );

    ASSERT( compare_sets_of_sets(Solution().run({1, 2}), {{}, {1}, {2}, {1, 2}}) );

    ASSERT( compare_sets_of_sets(Solution().run({1, 2, 3}), {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}) );

    // Test performance
    const int size = 20;
    std::vector<int> nums(size);
    for (int i = 0; i < size; i++)
        nums[i] = i + 1;

    std::cout << "Run for " << size << " numbers" << std::endl;
    clock_t start = clock();

    Solution().run(nums);

    double elapsed_secs = double(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Elapsed: " << elapsed_secs << " seconds" << std::endl;
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
