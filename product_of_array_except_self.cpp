// Problem: https://leetcode.com/problems/product-of-array-except-self/

#include <vector>
#include "utils.h"

namespace product_of_array_except_self {

class Solution {
public:
    // Note: For each 0 <= i <= n-1, product of all elements except i-th must
    // fit into int. If the nums is empty, returns empty array. If it contains
    // one element, returns {1}.
    //
    // Time: O(n), Space: O(n) for result, n - number of elements
    //
    std::vector<int> run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return {};

        std::vector<int> result(nums.size(), 1);

        for (int i = 1, p = 1; i < nums.size(); i++) {
            p *= nums[i - 1];
            result[i] = p;
        }
        for (int i = nums.size() - 2, p = 1; i >= 0; i--) {
            p *= nums[i + 1];
            result[i] *= p;
        }

        return result;
    }
};

int main()
{
    typedef std::vector<int> Result;

    ASSERT(( Solution().run({}) == Result{} ));

    ASSERT(( Solution().run({0})  == Result{1} ));
    ASSERT(( Solution().run({1})  == Result{1} ));
    ASSERT(( Solution().run({-1}) == Result{1} ));

    ASSERT(( Solution().run({2, -3}) == Result{-3, 2} ));
    ASSERT(( Solution().run({2, -3, 4}) == Result{-12, 8, -6} ));
    ASSERT(( Solution().run({2, 0, 3}) == Result{0, 6, 0} ));
    ASSERT(( Solution().run({1, 2, 3, 4, 5}) == Result{120, 60, 40, 30, 24} ));

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();
    ASSERT(( Solution().run({1, MAX, 1}) == Result{MAX, 1, MAX} ));
    ASSERT(( Solution().run({1, MIN, 1}) == Result{MIN, 1, MIN} ));
    ASSERT(( Solution().run({MAX, MIN}) == Result{MIN, MAX} ));

    return 0;
}

}
