// Problem: https://leetcode.com/problems/container-with-most-water/

#include <vector>
#include <math.h>
#include "utils.h"

namespace container_with_most_water {

class Solution
{
public:
    // Time: O(n), Space: O(1)
    int run(std::vector<int>& heights)
    {
        int max_area = 0;

        for (int i1 = 0, i2 = heights.size() - 1; i1 < i2;) {
            const int h1 = heights[i1];
            const int h2 = heights[i2];
            const int w  = i2 - i1;

            const int area = std::min(h2, h1) * w;
            max_area = std::max(max_area, area);

            if (h1 > h2) {
                i2--;
            } else {
                i1++;
            }
        }

        return max_area;
    }
};

int main()
{
    std::vector<int> v;
    ASSERT(Solution().run(v = {}) == 0);
    ASSERT(Solution().run(v = {1}) == 0);
    ASSERT(Solution().run(v = {1, 1}) == 1);
    ASSERT(Solution().run(v = {1, 5, 8, 5, 1}) == 10);
    ASSERT(Solution().run(v = {1, 8, 6, 2, 5, 4, 8, 3, 7}) == 49);
    return 0;
}

}
