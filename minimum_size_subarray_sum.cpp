// Problem: https://leetcode.com/problems/minimum-size-subarray-sum/

#include <vector>
#include "utils.h"

namespace minimum_size_subarray_sum {

class Solution {
public:
    // Note: The numbers must be > 0.
    //
    // Time: O(n), Space: O(1)
    //
    int run(int s, const std::vector<int>& nums)
    {
        if (!nums.size())
            return 0;

        int result    = 0;
        int sum_start = 0;
        int sum       = 0;

        for (int i = 0; i < nums.size(); i++) {
            sum += nums[i];

            while (sum >= s && sum_start <= i) {
                const int sum_length = i - sum_start + 1;
                if (result > sum_length || result == 0)
                    result = sum_length;

                sum -= nums[sum_start];
                sum_start++;
            }
        }
        return result;
    }
};

int main()
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

    return 0;
}

}
