// Problem: https://leetcode.com/problems/arithmetic-slices/

#include "utils.h"
#include <vector>

namespace arithmetic_slices {

class Solution {
public:
    // Note: If the numbers contains < 3 elements, returns 0.
    //
    // Time: O(n), Space: O(1), n - number of integers
    //
    int run(const std::vector<int>& numbers)
    {
        // Idea:
        // For an arithmetic sequence, any subsequence (of size >= 3) is also
        // arithmetic. If the sequence consists of n numbers, there are n-2
        // subsequences of size 3, n-3 of size 4, n-4 of size 5, ..., 1 of size n,
        // i.e. (n - 2) * (n - 1) / 2 arithmetic subsequences in total. So, to
        // count all possible arithmetic subsequences, we just need to find all
        // the sequences. This can be done by going from the first to the last
        // number and comparing d1 = (n[i] - n[i-1]) with d2 = (n[i+1] - n[i]):
        // if d1 == d2, then the current sequence continues, otherwise it's
        // finished at i and the new one begins also at i.

        if (numbers.size() < 3)
            return 0;

        int          result         = 0;
        int          current_length = 2;
        unsigned int current_delta  = numbers[1] - numbers[0];  // unsigned to avoid overflow

        for (int i = 2; i <= numbers.size(); i++) {
            const bool         end   = (i == numbers.size());
            const unsigned int delta = !end ? numbers[i] - numbers[i - 1] : 0;

            if (current_delta == delta && !end) {
                current_length++;
            } else {
                result += (current_length - 2) * (current_length - 1) / 2;
                current_length = 2;
                current_delta  = delta;
            }
        }

        return result;
    }
};

int main()
{
    const auto test = [](const std::vector<int>& numbers, int expected)
    {
        ASSERT_EX( Solution().run(numbers) == expected,
            to_string(numbers) + " -> " + to_string(expected) );
    };
    
    test({}, 0);
    test({0}, 0);
    test({0, 1}, 0);

    test({0, 0, 0}, 1);
    test({1, 1, 1}, 1);
    test({-1, -1, -1}, 1);

    test({0, 1, 2}, 1);
    test({0, 1, 3}, 0);
    test({0, 2, 2}, 0);
    test({1, 1, 2}, 0);
    test({0, 1, 0}, 0);

    test({1, 3, 5}, 1);
    test({2, 4, 6}, 1);

    test({0, 1, 2, 3}, 3);
    test({0, 1, 2, 4}, 1);
    test({1, 1, 2, 3}, 1);
    test({0, 1, 3, 4}, 0);
    test({0, 1, 3, 2}, 0);
    
    test({0, 1, 2, 3, 4}, 6);
    test({0, 1, 2, 3, 5}, 3);
    test({5, 1, 2, 3, 4}, 3);
    test({5, 1, 2, 3, 5}, 1);
    test({5, 5, 2, 3, 4}, 1);
    test({0, 1, 2, 5, 5}, 1);
    test({0, 5, 2, 5, 4}, 0);

    test({0, 1, 2, 1, 0}, 2);
    test({0, 1, 2, 1, 1}, 1);
    test({0, 1, 0, 1, 0}, 0);
    test({0, 1, 2, 0, -2}, 2);

    test({0, 1, 2, 0, 1, 2}, 2);
    test({0, 1, 2, 2, 1, 0}, 2);
    test({0, 1, 2, 3, 0, 1, 2}, 4);
    test({0, 1, 2, 0, 1, 2, 3}, 4);
    test({0, 1, 2, 0, 0, 1, 2}, 2);
    test({0, 1, 2, 3, 0, 0, 1, 2}, 4);
    test({0, 1, 2, 0, 0, 1, 2, 3}, 4);
    test({0, 1, 2, 1, 0, 1, 2, 3}, 5);

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    test({MIN}, 0);
    test({MAX}, 0);
    test({MIN, MIN}, 0);
    test({MAX, MAX}, 0);
    test({MIN, MAX}, 0);
    test({MIN, MIN, MIN}, 1);
    test({MAX, MAX, MAX}, 1);
    test({MAX, MIN, MAX}, 0);
    test({MIN, MIN, MIN, MIN}, 3);
    test({MAX, MAX, MAX, MAX}, 3);
    test({MIN, MAX, MAX, MAX}, 1);
    test({MAX, MAX, MAX, MIN}, 1);
    test({MIN, MAX, MIN, MAX}, 0);
    test({MIN, MAX, MAX, MAX, MAX-1, MAX-2, MAX-3, MAX, MIN, MIN, MIN, MIN+1, MIN+2, MAX}, 6);
    
    return 0;
}

}
