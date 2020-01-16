// Problem: https://leetcode.com/problems/evaluate-reverse-polish-notation/

#include <vector>
#include <string>
#include "utils.h"

namespace evaluate_reverse_polish_notation {

class Solution {
public:
    // Note: The tokens must represent a valid expression. Each token must be
    // an integer or one of the following operators: "+", "-", "*", "/".
    //
    // Time: O(n), Space: O(n), n - number of tokens
    //
    int run(const std::vector<std::string>& tokens)
    {
        if (!tokens.size())
            return 0;

        std::vector<int> operands;

        for (const std::string& token : tokens) {
            const int i_op = (int) operands.size() - 1;

            int result = 0;
            if (token == "+") {
                result = operands[i_op - 1] + operands[i_op];
            } else if (token == "-") {
                result = operands[i_op - 1] - operands[i_op];
            } else if (token == "*") {
                result = operands[i_op - 1] * operands[i_op];
            } else if (token == "/") {
                result = operands[i_op - 1] / operands[i_op];
            } else {
                operands.push_back(std::stoi(token));
                continue;
            }

            operands.pop_back();
            operands.back() = result;
        }

        return operands.back();
    }
};


void test(const std::vector<std::string>& expression, int expected)
{
    ASSERT_EX( Solution().run(expression) == expected,
        to_string(expression) + " -> " + to_string(expected) );
}

int main()
{
    test({}, 0);

    test({"0", "0", "+"}, 0);
    test({"0", "0", "-"}, 0);
    test({"0", "0", "*"}, 0);
    test({"+0", "-0", "+"}, 0);

    test({"1", "-2", "+"}, -1);
    test({"1", "-2", "-"}, 3);
    test({"1", "-2", "*"}, -2);
    test({"1", "-2", "/"}, 0);

    test({"4", "-3", "2", "+", "+"}, 3);
    test({"4", "-3", "2", "+", "-"}, 5);
    test({"4", "-3", "2", "+", "*"}, -4);
    test({"4", "-3", "2", "+", "/"}, -4);
    test({"4", "-3", "2", "*", "*"}, -24);
    test({"4", "-3", "2", "*", "+"}, -2);
    test({"4", "-3", "2", "*", "-"}, 10);
    test({"4", "-3", "2", "*", "/"}, 0);
    test({"4", "-3", "2", "/", "/"}, -4);
    test({"4", "-3", "2", "/", "*"}, -4);
    test({"4", "-3", "2", "/", "+"}, 3);
    test({"4", "-3", "2", "/", "-"}, 5);
    test({"4", "-3", "2", "-", "-"}, 9);
    test({"4", "-3", "2", "-", "+"}, -1);
    test({"4", "-3", "2", "-", "*"}, -20);
    test({"4", "-3", "2", "-", "/"}, 0);

    test({"4", "10", "+", "-1", "2", "3", "*", "-", "/"}, -2);

    return 0;
}

}
