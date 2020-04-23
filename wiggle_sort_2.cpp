// Problem: https://leetcode.com/problems/wiggle-sort-ii/

#include <vector>
#include "utils.h"

namespace wiggle_sort_2 {

class Solution {
public:
    // Time: O(n * log(n)), Space: O(1)
    void run(std::vector<int>& nums)
    {
        std::sort(nums.begin(), nums.end());

        // Insert the second (largest) half into the first (lowest) one, such
        // that each element of the largest half becomes a peak:
        //
        //      nums[h+1]  nums[h+2]             nums[n-1]      <- second half
        //         |         |                       |
        // nums[0]   nums[1]   nums[2] ... nums[h-1]   nums[h]  <- first half
        //
        for (int i1 = 1, i2 = (nums.size() / 2) + (nums.size() % 2); i1 < i2 && i2 < nums.size();) {
            std::swap(nums[i1++], nums[i2]);
            std::swap(nums[i1++], nums[i2++]);
        }
    }
};


void test(const std::vector<int>& nums)
{
    std::vector<int> result = nums;

    Solution().run(result);

    for (int i = 1; i < result.size(); i++) {
        if ((result[i - 1] < result[i]) != (i % 2))
            ASSERT_EX( false, to_string(nums) );
    }
    ASSERT_EX( true, to_string(nums) );
}

int main()
{
    test({});

    test({-1});
    test({0});
    test({1});

    test({0, 1});
    test({1, 0});

    test({0, 1, 1});
    test({1, 0, 1});
    test({1, 1, 0});

    test({0, 0, 1, 1});
    test({0, 1, 0, 1});
    test({0, 1, 1, 0});
    test({1, 0, 1, 0});
    test({1, 1, 0, 0});
    test({1, 0, 0, 1});

    test({2, 2, 1, 3});
    test({2, 1, 2, 3});
    test({1, 3, 2, 2});

    test({1, 1, 1, 3, 3, 2});
    test({3, 1, 2, 1, 3, 1});
    test({1, 1, 1, 3, 3, 3});
    test({3, 3, 3, 1, 1, 1});
    test({3, 1, 3, 1, 3, 1});

    test({2, 3, 3, 3, 1, 1, 1});
    test({2, 3, 3, 1, 1, 1, 1});
    test({3, 3, 3, 1, 1, 1, 1});

    test({1, 101, 3, 3, -1, 1, -3, 4, 0, 4, 90});

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    test({MAX});
    test({MIN});
    test({MAX, MIN});
    test({MAX, MIN, MIN});

    return 0;
}

}
