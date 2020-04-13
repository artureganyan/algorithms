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
        // Idea:
        // Let's note the following:
        // 1. For each number nums[i], there are one or more increasing
        //    sequences of the maximum length that end at nums[i] (at least, of
        //    the length 1). We only need to remember the length, not the
        //    sequences.
        // 2. If, on some range [0, i), there are two increasing sequences of
        //    the same maximum length, only the one which ends at the lower
        //    number matters - any number on [i, n) that continues the "higher"
        //    sequence also continues the "lower" one, but not vice versa.
        //
        // So, if we iterate the numbers from 0 to n-1, then at step i we have
        // at most i increasing sequences of different lengths (follows from 1),
        // such that the longer sequence ends at the larger number (follows
        // from 2). The nums[i] must continue the longest possible sequence,
        // i.e. which ends at the largest number n < nums[i]. Since its length
        // is increased by 1, then, if there is another sequence of this length
        // which ends at the number > nums[i], it can be forgotten:
        //
        // nums[i]                  | Step | Sequences (ends at -> length)
        //  7|                      | 0    | 1 -> 1
        //  6|     *                | 1    | 1 -> 1, 3 -> 2
        //  5|       *              | 2    | 1 -> 1, 3 -> 2, 6 -> 3
        //  4|         *     *      | 3    | 1 -> 1, 3 -> 2, 5 -> 3
        //  3|   *         *        | 4    | 1 -> 1, 3 -> 2, 4 -> 3
        //  2|           *          | 5    | 1 -> 1, 2 -> 2, 4 -> 3
        //  1| *                    | 6    | 1 -> 1, 2 -> 2, 3 -> 3
        //    -----------------> i  | 7    | 1 -> 1, 2 -> 2, 3 -> 3, [4 -> 4]
        //     0 1 2 3 4 5 6 7

        if (!nums.size())
            return 0;

        typedef std::map<int /*value*/, int /*length*/> MaxLengthByValue;

        MaxLengthByValue max_lengths;
        int              result = 0;

        for (int i = 0; i < nums.size(); i++) {
            const int n  = nums[i];
            auto      ni = max_lengths.lower_bound(n);

            // The sequence ending at n already exists
            if (ni != max_lengths.end() && ni->first == n)
                continue;

            // Continue the sequence ending at the largest number which is < n
            int max_length = 0;
            if (max_lengths.size() && ni != max_lengths.begin())
                max_length = std::prev(ni)->second;
            max_length++;

            ni = max_lengths.insert(std::make_pair(n, max_length)).first;

            // If there is a "higher" sequence with the same length, remove it
            ni = std::next(ni);
            if (ni != max_lengths.end() && ni->second == max_length)
                max_lengths.erase(ni);

            // Update the total maximum
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
