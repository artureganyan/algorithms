// Problem: https://leetcode.com/problems/game-of-life/

#include <vector>
#include "utils.h"

namespace game_of_life {

class Solution {
public:
    // Note: The grid must be a rectangle and must contain only the following
    // values: 0 for the dead cell, 1 for the live one.
    //
    // Time: O(n * m), Space: O(1), n - number of rows, m - number of columns
    //
    void run(std::vector<std::vector<int>>& grid)
    {
        if (!grid.size() || !grid[0].size())
            return;

        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[r].size(); c++) {
                const int live_neighbours = countLiveNeighbors(grid, r, c);

                int& cell = grid[r][c];
                if (isLiveState(cell)) {
                    if (live_neighbours < 2 || live_neighbours > 3)
                        cell = State_Dying;
                } else {
                    if (live_neighbours == 3)
                        cell = State_Reviving;
                }
            }
        }

        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[r].size(); c++) {
                int& cell = grid[r][c];
                if (cell == State_Dying) {
                    cell = State_Dead;
                } else if (cell == State_Reviving) {
                    cell = State_Live;
                }
            }
        }
    }

private:
    enum State
    {
        State_Dying    = -1,
        State_Dead     = 0,
        State_Live     = 1,
        State_Reviving = 2
    };

    inline static bool isLiveState(int state)
    {
        return state == State_Live || state == State_Dying;
    }

    int countLiveNeighbors(const std::vector<std::vector<int>>& grid, int r, int c) const
    {
        const int r1 = std::max(r - 1, 0);
        const int r2 = std::min(r + 1, (int) grid.size() - 1);
        const int c1 = std::max(c - 1, 0);
        const int c2 = std::min(c + 1, (int) grid[r].size() - 1);

        int count_live = 0;
        for (int ri = r1; ri <= r2; ri++) {
            for (int ci = c1; ci <= c2; ci++) {
                if (isLiveState(grid[ri][ci]))
                    count_live++;
            }
        }
        if (isLiveState(grid[r][c]))
            count_live--;

        return count_live;
    }
};


typedef std::vector<std::vector<int>> Grid;

void test(const Grid& source, const Grid& expected)
{
    auto result = source;

    Solution().run(result);

    ASSERT_EX( result == expected,
        to_string(source) + " -> " + to_string(expected) );
}

void test(const Grid& source, const Grid& expected, int rotates)
{
    for (int r = 0; r < rotates; r++) {
        const auto source_rotated   = rotate(source, r);
        const auto expected_rotated = rotate(expected, r);
        test(source_rotated, expected_rotated);
    }
}

int main()
{
    test({}, {});
    test({{}}, {{}});

    test({{0}}, {{0}});
    test({{1}}, {{0}});

    test({{0, 0}}, {{0, 0}}, 2);
    test({{1, 0}}, {{0, 0}}, 2);
    test({{0, 1}}, {{0, 0}}, 2);
    test({{1, 1}}, {{0, 0}}, 2);

    test(
        {{1, 0},
         {0, 0}},

        {{0, 0},
         {0, 0}}, 4
    );

    test(
        {{1, 1},
         {0, 0}},

        {{0, 0},
         {0, 0}}, 4
    );

    test(
        {{1, 0},
         {0, 1}},

        {{0, 0},
         {0, 0}}, 2
    );

    test(
        {{1, 1},
         {1, 0}},

        {{1, 1},
         {1, 1}}, 4
    );

    test(
        {{1, 1},
         {1, 1}},

        {{1, 1},
         {1, 1}}, 1
    );

    test(
        {{0, 0, 0},
         {0, 1, 0},
         {0, 0, 0}},

        {{0, 0, 0},
         {0, 0, 0},
         {0, 0, 0}}, 1
    );

    test(
        {{1, 0, 1},
         {0, 1, 0},
         {1, 0, 1}},

        {{0, 1, 0},
         {1, 0, 1},
         {0, 1, 0}}, 1
    );

    test(
        {{0, 1, 0},
         {1, 1, 1},
         {0, 1, 0}},

        {{1, 1, 1},
         {1, 0, 1},
         {1, 1, 1}}, 1
    );

    test(
        {{1, 0, 1},
         {1, 1, 1},
         {1, 0, 1}},

        {{1, 0, 1},
         {1, 0, 1},
         {1, 0, 1}}, 2
    );

    test(
        {{1, 1, 1},
         {1, 1, 1},
         {1, 0, 1}},

        {{1, 0, 1},
         {0, 0, 0},
         {1, 0, 1}}, 4
    );

    test(
        {{1, 1, 1},
         {1, 1, 1},
         {1, 1, 1}},

        {{1, 0, 1},
         {0, 0, 0},
         {1, 0, 1}}, 1
    );

    test(
        {{1, 0, 0, 1, 1},
         {0, 0, 1, 0, 1},
         {0, 1, 1, 1, 0},
         {1, 0, 1, 0, 0}},

        {{0, 0, 0, 1, 1},
         {0, 0, 0, 0, 1},
         {0, 0, 0, 0, 0},
         {0, 0, 1, 1, 0}}, 1
    );

    return 0;
}

}
