// Problem: https://leetcode.com/problems/h-index/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace h_index {

class Solution {
public:
    // Note: The citations should contain non-negative integers only. If it
    // will contain negative values, they will be handled the same way as 0.
    // If the citations are empty or contain no positive values, returns 0.
    //
    // Time: O(n * log(n)), Space: O(1), n - number of elements
    //
    int run(std::vector<int>& citations)
    {
        std::sort(citations.begin(), citations.end());

        // Note: The search could be done in O(log(n)) time, as in h_index_2 problem
        for (int i = 0; i < citations.size(); i++) {
            const int h_index = citations.size() - i;
            if (citations[i] >= h_index)
                return h_index;
        }
        return 0;
    }
};

int main()
{
    std::vector<int> v;

    ASSERT( Solution().run(v = {}) == 0 );

    ASSERT( Solution().run(v = {0}) == 0 );

    ASSERT( Solution().run(v = {1}) == 1 );
    ASSERT( Solution().run(v = {1, 0}) == 1 );
    ASSERT( Solution().run(v = {1, 0, 1}) == 1 );

    ASSERT( Solution().run(v = {3}) == 1 );
    ASSERT( Solution().run(v = {3, 2}) == 2 );
    ASSERT( Solution().run(v = {3, 2, 3}) == 2 );
    ASSERT( Solution().run(v = {3, 3, 3}) == 3 );
    ASSERT( Solution().run(v = {3, 3, 3, 3}) == 3 );

    ASSERT( Solution().run(v = {4, 10, 2, 9, 1}) == 3 );
    ASSERT( Solution().run(v = {4, 10, 2, 4, 9, 1}) == 4 );

    return 0;
}

}
