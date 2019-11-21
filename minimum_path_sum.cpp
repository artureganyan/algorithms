// Problem: https://leetcode.com/problems/minimum-path-sum/

#include <vector>
#include <limits>
#include "utils.h"

namespace minimum_path_sum {

// The straightforward solution, recursively calculating paths from the start
// cell to the target

class Solution1 {
public:
    // Time: O(C(n + m, m)), Space: O(n + m), Recursion depth <= n + m
    // n - number of rows, m - number of columns,
    // C(n + m, m) - binomial coefficient (n + m)! / (m! * n!)
    //
    // Note: The grid must be a rectangle, and the minimum path must be
    // <= std::numeric_limits<int>::max().
    //
    // Note: Time complexity is estimated by the number of paths, which is
    // the binomial coefficient as shown in unique_paths. Space complexity
    // depends on the recursion depth, which is determined by the path
    // length n + m - 1.
    //
    int run(std::vector<std::vector<int>>& grid)
    {
        if (!grid.size() || !grid[0].size())
            return 0;

        return findMinimumPath(grid, 0, 0);
    }

private:
    int findMinimumPath(std::vector<std::vector<int>>& grid, int r, int c)
    {
        if (!grid.size() || r >= grid.size() || c >= grid[0].size())
            return -1;

        const int min_right = findMinimumPath(grid, r, c + 1);
        const int min_down  = findMinimumPath(grid, r + 1, c);

        int min = 0;
        if (min_right >= 0 && min_down >= 0) {
            min = std::min(min_right, min_down);
        } else if (min_right >= 0) {
            min = min_right;
        } else if (min_down >= 0) {
            min = min_down;
        } else {
            // Right-bottom corner
        }
        return grid[r][c] + min;
    }
};


// Non-recursive solution, finding the path from the target cell to the start

class Solution2 {
public:
    // Time: O(n * m), Space: O(1), n - number of rows, m - number of columns
    //
    // Note: The grid must be a rectangle, and the minimum path must be
    // <= std::numeric_limits<int>::max();
    //
    int run(std::vector<std::vector<int>>& grid)
    {
        // Idea:
        // Find paths in reverse order, moving the start from the target cell to (0, 0):
        // 1. Put the start to the target cell (t = rows_count - 1, l = cols_count - 1).
        //    Then there is only 1 path, i.e. the cell itself.
        // 2. Go to the outer rectangle, which top-left corner is (t - 1, l - 1) if this
        //    is a square.
        // 3. Calculate minimum paths for the top and left borders of the outer rectangle.
        //    For each cell (r, c), the minimum path is choosen between the minimums for
        //    (r + 1, c) and (r, c + 1), because we can step only right or down.
        // 4. Repeat 2 and 3 until we reach (0, 0).
        //
        // Source:  Solution:
        // 1 3 1    * * *    * * *    7 7 4
        // 2 1 2    * * * -> * 4 3 -> 6 4 3 -> Minimum path is 7
        // 1 3 1    * * 1    * 4 1    5 4 1

        if (!grid.size() || !grid[0].size())
            return 0;

        const int rows_count = grid.size();
        const int cols_count = grid[0].size();

        // Start from the target cell
        int t = rows_count - 1;
        int l = cols_count - 1;
        int b = t;
        int r = l;

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
                calculatePath(grid, ri, ci);
            }
            // Count left border of the outer rectangle
            for (int ri = b, ci = l; ri > t; ri--) {
                calculatePath(grid, ri, ci);
            }
            // Count top-left corner of the outer rectangle
            calculatePath(grid, t, l);

        } while (t != 0 || l != 0);

        return grid[0][0];
    }

private:
    void calculatePath(std::vector<std::vector<int>>& grid, int r, int c) const
    {
        int min_right = -1;
        int min_down  = -1;
        if (r < grid.size() - 1) {
            min_right = grid[r + 1][c];
        }
        if (c < grid[r].size() - 1) {
            min_down = grid[r][c + 1];
        }

        int min = 0;
        if (min_right >= 0 && min_down >= 0) {
            min = std::min(min_right, min_down);
        } else if (min_right >= 0) {
            min = min_right;
        } else if (min_down >= 0) {
            min = min_down;
        }
        grid[r][c] += min;
    }
};


template <typename Solution>
void test()
{
    const int MAX = std::numeric_limits<int>::max();

    std::vector<std::vector<int>> grid;

    ASSERT(( Solution().run(grid = {}) == 0 ));
    ASSERT(( Solution().run(grid = {{}}) == 0 ));
    ASSERT(( Solution().run(grid = {{0}}) == 0 ));
    ASSERT(( Solution().run(grid = {{1}}) == 1 ));
    ASSERT(( Solution().run(grid = {{MAX}}) == MAX ));

    ASSERT(( Solution().run(grid = {{0, 0}}) == 0 ));
    ASSERT(( Solution().run(grid = {{0, 1}}) == 1 ));
    ASSERT(( Solution().run(grid = {{1, 0}}) == 1 ));
    ASSERT(( Solution().run(grid = {{1, 2}}) == 3 ));

    ASSERT(( Solution().run(grid = {{0, 0},
                                    {0, 0}}) == 0 ));

    ASSERT(( Solution().run(grid = {{1, 1},
                                    {1, 1}}) == 3 ));

    ASSERT(( Solution().run(grid = {{1, 3},
                                    {2, 1}}) == 4 ));

    ASSERT(( Solution().run(grid =
             {{1, 1, 1},
              {1, 2, 1},
              {1, 1, 1}}) == 5 ));

    ASSERT(( Solution().run(grid =
             {{1, 2, 3},
              {2, 2, 3},
              {4, 2, 1}}) == 8 ));

    ASSERT(( Solution().run(grid =
             {{0, MAX, 0},
              {MAX, 0, 0},
              {0,   0, 0}}) == MAX ));

    ASSERT(( Solution().run(grid =
             {{MAX / 2, MAX / 2, 0},
              {MAX / 2, 0, 0},
              {0,       0, 0}}) == (MAX / 2) * 2 ));

    ASSERT(( Solution().run(grid =
             {{1, 1, 2, 1, 1},
              {1, 1, 1, 2, 1},
              {1, 1, 1, 1, 1},
              {1, 2, 1, 1, 1},
              {1, 1, 2, 1, 1}}) == 9 ));
}

int main()
{
    test<Solution1>();
    test<Solution2>();
    return 0;
}

}
