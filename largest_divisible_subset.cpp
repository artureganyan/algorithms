// Problem: https://leetcode.com/problems/largest-divisible-subset/

#include <vector>
#include <unordered_map>
#include "utils.h"

namespace largest_divisible_subset {

class Solution {
public:
    // Note: The nums must contain positive and unique numbers only. If the
    // nums is empty, returns empty result. If there are no two divisible
    // numbers, i.e. no nums[i1] and nums[i2] such that nums[i1] % nums[i2] == 0
    // except i1 == i2, returns any of the numbers (also when the nums contains
    // a single number only). If there are negative, zero or duplicated numbers,
    // the behavior is undefined.
    //
    // Time: O(n^2), Space: O(n), n - number of integers
    //
    std::vector<int> run(const std::vector<int>& nums)
    {
        if (nums.size() <= 1)
            return nums;

        // For each number nums[i], find the longest chain of dividends that
        // ends at this number: d1 % d2 = d2 % d3 = ... = dk % nums[i] = 0,
        // d1 > d2 > ... > dk > nums[i]. Also, find the number with the longest
        // such chain.
        struct Dividend
        {
            Dividend(int v = -1, int d = 0) : value(v), dividends(d) {}

            int value;
            int dividends;
        };

        std::unordered_map<int /*num*/, Dividend> dividends;
        int longest_chain_length = 0;
        int longest_chain_start  = nums.front(); // For the case if there are no dividends

        // ... Iterate the numbers in descending order, to know at step i that
        // 1) all the dividends of nums[i] are already found,
        // 2) nums[i] can be a dividend for nums[k > i] only
        std::vector<int> nums_sorted = nums;
        std::sort(nums_sorted.begin(), nums_sorted.end(), std::greater<int>());

        for (int i1 = 0; i1 < nums_sorted.size(); i1++) {
            const int       n1 = nums_sorted[i1];
            const Dividend& d1 = dividends[n1];

            for (int i2 = i1 + 1; i2 < nums_sorted.size(); i2++) {
                const int n2 = nums_sorted[i2];
                assert(n2 > 0 && n2 != n1 && "The numbers must be positive and unique");
                if (n1 % n2 != 0)
                    continue;

                Dividend& d2 = dividends[n2];
                if (d1.dividends + 1 <= d2.dividends)
                    continue;

                d2.value     = n1;
                d2.dividends = d1.dividends + 1;

                if (d2.dividends > longest_chain_length) {
                    longest_chain_length = d2.dividends;
                    longest_chain_start  = n2;
                }
            }
        }

        // Convert the chain of dividends to the result
        std::vector<int> result;
        result.reserve(longest_chain_length + 1);
        for (int n = longest_chain_start; n >= 0; n = dividends[n].value)
            result.push_back(n);

        return result;
    }
};


int main()
{
    ASSERT( compare_sets(Solution().run({}), {}) );

    ASSERT( compare_sets(Solution().run({1}), {1}) );
    ASSERT( compare_sets(Solution().run({2}), {2}) );

    ASSERT( compare_sets(Solution().run({1, 2}), {1, 2}) );
    ASSERT( compare_sets(Solution().run({2, 1}), {1, 2}) );

    ASSERT( is_any_of_sets(Solution().run({2, 3}), {{2}, {3}}) );

    ASSERT( is_any_of_sets(Solution().run({1, 2, 3}), {{1, 2}, {1, 3}}) );
    ASSERT( is_any_of_sets(Solution().run({1, 2, 3, 6}), {{1, 2, 6}, {1, 3, 6}}) );

    ASSERT( compare_sets(Solution().run({1, 2, 3, 4}), {1, 2, 4}) );
    ASSERT( compare_sets(Solution().run({1, 2, 3, 4, 6, 8}), {1, 2, 4, 8}) );

    ASSERT( compare_sets(Solution().run({1000, 3, 2, 4, 6, 8, 90, 7, 100}), {2, 4, 100, 1000}) );

    const int MAX = std::numeric_limits<int>::max();

    ASSERT( compare_sets(Solution().run({MAX}), {MAX}) );
    ASSERT( compare_sets(Solution().run({1, MAX}), {1, MAX}) );
    ASSERT( is_any_of_sets(Solution().run({MAX, MAX-1, MAX/2}), {{MAX/2, MAX}, {MAX/2, MAX-1}}) );

    return 0;
}

}
