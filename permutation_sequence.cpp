// Problem: https://leetcode.com/problems/permutation-sequence/

#include <vector>
#include <array>
#include <string>
#include "utils.h"

namespace permutation_sequence {

class Solution1 {
public:
    // Returns k-th permutation of the sequence "12...n".
    //
    // Time: O(n^2), Space: O(n)
    //
    std::string run(int n, int k)
    {
        if (n < 1 || n > 9 || k < 1)
            return {};

        int count = 1;
        for (int i = 2; i <= n; i++)
            count *= i;

        if (k > count)
            return {};

        // Idea:
        // For any given n > 0 and digits = {d1, d2, ..., dn}, there are (n-1)!
        // permutations starting with digit d1, (n-1)! starting with d2, etc.
        // So the k-th permutation starts with digits[i], i = floor((k-1) / (n-1)!),
        // 1 <= k <= n!. Then repeat the same for the rest n-1 digits:
        // k = k - i * (n-1)!, n = n - 1, digits = digits.without(i).
        std::vector<char> digits(n);
        for (int i = 0; i < n; i++)
            digits[i] = '1' + i;

        std::string result(n, 0);
        for (int i = 0; i < n; i++) {
            count /= (n - i);
            assert(count > 0);

            const int index = (k - 1) / count;
            assert(0 <= index && index < n);

            result[i] = digits[index];
            digits.erase(digits.begin() + index);

            k -= index * count;
            assert(k >= 0);
        }
        return result;
    }
};


// The same as Solution1, but stores digits in the integer and removes them
// in O(1) time.
//
// Warning: The actual performance may be similar to or even worse than the
// Solution1. It depends on the number of divisions in the code, and on the
// optimization. Also, the Solution1 can be adapted to an alphabet of the
// arbitrary size, while this one can't.

class Solution2 {
public:
    // Returns k-th permutation of the sequence "12...n".
    //
    // Time: O(n), Space: O(n)
    //
    std::string run(int n, int k)
    {
        if (n < 1 || n > 9 || k < 1)
            return {};

        int count = 1;
        for (int i = 2; i <= n; i++)
            count *= i;

        if (k > count)
            return {};

        int digits = 123456789;
        const std::array<int, 9> pows = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

        std::string result(n, 0);
        for (int i = 0; i < n; i++) {
            count /= (n - i);
            assert(count > 0);

            const int index = (k - 1) / count;
            assert(0 <= index && index < n);

            const int index_pow    = pows[pows.size() - index - i - 1];
            const int digits_left  = digits / index_pow;
            const int digits_right = digits % index_pow;
            result[i] = '0' + (digits_left % 10);
            digits = (digits_left / 10) * index_pow + digits_right;

            k -= index * count;
            assert(k >= 0);
        }
        return result;
    }
};


template <typename Solution>
void test()
{
    ASSERT(Solution().run(0, 0) == std::string());
    ASSERT(Solution().run(0, 1) == std::string());
    ASSERT(Solution().run(10, 1) == std::string());
    ASSERT(Solution().run(1, 0) == std::string());
    ASSERT(Solution().run(1, 1) == "1");
    ASSERT(Solution().run(1, 2) == std::string());
    ASSERT(Solution().run(2, 1) == "12");
    ASSERT(Solution().run(2, 2) == "21");
    ASSERT(Solution().run(3, 1) == "123");
    ASSERT(Solution().run(3, 2) == "132");
    ASSERT(Solution().run(3, 3) == "213");
    ASSERT(Solution().run(3, 4) == "231");
    ASSERT(Solution().run(3, 5) == "312");
    ASSERT(Solution().run(3, 6) == "321");
    ASSERT(Solution().run(3, 7) == std::string());
    ASSERT(Solution().run(9, 1) == "123456789");
    ASSERT(Solution().run(9, 40320) == "198765432");
    ASSERT(Solution().run(9, 40321) == "213456789");
    ASSERT(Solution().run(9, 362880) == "987654321");
    ASSERT(Solution().run(9, 362881) == std::string());

    // Test performance
    double elapsed_secs_avg = 0;

    for (int n = 1; n <= 9; n++) {
        int count = 1;
        for (int i = 2; i <= n; i++)
            count *= i;

        std::cout << "Run for " << count << " permutations, n = " << n << std::endl;
        clock_t start = clock();

        for (int i = 1; i <= count; i++)
            Solution().run(n, i);

        double elapsed_secs = double(clock() - start) / CLOCKS_PER_SEC;
        std::cout << "Elapsed: " << elapsed_secs << " seconds" << std::endl;

        elapsed_secs_avg += elapsed_secs / 9.0;
    }

    std::cout << "Elapsed avg: " << elapsed_secs_avg << " seconds" << std::endl;
    std::cout << std::endl;
}

int main()
{
    test<Solution1>();
    test<Solution2>();
    return 0;
}

}
