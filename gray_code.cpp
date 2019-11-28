// Problem: https://leetcode.com/problems/gray-code/

#include <vector>
#include <limits>
#include <bitset>
#include <unordered_set>
#include "utils.h"

namespace gray_code {

class Solution {
public:
    // Time: O(n * 2^(n-1)), Space: O(2^n), n - number of bits
    //
    // Note: The time complexity is less than O(n * 2^n) because we don't
    // spend time on n*2^(n-1) zero bits. We also preallocate 2^n zero
    // integers, but this is not a bit-by-bit process, it requires only
    // O(2^n) time. So the total number of operations is about
    // n*2^(n-1) + 2^n, approximated by n*2^(n-1).
    //
    std::vector<int> run(int n)
    {
        if (n < 0 || n > std::numeric_limits<int>::digits)
            throw std::runtime_error("n must be within [0, int bit size]");

        if (n == 0)
            return {};

        // Suppose we have the Gray code of length n-1, i.e. 2^(n-1)
        // sequences. If add zero to each sequence, we will get the half of
        // the code of length n. If add one, we will get the second half,
        // except that it can't be directly appended to the first one. But,
        // if put its sequences in reverse order, it will remain the Gray code
        // and can be appended to the first half:
        //
        // 0 -> 0|0 -> 0|0 0  // 0 + {code of n-1}
        // 1    0|1    0|0 1  //
        //      ---    0|1 1  //
        //      1|1    0|1 0  //
        //      1|0    -----
        //             1|1 0  // 1 + {code of n-1 in reverse order}
        //             1|1 1  //
        //             1|0 1  //
        //             1|0 0  //
        //
        // And we can build this code by columns, writing 2^(n-1) zeros and
        // 2^(n-1) ones to the first column, 2^(n-2) zeros and 2*2^(n-2) ones
        // and 2^(n-2) zeros to the second one, and so on.

        std::vector<int> result(1 << n, 0);

        int mask       = 1;
        int block_size = result.size() / 2;

        for (int i = 0; i < n; i++) {
            for (int r = 0; r < result.size();) {
                r += block_size;                            // 0

                for (int r0 = r; r < r0 + block_size; r++)
                    result[r] |= mask;                      // 1

                if (r >= result.size())
                    break;

                for (int r0 = r; r < r0 + block_size; r++)
                    result[r] |= mask;                      // 1

                r += block_size;                            // 0
            }

            mask <<= 1;
            block_size /= 2;
        }

        return result;
    }
};


bool verify(const std::vector<int>& gray_code, int n)
{
    if (n <= 0)
        return gray_code.empty();

    if (gray_code.size() != (1 << n))
        return false;

    std::unordered_set<int> code_set;
    code_set.insert(gray_code.front());

    for (int i = 1; i < gray_code.size(); i++) {
        const int code      = gray_code[i];
        const int code_prev = gray_code[i - 1];

        std::bitset<32> difference(code ^ code_prev);
        if (difference.count() != 1)
            return false;

        if (code_set.find(code) != code_set.end())
            return false;
        code_set.insert(code);
    }

    return true;
}

int main()
{
    for (int n = 0; n <= 5; n++)
        ASSERT_EX( verify(Solution().run(n), n), to_string(n) );

    return 0;
}

}
