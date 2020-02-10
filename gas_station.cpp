// Problem: https://leetcode.com/problems/gas-station/

#include <vector>
#include "utils.h"

namespace gas_station {

class Solution {
public:
    // Note: The gas and cost arrays must be of the same size and contain
    // values >= 0. The potential overflow is not detected.
    //
    // Time: O(n), Space: O(1), n - number of stations
    //
    int run(const std::vector<int>& gas, const std::vector<int>& cost)
    {
        // Idea:
        // If we move through the station i, our gas changes by
        // d[i] = gas[i] - cost[i]. Initially the gas is 0, so we can only
        // start at the station where d[i] >= 0. Suppose that d[0] >= 0, we
        // start from the first station and move until the station i where our
        // gas drops below 0, i.e. d[0] + d[1] + ... + d[i] < 0. Then the
        // stations 0, 1, 2, ..., i are not the answer (because we checked that
        // the sums (d[0] + d[1] + ... + d[k]) >= 0 for any k=0..i-1, so the
        // rest sums (d[k+1] + d[k+2] + ... + d[i]) < 0). Suppose that then we
        // start from the station i+1 (d[i+1] >= 0), and also fail at some
        // station, and so on, until we start from the station x and reach the
        // last station without dropping the gas below 0. So we have:
        // fail    = d[0] + d[1]   + ... + d[x-1] <  0, and
        // success = d[x] + d[x+1] + ... + d[n-1] >= 0
        // Then the station x is the answer if success >= |fail|, i.e. if we
        // have enough gas to continue from the station 0 and finish the circle
        // at the station x.

        if (gas.size() != cost.size())
            throw std::runtime_error("Gas and cost arrays must be of the same size");

        if (!gas.size())
            return -1;

        int start       = -1;
        int current_gas = 0;
        int total_gas   = 0;

        for (int i = 0; i < gas.size(); i++) {
            const int gas_delta = gas[i] - cost[i];

            if (start >= 0) {
                current_gas += gas_delta;
                if (current_gas < 0) {
                    start = -1;
                }
            } else if (gas_delta >= 0) {
                current_gas = gas_delta;
                start = i;
            }

            total_gas += gas_delta;
        }
        if (total_gas < 0)
            start = -1;

        return start;
    }
};

int main()
{
    ASSERT(Solution().run({}, {}) == -1);

    ASSERT(Solution().run({0}, {1}) == -1);
    ASSERT(Solution().run({0}, {0}) == 0);
    ASSERT(Solution().run({1}, {0}) == 0);
    ASSERT(Solution().run({1}, {1}) == 0);
    ASSERT(Solution().run({1}, {2}) == -1);

    ASSERT(Solution().run({1, 1}, {1, 1}) == 0);
    ASSERT(Solution().run({1, 1}, {2, 1}) == -1);
    ASSERT(Solution().run({1, 1}, {1, 2}) == -1);
    ASSERT(Solution().run({1, 2}, {2, 1}) == 1);

    ASSERT(Solution().run({2, 1, 1}, {2, 1, 1}) == 0);
    ASSERT(Solution().run({1, 2, 1}, {2, 1, 1}) == 1);
    ASSERT(Solution().run({1, 1, 2}, {2, 1, 1}) == 1);
    ASSERT(Solution().run({2, 1, 1}, {1, 2, 1}) == 0);
    ASSERT(Solution().run({1, 2, 1}, {1, 2, 1}) == 0);
    ASSERT(Solution().run({1, 1, 2}, {1, 2, 1}) == 2);
    ASSERT(Solution().run({2, 1, 1}, {1, 1, 2}) == 0);
    ASSERT(Solution().run({1, 2, 1}, {1, 1, 2}) == 0);
    ASSERT(Solution().run({1, 1, 2}, {1, 1, 2}) == 0);

    ASSERT(Solution().run({1, 1, 1, 1, 5}, {5, 1, 1, 1, 1}) == 1);
    ASSERT(Solution().run({1, 1, 1, 1, 3}, {1, 2, 1, 2, 1}) == 4);

    ASSERT(Solution().run({1, 3, 5, 0, 1}, {2, 4, 0, 1, 1}) == 2);

    return 0;
}

}
