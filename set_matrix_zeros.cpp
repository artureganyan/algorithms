// Problem: https://leetcode.com/problems/set-matrix-zeroes/

#include <vector>
#include "utils.h"

namespace set_matrix_zeros {

class Solution {
public:
    // Time: O(n * m), Space: O(1), n - number of rows, m - number of columns
    //
    // Note: The matrix must be a rectangle.
    //
    void run(std::vector<std::vector<int>>& matrix)
    {
        if (!matrix.size() || !matrix[0].size())
            return;

        // Check if the first row should be zero
        bool zero_r0 = false;
        for (int c = 0; c < matrix[0].size(); c++) {
            if (matrix[0][c] == 0) {
                zero_r0 = true;
                break;
            }
        }
        // Check if the first column should be zero
        bool zero_c0 = false;
        for (int r = 0; r < matrix.size(); r++) {
            if (matrix[r][0] == 0) {
                zero_c0 = true;
                break;
            }
        }

        // Now we can use the first row/column to store zeros for the rest
        // columns/rows that should be zero:
        // - If a row should be zero, set its first column to zero.
        // - If a column should be zero, set its first row to zero.
        for (int r = 1; r < matrix.size(); r++) {
            for (int c = 1; c < matrix[r].size(); c++) {
                if (matrix[r][c] == 0) {
                    matrix[r][0] = 0;
                    matrix[0][c] = 0;
                }
            }
        }
        // Put zero into each cell for which the first row or column contains zero
        for (int r = 1; r < matrix.size(); r++) {
            for (int c = 1; c < matrix[r].size(); c++) {
                if (matrix[r][0] == 0 || matrix[0][c] == 0)
                    matrix[r][c] = 0;
            }
        }

        // Set the first row/column to zero if needed
        if (zero_r0) {
            for (int c = 0; c < matrix[0].size(); c++)
                matrix[0][c] = 0;
        }
        if (zero_c0) {
            for (int r = 0; r < matrix.size(); r++)
                matrix[r][0] = 0;
        }
    }
};


typedef std::vector<std::vector<int>> Matrix;

void test(const Matrix& matrix, const Matrix& expected, int rotate_count = 1)
{
    for (int i = 0; i < rotate_count; i++) {
        Matrix matrix_i   = rotate(matrix, i);
        Matrix expected_i = rotate(expected, i);

        Matrix result = matrix_i;
        Solution().run(result);

        ASSERT_EX(result == expected_i, to_string(matrix_i) + " -> " + to_string(result));
    }
}

int main()
{
    test({}, {});
    test({{}}, {{}});

    test({{-1}}, {{-1}});
    test({{ 0}}, {{ 0}});
    test({{ 1}}, {{ 1}});

    test({{-1, 2}}, {{-1, 2}}, 2);
    test({{-1, 0}}, {{ 0, 0}}, 4);
    test({{ 0, 0}}, {{ 0, 0}}, 2);

    test({{-1, 0, 1}}, {{0, 0, 0}}, 2);

    test({{0,  2},
          {3, -4}},

         {{0,  0},
          {0, -4}}, 4);

    test({{ 0, 2, 3},
          {-4, 5, 6}},

         {{ 0, 0, 0},
          { 0, 5, 6}}, 4);

    test({{ 1, 0, 3},
          {-4, 5, 6}},

         {{ 0, 0, 0},
          {-4, 0, 6}}, 4);

    test({{1, 0, 1, 0, 1},
          {1, 1, 1, 1, 1},
          {1, 0, 1, 0, 1},
          {1, 1, 1, 1, 1},
          {1, 0, 1, 0, 1}},

         {{0, 0, 0, 0, 0},
          {1, 0, 1, 0, 1},
          {0, 0, 0, 0, 0},
          {1, 0, 1, 0, 1},
          {0, 0, 0, 0, 0}}, 2);

    return 0;
}

}
