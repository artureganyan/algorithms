// Problem: https://leetcode.com/problems/range-sum-query-mutable/

#include <vector>
#include "utils.h"

namespace range_sum_query_mutable {

class NumArray {
public:
    // Time: O(n), Space: O(n) for the object lifetime, n - number of integers
    NumArray(const std::vector<int>& nums)
    {
        init(nums);
    }

    // Time: n-i updates, Space: O(1), n - number of integers
    void update(int i, int val)
    {
        if (i < 0 || i >= sums_.size())
            throw std::out_of_range("Index out of range");

        const int ni    = sum(i, i);
        const int delta = val - ni;

        for (; i < sums_.size(); i++)
            sums_[i] += delta;
    }

    // Time: O(1), Space: O(1)
    int sumRange(int i, int j) const
    {
        if (i < 0 || i >= sums_.size() || j < 0 || j >= sums_.size())
            throw std::out_of_range("Index out of range");

        if (i > j)
            throw std::runtime_error("Incorrect range");

        return sum(i, j);
    }

private:
    void init(const std::vector<int>& nums)
    {
        sums_.resize(nums.size());

        for (int i = 0, sum = 0; i < nums.size(); i++) {
            sum += nums[i];
            sums_[i] = sum;
        }
    }

    inline int sum(int i, int j) const
    {
        return sums_[j] - (i > 0 ? sums_[i - 1] : 0);
    }

    std::vector<int> sums_;
};


void test(const std::vector<int>& nums, bool with_updates = false)
{
    std::vector<int> values = nums;
    NumArray         a(values);

    for (int i1 = 0; i1 < nums.size(); i1++) {
        int sum = 0;
        for (int i2 = i1; i2 < nums.size(); i2++) {
            int& v = values[i2];
            if (with_updates && i1 == 0) {
                v += i2 % 2 ? (i2 + 1) : -(i2 + 1);
                a.update(i2, v);
            }
            sum += v;

            ASSERT_EX( a.sumRange(i1, i2) == sum,
                to_string(values) + ", " +
                (with_updates && i1 == 0 ? "update(" + to_string(i2) + ", " + to_string(v) + "), " : "") +
                "[" + to_string(i1) + ", " + to_string(i2) + "] -> " +
                to_string(sum)
            );
        }
    }
}

int main()
{
    test({});

    test({0});
    test({1});
    test({-1});

    test({0}, true);

    test({1, 1});
    test({1, 3});
    test({3, 1});

    test({-2, 0, 3});

    test({9, -10, 9, 0, -5, -1, 100});
    test({9, -10, 9, 0, -5, -1, 100}, true);

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::max();

    test({MAX});
    test({MAX-1, 1});
    test({MAX/2, MAX/2});

    test({MIN});
    test({MIN+1, 1});
    test({MIN/2, MIN/2});

    return 0;
}

}
