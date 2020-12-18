// Problem: https://leetcode.com/problems/pacific-atlantic-water-flow/

#include "utils.h"
#include <vector>
#include <deque>

namespace pacific_atlantic_water_flow {

class Solution {
public:
    // Note: The matrix must be rectangular. If the matrix is empty, the result
    // is also empty. If the matrix has only one row or column, returns all the
    // cells.
    //
    // Note: The original problem allows the non-negative values only. This
    // solution accepts any values.
    //
    // Time: O(n * m), Space: O(n * m), n - number of rows, m - number of columns
    //
    std::vector<std::vector<int>> run(const std::vector<std::vector<int>>& matrix)
    {
        // Idea:
        // We need to find the cells from which we can go down to the both
        // oceans. Any cell that is connected with the ocean via downward path
        // can be found by going upward from that ocean. Therefore, for each
        // ocean, we can go upward to all possible directions and mark every
        // reached cell as connected with that ocean. Then the cells connected
        // with both oceans are what we need.
        //
        // Note: Going upward can be implemented via the search like BFS.
        // Although the search should run twice, for the first and the second
        // oceans, both runs can go simultaneously if they do not erase the
        // results of each other.

        if (!matrix.size() || !matrix[0].size())
            return {};

        const int row_count = matrix.size();
        const int col_count = matrix[0].size();

        // Flow upward from the oceans
        Flow flow(row_count, col_count);

        for (int c = 0; c < col_count; c++) {
            flow.values[0][c]             |= OceanType_Pacific;
            flow.values[row_count - 1][c] |= OceanType_Atlantic;
            flow.next_cells.push_back({0, c});
            flow.next_cells.push_back({row_count - 1, c});
        }

        for (int r = 0; r < row_count; r++) {
            flow.values[r][0]             |= OceanType_Pacific;
            flow.values[r][col_count - 1] |= OceanType_Atlantic;
            flow.next_cells.push_back({r, 0});
            flow.next_cells.push_back({r, col_count - 1});
        }

        flowUpward(flow, matrix);

        // Collect the required cells
        std::vector<std::vector<int>> result;
        const int                     result_flags = OceanType_All;

        for (int r = 0; r < row_count; r++) {
            for (int c = 0; c < col_count; c++) {
                if ((flow.values[r][c] & result_flags) == result_flags)
                    result.push_back({r, c});
            }
        }

        return result;
    }

private:
    enum OceanType
    {
        OceanType_Atlantic = 1,
        OceanType_Pacific  = 2,
        OceanType_All      = OceanType_Atlantic | OceanType_Pacific
    };

    struct Cell
    {
        int r;
        int c;
    };

    struct Flow
    {
        Flow(int row_count, int col_count)
            : values(row_count, std::vector<int>(col_count, 0)) {}

        std::vector<std::vector<int>>  values;
        std::deque<Cell>               next_cells;
    };

    typedef std::vector<std::vector<int>> HeightMatrix;

    void flowUpward(Flow& flow, const HeightMatrix& heights) const
    {
        while (flow.next_cells.size()) {
            const Cell& cell = flow.next_cells.front();

            flowUpwardFrom(flow, heights, cell, -1, 0);
            flowUpwardFrom(flow, heights, cell, +1, 0);
            flowUpwardFrom(flow, heights, cell, 0, -1);
            flowUpwardFrom(flow, heights, cell, 0, +1);

            flow.next_cells.pop_front();
        }
    }

    void flowUpwardFrom(Flow& flow, const HeightMatrix& heights, const Cell& cell, int dr, int dc) const
    {
        const int r = cell.r + dr;
        const int c = cell.c + dc;

        if (0 <= r && r < heights.size() && 0 <= c && c < heights[0].size()) {
            const int cell_height = heights[cell.r][cell.c];

            if (heights[r][c] >= cell_height) {
                const int cell_value  = flow.values[cell.r][cell.c];

                if ((flow.values[r][c] & cell_value) != cell_value) {
                    flow.values[r][c] |= cell_value;
                    flow.next_cells.push_back({r, c});
                }
            }
        }
    }
};


int main()
{
    const auto test = [](const std::vector<std::vector<int>>& matrix,
                         const std::vector<std::vector<int>>& expected)
    {
        ASSERT_EX( compare_sets(Solution().run(matrix), expected),
            to_string(matrix) + " -> " + to_string(expected) );
    };

    test({}, {});
    test({{}}, {});

    // 1x1
    test({{0}}, {{0, 0}});
    test({{1}}, {{0, 0}});
    
    // 1x2
    test({{0, 0}}, {{0, 0}, {0, 1}});

    test({{0},
          {0}},
         {{0, 0}, {1, 0}});
    
    test({{0, 1}}, {{0, 0}, {0, 1}});
    test({{1, 0}}, {{0, 0}, {0, 1}});

    test({{0},
          {1}},
         {{0, 0}, {1, 0}});

    test({{1},
          {0}},
         {{0, 0}, {1, 0}});

    // 2x2
    test({{0, 0},
          {0, 0}},
         {{0, 0}, {0, 1}, {1, 0}, {1, 1}});

    test({{0, 1},
          {1, 0}},
         {{0, 1}, {1, 0}});

    test({{1, 0},
          {0, 1}},
         {{0, 0}, {0, 1}, {1, 0}, {1, 1}});

    test({{1, 1},
          {1, 0}},
         {{0, 0}, {0, 1}, {1, 0}});

    test({{1, 1},
          {0, 1}},
         {{0, 0}, {0, 1}, {1, 0}, {1, 1}});

    test({{0, 1},
          {1, 1}},
         {{0, 1}, {1, 0}, {1, 1}});

    test({{1, 0},
          {1, 1}},
         {{0, 0}, {0, 1}, {1, 0}, {1, 1}});

    test({{1, 1},
          {1, 1}},
         {{0, 0}, {0, 1}, {1, 0}, {1, 1}});

    // 2x3
    test({{0, 1, 0},
          {1, 0, 1}},
         {{0, 1}, {0, 2}, {1, 0}, {1, 2}});

    test({{1, 0},
          {0, 1},
          {1, 0}},
         {{0, 0}, {0, 1}, {1, 1}, {2, 0}});

    // 3x3
    test({{1, 1, 2},
          {1, 0, 1},
          {2, 1, 1}},
         {{0, 2}, {2, 0}});

    test({{1, 1, 2},
          {1, 2, 1},
          {2, 1, 1}},
         {{0, 2}, {1, 1}, {2, 0}});

    test({{1, 1, 2},
          {1, 1, 1},
          {2, 1, 1}},
         {{0, 0}, {0, 1}, {0, 2},
          {1, 0}, {1, 1}, {1, 2},
          {2, 0}, {2, 1}, {2, 2}});

    test({{0, 1, 0},
          {1, 0, 1},
          {0, 1, 0}},
         {        {0, 1}, {0, 2},
          {1, 0},         {1, 2},
          {2, 0}, {2, 1}        });

    // 5x5
    test({{1, 4, 7, 3, 4},
          {4, 3, 6, 2, 3},
          {3, 2, 5, 4, 5},
          {4, 3, 6, 3, 1},
          {5, 4, 3, 2, 1}},
         {{0, 2}, {0, 4}, {4, 0}});

    // Maximum value
    const int MAX = std::numeric_limits<int>::max();

    test({{MAX-1, MAX-2, MAX},
          {MAX,   MAX-3, MAX-4},
          {MAX,   MAX-1, MAX}},
         {{0, 0}, {0, 1}, {0, 2},
          {1, 0},
          {2, 0}                });

    return 0;
}

}
