// Problem: https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/

#include <vector>
#include "utils.h"

namespace best_time_to_buy_and_sell_stock_with_cooldown {

// Note: It's not clear from the problem, but suppose that:
// 1. We can skip (i.e. don't buy a stock) any day, not only the one after we
//    have bought a stock.
// 2. We can buy only one stock per day (otherwise, if there is some profit,
//    it can be made as large as we want by bying many stocks).
// 3. The prices array is not a cyclic schedule, e.g. we can buy at the first
//    day and sell at the last, without a cooldown obligation.


// Straightforward recursive solution

class Solution {
public:
    // Note: The prices must contain non-negative numbers only. The profit must
    // fit into int. If there is no profit or the prices is empty, returns 0.
    //
    int run(const std::vector<int>& prices)
    {
        return getMaxProfit(prices, 0, 0, 0);
    }

private:
    // Note: If the stock was sold, the stock_price must be set to negative. If
    // there is no stock, the stock_price must be 0. If there is a stock bought
    // with zero price, the stock_price must be set to 1.
    //
    int getMaxProfit(const std::vector<int>& prices, int day, int stock_price, int balance) const
    {
        if (day >= prices.size())
            return balance;

        const int current_price = prices[day];

        // We have no stock, so can buy or wait
        if (stock_price == 0) {
            const int buy  = getMaxProfit(prices, day + 1, std::max(current_price, 1), balance - current_price);
            const int wait = getMaxProfit(prices, day + 1, stock_price,                balance);
            return std::max(buy, wait);

        // We have a stock, so can sell or wait
        } else if (stock_price > 0) {
            // There is a profit
            if (current_price >= stock_price) {
                const int sell = getMaxProfit(prices, day + 1, -stock_price, balance + current_price);
                const int wait = getMaxProfit(prices, day + 1, stock_price,  balance);
                return std::max(sell, wait);
            // No profit, just wait
            } else {
                return getMaxProfit(prices, day + 1, stock_price, balance);
            }

        // We have sold a stock, so must wait (the cooldown)
        } else if (stock_price < 0) {
            return getMaxProfit(prices, day + 1, 0, balance);
        }
    }
};

int main()
{
    ASSERT( Solution().run({}) == 0 );

    ASSERT( Solution().run({0}) == 0 );
    ASSERT( Solution().run({1}) == 0 );

    ASSERT( Solution().run({0, 0}) == 0 );
    ASSERT( Solution().run({1, 0}) == 0 );
    ASSERT( Solution().run({0, 1}) == 1 );
    ASSERT( Solution().run({1, 1}) == 0 );

    ASSERT( Solution().run({0, 0, 1}) == 1 );
    ASSERT( Solution().run({0, 1, 0}) == 1 );
    ASSERT( Solution().run({0, 1, 1}) == 1 );
    ASSERT( Solution().run({1, 0, 0}) == 0 );
    ASSERT( Solution().run({1, 0, 1}) == 1 );
    ASSERT( Solution().run({1, 1, 0}) == 0 );
    ASSERT( Solution().run({1, 1, 1}) == 0 );

    ASSERT( Solution().run({1, 2, 3}) == 2 );
    ASSERT( Solution().run({2, 1, 3}) == 2 );
    ASSERT( Solution().run({2, 3, 1}) == 1 );
    ASSERT( Solution().run({3, 2, 1}) == 0 );

    ASSERT( Solution().run({1, 2, 5, 0, 10}) == 11 );

    const int MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run({MAX}) == 0 );
    ASSERT( Solution().run({MAX, MAX}) == 0 );
    ASSERT( Solution().run({MAX-1, MAX}) == 1 );
    ASSERT( Solution().run({1, MAX}) == MAX-1 );

    return 0;
}

}
