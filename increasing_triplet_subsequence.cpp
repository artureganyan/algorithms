// Problem: https://leetcode.com/problems/increasing-triplet-subsequence/

#include <vector>
#include "utils.h"

namespace increasing_triplet_subsequence {

class Solution {
public:
    // Time: O(n), Space: O(1), n - number of elements
    bool run(const std::vector<int>& nums)
    {
        // Idea:
        // We need to check if there are 3 elements such that:
        // nums[i1] < nums[i2] < nums[i3], i1 < i2 < i3
        //
        // Let's say we iterate through the elements. First, we need to find
        // some nums[i1] < nums[i2]. The first such elements are always adjacent,
        // i.e. i2 = i1+1 - otherwise, if nums[i1] >= nums[i1+1], then any
        // nums[i2] which is > nums[i1] is also > nums[i1+1]. If we find these
        // n1=nums[i1] and n2=nums[i2], then, for each next element n=nums[i]
        // (i > i2) there are the following cases:
        // 1. n  > n2 => the answer is "yes"
        // 2. n <= n2 and n > n1 => any next element that is > n2 is also > n,
        //    but vice versa may be not true, so replace n2 with n
        // 3. n <= n1 => if nums[i-1] < nums[i], then n1 and n2 must be replaced
        //    with nums[i-1] and nums[i], for the same reason as in 2.

        if (!nums.size())
            return false;

        int n1 = std::numeric_limits<int>::max();
        int n2 = std::numeric_limits<int>::max();

        for (int i = 1; i < nums.size(); i++) {
            const int n = nums[i];

            if (n > n2) {
                return true;
            } else if (n > n1) {
                n2 = n;
            } else if (n > nums[i - 1]) {
                n1 = nums[i - 1];
                n2 = n;
            }
        }
        return false;
    }
};

int main()
{
    ASSERT( Solution().run({}) == false );

    ASSERT( Solution().run({0}) == false );
    ASSERT( Solution().run({1}) == false );
    ASSERT( Solution().run({-1}) == false );

    ASSERT( Solution().run({1, 1}) == false );

    ASSERT( Solution().run({1, 1, 1}) == false );
    ASSERT( Solution().run({1, 2, 1}) == false );
    ASSERT( Solution().run({1, 2, 2}) == false );
    ASSERT( Solution().run({1, 2, 3}) == true );

    ASSERT( Solution().run({1, 2, 3, 1}) == true );
    ASSERT( Solution().run({1, 2, 1, 3}) == true );
    ASSERT( Solution().run({1, 1, 2, 3}) == true );
    ASSERT( Solution().run({2, 1, 2, 3}) == true );
    ASSERT( Solution().run({1, 2, 2, 3}) == true );
    ASSERT( Solution().run({3, 1, 2, 3}) == true );
    ASSERT( Solution().run({1, 2, 3, 3}) == true );

    ASSERT( Solution().run({10, 7, 8, 3, -1, 0, 3}) == true );
    ASSERT( Solution().run({10, 7, 8, 3, 0, -1, 3}) == false );
    ASSERT( Solution().run({10, 7, 8, 3, 0, -1, 9}) == true );

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    ASSERT( Solution().run({MAX, MAX, MAX}) == false );
    ASSERT( Solution().run({MAX-2, MAX-3, MAX}) == false );
    ASSERT( Solution().run({MAX-2, MAX-1, MAX}) == true );

    ASSERT( Solution().run({MIN, MIN, MIN}) == false );
    ASSERT( Solution().run({MIN, MIN+2, MIN+1}) == false );
    ASSERT( Solution().run({MIN, MIN+1, MIN+2}) == true );

    ASSERT( Solution().run({MAX, MIN, MAX-1, MIN+1, MIN+2, MIN, MAX-2}) == true );

    return 0;
}

}
