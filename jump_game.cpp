// Problem: https://leetcode.com/problems/jump-game/

#include <vector>
#include "utils.h"

namespace jump_game {

class Solution {
public:
    // Time: O(n), Space: O(1)
    bool run(std::vector<int>& nums)
    {
        if (!nums.size())
            return false;

        int max_jump = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (i > max_jump)
                return false;

            max_jump = std::max(max_jump, i + nums[i]);
            if (max_jump >= nums.size() - 1)
                return true;
        }
        return true;

        // More illustrative, but will iterate over all nums even if nums[0]
        // finishes the game
        /*
        int max_jump = 0;
        int i = 0;
        for (; i <= max_jump && i < nums.size(); i++)
            max_jump = std::max(max_jump, i + nums[i]);

        return i == nums.size();
        */
    }
};

int main()
{
    std::vector<int> v;
    ASSERT(Solution().run(v = {}) == false);
    ASSERT(Solution().run(v = {0}) == true);
    ASSERT(Solution().run(v = {0, 0}) == false);
    ASSERT(Solution().run(v = {1, 0}) == true);
    ASSERT(Solution().run(v = {1, 0, 0}) == false);
    ASSERT(Solution().run(v = {1, 1, 0}) == true);
    ASSERT(Solution().run(v = {2, 3, 1, 1, 4}) == true);
    ASSERT(Solution().run(v = {3, 2, 1, 0, 4}) == false);
    return 0;
}

}
