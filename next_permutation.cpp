// Problem: https://leetcode.com/problems/next-permutation/

#include <vector>
#include "utils.h"

namespace next_permutation {

class Solution {
public:
    // Time: O(n), Space: O(1)
    void run(std::vector<int>& nums)
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
    std::vector<int> v;
    ASSERT(( Solution().run(v = {}), v == std::vector<int>{} ));
    ASSERT(( Solution().run(v = {1}), v == std::vector<int>{1} ));
    ASSERT(( Solution().run(v = {1, 2}), v == std::vector<int>{2, 1} ));
    ASSERT(( Solution().run(v = {1, 2, 3}), v == std::vector<int>{1, 3, 2} ));
    ASSERT(( Solution().run(v = {1, 3, 2}), v == std::vector<int>{2, 1, 3} ));
    ASSERT(( Solution().run(v = {2, 1, 3}), v == std::vector<int>{2, 3, 1} ));
    ASSERT(( Solution().run(v = {2, 3, 1}), v == std::vector<int>{3, 1, 2} ));
    ASSERT(( Solution().run(v = {3, 1, 2}), v == std::vector<int>{3, 2, 1} ));
    ASSERT(( Solution().run(v = {3, 2, 1}), v == std::vector<int>{1, 2, 3} ));
    ASSERT(( Solution().run(v = {1, 1, 1}), v == std::vector<int>{1, 1, 1} ));
    ASSERT(( Solution().run(v = {1, 1, 2}), v == std::vector<int>{1, 2, 1} ));
    ASSERT(( Solution().run(v = {1, 2, 1}), v == std::vector<int>{2, 1, 1} ));

    return 0;
}

}
