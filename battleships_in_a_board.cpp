// Problem: https://leetcode.com/problems/battleships-in-a-board/

#include "utils.h"
#include <vector>

namespace battleships_in_a_board {

class Solution {
public:
    // Note: The board must be rectangular and contain only the following
    // characters: 'X' for ship, '.' for empty cell. If the board is empty,
    // returns 0.
    //
    // Note: The original problem allows only horizontal (1 x t), vertical
    // (t x 1) and one-cell (1 x 1) ships. This solutions allows any ships.
    //
    // Time: O(n * m), Space: O(1), n - number of rows, m - number of columns 
    //
    int run(const std::vector<std::vector<char>>& board)
    {
        // Idea:
        // Let's say we go cell by cell (in some order), and the current cell
        // (r, c) is a ship. We need to count the ships, so the only thing we
        // need to know is if we have already counted the current ship: if not,
        // we increment the counter, otherwise just skip this cell. Let's say
        // we go from the top-left corner to the bottom-right, row by row. Then
        // the current ship is already counted on one of the previous steps if
        // one of the previous cells (r-1, c) or (r, c-1) is also a ship. And
        // if we would go in different order, we would just check different
        // previous cells.

        if (!board.size() || !board[0].size())
            return 0;

        const int row_count = board.size();
        const int col_count = board[0].size();

        int count = 0;

        for (int r = 0; r < row_count; r++) {
            for (int c = 0; c < col_count; c++) {
                // This is not a ship
                if (board[r][c] == SPACE)
                    continue;
                // This is a ship, check if it is already counted
                if ((r > 0 && board[r - 1][c] == SHIP) ||
                    (c > 0 && board[r][c - 1] == SHIP))
                    continue;
                // It is not counted yet
                count++;
            }
        }

        return count;
    }

    static const char SHIP  = 'X';
    static const char SPACE = '.';
};

int main()
{
    const auto test = [](const std::vector<std::vector<char>>& board, int expected, int rotates = 1)
    {
        for (int r = 0; r < rotates; r++) {
            const auto board_rotated = rotate(board);

            ASSERT_EX( Solution().run(board_rotated) == expected,
                to_string(board_rotated) + " -> " + to_string(expected) );
        }
    };

    const char X = Solution::SHIP;
    const char O = Solution::SPACE;

    test({}, 0);
    test({{}}, 0);

    test({{O}}, 0);
    test({{X}}, 1);

    test({{O, O}}, 0, 2);
    test({{X, O}}, 1, 4);
    test({{X, X}}, 1, 2);

    test({{O, O, O}}, 0, 2);
    test({{X, O, O}}, 1, 4);
    test({{O, X, O}}, 1, 2);
    test({{X, X, O}}, 1, 4);
    test({{X, O, X}}, 2, 2);
    test({{X, X, X}}, 1, 2);

    test({{O, O},
          {O, O}}, 0);

    test({{X, O},
          {O, O}}, 1, 4);

    test({{X, X},
          {O, O}}, 1, 4);

    test({{X, O},
          {O, X}}, 2, 2);

    test({{O, O, O},
          {O, O, O}}, 0, 2);

    test({{X, O, X},
          {O, O, X}}, 2, 4);

    test({{X, O, O},
          {O, X, X}}, 2, 4);

    test({{X, O, X},
          {O, X, O}}, 3, 2);

    test({{O, O, O},
          {O, O, O},
          {O, O, O}}, 0, 1);

    test({{O, O, O},
          {O, X, O},
          {O, O, O}}, 1, 1);

    test({{X, O, O},
          {O, X, O},
          {O, O, X}}, 3, 2);

    test({{X, O, X},
          {O, X, O},
          {X, O, X}}, 5, 1);

    test({{X, O, X},
          {O, O, X},
          {X, O, X}}, 3, 2);

    test({{X, O, O},
          {X, O, X},
          {O, O, X}}, 2, 2);

    test({{X, O, O, X, X},
          {O, X, O, O, O},
          {O, X, O, X, O},
          {O, O, O, X, O},
          {X, X, O, X, O}}, 5, 2);

    return 0;
}

}
