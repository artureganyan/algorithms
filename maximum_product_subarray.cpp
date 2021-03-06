// Problem: https://leetcode.com/problems/maximum-product-subarray/

#include <vector>
#include "utils.h"

namespace maximum_product_subarray {

class Solution {
public:
    // Note: The maximum/minimum subproducts of the nums must fit in int.
    //
    // Time: O(n), Space: O(1), n - number of elements
    //
    int run(const std::vector<int>& nums)
    {
        // Idea:
        // Iterate over the numbers, calculate the following products and find
        // the maximum:
        //
        // nums        =   1 10 -2 3 4 -1 1 -10 0 5 6
        // product1    = 1*1*10*-2*3*4*-1*1*-10,1*5*6
        // product2    = 0,      1*3*4*-1*1*-10,0
        // max_product = ?,1,10,    12,     120
        //
        // The product1 starts at the each sequence of non-zero numbers.
        // The product2 starts after the first negative number in the product1.
        // When the product1 < 0, the product2 holds its largest positive
        // suffix. In the example above, this is 3*4*-1*1*-10, which gives
        // the maximum.

        if (!nums.size())
            return 0;

        int max_product = std::numeric_limits<int>::min();
        int product1    = 1;
        int product2    = 0;

        for (const int n : nums) {
            product1 *= n;
            max_product = std::max(max_product, product1);

            if (product2 != 0) {
                product2 *= n;
                max_product = std::max(max_product, product2);
            } else if (n < 0) {
                product2 = 1;
            }

            if (product1 == 0) {
                product1 = 1;
                product2 = 0;
            }
        }

        return max_product;
    }
};

int main()
{
    ASSERT( Solution().run({}) == 0 );

    ASSERT( Solution().run({0}) == 0 );
    ASSERT( Solution().run({1}) == 1 );
    ASSERT( Solution().run({-1}) == -1 );

    ASSERT( Solution().run({2}) == 2 );
    ASSERT( Solution().run({2, 3}) == 6 );
    ASSERT( Solution().run({2, 3, 4}) == 24 );
    ASSERT( Solution().run({2, 0, 3}) == 3 );
    ASSERT( Solution().run({0, 2, 0}) == 2 );
    ASSERT( Solution().run({2, -2, 3}) == 3 );
    ASSERT( Solution().run({-2, 3, -2}) == 12 );

    ASSERT( Solution().run({-2, -3}) == 6 );
    ASSERT( Solution().run({-2, -3, -4}) == 12 );
    ASSERT( Solution().run({-3, -4, -2}) == 12 );
    ASSERT( Solution().run({-2, 0}) == 0 );
    ASSERT( Solution().run({0, -2}) == 0 );

    ASSERT( Solution().run({0, 1, -1, 2, 0, -1, 2, -3, -2, 0}) == 12 );

    return 0;
}

}
