// Desription: https://leetcode.com/problems/max-increase-to-keep-city-skyline/

#include <vector>
#include "utils.h"

namespace max_increase_to_keep_city_skyline {

class Solution
{
public:
    // Time: O(n^2), Space: O(1), n = width and height of grid
    //
    // grid must be a square
    //
    int run(const std::vector<std::vector<int>>& grid)
    {
        if (!grid.size())
            return 0;

        // Find skyline for x axis
        std::vector<int> x_skyline(grid.size(), -1);
        for (size_t x = 0; x < grid.size(); x++) {
            int &h_max = x_skyline[x];
            for (size_t y = 0; y < grid.size(); y++) {
                int h = grid[y][x];
                if (h_max < h)
                    h_max = h;
            }
        }

        // Find skyline for y axis
        std::vector<int> y_skyline(grid.size(), -1);
        for (size_t y = 0; y < grid.size(); y++) {
            int &h_max = y_skyline[y];
            for (size_t x = 0; x < grid.size(); x++) {
                int h = grid[y][x];
                if (h_max < h)
                    h_max = h;
            }
        }

        // Since skyline[i] is a maximum along line i, and must remain
        // unchanged, each grid cell at (y, x) can be increased up to
        // min(x_skyline[x], y_skyline[y])
        int max_increase = 0;
        for (size_t x = 0; x < grid.size(); x++) {
            for (size_t y = 0; y < grid.size(); y++) {
                int h_max = std::min(x_skyline[x], y_skyline[y]);
                int h = grid[y][x];
                if (h < h_max)
                    max_increase += h_max - h;
            }
        }

        return max_increase;
    }
};

int main()
{
    std::vector<std::vector<int>> grid = {
      // 9 4 8 7
        {3,0,8,4}, // 8
        {2,4,5,7}, // 7
        {9,2,6,3}, // 9
        {0,3,1,0}, // 3
    };
    ASSERT(Solution().run(grid) == 35);

    grid = {
      // 1 1 1 1
        {0,0,0,1}, // 1
        {0,0,1,0}, // 1
        {0,1,0,0}, // 1
        {1,0,0,0}, // 1
    };
    ASSERT(Solution().run(grid) == 12);

    grid = {
      // 0 0 0 0
        {0,0,0,0}, // 0
        {0,0,0,0}, // 0
        {0,0,0,0}, // 0
        {0,0,0,0}, // 0
    };
    ASSERT(Solution().run(grid) == 0);

    ASSERT(Solution().run({}) == 0);

    return 0;
}

}
