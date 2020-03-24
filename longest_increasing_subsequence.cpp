// Problem: https://leetcode.com/problems/longest-increasing-subsequence/

#include <vector>
#include <map>
#include "utils.h"

namespace longest_increasing_subsequence {

// Straightforward solution

class Solution1 {
public:
    // Time: O(n^2), Space: O(n), n - number of integers
    int run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return 0;

        std::vector<int> max_lengths(nums.size(), 0);
        int              result = 0;

        for (int i1 = 0; i1 < nums.size(); i1++) {
            const int n = nums[i1];

            int& max_length = max_lengths[i1];
            for (int i2 = 0; i2 < i1; i2++) {
                if (nums[i2] < n)
                    max_length = std::max(max_length, max_lengths[i2]);
            }
            max_length++;

            result = std::max(result, max_length);
        }

        return result;
    }
};


// Optimized to find and update candidate subsequences in O(log(n)) time,
// giving O(n * log(n)) total time.

class Solution2 {
public:
    // Time: O(n * log(n)), Space: O(n), n - number of integers
    int run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return 0;

        typedef std::map<int /*value*/, int /*length*/> MaxLengthByValue;

        MaxLengthByValue max_lengths;
        int              result = 0;

        for (int i = 0; i < nums.size(); i++) {
            const int n  = nums[i];
            auto      ni = max_lengths.lower_bound(n);

            if (ni != max_lengths.end() && ni->first == n)
                continue;

            int max_length = 0;
            if (max_lengths.size() && ni != max_lengths.begin())
                max_length = std::prev(ni)->second;
            max_length++;

            ni = max_lengths.insert(std::make_pair(n, max_length)).first;

            ni = std::next(ni);
            if (ni != max_lengths.end() && ni->second == max_length)
                max_lengths.erase(ni);

            result = std::max(result, max_length);
        }

        return result;
    }
};


template <typename Solution>
void test()
{
    ASSERT( Solution().run({}) == 0 );

    ASSERT( Solution().run({0}) == 1 );
    ASSERT( Solution().run({1}) == 1 );
    ASSERT( Solution().run({-1}) == 1 );

    ASSERT( Solution().run({0, 1}) == 2 );
    ASSERT( Solution().run({1, 0}) == 1 );
    ASSERT( Solution().run({1, 1}) == 1 );

    ASSERT( Solution().run({0, 1, 2}) == 3 );
    ASSERT( Solution().run({0, 2, 1}) == 2 );
    ASSERT( Solution().run({2, 1, 0}) == 1 );

    ASSERT( Solution().run({3, 1, 2, 4}) == 3 );
    ASSERT( Solution().run({3, 1, 4, 2, 5}) == 3 );

    ASSERT( Solution().run({1, 2, 1, 2, 3, 1, 2}) == 3 );

    ASSERT( Solution().run({3, -1, 0, 3, 2, 3}) == 4 );
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
