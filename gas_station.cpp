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
