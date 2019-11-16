// Problem: https://leetcode.com/problems/search-a-2d-matrix/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace search_in_2d_matrix {

class Solution {
public:
    // Time: O(log(n)), Space: O(1)
    //
    // Note: Each row must be sorted in ascending order, and its first cell
    // must be greater than the last cell of the previous row.
    //
    bool run(const std::vector<std::vector<int>>& matrix, int target)
    {
        if (!matrix.size() || !matrix[0].size())
            return false;

        auto row = std::upper_bound(matrix.begin(), matrix.end(), target,
            [](int value, const auto& row)
            {
                return value < row[0];
            });

        if (row == matrix.begin())
            return false;
        row--;

        return std::binary_search(row->begin(), row->end(), target);
    }
};


typedef std::vector<std::vector<int>> Matrix;

void test(const Matrix& matrix, int target, bool expected)
{
    ASSERT_EX( Solution().run(matrix, target) == expected,
               to_string(matrix) + ", " + to_string(target) + " == " + to_string(expected) );
}

int main()
{
    Matrix m;

    test({}, 0, false);
    test({{}}, 0, false);


    m = {{0}};

    test(m, -1, false);
    test(m,  0, true);
    test(m,  1, false);


    m = {{-1, 3}};

    test(m, -2, false);
    test(m, -1, true);
    test(m,  0, false);
    test(m,  3, true);
    test(m,  4, false);


    m = {{-1},
         { 3}};

    test(m, -2, false);
    test(m, -1, true);
    test(m,  0, false);
    test(m,  3, true);
    test(m,  4, false);


    m = {{-3, -1,  4},
         { 6,  9, 12},
         {15, 17, 20}};

    test(m, -4, false);
    test(m, -3, true);
    test(m,  4, true);
    test(m,  5, false);
    test(m,  9, true);
    test(m, 14, false);
    test(m, 20, true);
    test(m, 21, false);

    return 0;
}

}
