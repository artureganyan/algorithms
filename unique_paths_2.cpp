// Problem: https://leetcode.com/problems/unique-paths-ii/

#include <vector>
#include "utils.h"

namespace unique_paths_2 {

// Note: All solutions return int as required by the initial problem, but
// 32-bit signed integer can hold the result for at most the 17x18 grid.


// The straightforward solution, recursively counting paths from the start
// cell to the target

class Solution1 {
public:
    // Time: O(C(n + m, m)), Space: O(n + m), Recursion depth <= n + m
    // n - number of rows, m - number of columns,
    // C(n + m, m) - binomial coefficient (n + m)! / (m! * n!)
    //
    // Note: The grid must be a rectangle.
    //
    // Note: Time complexity is estimated by the number of paths for a grid
    // without obstacles, which is the binomial coefficient as shown in
    // unique_paths. Space complexity depends on the recursion depth, which
    // is determined by the path length n + m - 1.
    //
    int run(std::vector<std::vector<int>>& grid)
    {
        return getPathsCount(grid, 0, 0);
    }

private:
    int getPathsCount(std::vector<std::vector<int>>& grid, int r, int c)
    {
        if (!grid.size() || r >= grid.size() || c >= grid[0].size() || grid[r][c] == 1)
            return 0;

        if (r == grid.size() - 1 && c == grid[0].size() - 1)
            return 1;

        // Go right + go down
        return getPathsCount(grid, r, c + 1) +
               getPathsCount(grid, r + 1, c);
    }
};


// Non-recursive solution, counting paths from the target cell to the start

class Solution2 {
public:
    // Time: O(n * m), Space: O(1), n - number of rows, m - number of columns
    //
    // Note: The grid must be a rectangle.
    //
    int run(std::vector<std::vector<int>>& grid)
    {
        // Idea:
        // Count paths in reverse order, moving the start from the target cell to (0, 0):
        // 1. Put the start to the target cell (t = rows_count - 1, l = cols_count - 1).
        //    Then there is only 1 path, i.e. the cell itself, or no path if it contains
        //    an obstacle.
        // 2. Go to the outer rectangle, which top-left corner is (t - 1, l - 1) if this
        //    is a square.
        // 3. Calculate counts for the top and left borders of the outer rectangle. For
        //    each cell (r, c), the count is a sum of counts for (r + 1, c) and (r, c + 1),
        //    because we can step only right or down. If the cell contains an obstacle,
        //    the count is 0.
        // 4. Repeat 2 and 3 until we reach (0, 0).
        //
        // 0 * 0    0 * 0    2 0 1
        // 0 0 0 -> 0 2 1 -> 2 2 1
        // * 0 1    * 1 1    0 1 1

        if (!grid.size() || !grid[0].size())
            return 0;

        const int rows_count = grid.size();
        const int cols_count = grid[0].size();

        // Start from the target cell
        int t = rows_count - 1;
        int l = cols_count - 1;
        int b = t;
        int r = l;

        if (grid[t][l] == 1)
            return 0;

        if (rows_count == 1 && cols_count == 1)
            return 1;

        grid[t][l] = 1;

        do {
            // Go up and left. If not possible, decrease the right/bottom border
            // because everything at the right/bottom is already counted.
            if (t > 0) {
                t -= 1;
            } else {
                r = l - 1;
            }
            if (l > 0) {
                l -= 1;
            } else {
                b = t - 1;
            }
            // Count top border of the outer rectangle
            for (int ci = r, ri = t; ci > l; ci--) {
                calculateCount(grid, ri, ci);
            }
            // Count left border of the outer rectangle
            for (int ri = b, ci = l; ri > t; ri--) {
                calculateCount(grid, ri, ci);
            }
            // Count top-left corner of the outer rectangle
            calculateCount(grid, t, l);

        } while (t != 0 || l != 0);

        return grid[0][0];
    }

private:
    inline void calculateCount(std::vector<std::vector<int>>& grid, int r, int c) const
    {
        int& cell = grid[r][c];
        // Empty cell
        if (cell == 0) {
            if (r < grid.size() - 1)
                cell += grid[r + 1][c];
            if (c < grid[r].size() - 1)
                cell += grid[r][c + 1];
        // Obstacle
        } else {
            cell = 0;
        }
    }
};


template <typename Solution>
void test()
{
    std::vector<std::vector<int>> grid;

    ASSERT(( Solution().run(grid = {}) == 0 ));
    ASSERT(( Solution().run(grid = {{}}) == 0 ));
    ASSERT(( Solution().run(grid = {{0}}) == 1 ));
    ASSERT(( Solution().run(grid = {{1}}) == 0 ));

    ASSERT(( Solution().run(grid = {{0, 0}}) == 1 ));

    ASSERT(( Solution().run(grid = {{0, 0},
                                    {0, 0}}) == 2 ));

    ASSERT(( Solution().run(grid = {{1, 0},
                                    {0, 0}}) == 0 ));

    ASSERT(( Solution().run(grid = {{0, 0},
                                    {0, 1}}) == 0 ));

    ASSERT(( Solution().run(grid = {{0, 1},
                                    {1, 0}}) == 0 ));

    ASSERT(( Solution().run(grid = {{0, 1},
                                    {0, 0}}) == 1 ));

    ASSERT(( Solution().run(grid =
             {{0, 0, 0},
              {0, 0, 0},
              {0, 0, 0}}) == 6 ));

    ASSERT(( Solution().run(grid =
             {{0, 0, 0},
              {0, 1, 0},
              {0, 0, 0}}) == 2 ));

    ASSERT(( Solution().run(grid =
             {{0, 1, 0},
              {0, 0, 0},
              {0, 1, 0}}) == 1 ));

    ASSERT(( Solution().run(grid =
             {{0, 1, 0},
              {0, 0, 1},
              {0, 1, 0}}) == 0 ));

    ASSERT(( Solution().run(grid =
             {{0, 0, 1},
              {0, 1, 0},
              {1, 0, 0}}) == 0 ));

    ASSERT(( Solution().run(grid =
             {{0, 1, 1},
              {0, 0, 0},
              {1, 1, 0}}) == 1 ));

    ASSERT(( Solution().run(grid =
             {{0, 0, 1, 0},
              {1, 0, 1, 0},
              {0, 0, 1, 0},
              {0, 1, 1, 0},
              {0, 0, 0, 0}}) == 0 ));

    ASSERT(( Solution().run(grid =
             {{0, 1, 0, 0, 0},
              {0, 1, 0, 0, 0},
              {0, 0, 0, 0, 0},
              {1, 1, 0, 0, 0},
              {0, 0, 0, 0, 0}}) == 6 ));

    ASSERT(( Solution().run(grid =
             {{0, 0, 1, 0, 0},
              {1, 0, 0, 0, 0},
              {0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0}}) == 20 ));

    ASSERT(( Solution().run(grid =
             {{0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0}}) == 70 ));
}

int main()
{
    test<Solution1>();
    test<Solution2>();
    return 0;
}

}
