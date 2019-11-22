// Problem: https://leetcode.com/problems/word-search/

#include <vector>
#include "utils.h"

namespace word_search {

class Solution {
public:
    bool run(const std::vector<std::vector<char>>& grid, const std::string& word)
    {
        if (!grid.size() || !grid[0].size() || !word.size())
            return false;

        const int rows_count = grid.size();
        const int cols_count = grid[0].size();

        if (word.size() > rows_count * cols_count)
            return false;

        std::vector<std::vector<bool>> visited_cells(rows_count, std::vector<bool>(cols_count, 0));

        for (int r = 0; r < rows_count; r++) {
            for (int c = 0; c < cols_count; c++) {
                if (search(grid, r, c, word, 0, visited_cells))
                    return true;
            }
        }
        return false;
    }

private:
    // Note: This implementation always goes by 4 directions from the each cell,
    // while it needs at most 3. This can be optimized by placing the check of
    // the borders and visited cells before the each recursive call.
    //
    bool search(const std::vector<std::vector<char>>& grid, int r, int c,
        const std::string& word, int word_begin, std::vector<std::vector<bool>>& visited_cells) const
    {
        if (word_begin == word.size())
            return true;

        if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size() || visited_cells[r][c])
            return false;

        if (grid[r][c] != word[word_begin])
            return false;

        visited_cells[r][c] = 1;

        const bool found =
            search(grid, r + 1, c, word, word_begin + 1, visited_cells) ||
            search(grid, r - 1, c, word, word_begin + 1, visited_cells) ||
            search(grid, r, c + 1, word, word_begin + 1, visited_cells) ||
            search(grid, r, c - 1, word, word_begin + 1, visited_cells);

        visited_cells[r][c] = 0;

        return found;
    }
};


int main()
{
    ASSERT(( Solution().run({}, std::string()) == false ));
    ASSERT(( Solution().run({}, "test") == false ));
    ASSERT(( Solution().run({{}}, std::string()) == false ));
    ASSERT(( Solution().run({{}}, "test") == false ));

    ASSERT(( Solution().run({{'a'}}, "") == false ));
    ASSERT(( Solution().run({{'a'}}, "a") == true ));
    ASSERT(( Solution().run({{'a'}}, "b") == false ));
    ASSERT(( Solution().run({{'a'}}, "aa") == false ));

    ASSERT(( Solution().run({{'a', 'b'}}, "ab") == true ));
    ASSERT(( Solution().run({{'a', 'b'}}, "ba") == true ));

    ASSERT(( Solution().run(
             {{'a', 'a'},
              {'a', 'a'}}, "a") == true ));

    ASSERT(( Solution().run(
             {{'a', 'a'},
              {'a', 'a'}}, "aa") == true ));

    ASSERT(( Solution().run(
             {{'a', 'a'},
              {'a', 'a'}}, "aaa") == true ));

    ASSERT(( Solution().run(
             {{'a', 'a'},
              {'a', 'a'}}, "aaaa") == true ));

    ASSERT(( Solution().run(
             {{'a', 'a'},
              {'a', 'a'}}, "aaaaa") == false ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "abcd") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "adcb") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "badc") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "bcda") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "cbad") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "cdab") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "dabc") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "dcba") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "abcda") == false ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "abd") == false ));

    ASSERT(( Solution().run(
             {{'a', 'b'},
              {'d', 'c'}}, "ac") == false ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "abcfedghi") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "abcfihgde") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "abedghifc") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "ebcfihgda") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "efihgdabc") == true ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "abefcb") == false ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "abcd") == false ));

    ASSERT(( Solution().run(
             {{'a', 'b', 'c'},
              {'d', 'e', 'f'},
              {'g', 'h', 'i'}}, "abcfeb") == false ));

    return 0;
}

}
