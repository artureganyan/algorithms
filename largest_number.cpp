// Problem: https://leetcode.com/problems/largest-number/

#include <vector>
#include <string>
#include "utils.h"

namespace largest_number {

class Solution {
public:
    // Time: O(n * log(n)), Space: O(n), n - number of integers
    //
    // Note: The actual time and space also depends on the number of digits in
    // each integer, but since this number <= some constant, it's not present
    // in the complexities.
    //
    std::string run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return {};

        // Convert numbers to strings
        std::vector<std::string> nums_string;
        for (const auto n : nums)
            nums_string.push_back(std::to_string(n));

        // Sort strings in descending order
        std::sort(nums_string.begin(), nums_string.end(), std::greater<std::string>());

        // Concatenate strings to the result
        // ... If the first string is "0", the rest, if exist, are also zeros
        if (nums_string.front() == "0")
            return nums_string.front();

        std::string result;
        for (const auto& s : nums_string)
            result += s;
        return result;
    }
};

int main()
{
    ASSERT( Solution().run({}) == std::string() );

    ASSERT( Solution().run({0}) == "0" );
    ASSERT( Solution().run({0, 0}) == "0" );
    ASSERT( Solution().run({0, 0, 0}) == "0" );

    ASSERT( Solution().run({1}) == "1" );

    ASSERT( Solution().run({0, 1}) == "10" );
    ASSERT( Solution().run({1, 0}) == "10" );
    ASSERT( Solution().run({1, 1}) == "11" );

    ASSERT( Solution().run({0, 0, 1}) == "100" );
    ASSERT( Solution().run({0, 1, 0}) == "100" );
    ASSERT( Solution().run({1, 0, 0}) == "100" );

    ASSERT( Solution().run({10, 9}) == "910" );
    ASSERT( Solution().run({9, 100, 89}) == "989100" );
    ASSERT( Solution().run({78, 89, 90}) == "908978" );

    ASSERT( Solution().run({22, 1, 0, 54, 333, 0, 9, 876}) == "98765433322100" );

    return 0;
}

}
