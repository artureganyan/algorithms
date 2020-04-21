// Problem: https://leetcode.com/problems/coin-change/

#include <vector>
#include "utils.h"

namespace coin_change {

class Solution {
public:
    // Note: If the value is 0, returns 0. Otherwise, if it can not be
    // represented by the coins, or the coins is empty, returns -1.
    //
    int run(const std::vector<int>& coins, int value)
    {
        return getMinimumNumberOfCoins(coins, 0, value);
    }

private:
    int getMinimumNumberOfCoins(const std::vector<int>& coins, int coins_start, int value) const
    {
        if (value == 0)
            return 0;

        int result = -1;

        for (int i = coins_start; i < coins.size(); i++) {
            const int coin = coins[i];

            for (int coin_sum = 0, coin_count = 0; coin_sum <= value; coin_sum += coin, coin_count++) {
                int total_count = getMinimumNumberOfCoins(coins, coins_start + 1, value - coin_sum);
                if (total_count < 0)
                    continue;

                total_count += coin_count;

                if (result >= 0)
                    result = std::min(result, total_count); else
                    result = total_count;
            }
        }

        return result;
    }
};

int main()
{
    ASSERT( Solution().run({}, 0) == 0 );
    ASSERT( Solution().run({}, -1) == -1 );
    ASSERT( Solution().run({}, 1) == -1 );

    ASSERT( Solution().run({1}, 0) == 0 );
    ASSERT( Solution().run({1}, 1) == 1 );
    ASSERT( Solution().run({1}, 2) == 2 );
    ASSERT( Solution().run({1}, 3) == 3 );

    ASSERT( Solution().run({1, 2}, 1) == 1 );
    ASSERT( Solution().run({1, 2}, 2) == 1 );
    ASSERT( Solution().run({1, 2}, 3) == 2 );
    ASSERT( Solution().run({1, 2}, 4) == 2 );
    ASSERT( Solution().run({1, 2}, 5) == 3 );

    ASSERT( Solution().run({2}, 1) == -1 );
    ASSERT( Solution().run({2}, 2) == 1 );
    ASSERT( Solution().run({2}, 3) == -1 );

    ASSERT( Solution().run({5, 1, 4}, 3) == 3 );
    ASSERT( Solution().run({5, 1, 4}, 4) == 1 );
    ASSERT( Solution().run({5, 1, 4}, 5) == 1 );
    ASSERT( Solution().run({5, 1, 4}, 8) == 2 );
    ASSERT( Solution().run({5, 1, 4}, 9) == 2 );
    ASSERT( Solution().run({5, 1, 4}, 10) == 2 );
    ASSERT( Solution().run({5, 1, 4}, 11) == 3 );
    ASSERT( Solution().run({5, 1, 4}, 12) == 3 );

    ASSERT( Solution().run({5, 9, 10, 101}, 16) == -1 );
    ASSERT( Solution().run({5, 3, 10, 101}, 119) == 4 );

    return 0;
}

}
