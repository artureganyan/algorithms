// Problem: https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/

#include <vector>
#include "utils.h"

namespace best_time_to_buy_and_sell_stock_with_cooldown {

// Note: It's not clear from the problem, but suppose that:
// 1. We can skip (i.e. don't buy a stock) any day, not only the one after we
//    have bought a stock.
// 2. We can buy only one stock per day (otherwise, if there is some profit,
//    it can be made as large as we want by bying many stocks).
// 3. The prices array is not a cyclic schedule, e.g. we can buy on the first
//    day and sell on the last, without a cooldown obligation.


// Straightforward recursive solution

class Solution1 {
public:
    // Note: The prices must contain non-negative numbers only. The profit must
    // fit into int. If there is no profit or the prices is empty, returns 0.
    //
    int run(const std::vector<int>& prices)
    {
        return getMaxProfit(prices, 0, -1, 0);
    }

private:
    // Note: If there is no bought stock, the stock_price must be negative.
    int getMaxProfit(const std::vector<int>& prices, int day, int stock_price, int balance) const
    {
        if (day >= prices.size())
            return balance;

        // The first choice is to buy or sell if we can
        const int current_price = prices[day];
        int       transaction   = 0;

        // ... We have no stock, so buy
        if (stock_price < 0) {
            transaction = getMaxProfit(prices, day + 1, current_price, balance - current_price);

        // ... We have a stock and possible profit, so sell and cooldown
        } else if (current_price > stock_price) {
            transaction = getMaxProfit(prices, day + 2, -1, balance + current_price);
        }

        // The second choice is to wait
        const int wait = getMaxProfit(prices, day + 1, stock_price, balance);

        return std::max(transaction, wait);
    }
};


// Linear time solution

class Solution2 {
public:
    // Note: The prices must contain non-negative numbers only. The profit must
    // fit into int. If there is no profit or the prices is empty, returns 0.
    //
    // Time: O(n), Space: O(1), n - number of prices
    //
    int run(const std::vector<int>& prices)
    {
        if (!prices.size())
            return 0;

        const int BUY_UNDEFINED  = std::numeric_limits<int>::max();
        const int SELL_UNDEFINED = std::numeric_limits<int>::min();

        int buy    = BUY_UNDEFINED;
        int sell   = SELL_UNDEFINED;
        int profit = 0;

        for (int i = 0; i < prices.size(); i++) {
            const int price = prices[i];

            // Find the minimum buy price, until it starts increasing
            if (buy == BUY_UNDEFINED || (sell == SELL_UNDEFINED && buy > price)) {
                buy = price;

            // Find the maximum sell price, until it starts decreasing
            } else if (sell == SELL_UNDEFINED || price >= sell) {
                sell = price;

            // We have (min buy, max sell) pair, and the current price decreased
            } else {
                assert(i >= 2);

                // We are at the price i, which is less than i-1, so going to
                // sell on the day i-1 and loose the day i on the cooldown.
                // But, if the day i+1 is available, we need to check what is
                // better - sell on i-1 or sell on i-2 & buy on i. This depends
                // on the prices i-2, i-1, i, i+1.
                const int profit1 = prices[i - 1] - prices[i - 2];
                const int profit2 = i < prices.size() - 1 ? prices[i + 1] - prices[i] : 0;

                // ... The cooldown on the day i is ok
                if (profit1 >= profit2) {
                    profit += sell - buy;

                // ... Better to sell on i-2 and buy on i. If i == 2, then
                // prices[i - 2] - buy == 0, i.e. we just discard the current
                // (buy, sell) pair.
                } else {
                    profit += prices[i - 2] - buy;
                    i--;
                }

                buy  = BUY_UNDEFINED;
                sell = SELL_UNDEFINED;
            }
        }

        // Do the last sell, if available
        if (buy != BUY_UNDEFINED && sell != SELL_UNDEFINED)
            profit += sell - buy;

        return profit;
    }
};


template <typename Solution>
void test()
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

    ASSERT( Solution().run({1, 2, 5, 0, 3}) == 4 );
    ASSERT( Solution().run({1, 2, 5, 0, 10}) == 11 );
    ASSERT( Solution().run({1, 2, 5, 5, 0, 10}) == 14 );

    const int MAX = std::numeric_limits<int>::max();
    ASSERT( Solution().run({MAX}) == 0 );
    ASSERT( Solution().run({MAX, MAX}) == 0 );
    ASSERT( Solution().run({MAX-1, MAX}) == 1 );
    ASSERT( Solution().run({1, MAX}) == MAX-1 );
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
