// Problem: https://leetcode.com/problems/spiral-matrix-ii/

#include <vector>
#include "utils.h"

namespace spiral_matrix_2 {

class Solution {
public:
    // Time: O(n^2), Space: O(n^2)
    std::vector<std::vector<int>> run(int n)
    {
        if (n <= 0)
            return {};

        std::vector<std::vector<int>> result(n);
        for (int r = 0; r < n; r++)
            result[r].resize(n);

        // Iterate through the contours with top-left corner (t, l) and
        // bottom-right (b, r), going from the outer contour to the center one:
        //
        // (t, l)
        //    > --- >
        //    > ... |
        //    | ... |
        //    ^ --- <
        //        (b, r)
        //
        // Note: This will also work with rectangular matrices, so it checks for
        // 1xn and nx1 sizes.
        int value = 1;

        for (int l = 0, t = 0, r = n - 1, b = n - 1;
             l <= r && t <= b; l++, r--, t++, b--)
        {
            for (int i = l; i <= r; i++)
                result[t][i] = value++;

            for (int i = t + 1; i <= b; i++)
                result[i][r] = value++;

            if (t < b) {
                for (int i = r - 1; i >= l; i--)
                    result[b][i] = value++;
            }

            if (l < r) {
                for (int i = b - 1; i >= t + 1; i--)
                    result[i][l] = value++;
            }
        }

        return result;
    }
};

int main()
{
    typedef std::vector<std::vector<int>> Matrix;

    ASSERT(( Solution().run(-1) == Matrix{} ));
    ASSERT(( Solution().run(0) == Matrix{} ));
    ASSERT(( Solution().run(1) == Matrix{{1}} ));

    ASSERT(( Solution().run(2) == Matrix{{1, 2},
                                         {4, 3}} ));

    ASSERT(( Solution().run(3) == Matrix{{1, 2, 3},
                                         {8, 9, 4},
                                         {7, 6, 5}} ));
    return 0;
}

}
