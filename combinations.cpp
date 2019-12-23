// Problem: https://leetcode.com/problems/combinations/

#include <vector>
#include "utils.h"

namespace combinations {

// Straightforward solution, with excessive copying of partial results

class Solution1 {
public:
    // Time: O(C(n, k) * k^2), Space: O(C(n, k) * k), Recursion depth: k + 2
    // C(n, k) = n! / (k! * (n - k)!)
    //
    // Note: The time complexity contains k^2 because each of C(n, k)
    // combination is built by coping partial results of length 1, 2, ..., k,
    // giving about k * (k+1) / 2 copies.
    //
    std::vector<std::vector<int>> run(int n, int k)
    {
        if (n <= 0 || k <= 0 || k > n)
            return {};

        std::vector<int> nums(n);
        for (int i = 0; i < nums.size(); i++)
            nums[i] = i + 1;

        return generateCombinations(nums, 0, k);
    }

private:
    std::vector<std::vector<int>> generateCombinations(const std::vector<int>& nums, int nums_begin, int k) const
    {
        if (!nums.size() || (nums.size() - nums_begin) < k)
            return {};

        if (k == 0)
            return {{}}; // 1 empty combination

        std::vector<std::vector<int>> result;

        for (int i = nums_begin; i < nums.size(); i++) {
            std::vector<std::vector<int>> combinations = generateCombinations(nums, i + 1, k - 1);
            for (std::vector<int>& combination : combinations) {
                combination.push_back(nums[i]);
                result.push_back(combination);
            }
        }

        return result;
    }
};


// Optimized to avoid copying of partial results

class Solution2 {
public:
    // Time: O(C(n, k) * k), Space: O(C(n, k) * k), Recursion depth: k + 2
    // C(n, k) = n! / (k! * (n - k)!)
    std::vector<std::vector<int>> run(int n, int k)
    {
        if (n <= 0 || k <= 0 || k > n)
            return {};

        std::vector<int> nums(n);
        for (int i = 0; i < nums.size(); i++)
            nums[i] = i + 1;

        std::vector<std::vector<int>> result;
        generateCombinations(result, nums, 0, k);
        return result;
    }

private:
    void generateCombinations(std::vector<std::vector<int>>& result, const std::vector<int>& nums, int nums_begin, int k) const
    {
        if (!nums.size() || (nums.size() - nums_begin) < k)
            return;

        if (k == 0) {
            result.push_back({});
            return;
        }

        for (int i = nums_begin; i < nums.size(); i++) {
            const int prev_size = result.size();
            generateCombinations(result, nums, i + 1, k - 1);
            for (int r = prev_size; r < result.size(); r++)
                result[r].push_back(nums[i]);
        }
    }
};


template <typename Solution>
void test()
{
    ASSERT( compare_sets_of_sets(Solution().run(-1, -1), {}) );
    ASSERT( compare_sets_of_sets(Solution().run(-1,  0), {}) );
    ASSERT( compare_sets_of_sets(Solution().run(-1,  1), {}) );
    ASSERT( compare_sets_of_sets(Solution().run( 0, -1), {}) );
    ASSERT( compare_sets_of_sets(Solution().run( 0,  0), {}) );
    ASSERT( compare_sets_of_sets(Solution().run( 0,  1), {}) );
    ASSERT( compare_sets_of_sets(Solution().run( 1, -1), {}) );
    ASSERT( compare_sets_of_sets(Solution().run( 1,  0), {}) );

    ASSERT( compare_sets_of_sets(Solution().run(1, 1), {{1}}) );
    ASSERT( compare_sets_of_sets(Solution().run(1, 2), {}) );

    ASSERT( compare_sets_of_sets(Solution().run(2, 1), {{1}, {2}}) );
    ASSERT( compare_sets_of_sets(Solution().run(2, 2), {{1, 2}}) );
    ASSERT( compare_sets_of_sets(Solution().run(2, 3), {}) );

    ASSERT( compare_sets_of_sets(Solution().run(3, 1), {{1}, {2}, {3}}) );
    ASSERT( compare_sets_of_sets(Solution().run(3, 2), {{1, 2}, {1, 3}, {2, 3}}) );
    ASSERT( compare_sets_of_sets(Solution().run(3, 3), {{1, 2, 3}}) );
    ASSERT( compare_sets_of_sets(Solution().run(3, 4), {}) );

    ASSERT( compare_sets_of_sets(Solution().run(4, 1), {{1}, {2}, {3}, {4}}) );
    ASSERT( compare_sets_of_sets(Solution().run(4, 2), {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}) );
    ASSERT( compare_sets_of_sets(Solution().run(4, 3), {{1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}}) );
    ASSERT( compare_sets_of_sets(Solution().run(4, 4), {{1, 2, 3, 4}}) );
    ASSERT( compare_sets_of_sets(Solution().run(4, 5), {}) );

    // Test performance
    const int n = 15;
    std::cout << "Test performance for n=1.." << n << ", k=1.." << n << std::endl;
    clock_t start = clock();

    for (int i = 1; i <= n; i++) {
        for (int k = 1; k <= n; k++)
            Solution().run(n, k);
    }

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
