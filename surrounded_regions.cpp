// Problem: https://leetcode.com/problems/surrounded-regions/

#include <deque>
#include "utils.h"

namespace surrounded_regions {

class Solution {
public:
    static const char X = 'X';
    static const char O = 'O';

    // Note: The grid must be a rectangle.
    //
    // Time: O(w * h), Space: O(w + h), w - width of the grid, h - height of the grid
    //
    void run(std::vector<std::vector<char>>& grid)
    {
        // Idea:
        // The cell O is surrounded by X if there is no path from this cell to
        // the grid's border. For an arbitrary O cell, we don't know in advance
        // if the path exists, but we know that the path ends at the border. So
        // it's simplier to start from the border, find all O cells that are
        // _not_ surrounded by X, and mark the rest O cells as surrounded.

        if (!grid.size() || !grid[0].size())
            return;

        // Find all not-surrounded cells and mark them as N
        std::deque<Cell> not_surrounded_cells;
        const char       N = 'N';

        // ... Find all O cells at the border
        for (int i = 0, t = 0, b = grid.size() - 1; i < grid[0].size(); i++) {
            updateCell(grid, not_surrounded_cells, t, i, O, N);
            updateCell(grid, not_surrounded_cells, b, i, O, N);
        }
        for (int i = 1, l = 0, r = grid[0].size() - 1; i < grid.size() - 1; i++) {
            updateCell(grid, not_surrounded_cells, i, l, O, N);
            updateCell(grid, not_surrounded_cells, i, r, O, N);
        }

        // ... Find all O cells connected with the border via breadth-first search (BFS)
        while (not_surrounded_cells.size()) {
            const Cell& cell = not_surrounded_cells.front();

            updateCell(grid, not_surrounded_cells, cell.r - 1, cell.c, O, N);
            updateCell(grid, not_surrounded_cells, cell.r + 1, cell.c, O, N);
            updateCell(grid, not_surrounded_cells, cell.r, cell.c - 1, O, N);
            updateCell(grid, not_surrounded_cells, cell.r, cell.c + 1, O, N);

            not_surrounded_cells.pop_front();
        }

        // Mark the rest O cells as surrounded, and restore values for not-surrounded cells
        for (int r = 0; r < grid.size(); r++) {
            auto& row = grid[r];
            for (int c = 0; c < row.size(); c++) {
                auto& cell = row[c];
                if (cell == O) {
                    cell = X;
                } else if (cell == N) {
                    cell = O;
                }
            }
        }
    }

private:
    struct Cell
    {
        int r;
        int c;
    };

    inline void updateCell(std::vector<std::vector<char>>& grid, std::deque<Cell>& updated_cells,
        int r, int c, char prev_value, char new_value) const
    {
        if (r < 0 || r >= grid.size() || c < 0 || c >= grid[r].size())
            return;

        auto& cell = grid[r][c];
        if (cell == prev_value) {
            cell = new_value;
            updated_cells.push_back({r, c});
        }
    }
};


typedef std::vector<std::vector<char>> Grid;

void test(const Grid& source, const Grid& expected)
{
    Grid result = source;

    Solution().run(result);

    ASSERT_EX( result == expected, to_string(source) + " -> " + to_string(expected) );
}

void test_rotated(const Grid& source, const Grid& expected, int count = 1)
{
    for (int c = 0; c < count; c++) {
        Grid source_rotated   = rotate(source, c);
        Grid expected_rotated = rotate(expected, c);
        test(source_rotated, expected_rotated);
    }
}

int main()
{
    const auto X = Solution::X;
    const auto O = Solution::O;

    test({},   {});
    test({{}}, {{}});

    test({{X}}, {{X}});
    test({{O}}, {{O}});

    test_rotated({{X, X}}, {{X, X}}, 2);
    test_rotated({{X, O}}, {{X, O}}, 4);
    test_rotated({{O, O}}, {{O, O}}, 2);

    test_rotated({{X, O, X}}, {{X, O, X}}, 2);
    test_rotated({{O, X, O}}, {{O, X, O}}, 2);

    test_rotated({{X, X, X},
                  {X, O, X}},

                 {{X, X, X},
                  {X, O, X}}, 4);

    test_rotated({{X, X, X},
                  {X, O, X},
                  {X, X, X}},

                 {{X, X, X},
                  {X, X, X},
                  {X, X, X}}, 1);

    test_rotated({{X, O, X},
                  {X, O, X},
                  {X, X, X}},

                 {{X, O, X},
                  {X, O, X},
                  {X, X, X}}, 4);

    test_rotated({{X, X, X},
                  {X, O, X},
                  {O, O, X},
                  {X, X, X}},

                 {{X, X, X},
                  {X, O, X},
                  {O, O, X},
                  {X, X, X}}, 4);

    test_rotated({{X, X, X},
                  {X, X, O},
                  {X, O, X},
                  {X, X, X}},

                 {{X, X, X},
                  {X, X, O},
                  {X, X, X},
                  {X, X, X}}, 4);

    test_rotated({{O, X, X, X, X, X, X, X, X},
                  {X, O, O, O, X, X, O, X, X},
                  {X, O, X, X, O, O, O, O, X},
                  {X, X, O, O, X, X, O, X, X},
                  {X, O, X, X, O, X, X, O, O},
                  {X, O, X, X, X, X, X, X, X}},

                 {{O, X, X, X, X, X, X, X, X},
                  {X, X, X, X, X, X, X, X, X},
                  {X, X, X, X, X, X, X, X, X},
                  {X, X, X, X, X, X, X, X, X},
                  {X, O, X, X, X, X, X, O, O},
                  {X, O, X, X, X, X, X, X, X}}, 2);

    return 0;
}

}
