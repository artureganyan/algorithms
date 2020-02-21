// Problem: https://leetcode.com/problems/summary-ranges/

#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include "utils.h"

namespace summary_ranges {

class Solution {
public:
    // Note: The nums must be sorted in ascending order and must not contain
    // duplicates.
    //
    // Time: O(n), Space: O(n), n - number of integers
    //
    std::vector<std::string> run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return {};

        std::vector<std::string> result;
        int                      range_left = nums[0];

        for (int i = 1; i < nums.size(); i++) {
            const int num      = nums[i];
            const int num_prev = nums[i - 1];

            if (num == num_prev + 1)
                continue;

            result.push_back(range_to_string(range_left, num_prev));
            range_left = num;
        }
        result.push_back(range_to_string(range_left, nums.back()));

        return result;
    }

private:
    std::string range_to_string(int left, int right) const
    {
        std::stringstream s;
        if (left < right) {
            s << left << "->" << right;
        } else if (left == right) {
            s << left;
        } else {
            throw std::runtime_error("Incorrect range");
        }
        return s.str();
    }
};

int main()
{
    typedef std::vector<std::string> Result;

    ASSERT(( Solution().run({}) == Result{} ));

    ASSERT(( Solution().run({1}) == Result{"1"} ));
    ASSERT(( Solution().run({1, 2}) == Result{"1->2"} ));
    ASSERT(( Solution().run({1, 2, 3}) == Result{"1->3"} ));

    ASSERT(( Solution().run({1, 3}) == Result{"1", "3"} ));
    ASSERT(( Solution().run({1, 3, 4}) == Result{"1", "3->4"} ));
    ASSERT(( Solution().run({1, 2, 4}) == Result{"1->2", "4"} ));
    ASSERT(( Solution().run({1, 2, 4, 5}) == Result{"1->2", "4->5"} ));
    ASSERT(( Solution().run({1, 2, 4, 6, 7}) == Result{"1->2", "4", "6->7"} ));

    ASSERT(( Solution().run({-5, -3, -2, 0, 1, 2, 4, 6, 7}) == Result{"-5", "-3->-2", "0->2", "4", "6->7"} ));

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    ASSERT(( Solution().run({MIN, MAX}) == Result{to_string(MIN), to_string(MAX)} ));
    ASSERT(( Solution().run({MIN, MIN+1, MAX-1, MAX}) == Result{
            to_string(MIN) + "->" + to_string(MIN+1),
            to_string(MAX-1) + "->" + to_string(MAX)
        } ));

    return 0;
}

}
