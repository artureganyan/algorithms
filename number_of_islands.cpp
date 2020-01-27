// Problem: https://leetcode.com/problems/number-of-islands/

#include <vector>
#include "utils.h"

namespace number_of_islands {

class Solution {
public:
    // Note: The grid must be a rectangle and contain only '0' and '1'.
    //
    // Time: O(w * h), Space: O(w * h),
    // w - width of the grid, h - height of the grid
    //
    // Note: This solution uses the depth-first search (DFS) to iterate over
    // each cell of the "island", so that for the grid of all '1' it will
    // require O(w * h) space.
    //
    int run(std::vector<std::vector<char>>& grid)
    {
        if (!grid.size() || !grid[0].size())
            return 0;

        int result = 0;

        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[r].size(); c++) {
                if (grid[r][c] == '1') {
                    updateConnectedCells(grid, r, c, '1', '0');
                    result++;
                }
            }
        }

        return result;
    }

private:
    static void updateConnectedCells(std::vector<std::vector<char>>& grid, int r, int c,
        char from_value, char to_value)
    {
        struct Cell
        {
            int r;
            int c;
        };

        std::vector<Cell> connected_cells = {{r, c}};

        while (connected_cells.size()) {
            const Cell cell = connected_cells.back();
            connected_cells.pop_back();

            if (cell.r < 0 || cell.r >= grid.size() ||
                cell.c < 0 || cell.c >= grid[cell.r].size())
                continue;

            auto& cell_value = grid[cell.r][cell.c];
            if (cell_value != from_value)
                continue;
            cell_value = to_value;

            connected_cells.push_back({cell.r - 1, cell.c});
            connected_cells.push_back({cell.r + 1, cell.c});
            connected_cells.push_back({cell.r, cell.c - 1});
            connected_cells.push_back({cell.r, cell.c + 1});
        }
    }
};


typedef std::vector<std::vector<char>> Grid;

void test(const Grid& source, int expected, int rotate_count = 1)
{
    for (int i = 0; i < rotate_count; i++) {
        Grid source_rotated = rotate(source);

        ASSERT_EX( Solution().run(source_rotated) == expected,
            to_string(source) + " -> " + to_string(expected) );
    }
}

int main()
{
    test({}, 0);
    test({{}}, 0);

    test({{'0'}}, 0);
    test({{'1'}}, 1);

    test({{'0', '0'}}, 0, 2);
    test({{'1', '0'}}, 1, 4);
    test({{'1', '1'}}, 1, 2);

    test({{'0', '0'},
          {'0', '0'}}, 0);

    test({{'0', '1'},
          {'0', '0'}}, 1, 4);

    test({{'0', '1'},
          {'1', '0'}}, 2, 2);

    test({{'1', '1'},
          {'1', '0'}}, 1, 4);

    test({{'1', '1'},
          {'1', '1'}}, 1);

    test({{'0', '1'},
          {'1', '0'},
          {'0', '1'}}, 3);

    test({{'1', '1'},
          {'1', '0'},
          {'0', '1'}}, 2);

    test({{'0', '0', '0'},
          {'0', '1', '0'},
          {'0', '0', '0'}}, 1);

    test({{'1', '1', '1'},
          {'1', '0', '1'},
          {'1', '0', '1'},
          {'1', '1', '1'}}, 1);

    test({{'1', '1', '1'},
          {'0', '0', '1'},
          {'1', '0', '0'},
          {'1', '1', '1'}}, 2);

    test({{'1', '1', '1'},
          {'0', '1', '0'},
          {'0', '1', '0'},
          {'1', '1', '1'}}, 1, 2);

    test({{'1', '0', '1', '1'},
          {'0', '1', '0', '0'},
          {'0', '1', '0', '1'},
          {'1', '0', '1', '1'},
          {'1', '1', '0', '1'},
          {'1', '1', '1', '0'}}, 5);

    return 0;
}

}
