// Problem: https://leetcode.com/problems/lexicographical-numbers/

#include "utils.h"
#include <vector>
#include <array>

namespace lexicographical_numbers {

class Solution {
public:
    // Note: If n <= 0, returns empty result.
    //
    // Time: O(n), Space: O(n) for result, n - number of integers
    //
    // Note: Although we have the nested cycle that removes trailing zero
    // digits, the time complexity is linear because this cycle does at most
    // d_max-1 iterations, where d_max is the maximum count of digits. So even
    // if all the n numbers would have d_max-1 zeros, the time would be
    // < n*d_max. In reality, there is 1 trailing zero per each 10 numbers, +1
    // per each 100, +1 per each 1000, etc; and giving that the group of d
    // numbers {x0, x00, x000, ...} is handled with a single pass of d
    // iterations, the total nested iterations are floor(n/10).
    //
    std::vector<int> run(int n)
    {
        if (n <= 0)
            return {};

        std::vector<int> result;
        result.reserve(n);

        // Put the numbers [1, n] to the result in the lexicographical order
        int i = 1;
        do {
            const int i_prev = i;
            result.push_back(i);

            // First, try to add digit: i0, i00, i000, ...
            i = i_prev * 10;
            if (i <= n)
                continue;

            // Can't add digit, so increase the last one: i001, i002, ..., i009
            i = i_prev + 1;
            // ... The last digit is not overflowed but the number is > n, so
            //     remove the last digit and increase by 1: e.g. i08 > n => i1
            if ((i % 10) != 0 && i > n) {
                i /= 10;
                i += 1;
            }
            // ... The last digit(s) is overflowed, remove it: i099+1=i100 -> i1
            while ((i % 10) == 0)
                i /= 10;

            // If get back to 1, stop, because we have one of the followings:
            // 1. i < 10 && i > n => i = i/10 + 1 = 1
            // 2. i = 99..9, i = i + 1 = 100..0 => i = i/10/10/... = 1
        } while (i != 1);

        assert(result.size() == n);
        return result;
    }
};

void test(int n)
{
    // Returns true if n1 < n2 lexicographically
    const auto compare = [](int n1, int n2) -> bool
    {
        std::array<int, 10> digits1;
        std::array<int, 10> digits2;
        int                 digits1_count = 0;
        int                 digits2_count = 0;

        for (; n1; n1 /= 10)
            digits1[digits1_count++] = n1 % 10;

        for (; n2; n2 /= 10)
            digits2[digits2_count++] = n2 % 10;

        for (int i1 = digits1_count - 1, i2 = digits2_count - 1; i1 >= 0 && i2 >= 0; i1--, i2--) {
            const int d1 = digits1[i1];
            const int d2 = digits2[i2];
            if (d1 < d2)
                return true;
            if (d1 > d2)
                return false;
        }
        return digits1_count <= digits2_count;
    };

    // Generate result
    const auto result = Solution().run(n);

    // Check result
    const bool correct_size = (result.size() == std::max(n, 0));
    bool       success      = correct_size;
    if (success) {
        for (int i = 1; i < result.size(); i++) {
            if (!compare(result[i - 1], result[i])) {
                success = false;
                break;
            }
        }
    }
    ASSERT_EX( success, to_string(n) + " -> " + to_string(result) +
        (!correct_size ? "\nWrong result size" : "") );
}

int main()
{
    test(-1);
    test(0);

    for (int i = 1; i <= 100; i++)
        test(i);

    test(101);
    test(109);
    test(110);
    test(111);
    test(119);
    test(123);
    test(190);
    test(198);
    test(199);
    test(891);
    test(899);
    test(900);
    test(909);
    test(998);
    test(999);
    test(1000);
    test(10000);
    test(100000);
    test(1203040);
    test(5000000);  // The maximum required in the problem

    return 0;
}

}
