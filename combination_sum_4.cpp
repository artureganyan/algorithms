// Problem: https://leetcode.com/problems/combination-sum-iv/

#include <vector>
#include <numeric>
#include "utils.h"

namespace combination_sum_4 {

// Straightforward recursive solution

class Solution1 {
public:
    // Note: If the nums is empty, or the target <= 0, returns 0. The sums with
    // the different order of terms (like 1 + 2 and 2 + 1) are counted as
    // different combinations.
    //
    int run(const std::vector<int>& nums, int target)
    {
        if (!nums.size() || target <= 0)
            return 0;

        std::vector<int> nums_sorted = nums;
        std::sort(nums_sorted.begin(), nums_sorted.end());

        return getNumberOfSums(nums_sorted, target);
    }

private:
    // Note: The nums must be sorted in ascending order
    int getNumberOfSums(const std::vector<int>& nums, int target) const
    {
        if (target == 0)
            return 1;

        int result = 0;

        for (int i = 0; i < nums.size(); i++) {
            const int n = nums[i];
            if (n > target)
                break;

            result += getNumberOfSums(nums, target - n);
        }

        return result;
    }
};


// Recursive solution optimized

class Solution2 {
public:
    // Note: If the nums is empty, or the target <= 0, returns 0. The sums with
    // the different order of terms (like 1 + 2 and 2 + 1) are counted as
    // different combinations.
    //
    int run(const std::vector<int>& nums, int target)
    {
        if (!nums.size() || target <= 0)
            return 0;

        std::vector<int> nums_sorted = nums;
        std::sort(nums_sorted.begin(), nums_sorted.end());

        std::vector<int> terms_count;
        return getNumberOfSums(nums_sorted, 0, target, terms_count);
    }

private:
    // Note: The nums must be sorted in ascending order
    int getNumberOfSums(const std::vector<int>& nums, int nums_start, int target,
        std::vector<int>& terms_count) const
    {
        int result = 0;

        // If the sum is ready, calculate the number of all possible combinations of its terms
        if (target == 0) {
            result = 1;
            int terms_total_count = std::accumulate(terms_count.begin(), terms_count.end(), 0);
            for (const int c : terms_count) {
                result *= getCombinations(terms_total_count, c);
                terms_total_count -= c;
            }
            return result;
        }

        for (int i = nums_start; i < nums.size(); i++) {
            const int n = nums[i];
            if (n > target)
                break;

            const int n_max_count = target / n;
            for (int c = 1; c <= n_max_count; c++) {
                terms_count.push_back(c);
                result += getNumberOfSums(nums, i + 1, target - n * c, terms_count);
                terms_count.pop_back();
            }
        }

        return result;
    }

    // Returns the number of combinations of m elements from n elements. If
    // n < 0, or n == 0 and m != 0, or m < 0, or n < m, returns 0. If m == 0,
    // returns 1.
    //
    int getCombinations(int n, int m) const
    {
        if (n < 0 || (n == 0 && m != 0) || m < 0 || n < m)
            return 0;

        if (m == 0)
            return 1;

        // n! / m! = (m + 1) * (m + 2) * ... * n
        unsigned long long nf_div_mf = 1;
        for (int i = m + 1; i <= n; i++)
            nf_div_mf *= i;

        // (n - m)!
        unsigned long long n_minus_m_f = 1;
        for (int i = 2; i <= (n - m); i++)
            n_minus_m_f *= i;

        // C(n, m)
        const unsigned long long C_n_m = nf_div_mf / n_minus_m_f;
        return static_cast<int>(C_n_m);
    }
};


template <typename Solution>
void test()
{
    ASSERT( Solution().run({}, 0) == 0 );
    ASSERT( Solution().run({}, 1) == 0 );

    ASSERT( Solution().run({1}, 0) == 0 );
    ASSERT( Solution().run({1}, 1) == 1 );          // 1
    ASSERT( Solution().run({1}, 2) == 1 );          // 1 1
    ASSERT( Solution().run({1}, 3) == 1 );          // 1 1 1

    ASSERT( Solution().run({1, 2}, 1) == 1 );       // 1
    ASSERT( Solution().run({1, 2}, 2) == 2 );       // 1 1, 2
    ASSERT( Solution().run({1, 2}, 3) == 3 );       // 1 1 1, 1 2, 2 1

    ASSERT( Solution().run({1, 2, 3}, 1) == 1 );    // 1
    ASSERT( Solution().run({1, 2, 3}, 2) == 2 );    // 1 1, 2
    ASSERT( Solution().run({1, 2, 3}, 3) == 4 );    // 1 1 1, 1 2, 2 1, 3
    ASSERT( Solution().run({1, 2, 3}, 4) == 7 );    // 1 1 1 1, 1 1 2, 1 2 1, 2 1 1, 1 3, 3 1, 4

    ASSERT( Solution().run({2}, 1) == 0 );
    ASSERT( Solution().run({2}, 2) == 1 );
    ASSERT( Solution().run({2}, 3) == 0 );

    ASSERT( Solution().run({3, 5}, 1) == 0 );
    ASSERT( Solution().run({3, 5}, 2) == 0 );
    ASSERT( Solution().run({3, 5}, 3) == 1 );
    ASSERT( Solution().run({3, 5}, 4) == 0 );
    ASSERT( Solution().run({3, 5}, 5) == 1 );
    ASSERT( Solution().run({3, 5}, 6) == 1 );
    ASSERT( Solution().run({3, 5}, 7) == 0 );
    ASSERT( Solution().run({3, 5}, 8) == 2 );       // 3 5, 5 3
    ASSERT( Solution().run({3, 5}, 9) == 1 );
    ASSERT( Solution().run({3, 5}, 10) == 1 );
    ASSERT( Solution().run({3, 5}, 11) == 3 );      // 3 3 5, 3 5 3, 5 3 3
    ASSERT( Solution().run({3, 5}, 15) == 2 );      // 3 3 3 3 3, 5 5 5
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
