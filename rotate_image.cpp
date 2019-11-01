// Problem: https://leetcode.com/problems/rotate-image/

#include <vector>
#include "utils.h"

namespace rotate_image {

class Solution {
public:
    // Time: O(n^2), Space: O(1), n = matrix.size()
    //
    // Note: The matrix must be a square
    //
    void run(std::vector<std::vector<int>>& matrix)
    {
        if (!matrix.size())
            return;

        // Rotate contours with top-left corner (i1, i1) and bottom-right (i2, i2),
        // going from the outer contour to the center one:
        //
        // (i1,i1)
        //   e1 --- e2
        //    |     |
        //    |     |
        //   e4 --- e3
        //       (i2,i2)
        //
        for (int i1 = 0, i2 = matrix.size() - 1; i1 < i2; i1++, i2--) {
            for (int i = i1; i < i2; i++) {
                int&      e1 = matrix[i1][i];
                const int v1 = e1;

                int&      e2 = matrix[i][i2];
                const int v2 = e2;

                int&      e3 = matrix[i2][i2 - (i - i1)];
                const int v3 = e3;

                int&      e4 = matrix[i2 - (i - i1)][i1];
                const int v4 = e4;

                e1 = v4;
                e2 = v1;
                e3 = v2;
                e4 = v3;
            }
        }
    }
};

int main()
{
    typedef std::vector<std::vector<int>> Matrix;
    Matrix m;

    ASSERT(( Solution().run(m = {}), m == Matrix{} ));
    ASSERT(( Solution().run(m = {{1}}), m == Matrix{{1}} ));
    ASSERT(( Solution().run(m = {{1, 2}, {3, 4}}), m == Matrix{{3, 1}, {4, 2}} ));
    ASSERT(( Solution().run(m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}), m == Matrix{{7, 4, 1}, {8, 5, 2}, {9, 6, 3}} ));

    return 0;
}

}
