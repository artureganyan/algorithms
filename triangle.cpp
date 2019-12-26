// Problem: https://leetcode.com/problems/triangle/

#include <vector>
#include "utils.h"

namespace triangle {

// Note: The triangle must have the following format:
// {
//     {  1  },
//     { 2,3 },
//     {4,5,6},
//     { ... }
// }


// Recursive solution

class Solution1 {
public:
    // Time: O(2^r), Space: O(r), Recursion depth <= r + 2, r - number of rows
    int run(const std::vector<std::vector<int>>& triangle)
    {
        return findMinimumSum(triangle, 0, 0, 0);
    }

private:
    int findMinimumSum(const std::vector<std::vector<int>>& triangle,
        int row, int column, int current_sum) const
    {
        if (row >= triangle.size() || column >= triangle[row].size())
            return current_sum;

        const int sum           = current_sum + triangle[row][column];
        const int min_sum_left  = findMinimumSum(triangle, row + 1, column, sum);
        const int min_sum_right = findMinimumSum(triangle, row + 1, column + 1, sum);

        return std::min(min_sum_left, min_sum_right);
    }
};


// Non-recursive solution, does not modify the triangle

class Solution2 {
public:
    // Time: O(n), Space: O(r), n - number of elements (i.e. r*(r+1)/2), r - number of rows
    int run(const std::vector<std::vector<int>>& triangle)
    {
        if (!triangle.size() || !triangle.front().size())
            return 0;

        std::vector<int> min_sums;
        min_sums = triangle.back();

        for (int r = triangle.size() - 2; r >= 0; r--) {
            const auto& row = triangle[r];
            for (int c = 0; c < row.size(); c++)
                min_sums[c] = std::min(min_sums[c], min_sums[c + 1]) + row[c];
        }

        return min_sums[0];
    }
};


// Non-recursive solution, modifies the triangle

class Solution3 {
public:
    // Time: O(n), Space: O(1), n - number of elements
    int run(std::vector<std::vector<int>>& triangle)
    {
        if (!triangle.size() || !triangle.front().size())
            return 0;

        for (int r = triangle.size() - 2; r >= 0; r--) {
            auto&       row       = triangle[r];
            const auto& row_below = triangle[r + 1];
            for (int c = 0; c < row.size(); c++)
                row[c] += std::min(row_below[c], row_below[c + 1]);
        }

        return triangle[0][0];
    }
};


template <typename Solution>
void test()
{
    std::vector<std::vector<int>> v;

    ASSERT( Solution().run(v = {{}}) == 0 );

    ASSERT( Solution().run(v = {{ 0}}) ==  0 );
    ASSERT( Solution().run(v = {{ 1}}) ==  1 );
    ASSERT( Solution().run(v = {{-1}}) == -1 );

    ASSERT( Solution().run(v = {
                {1},
               {1,1}
            }) == 2 );

    ASSERT( Solution().run(v = {
                {1},
               {2,3}
            }) == 3 );

    ASSERT( Solution().run(v = {
                {1},
               {3,2}
            }) == 3 );

    ASSERT( Solution().run(v = {
                {1},
              {1, -1}
            }) == 0 );

    ASSERT( Solution().run(v = {
                {1},
               {3,2},
              {1,3,4}
            }) == 5 );

    ASSERT( Solution().run(v = {
                {1},
               {3,2},
              {2,1,3}
            }) == 4 );

    ASSERT( Solution().run(v = {
                {1},
               {3,2},
              {2,3,1}
            }) == 4 );

    ASSERT( Solution().run(v = {
                {1},
               {2,3},
              {2,1,3}
            }) == 4 );
}

int main()
{
    test<Solution1>();
    test<Solution2>();
    test<Solution3>();

    return 0;
}

}
