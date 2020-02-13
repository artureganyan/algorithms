// Problem: https://leetcode.com/problems/maximal-square/

#include <vector>
#include "utils.h"

namespace maximal_square {

class Solution {
public:
    // Note: The matrix must be a rectangle, and contain '0' for the empty
    // cells and '1' for the non-empty.
    //
    // Time: O(n), Space: O(n), n - number of cells in the matrix
    //
    int run(const std::vector<std::vector<char>>& matrix)
    {
        // Idea:
        // For each cell (r, c), find the maximum square with the bottom-right
        // corner at this cell. This can be done iteratively by using the same
        // value of the adjacent cells (r-1, c) and (r, c-1) as follows:
        //
        // Let's say we are at the non-empty cell (r, c), and already know that
        // the maximum squares for (r-1, c) and (r, c-1) have the sizes s1 and s2
        // respectively. Then, if s1 != s2, these squares overlap, so the size
        // for (r, c) is min(s1, s2) + 1. Otherwise, s1 == s2 = s, and the size
        // is s plus, if the top-left corner (r-s, c-s) is not empty, 1 cell.
        //
        // case 1:                 case 2:
        // b b b b b 0
        // b b b a a a             ? a a a
        // b b b a b a             b a b a
        // b b b a a a             b a a a
        // b b b b b c             b b b c
        //
        // size of:                size of:
        // a = 3                   a = 3
        // b = 5                   b = 3
        // c = min(a, b) + 1 = 4   c = 3 or 4, depending on the cell "?"
        //
        // Finally, we start from the top-left corner of the matrix and go by
        // rows (or by columns) to the bottom-right, calculating the maximum
        // square for each cell and finding the largest one among all cells.

        if (!matrix.size() || !matrix[0].size())
            return 0;

        std::vector<std::vector<int>> max_squares(matrix.size());
        for (int r = 0; r < matrix.size(); r++)
            max_squares[r].resize(matrix[r].size(), 0);

        const char EMPTY      = '0';
        int        max_square = 0;

        for (int r = 0; r < matrix.size(); r++) {
            for (int c = 0; c < matrix[r].size(); c++) {
                if (matrix[r][c] == EMPTY)
                    continue;

                const int s1 = r > 0 ? max_squares[r - 1][c] : 0;
                const int s2 = c > 0 ? max_squares[r][c - 1] : 0;

                int& s = max_squares[r][c];
                if (s1 == s2) {
                    s = s1 + (matrix[r - s1][c - s1] != EMPTY ? 1 : 0);
                } else {
                    s = std::min(s1, s2) + 1;
                }

                max_square = std::max(max_square, s);
            }
        }

        return max_square * max_square;
    }
};


void test(const std::vector<std::vector<char>>& matrix, int expected, int rotates = 1)
{
    for (int c = 0; c < rotates; c++) {
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
