// Problem: https://leetcode.com/problems/merge-intervals/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace merge_intervals {

class Solution {
public:
    // Time: O(n * log(n)), Space: O(n), n - number of intervals
    //
    // Note: Each interval must have 2 values such that interval[0] <= interval[1]
    //
    std::vector<std::vector<int>> run(const std::vector<std::vector<int>>& intervals)
    {
        if (!intervals.size())
            return {};

        // Sort intervals by the left border
        typedef std::vector<int> Interval;
        std::vector<Interval> result = intervals;

        std::sort(result.begin(), result.end(),
            [](const Interval& a, const Interval& b) {
                return a[0] < b[0];
            });

        // Merge intervals
        int ri = 0;
        for (int vi = 1; vi < result.size(); vi++) {
            Interval&       r = result[ri];
            const Interval& v = result[vi];

            // Merge v with the result
            if (v[0] <= r[1]) {
                if (v[1] > r[1])
                    r[1] = v[1];
            // Keep v in the result
            } else {
                ri++;
                if (vi > ri)
                    result[ri] = v;
            }
        }
        result.resize(ri + 1);

        return result;
    }
};

int main()
{
    typedef std::vector<std::vector<int>> Intervals;

    // Note: Assume that the result is sorted
    ASSERT(( Solution().run({}) == Intervals{} ));
    ASSERT(( Solution().run({{0, 0}}) == Intervals{{0, 0}} ));
    ASSERT(( Solution().run({{0, 0}, {0, 0}}) == Intervals{{0, 0}} ));
    ASSERT(( Solution().run({{0, 0}, {1, 1}}) == Intervals{{0, 0}, {1, 1}} ));
    ASSERT(( Solution().run({{0, 1}, {1, 2}}) == Intervals{{0, 2}} ));
    ASSERT(( Solution().run({{0, 2}, {1, 2}}) == Intervals{{0, 2}} ));
    ASSERT(( Solution().run({{0, 3}, {1, 2}}) == Intervals{{0, 3}} ));
    ASSERT(( Solution().run({{0, 3}, {1, 5}}) == Intervals{{0, 5}} ));
    ASSERT(( Solution().run({{1, 2}, {0, 1}}) == Intervals{{0, 2}} ));
    ASSERT(( Solution().run({{-1, 2}, {0, 3}}) == Intervals{{-1, 3}} ));
    ASSERT(( Solution().run({{-3, -1}, {-2, -1}}) == Intervals{{-3, -1}} ));
    ASSERT(( Solution().run({{1, 3}, {1, 2}, {1, 1}, {0, 3}, {1, 1}, {4, 5}, {1, 2}}) == Intervals{{0, 3}, {4, 5}} ));

    for (int i = 0; i < 100; i++) {
        Intervals       intervals = {{1, 5}, {0, 2}, {-1, 0}, {-3, -2}, {8, 10}, {12, 15}, {10, 11}, {12, 17}};
        const Intervals expected  = {{-3, -2}, {-1, 5}, {8, 11}, {12, 17}};

        if (i > 0)
            std::random_shuffle(intervals.begin(), intervals.end());

        if (Solution().run(intervals) != expected)
            ASSERT_EX(false, to_string(intervals));
    }
    ASSERT_EX(true, "test with shuffled input");

    return 0;
}

}
