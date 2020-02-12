// Problem: https://leetcode.com/problems/maximal-square/

#include <vector>
#include "utils.h"

namespace maximal_square {

class Solution {
public:
    // Note: The matrix must be a rectangle.
    //
    // Time: O(n), Space: O(n), n - number of cells in the matrix
    //
    int run(const std::vector<std::vector<char>>& matrix)
    {
        if (!matrix.size() || !matrix[0].size())
            return 0;

        std::vector<std::vector<int>> max_squares(matrix.size());
        for (int r = 0; r < matrix.size(); r++)
            max_squares[r].resize(matrix[r].size(), 0);

        const char ONE        = '1';
        int        max_square = 0;

        for (int r = 0; r < matrix.size(); r++) {
            for (int c = 0; c < matrix[r].size(); c++) {
                if (matrix[r][c] != ONE)
                    continue;

                const int s1    = r > 0 ? max_squares[r - 1][c] : 0;
                const int s2    = c > 0 ? max_squares[r][c - 1] : 0;
                const int s_min = std::min(s1, s2);

                int& s = max_squares[r][c];
                if (matrix[r - s_min][c - s_min] == ONE) {
                    s = s_min + 1;
                } else {
                    s = s_min;
                }

                max_square = std::max(max_square, s);
            }
        }

        return max_square * max_square;
    }
};


void test(const std::vector<std::vector<char>>& matrix, int expected, int count = 1)
{
    for (int c = 0; c < count; c++) {
        const auto matrix_rotated = rotate(matrix, c);
        ASSERT_EX( Solution().run(matrix_rotated) == expected,
            to_string(matrix_rotated) + " -> " + to_string(expected) );
    }
}

int main()
{
    test({}, 0);
    test({{}}, 0);

    test({{'0'}}, 0);
    test({{'1'}}, 1);

    test({{'0', '0'}}, 0, 2);
    test({{'0', '1'}}, 1, 4);
    test({{'1', '1'}}, 1, 2);

    test({{'0', '1'},
          {'0', '0'},}, 1, 4);

    test({{'0', '1'},
          {'1', '0'},}, 1, 2);

    test({{'1', '1'},
          {'1', '0'},}, 1, 4);

    test({{'1', '1'},
          {'1', '1'},}, 4);

    test({{'1', '1', '1'},
          {'1', '0', '1'},
          {'1', '1', '1'}}, 1);

    test({{'1', '0', '1'},
          {'0', '1', '0'},
          {'1', '0', '1'}}, 1);

    test({{'1', '1', '1'},
          {'0', '1', '0'},
          {'1', '1', '1'}}, 1);

    test({{'1', '1', '0'},
          {'0', '1', '0'},
          {'0', '1', '1'}}, 1);

    test({{'1', '0', '0', '0', '0'},
          {'1', '1', '0', '0', '0'},
          {'1', '1', '1', '0', '0'},
          {'1', '1', '1', '1', '1'},
          {'1', '1', '1', '1', '0'},
          {'1', '1', '1', '1', '0'},
          {'1', '1', '0', '0', '0'},
          {'1', '0', '0', '0', '0'}}, 9, 4);

    test({{'1', '1', '0', '1', '1', '1'},
          {'1', '1', '1', '0', '1', '1'},
          {'0', '1', '1', '1', '1', '1'},
          {'0', '1', '1', '1', '1', '1'},
          {'1', '1', '1', '1', '0', '1'},
          {'1', '0', '1', '1', '1', '1'}}, 9, 4);

    return 0;
}

}
