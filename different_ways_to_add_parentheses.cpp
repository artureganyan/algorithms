// Problem: https://leetcode.com/problems/different-ways-to-add-parentheses/

#include <vector>
#include <string>
#include "utils.h"

namespace different_ways_to_add_parentheses {

class Solution {
public:
    // Note: The expression must be valid and contain only the following
    // operators: +, -, *. If the expression is empty, returns empty
    // result.
    //
    std::vector<int> run(const std::string& expression)
    {
        const Expression e = stringToExpression(expression);
        return evaluateExpression(e, 0, e.size());
    }

private:
    union Token
    {
        char operation;
        int  operand;
    };

    typedef std::vector<Token> Expression;

    Expression stringToExpression(const std::string& s) const
    {
        Expression result;

        std::stringstream s_stream(s);
        while (s_stream) {
            Token token;
            if (s_stream >> token.operand)
                result.push_back(token);

            token.operation = 0;
            if (s_stream >> token.operation)
                result.push_back(token);
        }

        return result;
    }

    // Evaluates expression for all possible forms such that each operator has
    // its left and right operands surrounded with parentheses:
    //
    // expression = x1 op1 x2 op2 x3 ... xn opn xn+1
    //
    // 1. (x1) op1 ((x2) op2 ((x3) op3 ...))
    // 2. ((x1) op1 (x2)) op2 ((x3) op3 ...)
    // 3. (((x1) op1 (x2)) op2 (x3)) op3 (...)
    // 4. ((x1) op1 ((x2) op2 (x3))) op3 (...)
    // ...
    //
    // Note: The expression must be valid, and expression[start] must be an
    // operand.
    //
    std::vector<int> evaluateExpression(const std::vector<Token>& expression, int start, int end) const
    {
        if (start >= end || start >= expression.size() || end > expression.size())
            return {};

        if (end - start == 1)
            return {expression[start].operand};

        std::vector<int> result;

        for (int op = start + 1; op <= end - 2; op += 2) {
            const std::vector<int> operands1 = evaluateExpression(expression, start,  op);
            const std::vector<int> operands2 = evaluateExpression(expression, op + 1, end);
            const char             operation = expression[op].operation;

            for (const int o1 : operands1) {
                for (const int o2 : operands2) {
                    result.push_back(evaluateOperation(operation, o1, o2));
                }
            }
        }

        return result;
    }

    int evaluateOperation(char operation, int operand1, int operand2) const
    {
        switch (operation) {
            case '+': return operand1 + operand2;
            case '-': return operand1 - operand2;
            case '*': return operand1 * operand2;
            default : throw std::runtime_error("Unsupported operation");
        }
    }
};


int main()
{
    ASSERT( compare_sets(Solution().run(std::string()), {}) );

    ASSERT( compare_sets(Solution().run("0"), {0}) );
    ASSERT( compare_sets(Solution().run("1"), {1}) );
    ASSERT( compare_sets(Solution().run("-1"), {-1}) );

    ASSERT( compare_sets(Solution().run("0+0"), {0}) );
    ASSERT( compare_sets(Solution().run("0+1"), {1}) );
    ASSERT( compare_sets(Solution().run("1+0"), {1}) );
    ASSERT( compare_sets(Solution().run("1+1"), {2}) );
    ASSERT( compare_sets(Solution().run("1+10"), {11}) );
    ASSERT( compare_sets(Solution().run("10+1"), {11}) );
    ASSERT( compare_sets(Solution().run("-1+1"), {0}) );
    ASSERT( compare_sets(Solution().run("-0+1"), {1}) );

    ASSERT( compare_sets(Solution().run("0-0"), {0}) );
    ASSERT( compare_sets(Solution().run("0-1"), {-1}) );
    ASSERT( compare_sets(Solution().run("1-0"), {1}) );
    ASSERT( compare_sets(Solution().run("1-1"), {0}) );
    ASSERT( compare_sets(Solution().run("1-10"), {-9}) );
    ASSERT( compare_sets(Solution().run("10-1"), {9}) );

    ASSERT( compare_sets(Solution().run("0*0"), {0}) );
    ASSERT( compare_sets(Solution().run("0*1"), {0}) );
    ASSERT( compare_sets(Solution().run("1*0"), {0}) );
    ASSERT( compare_sets(Solution().run("1*1"), {1}) );
    ASSERT( compare_sets(Solution().run("10*1"), {10}) );
    ASSERT( compare_sets(Solution().run("1*10"), {10}) );

    ASSERT( compare_sets(Solution().run("1+2*3-4"), {-1, 3, -3, 5, 3}) );

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::max();

    ASSERT( compare_sets(Solution().run(to_string(MAX) + "+0"), {MAX}) );
    ASSERT( compare_sets(Solution().run(to_string(MAX) + "-1"), {MAX-1}) );
    ASSERT( compare_sets(Solution().run(to_string(MAX) + "*1"), {MAX}) );
    ASSERT( compare_sets(Solution().run(to_string(MIN) + "+1"), {MIN+1}) );
    ASSERT( compare_sets(Solution().run(to_string(MIN) + "-0"), {MIN}) );
    ASSERT( compare_sets(Solution().run(to_string(MIN) + "*1"), {MIN}) );

    return 0;
}

}
