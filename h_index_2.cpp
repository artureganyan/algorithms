// Problem: https://leetcode.com/problems/h-index/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace h_index_2 {

class Solution {
public:
    // Note: The citations must be sorted in ascending order and should contain
    // non-negative integers only. If it will contain negative values, they
    // will be handled the same way as 0. If the citations are empty or contain
    // no positive values, returns 0.
    //
    // Time: O(log(n)), Space: O(1), n - number of elements
    //
    int run(const std::vector<int>& citations)
    {
        int result = 0;
        int i1     = 0;
        int i2     = citations.size() - 1;

        while (i1 <= i2) {
            const int ih      = i1 + (i2 - i1) / 2;
            const int h_index = citations.size() - ih;

            if (citations[ih] >= h_index) {
                result = h_index;
                i2     = ih - 1;
            } else {
                i1     = ih + 1;
            }
        }
        return result;
    }
};

int main()
{
    ASSERT( Solution().run({}) == 0 );

    ASSERT( Solution().run({0}) == 0 );

    ASSERT( Solution().run({1}) == 1 );
    ASSERT( Solution().run({0, 1}) == 1 );
    ASSERT( Solution().run({0, 1, 1}) == 1 );

    ASSERT( Solution().run({3}) == 1 );
    ASSERT( Solution().run({2, 3}) == 2 );
    ASSERT( Solution().run({2, 3, 3}) == 2 );
    ASSERT( Solution().run({3, 3, 3}) == 3 );
    ASSERT( Solution().run({3, 3, 3, 3}) == 3 );

    ASSERT( Solution().run({1, 2, 4, 9, 10}) == 3 );
    ASSERT( Solution().run({1, 2, 4, 4, 9, 10}) == 4 );

    return 0;
}

}
