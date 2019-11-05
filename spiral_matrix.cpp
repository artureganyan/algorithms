// Problem: https://leetcode.com/problems/spiral-matrix/

#include <vector>
#include "utils.h"

namespace spiral_matrix {

class Solution {
public:
    // Time: O(n), Size: O(n), n = number of elements in the matrix
    //
    // Note: The matrix must be a rectangle
    //
    std::vector<int> run(const std::vector<std::vector<int>>& matrix)
    {
        if (!matrix.size() || !matrix[0].size())
            return {};

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
        std::vector<int> result;
        result.reserve(matrix.size() * matrix[0].size());

        for (int l = 0, t = 0, r = matrix[0].size() - 1, b = matrix.size() - 1;
             l <= r && t <= b; l++, r--, t++, b--) {

            for (int i = l; i <= r; i++)
                result.push_back(matrix[t][i]);

            for (int i = t + 1; i <= b; i++)
                result.push_back(matrix[i][r]);

            if (t < b) {
                for (int i = r - 1; i >= l; i--)
                    result.push_back(matrix[b][i]);
            }

            if (l < r) {
                for (int i = b - 1; i > t; i--)
                    result.push_back(matrix[i][l]);
            }
        }

        return result;
    }
};

int main()
{
    ASSERT(( Solution().run({}) == std::vector<int>{} ));
    ASSERT(( Solution().run({{}}) == std::vector<int>{} ));
    ASSERT(( Solution().run({{1}}) == std::vector<int>{1} ));

    ASSERT(( Solution().run({{1, 2, 3}}) ==
             std::vector<int>{1, 2, 3} ));

    ASSERT(( Solution().run({{1},
                             {2},
                             {3}}) ==
             std::vector<int>{1, 2, 3} ));

    ASSERT(( Solution().run({{1, 2},
                             {3, 4}}) ==
             std::vector<int>{1, 2, 4, 3} ));

    ASSERT(( Solution().run({{1, 2, 3},
                             {4, 5, 6}}) ==
             std::vector<int>{1, 2, 3, 6, 5, 4} ));

    ASSERT(( Solution().run({{1, 2},
                             {3, 4},
                             {5, 6}}) ==
             std::vector<int>{1, 2, 4, 6, 5, 3} ));

    ASSERT(( Solution().run({{1, 2, 3},
                             {4, 5, 6},
                             {7, 8, 9}}) ==
             std::vector<int>{1, 2, 3, 6, 9, 8, 7, 4, 5} ));

    return 0;
}

}
