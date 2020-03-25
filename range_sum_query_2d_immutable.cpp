// Problem: https://leetcode.com/problems/range-sum-query-2d-immutable/

#include <vector>
#include "utils.h"

namespace range_sum_query_2d_immutable {

class NumMatrix {
public:
    typedef std::vector<std::vector<int>> Matrix;

    // Time: O(n * m), Space: O(n * m) for the object lifetime
    // n - number of rows, m - number of columns
    //
    NumMatrix(const Matrix& matrix)
    {
        init(matrix);
    }

    // Note: The following must be true: 0 <= row1 <= row2, 0 <= col1 <= col2.
    //
    // Note: This method is not const in the original problem.
    //
    // Time: O(1), Space: O(1)
    //
    int sumRegion(int row1, int col1, int row2, int col2) const
    {
        if (row1 < 0 || row1 >= sums_.size() ||
            col1 < 0 || col1 >= sums_[0].size())
            throw std::out_of_range("Rectangle is out of range");

        if (row1 > row2 || col1 > col2)
            throw std::runtime_error("Incorrect rectangle");

        int result = sums_[row2][col2];

        if (row1)
            result -= sums_[row1 - 1][col2];

        if (col1)
            result -= sums_[row2][col1 - 1];

        if (row1 && col1)
            result += sums_[row1 - 1][col1 - 1];

        return result;
    }

private:
    void init(const Matrix& matrix)
    {
        sums_ = matrix;

        if (!matrix.size() || !matrix[0].size())
            return;

        const int row_count = sums_.size();
        const int col_count = sums_[0].size();

        sums_[0][0] = matrix[0][0];

        for (int r = 1; r < row_count; r++)
            sums_[r][0] += sums_[r - 1][0];

        for (int c = 1; c < col_count; c++)
            sums_[0][c] += sums_[0][c - 1];

        for (int r = 1; r < row_count; r++) {
            for (int c = 1; c < col_count; c++)
                sums_[r][c] += sums_[r - 1][c] + sums_[r][c - 1] - sums_[r - 1][c - 1];
        }
    }

    Matrix sums_;
};


void test(const NumMatrix::Matrix& matrix)
{
    if (!matrix.size() || !matrix[0].size())
        return;

    const auto sum = [](const NumMatrix::Matrix& matrix, int r1, int c1, int r2, int c2)
    {
        int result = 0;
        for (int r = r1; r <= r2; r++) {
            for (int c = c1; c <= c2; c++)
                result += matrix[r][c];
        }
        return result;
    };

    NumMatrix m(matrix);

    const int row_count = matrix.size();
    const int col_count = matrix[0].size();

    for (int w = 1; w <= row_count; w++) {
        for (int h = 1; h <= col_count; h++) {
            for (int r = 0; r <= row_count - w; r++) {
                for (int c = 0; c <= col_count - h; c++) {
                    const int r1 = r;
                    const int c1 = c;
                    const int r2 = r + w - 1;
                    const int c2 = c + h - 1;

                    const int expected = sum(matrix, r1, c1, r2, c2);

                    ASSERT_EX( m.sumRegion(r1, c1, r2, c2) == expected,
                        to_string(matrix) + ", " + to_string(std::vector<int>{r1, c1, r2, c2}) + " -> " +
                        to_string(expected));
                }
            }
        }
    }
}

int main()
{
    test({{0}});
    test({{1}});
    test({{-1}});

    test({{1, 2}});

    test({{1},
          {2}});

    test({{1, 2},
          {3, 4}});

    test({{-1,  2,  3,  4},
          { 5,  0,  7,  8},
          { 9, 10, 11, 12}});

    return 0;
}

}
