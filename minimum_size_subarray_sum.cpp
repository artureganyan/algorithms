// Problem: https://leetcode.com/problems/minimum-size-subarray-sum/

#include <vector>
#include <map>
#include <limits>
#include "utils.h"

namespace minimum_size_subarray_sum {

// Note: Both solutions allow zero values, but the original problem supposes
// positive integers only.


class Solution1 {
public:
    // Note: The target sum and the numbers must be >= 0.
    //
    // Time: O(n), Space: O(1)
    //
    int run(int s, const std::vector<int>& nums)
    {
        if (!nums.size())
            return 0;

        int          result    = 0;
        int          sum_start = 0;
        unsigned int sum       = 0; // Unsigned to avoid overflows

        for (int i = 0; i < nums.size(); i++) {
            sum += nums[i];
            if (sum < s)
                continue;

            while (sum >= s && sum_start <= i) {
                sum -= nums[sum_start];
                sum_start++;
            }

            const int sum_length = i - sum_start + 2;
            if (result > sum_length || result == 0)
                result = sum_length;
        }

        return result;
    }
};


// This solution is added because the original problem suggests finding the
// algorithm with O(n * log(n)) time.

class Solution2 {
public:
    // Note: The target sum and the numbers must be >= 0. Possible overflow is
    // not checked, so the sum of all numbers must fit into long long integer.
    //
    // Time: O(n * log(n)), Space: O(n)
    //
    int run(int s, const std::vector<int>& nums)
    {
        if (!nums.size())
            return 0;

        typedef std::map<long long /*sum*/, int /*index*/> PartialSums;

        int         result = 0;
        long long   sum    = 0;
        PartialSums partial_sums;

        for (int i = 0; i < nums.size(); i++) {
            const int n = nums[i];
            sum += n;
            if (n != 0)
                partial_sums[sum] = i;

            if (sum < s)
                continue;

            const auto sum_start  = partial_sums.upper_bound(sum - s);
            int        sum_length = 0;
            if (sum_start != partial_sums.end())
                sum_length = i - sum_start->second + 1; else
                sum_length = 1;

            if (result > sum_length || result == 0)
                result = sum_length;

            partial_sums.erase(partial_sums.begin(), sum_start);
        }

        return result;
    }
};


template <typename Solution>
void test(bool test_zeros = true)
{
    ASSERT( Solution().run(1, {}) == 0 );

    ASSERT( Solution().run(1, {1}) == 1 );
    ASSERT( Solution().run(1, {2}) == 1 );
    ASSERT( Solution().run(1, {1, 1}) == 1 );
    ASSERT( Solution().run(1, {1, 2}) == 1 );

    ASSERT( Solution().run(2, {1, 1}) == 2 );
    ASSERT( Solution().run(2, {1, 1, 2}) == 1 );
    ASSERT( Solution().run(2, {1, 2, 1}) == 1 );
    ASSERT( Solution().run(2, {2, 1, 1}) == 1 );

    ASSERT( Solution().run(3, {1, 1, 1}) == 3 );
    ASSERT( Solution().run(3, {1, 1, 1, 2}) == 2 );
    ASSERT( Solution().run(3, {1, 1, 1, 2, 3}) == 1 );

    ASSERT( Solution().run(5, {1, 1, 1, 1, 1, 2, 3, 1, 1}) == 2 );
    ASSERT( Solution().run(5, {2, 3, 10, 4, 1}) == 1 );

    const int INT_MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run(INT_MAX, {INT_MAX}) == 1 );
    ASSERT( Solution().run(INT_MAX, {INT_MAX - 1, INT_MAX - 1}) == 2 );

    if (test_zeros) {
        ASSERT( Solution().run(0, {}) == 0 );
        ASSERT( Solution().run(0, {0}) == 1 );
        ASSERT( Solution().run(0, {1}) == 1 );
        ASSERT( Solution().run(0, {0, 0}) == 1 );
        ASSERT( Solution().run(0, {1, 0}) == 1 );
        ASSERT( Solution().run(0, {0, 1}) == 1 );

        ASSERT( Solution().run(3, {0, 2, 2}) == 2 );
        ASSERT( Solution().run(3, {2, 2, 0}) == 2 );
        ASSERT( Solution().run(3, {2, 0, 2}) == 3 );
        ASSERT( Solution().run(3, {0, 1, 0, 2, 0}) == 3 );

        ASSERT( Solution().run(5, {0, 1, 3, 0, 0, 2, 0, 3, 0, 1, 0, 1, 0}) == 3 );
    }
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
