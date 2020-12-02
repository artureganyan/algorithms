// Problem: https://leetcode.com/problems/remove-k-digits/

#include "utils.h"
#include <string>

namespace remove_k_digits {

class Solution {
public:
    // Note: The num must contain only '0'-'9' characters. If the num is empty
    // or k < 0, returns the num. If the num contains leading zeros (initially
    // or after removing k digits), they are also removed. If the result is
    // empty string (when k >= num.size() or after removing leading zeros),
    // returns "0".
    //
    // Note: The original problem does not allow leading zeros for the num, but
    // this solution has no such restriction.
    //
    // Time: O(n), Space: O(n), n - number of characters
    //
    std::string run(const std::string& num, int k)
    {
        // Idea:
        // Let's move from the first (the most significant) to the last digit.
        // For each digit num[i], if num[i] > num[i+1] and we have not remove
        // enough digits, we must remove num[i]. Otherwise, no matter what we
        // would remove in [i+1, ...], the result would be greater. If num[i] < num[i+1],
        // we can not remove num[i] until we remove num[i+1], for the similar
        // reason. But this does not mean we must remove num[i+1] - there may
        // be better candidate in [i+2, ...]. If num[i] == num[i+1], there is
        // also no need to remove num[i] until we have num[i+1]. So, if
        // num[i] <= num[i+1], we skip num[i] but remember to check it if we
        // remove num[i+1]. If num[i] is the last digit and we have not remove
        // enough digits, then ... <= num[i-1] <= num[i], so we must remove
        // num[i]. Finally, after removing the required number of digits, we
        // should also remove the leading zeros which could occur if num[0] was
        // removed.

        if (!num.size())
            return {};

        if (k < 0)
            return num;

        if (num.size() <= k)
            return "0";

        // Remove target digits
        std::string result;
        result.reserve(num.size() - k);
        result += num.front();

        int remove = k;
        for (int i = 1; i < num.size();) {
            const auto c = num[i];
            if (result.back() > c && remove > 0) {
                result.pop_back();
                remove--;
            } else {
                result.push_back(c);
                i++;
            }
        }
        result.resize(result.size() - remove);
        remove = 0;

        // Remove leading zeros
        int leading_zeros = 0;
        while (result[leading_zeros] == '0')
            leading_zeros++;
        result = result.substr(leading_zeros);

        if (!result.size())
            return "0";
        return result;
    }
};

int main()
{
    const auto test = [](const std::string& s, int k, const std::string& expected)
    {
        ASSERT_EX( Solution().run(s, k) == expected,
            s + ", " + to_string(k) + " -> " + expected );
    };

    test({}, 0, {});

    for (int i = 0; i <= 9; i++) {
        const std::string s(1, '0' + i);
        test(s, 0, s);
        test(s, 1, "0");
    }

    test("10", 0, "10");
    test("10", 1, "0");
    test("10", 2, "0");

    test("11", 0, "11");
    test("11", 1, "1");
    test("11", 2, "0");

    test("12", 0, "12");
    test("12", 1, "1");
    test("21", 1, "1");
    test("12", 2, "0");

    test("100", 1, "0");
    test("100", 2, "0");
    test("100", 3, "0");

    test("101", 1, "1");
    test("101", 2, "0");
    test("110", 2, "0");
    test("101", 3, "0");

    test("123", 1, "12");
    test("132", 1, "12");
    test("312", 1, "12");
    test("123", 2, "1");
    test("132", 2, "1");
    test("312", 2, "1");
    test("123", 3, "0");

    test("253448067519", 1, "23448067519");
    test("253448067519", 2, "2344067519");
    test("253448067519", 3, "234067519");
    test("253448067519", 4, "23067519");
    test("253448067519", 5, "2067519");
    test("253448067519", 6, "67519");
    test("253448067519", 7, "6519");
    test("253448067519", 8, "519");
    test("253448067519", 9, "19");
    test("253448067519", 10, "1");
    test("253448067519", 11, "0");

    return 0;
}

}
