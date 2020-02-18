// Problem: https://leetcode.com/problems/basic-calculator-ii/

#include <string>
#include <vector>
#include <limits>
#include "utils.h"

namespace basic_calculator_2 {

class Solution {
public:
    // Note: The expression must be valid and contain only the following
    // characters: '0'-'9', '+', '-', '*', '/', ' '. For an empty expression,
    // returns 0.
    //
    // Time: O(n), Space: O(n), n - number of characters
    //
    int run(const std::string& s)
    {
        if (!s.size())
            return 0;

        struct Operation
        {
            int  left_operand;
            char type;
        };

        std::vector<Operation> operations;

        auto       i   = s.cbegin();
        const auto end = s.cend();

        while (i != end) {
            Operation op;
            op.left_operand = readOperand(i, end);
            op.type         = readOperator(i, end);

            while (operations.size()) {
                const Operation& op_last = operations.back();
                if (getOperationPriority(op_last.type) >= getOperationPriority(op.type)) {
                    op.left_operand = doOperation(op_last.type, op_last.left_operand, op.left_operand);
                    operations.pop_back();
                } else {
                    break;
                }
            }

            operations.push_back(op);
        }

        assert(operations.size() == 1);
        const Operation& result = operations.front();
        return result.left_operand;
    }

private:
    int readOperand(std::string::const_iterator& begin, const std::string::const_iterator& end) const
    {
        std::string s;

        for (; begin != end; begin++) {
            const char c = *begin;
            if (std::isspace(c)) {
                if (!s.size())
                    continue;
                break;
            }
            if (!std::isdigit(c))
                break;
            s += c;
        }

        return std::atoi(s.c_str());
    }

    char readOperator(std::string::const_iterator& begin, const std::string::const_iterator& end) const
    {
        while (begin != end) {
            const char c = *(begin++);
            if (std::isspace(c))
                continue;
            return c;
        }
        return 0;
    }

    int doOperation(char operation, int operand1, int operand2) const
    {
        switch (operation) {
            case '+': return operand1 + operand2;
            case '-': return operand1 - operand2;
            case '*': return operand1 * operand2;
            case '/': return operand1 / operand2;
        }
        return operand1;
    }

    int getOperationPriority(char operation) const
    {
        switch (operation) {
            case '+':
            case '-': return 1;
            case '*':
            case '/': return 2;
        }
        return 0;
    }
};

int main()
{
    ASSERT( Solution().run("") == 0 );

    ASSERT( Solution().run("0") == 0 );
    ASSERT( Solution().run("1") == 1 );
    ASSERT( Solution().run("123") == 123 );

    ASSERT( Solution().run("0+1") == 1 );
    ASSERT( Solution().run("1+0") == 1 );
    ASSERT( Solution().run("1+1") == 2 );
    ASSERT( Solution().run("1+2") == 3 );
    ASSERT( Solution().run("1+10") == 11 );
    ASSERT( Solution().run("10+1") == 11 );

    ASSERT( Solution().run("0-1") == -1 );
    ASSERT( Solution().run("1-0") == 1 );
    ASSERT( Solution().run("1-1") == 0 );
    ASSERT( Solution().run("1-2") == -1 );
    ASSERT( Solution().run("10-1") == 9 );
    ASSERT( Solution().run("1-10") == -9 );

    ASSERT( Solution().run("0*1") == 0 );
    ASSERT( Solution().run("1*0") == 0 );
    ASSERT( Solution().run("1*1") == 1 );
    ASSERT( Solution().run("1*2") == 2 );
    ASSERT( Solution().run("2*1") == 2 );
    ASSERT( Solution().run("10*1") == 10 );

    ASSERT( Solution().run("0/1") == 0 );
    ASSERT( Solution().run("1/1") == 1 );
    ASSERT( Solution().run("1/2") == 0 );
    ASSERT( Solution().run("2/1") == 2 );
    ASSERT( Solution().run("10/1") == 10 );

    ASSERT( Solution().run("1+2-3") == 0 );
    ASSERT( Solution().run("2-3+1") == 0 );

    ASSERT( Solution().run("1+2*3") == 7 );
    ASSERT( Solution().run("2*3+1") == 7 );

    ASSERT( Solution().run("4+4/2") == 6 );
    ASSERT( Solution().run("4/2+4") == 6 );

    ASSERT( Solution().run("1-2*3") == -5 );
    ASSERT( Solution().run("2*3-1") == 5 );

    ASSERT( Solution().run("4-4/2") == 2 );
    ASSERT( Solution().run("4/2-4") == -2 );

    ASSERT( Solution().run("2*8/4") == 4 );
    ASSERT( Solution().run("8/4*2") == 4 );

    ASSERT( Solution().run("1+2*8/4-2") == 3 );

    ASSERT( Solution().run(" ") == 0 );
    ASSERT( Solution().run("  ") == 0 );
    ASSERT( Solution().run("1 ") == 1 );
    ASSERT( Solution().run(" 1") == 1 );
    ASSERT( Solution().run(" 1 ") == 1 );
    ASSERT( Solution().run(" 1 + 2 ") == 3 );
    ASSERT( Solution().run(" 1  +  2 ") == 3 );

    const int         MAX     = std::numeric_limits<int>::max();
    const std::string MAX_STR = std::to_string(MAX);

    ASSERT( Solution().run(MAX_STR) == MAX );
    ASSERT( Solution().run(MAX_STR + "+0") == MAX );
    ASSERT( Solution().run(MAX_STR + "-0") == MAX );
    ASSERT( Solution().run(MAX_STR + "*0") == 0 );
    ASSERT( Solution().run(MAX_STR + "*1") == MAX );
    ASSERT( Solution().run(MAX_STR + "/1") == MAX );
    ASSERT( Solution().run(MAX_STR + "-1") == MAX-1 );
    ASSERT( Solution().run(MAX_STR + "/2") == MAX/2 );
    ASSERT( Solution().run(MAX_STR + "-" + MAX_STR) == 0 );
    ASSERT( Solution().run(MAX_STR + "/" + MAX_STR) == 1 );

    return 0;
}

}
