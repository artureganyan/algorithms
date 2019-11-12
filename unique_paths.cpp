// Problem: https://leetcode.com/problems/unique-paths/

#include <vector>
#include "utils.h"

namespace unique_paths {

// Note: All solutions return int as required by the initial problem, but
// 32-bit signed integer can hold the result for at most the 17x18 grid.


// The straightforward solution, recursively counting paths from the start
// cell to the target

class Solution1 {
public:
    // Time: O(C(n + m, m)), Space: O(n + m), Recursion depth < n + m
    // n - number of rows, m - number of columns,
    // C(n + m, m) - binomial coefficient (n + m)! / (m! * n!)
    //
    // Note: Time complexity is estimated by the number of paths, which is
    // the binomial coefficient as shown in the Solution3. Space complexity
    // depends on the recursion depth, which is determined by the path
    // length n + m - 1.
    //
    int run(int rows_count, int cols_count)
    {
        if (rows_count <= 0 || cols_count <= 0)
            return 0;

        if (rows_count == 1 || cols_count == 1)
            return 1;

        // Go right + go down
        return run(rows_count, cols_count - 1) +
               run(rows_count - 1, cols_count);
    }
};


// Non-recursive solution, counting paths from the target cell to the start

class Solution2 {
public:
    // Time: O(n * m), Space: O(n * m), n - number of rows, m - number of columns
    //
    // Note: It calculates all cells, but 1) we need only the (0, 0), 2) the
    // results are symmetrical: count(n, m) == count(m, n). So it could be
    // optimized.
    //
    int run(int rows_count, int cols_count)
    {
        if (rows_count <= 0 || cols_count <= 0)
            return 0;

        // Idea:
        // Count paths in reverse order, moving the start from the target cell to (0, 0):
        // 1. Put the start to the target cell (t = rows_count - 1, l = cols_count - 1).
        //    Then there is only 1 path, i.e. the cell itself.
        // 2. Go to the outer rectangle, which top-left corner is (t - 1, l - 1) if this
        //    is a square.
        // 3. Calculate counts for the top and left borders of the outer rectangle. For
        //    each cell (r, c), the count is a sum of counts for (r + 1, c) and (r, c + 1),
        //    because we can step only right or down.
        // 4. Repeat 2 and 3 until we reach (0, 0).
        //
        // 0 0 0    0 0 0    6 3 1
        // 0 0 0 -> 0 2 1 -> 3 2 1
        // 0 0 1    0 1 1    1 1 1

        // Allocate one more row and column with zeros to avoid excessive range checks
        std::vector<std::vector<int>> count(rows_count + 1, std::vector<int>(cols_count + 1, 0));

        // Start from the target cell
        int t = rows_count - 1;
        int l = cols_count - 1;
        int b = t;
        int r = l;

        count[t][l] = 1;

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
                calculateCount(count, ri, ci);
            }
            // Count left border of the outer rectangle
            for (int ri = b, ci = l; ri > t; ri--) {
                calculateCount(count, ri, ci);
            }
            // Count top-left corner of the outer rectangle
            calculateCount(count, t, l);

        } while (t != 0 || l != 0);

        return count[0][0];
    }

private:
    inline void calculateCount(std::vector<std::vector<int>>& count, int r, int c) const
    {
        int& cell = count[r][c];
        cell += count[r + 1][c];
        cell += count[r][c + 1];
    }
};


// This solution just calculates the binomial coefficient

class Solution3 {
public:
    // Time: O(n - m), Space: O(1), n - number of rows, m - number of columns
    //
    // Warning: This implementation uses floating-point calculation and produces
    // rounding error for large grids (e.g. test for 15x18 returned 1 path less
    // than expected).
    //
    int run(int rows_count, int cols_count)
    {
        // Idea:
        // If we look at the numbers of paths from each cell to the right-bottom
        // corner, we may notice that they are binomial coefficients
        // C(n, m) = n! / (m! * (n - m)!), written by diagonals:
        //
        // Numbers of paths.     n         n, excluding
        // They are C(n, m).               trivial cases
        //
        // 70 35 15 5 1      8 7 6 5 4      8 7 6 5 *
        // 35 20 10 4 1      7 6 5 4 3      7 6 5 4 *
        // 15 10  6 3 1      6 5 4 3 2      6 5 4 3 *
        //  5  4  3 2 1      5 4 3 2 1      5 4 3 2 *
        //  1  1  1 1 1      4 3 2 1 1      * * * * *
        //
        // For all cells except the right and bottom borders (which correspond
        // to trivial cases nx1 and 1xn):
        // n = (rows_count - 1) + (cols_count - 1)
        // m = (cols_count - 1)
        //
        // Finally, to reduce the number of multiplications, we can simplify
        // C(n, m) to ((m+1) * (m+2) * ... * n) / (n - m)! and swap rows_count
        // and cols_count if rows_count > cols_count (the result is the same,
        // but both the dividend and divisor are lesser).

        if (rows_count <= 0 || cols_count <= 0)
            return 0;

        if (rows_count == 1 || cols_count == 1)
            return 1;

        if (rows_count > cols_count)
            std::swap(rows_count, cols_count);

        // Calculate C(n, m) = n! / (m! * (n - m)!)
        // n, m
        const int n = rows_count - 1 + cols_count - 1;
        const int m = cols_count - 1;

        // n! / m! = (m + 1) * (m + 2) * ... * n
        double nf_div_mf = 1;
        for (int i = m + 1; i <= n; i++)
            nf_div_mf *= i;

        // (n - m)!
        double n_minus_m_f = 1;
        for (int i = 2; i <= (n - m); i++)
            n_minus_m_f *= i;

        // C(n, m)
        const double C_n_m = nf_div_mf / n_minus_m_f;
        return static_cast<int>(C_n_m);
    }
};


template <typename Solution>
void test(bool large_grid = false)
{
    auto test_symmetrical = [](int rows_count, int cols_count, int expected)
    {
        if (Solution().run(rows_count, cols_count) != expected)
            return false;

        if (rows_count != cols_count) {
            if (Solution().run(cols_count, rows_count) != expected)
                return false;
        }
        return true;
    };

    ASSERT( test_symmetrical(-1, -1, 0) );
    ASSERT( test_symmetrical(-1, 0, 0) );
    ASSERT( test_symmetrical(-1, 1, 0) );
    ASSERT( test_symmetrical(0, 0, 0) );
    ASSERT( test_symmetrical(1, 0, 0) );

    ASSERT( test_symmetrical(1, 1, 1) );
    ASSERT( test_symmetrical(1, 2, 1) );
    ASSERT( test_symmetrical(1, 3, 1) );
    ASSERT( test_symmetrical(2, 2, 2) );
    ASSERT( test_symmetrical(2, 3, 3) );
    ASSERT( test_symmetrical(2, 4, 4) );
    ASSERT( test_symmetrical(2, 5, 5) );
    ASSERT( test_symmetrical(3, 3, 6) );
    ASSERT( test_symmetrical(3, 4, 10) );
    ASSERT( test_symmetrical(3, 5, 15) );
    ASSERT( test_symmetrical(4, 4, 20) );
    ASSERT( test_symmetrical(4, 5, 35) );
    ASSERT( test_symmetrical(5, 5, 70) );
    ASSERT( test_symmetrical(10, 10, 48620) );

    if (large_grid)
        ASSERT( test_symmetrical(17, 18, 1166803110) );
}

int main()
{
    // Do not test large grid because of time complexity
    test<Solution1>();

    test<Solution2>(true);

    // Do not test large grid because of rounding error
    test<Solution3>();

    return 0;
}

}
