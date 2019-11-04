// Problem: https://leetcode.com/problems/powx-n/

#include <vector>
#include <math.h>
#include "utils.h"

namespace pow_x_n {

class Solution {
public:
    // Time: O(log(n)), Space: O(log(n)), n - power
    double run(double x, int n)
    {
        if (n == 0)
            return 1;

        std::vector<double> powers = {x};
        int power = 1;
        int next_power = 2;
        int n_abs = std::abs(n);

        // Double the power until we get the target power or the closest one:
        // x, x^2, x^4, ..., x^power
        while (next_power <= n_abs) {
            x *= x;
            power = next_power;
            next_power *= 2;
            powers.push_back(x);
        }
        next_power /= 2;

        // Multiply by previous results until we get the target power:
        // x^power * x^(power / 2), x^(power / 4), ..., x
        while (power < n_abs) {
            powers.pop_back();
            next_power /= 2;
            if (power + next_power <= n_abs) {
                x *= powers.back();
                power += next_power;
            }
        }
        assert(power == n_abs);

        // Check the target sign
        if (n < 0)
            return 1.0 / x;
        return x;
    }
};

inline bool compare(double a, double b)
{
    if (std::isinf(a) == std::isinf(b) && std::signbit(a) == std::signbit(b))
        return true;

    return std::abs(a - b) < 1e-12;
}

bool test(double value, int power)
{
    const double result   = Solution().run(value, power);
    const double expected = std::pow(value, power);

    std::cout << "" << value << "^" << power << " = " << result
              << ", expected " << expected << std::endl;

    return compare(result, expected);
}

int main()
{
    ASSERT(test(0, 0));
    ASSERT(test(0, 1));
    ASSERT(test(1, 0));
    ASSERT(test(1, 1));
    ASSERT(test(1, 2));
    ASSERT(test(-1, 0));
    ASSERT(test(-1, 1));
    ASSERT(test(-1, 2));
    ASSERT(test(1.5, 0));
    ASSERT(test(1.5, 1));
    ASSERT(test(1.5, 2));
    ASSERT(test(1.5, 3));
    ASSERT(test(1.5, -1));
    ASSERT(test(1.5, -2));
    ASSERT(test(1.5, -3));

    int    powers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 16, 17, 127, 128, 129, 1000, 10000};
    double values[] = {2};

    // Values
    for (int v = 0; v < sizeof(values) / sizeof(values[0]); v++) {
        // Value signs
        for (int value_sign = 1; value_sign >= -1; value_sign -= 2) {
            double value = values[v] * value_sign;
            std::cout << "value = " << value << std::endl;

            // Power signs
            for (int power_sign = 1; power_sign >= -1; power_sign -= 2) {
                // Powers
                for (int p = 0; p < sizeof(powers) / sizeof(powers[0]); p++) {
                    const int power = powers[p] * power_sign;
                    ASSERT(test(value, power));
                }
            }
            std::cout << std::endl;
        }
    }

    return 0;
}

}
