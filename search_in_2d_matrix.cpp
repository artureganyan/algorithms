// Problem: https://leetcode.com/problems/search-a-2d-matrix/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace search_in_2d_matrix {

// Uses standard algorithms

class Solution1 {
public:
    // Time: O(log(n)), Space: O(1), n - number of the matrix elements
    //
    // Note: Each row must be sorted in ascending order, and its first cell
    // must be greater than the last cell of the previous row.
    //
    // Note: Time complexity is log(rows_count) + log(columns_count) =
    // log(rows_count * columns_count) = log(n).
    //
    bool run(const std::vector<std::vector<int>>& matrix, int target)
    {
        if (!matrix.size() || !matrix[0].size())
            return false;

        auto row = std::upper_bound(matrix.begin(), matrix.end(), target,
            [](int value, const auto& row)
            {
                return value < row[0];
            });

        if (row == matrix.begin())
            return false;
        row--;

        return std::binary_search(row->begin(), row->end(), target);
    }
};


// Uses manually written binary search

class Solution2 {
public:
    // Time: O(log(n)), Space: O(1), n - number of the matrix elements
    //
    // Note: Each row must be sorted in ascending order, and its first cell
    // must be greater than the last cell of the previous row.
    //
    bool run(const std::vector<std::vector<int>>& matrix, int target)
    {
        if (!matrix.size() || !matrix[0].size())
            return false;

        const int rows_count = matrix.size();
        const int cols_count = matrix[0].size();

        int i1 = 0;
        int i2 = rows_count * cols_count - 1;

        while (i1 <= i2) {
            const int i = i1 + (i2 - i1) / 2;
            const int r = i / cols_count;
            const int c = i % cols_count;

            const int value = matrix[r][c];
            if (value < target) {
                i1 = i + 1;
            } else if (value > target) {
                i2 = i - 1;
            } else {
                return true;
            }
        }

        return false;
    }
};


// Uses the standard binary search with the custom iterator

class Solution3 {
public:
    // Time: O(log(n)), Space: O(1), n - number of the matrix elements
    //
    // Note: Each row must be sorted in ascending order, and its first cell
    // must be greater than the last cell of the previous row.
    //
    bool run(const std::vector<std::vector<int>>& matrix, int target)
    {
        if (!matrix.size() || !matrix[0].size())
            return false;

        const MatrixConstIterator begin(&matrix, 0, 0);
        const MatrixConstIterator end(&matrix, matrix.size(), 0);

        return std::binary_search(begin, end, target);
    }

private:
    typedef std::vector<std::vector<int>> Matrix;

    class MatrixConstIterator {
    public:
        typedef int difference_type;
        typedef int value_type;
        typedef int* pointer;
        typedef int& reference;
        typedef std::random_access_iterator_tag iterator_category;

        MatrixConstIterator()
            : matrix_(nullptr), r_(0), c_(0), columns_count_(0) {}

        MatrixConstIterator(const Matrix* matrix, size_t r, size_t c)
            : matrix_(matrix), r_(r), c_(c), columns_count_((*matrix)[0].size()) {}

        bool operator ==(const MatrixConstIterator& other) const
        {
            return matrix_ == other.matrix_ && r_ == other.r_ && c_ == other.c_;
        }

        bool operator !=(const MatrixConstIterator& other) const
        {
            return !(*this == other);
        }

        MatrixConstIterator operator +(size_t n) const
        {
            MatrixConstIterator it = *this;
            it += n;
            return it;
        }

        MatrixConstIterator operator -(size_t n) const
        {
            MatrixConstIterator it = *this;
            it -= n;
            return it;
        }

        int operator -(const MatrixConstIterator& other) const
        {
            if (matrix_ != other.matrix_)
                throw std::runtime_error("Incompatible MatrixConstIterators");

            return (r_ - other.r_) * columns_count_ + (c_ - other.c_);
        }

        MatrixConstIterator& operator +=(size_t n)
        {
            c_ += n;
            if (c_ >= columns_count_) {
                r_ += c_ / columns_count_;
                c_  = c_ % columns_count_;
            }
            return *this;
        }

        MatrixConstIterator& operator -=(size_t n)
        {
            return operator +=(-n);
        }

        const int& operator [](size_t i) const
        {
            return *(*this + i);
        }

        const int& operator *() const
        {
            return (*matrix_)[r_][c_];
        }

        MatrixConstIterator& operator ++()
        {
            return *this += 1;
        }

        MatrixConstIterator operator ++(int /*post-increment*/)
        {
            const MatrixConstIterator prev = *this;
            operator ++();
            return prev;
        }

        MatrixConstIterator& operator --()
        {
            return *this -= 1;
        }

        MatrixConstIterator operator --(int /*post-decrement*/)
        {
            const MatrixConstIterator prev = *this;
            operator --();
            return prev;
        }

        bool operator <(const MatrixConstIterator& other) const
        {
            if (matrix_ != other.matrix_)
                throw std::runtime_error("Incompatible MatrixConstIterators");

            return r_ < other.r_ || c_ < other.c_;
        }

    private:
        const Matrix* matrix_;
        size_t        columns_count_;
        size_t        r_;
        size_t        c_;
    };
};


typedef std::vector<std::vector<int>> Matrix;

template <typename Solution>
void test()
{
    auto test = [](const Matrix& matrix, int target, bool expected)
    {
        ASSERT_EX( Solution().run(matrix, target) == expected,
                   to_string(matrix) + ", " + to_string(target) + " == " + to_string(expected) );
    };

    Matrix m;

    test({}, 0, false);
    test({{}}, 0, false);


    m = {{0}};

    test(m, -1, false);
    test(m,  0, true);
    test(m,  1, false);


    m = {{-1, 3}};

    test(m, -2, false);
    test(m, -1, true);
    test(m,  0, false);
    test(m,  3, true);
    test(m,  4, false);


    m = {{-1},
         { 3}};

    test(m, -2, false);
    test(m, -1, true);
    test(m,  0, false);
    test(m,  3, true);
    test(m,  4, false);


    m = {{-3, -1,  4},
         { 6,  9, 12},
         {15, 17, 20}};

    test(m, -4, false);
    test(m, -3, true);
    test(m,  4, true);
    test(m,  5, false);
    test(m,  9, true);
    test(m, 14, false);
    test(m, 20, true);
    test(m, 21, false);


    // Test performance
    const int size = 1000;
    const int value_count = size * size;

    m = Matrix(size, std::vector<int>(size, 0));
    for (int r = 0, v = 0; r < size; r++) {
        for (int c = 0; c < size; c++, v++) {
            m[r][c] = v;
        }
    }

    std::cout << "Run for " << value_count << " elements" << std::endl;

    clock_t start = clock();
    for (int i = 0; i < value_count; i++)
        Solution().run(m, i);

    double elapsed_secs = double(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Elapsed: " << elapsed_secs << " seconds" << std::endl;
    std::cout << std::endl;
}

int main()
{
    test<Solution1>();
    test<Solution2>();
    test<Solution3>();

    return 0;
}

}
