// Problem: https://leetcode.com/problems/evaluate-reverse-polish-notation/

#include <vector>
#include <list>
#include <string>
#include "utils.h"

namespace evaluate_reverse_polish_notation {

// The straightforward solution

class Solution {
public:
    // Note: The tokens must be a valid expression. Each token must be an
    // integer or one of the following operators: "+", "-", "*", "/".
    //
    // Time: O(n), Space: O(n), n - number of tokens
    //
    int run(const std::vector<std::string>& tokens)
    {
        if (!tokens.size())
            return 0;

        enum Operation {
            Operation_Undefined,
            Operation_Add,
            Operation_Minus,
            Operation_Multiply,
            Operation_Divide
        };

        std::list<std::string> tokens_list(tokens.begin(), tokens.end());

        for (auto it = tokens_list.begin(); it != tokens_list.end(); it++) {
            const std::string token = *it;

            // Determine the operation
            Operation operation = Operation_Undefined;
            if (token == "+") operation = Operation_Add;      else
            if (token == "-") operation = Operation_Minus;    else
            if (token == "*") operation = Operation_Multiply; else
            if (token == "/") operation = Operation_Divide;

            if (operation == Operation_Undefined)
                continue;

            // Do the operation
            const int operand2 = std::stoi(*(--it));
            const int operand1 = std::stoi(*(--it));

            int result = 0;
            if (operation == Operation_Add)      result = operand1 + operand2; else
            if (operation == Operation_Minus)    result = operand1 - operand2; else
            if (operation == Operation_Multiply) result = operand1 * operand2; else
            if (operation == Operation_Divide)   result = operand1 / operand2;

            // Replace the operation with the result
            for (int i = 0; i < 3; i++)
                it = tokens_list.erase(it);

            it = tokens_list.insert(it, std::to_string(result));
        }

        return std::stoi(tokens_list.front());
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
