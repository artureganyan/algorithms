// Problem: https://leetcode.com/problems/zigzag-conversion/

#include <string>
#include "utils.h"

namespace zig_zag_conversion {

class Solution {
public:
    // Time: O(n), Space: O(n) for result, O(1) for processing
    //
    // Example:
    //
    // Given "1 2 3 4 5 6 7 8 9 A B C D E F" and row_count = 4,
    //
    // rearrange string
    //
    // 1     7     D
    // 2   6 8   C E
    // 3 5   9 B   F
    // 4     A
    //
    // and read line by line
    //
    // 1 7 D 2 6 8 C E 3 5 9 B F 4 A
    //
    std::string run(std::string s, int row_count)
    {
        if (row_count < 1)
            return std::string();

        if (row_count == 1)
            return s;

        std::string result;
        result.reserve(s.size());

        for (int r = 0; r < row_count; r++) {
            int step1 = 2 * (row_count - r - 1);
            int step2 = 2 * r;
            if (step1 == 0) step1 = step2;
            if (step2 == 0) step2 = step1;

            for (int i = r; i < s.size();) {
                result.push_back(s[i]);
                i += step1;
                std::swap(step1, step2);
            }
        }

        return result;
    }
};

int main()
{
    ASSERT(Solution().run("123456789ABCD", 1) == "123456789ABCD");
    ASSERT(Solution().run("123456789ABCD", 2) == "13579BD2468AC");
    ASSERT(Solution().run("123456789ABCD", 3) == "159D2468AC37B");
    ASSERT(Solution().run("123456789ABCD", 4) == "17D268C359B4A");
    ASSERT(Solution().run("123456789ABCD", 5) == "1928A37B46C5D");
    ASSERT(Solution().run("123456789ABCD", 0) == "");
    ASSERT(Solution().run("", 1) == "");
    ASSERT(Solution().run("", 2) == "");

    return 0;
}

}
