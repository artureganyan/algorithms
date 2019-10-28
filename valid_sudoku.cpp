// Problem: https://leetcode.com/problems/valid-sudoku/

#include <vector>
#include <bitset>
#include <array>
#include "utils.h"

namespace valid_sudoku {

class Solution {
public:
    const char EMPTY_CELL = '.';

    // Time: O(n), Space: O(n), n - number of cells
    //
    // Note: The grid must be of 9x9 cells
    //
    bool run(const std::vector<std::vector<char>>& grid)
    {
        const char* ERROR_WRONG_SIZE = "Grid must be of 9x9 cells";

        if (grid.size() != 9)
            throw std::runtime_error(ERROR_WRONG_SIZE);

        std::array<std::bitset<9>, 9> rows;
        std::array<std::bitset<9>, 9> columns;
        std::array<std::bitset<9>, 9> blocks;

        for (int r = 0; r < grid.size(); r++) {
            if (grid[r].size() != grid.size())
                throw std::runtime_error(ERROR_WRONG_SIZE);

            for (int c = 0; c < grid[r].size(); c++) {
                const char cell = grid[r][c];
                if (cell == EMPTY_CELL)
                    continue;

                const int value = cell - '1'; // Map 1..9 to 0..8
                if (value < 0 || value > 8)
                    return false;

                const int block = (r / 3) * 3 + (c / 3);
                if (rows[r].test(value) || columns[c].test(value) || blocks[block].test(value))
                    return false;

                rows[r].set(value);
                columns[c].set(value);
                blocks[block].set(value);
            }
        }

        return true;
    }

    // Time: O(n), Space: O(m)
    // n - number of cells,
    // m - number of valid characters (9 here, i.e. sqrt(n))
    //
    // Note: The grid must be of 9x9 cells
    //
    bool run2(const std::vector<std::vector<char>>& grid)
    {
        // Validate grid size
        const char* ERROR_WRONG_SIZE = "Grid must be of 9x9 cells";

        if (grid.size() != 9)
            throw std::runtime_error(ERROR_WRONG_SIZE);

        for (int r = 0; r < grid.size(); r++) {
            if (grid[r].size() != grid.size())
                throw std::runtime_error(ERROR_WRONG_SIZE);
        }

        // Validate grid values
        std::bitset<9> values;

        // ... By rows
        for (int r = 0; r < grid.size(); r++) {
            values.reset();
            for (int c = 0; c < grid.size(); c++) {
                if (!validate_cell(grid[r][c], values))
                    return false;
            }
        }

        // ... By columns
        for (int c = 0; c < grid.size(); c++) {
            values.reset();
            for (int r = 0; r < grid.size(); r++) {
                if (!validate_cell(grid[r][c], values))
                    return false;
            }
        }

        // ... By blocks
        for (int b = 0; b < 9; b++) {
            values.reset();
            for (int r0 = (b / 3) * 3, r = r0; r < r0 + 3; r++) {
                for (int c0 = (b % 3) * 3, c = c0; c < c0 + 3; c++) {
                    if (!validate_cell(grid[r][c], values))
                        return false;
                }
            }
        }

        return true;
    }

private:
    inline bool validate_cell(char cell, std::bitset<9>& values) const
    {
        if (cell == EMPTY_CELL)
            return true;

        const int v = cell - '1'; // Map 1..9 to 0..8
        if (v < 0 || v > 8 || values.test(v))
            return false;

        values.set(v);
        return true;
    }
};

int main()
{
    auto methods = {&Solution::run, &Solution::run2};

    for (auto method : methods) {
        ASSERT(( (Solution().*method)({
            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},

            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},

            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'}
            }) == true ));

        ASSERT(( (Solution().*method)({
            {'X','.','.', '.','.','.', '.','.','.'}, // Wrong character 'X'
            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},

            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},

            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','.'}
            }) == false ));

        ASSERT(( (Solution().*method)({
            {'1','.','.', '.','.','.', '.','.','.'},
            {'.','.','.', '1','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '1','.','.'},

            {'.','1','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','1','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','1','.'},

            {'.','.','1', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','1', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','1'}
            }) == true ));

        ASSERT(( (Solution().*method)({
            {'1','.','.', '.','.','.', '.','.','1'}, // The second '1' is wrong
            {'.','.','.', '1','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '1','.','.'},

            {'.','1','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','1','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','1','.'},

            {'.','.','1', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','1', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','1'}
            }) == false ));

        ASSERT(( (Solution().*method)({
            {'1','.','.', '.','.','.', '.','.','1'},
            {'.','.','.', '1','.','.', '.','.','.'},
            {'.','.','.', '.','.','.', '1','.','.'},

            {'.','1','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','1','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','1','.'},

            {'.','.','1', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','1', '.','.','.'},
            {'1','.','.', '.','.','.', '.','.','1'}  // The first '1' is wrong
            }) == false ));

        ASSERT(( (Solution().*method)({
            {'1','.','.', '.','.','.', '.','.','1'},
            {'.','.','.', '1','.','.', '.','.','.'},
            {'.','.','1', '.','.','.', '1','.','.'}, // The first '1' is wrong

            {'.','1','.', '.','.','.', '.','.','.'},
            {'.','.','.', '.','1','.', '.','.','.'},
            {'.','.','.', '.','.','.', '.','1','.'},

            {'.','.','1', '.','.','.', '.','.','.'},
            {'.','.','.', '.','.','1', '.','.','.'},
            {'.','.','.', '.','.','.', '.','.','1'}
            }) == false ));

        ASSERT(( (Solution().*method)({
            {'1','2','.', '3','4','.', '.','.','.'},
            {'.','.','.', '1','2','.', '3','4','.'},
            {'.','.','.', '.','.','.', '1','2','.'},

            {'5','6','.', '.','.','.', '.','.','.'},
            {'.','7','8', '5','6','.', '.','.','.'},
            {'.','.','.', '.','7','8', '.','5','6'},

            {'9','.','.', '8','.','.', '7','.','.'},
            {'.','8','.', '.','5','.', '.','3','.'},
            {'.','.','7', '.','.','4', '.','.','2'}
            }) == true ));
    }

    return 0;
}

}
