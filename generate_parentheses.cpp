// Problem: https://leetcode.com/problems/generate-parentheses/

#include <vector>
#include "utils.h"

namespace generate_parentheses {

class Solution
{
public:
    // Idea:
    // If {n} is a set of all combinations for n parentheses, it includes the following sets:
    // "()" + {n - 1}
    // "({1})" + {n - 2}
    // "({2})" + {n - 3}
    // ...
    // "({n - 1})" + {1}
    //
    // Note: for n = 0 returns {""}
    //
    std::vector<std::string> run(int n)
    {
        if (n == 0)
            return {std::string()};

        std::vector<std::string> result;

        for (int p = 1; p <= n; p++) {
            std::vector<std::string> inner = run2(p - 1);
            std::vector<std::string> outer = run2(n - p);
            for (size_t ii = 0; ii < inner.size(); ii++) {
                for (size_t io = 0; io < outer.size(); io++) {
                    result.push_back("(" + inner[ii] + ")" + outer[io]);
                }
            }
        }

        return result;
    }

    // Idea:
    // At each step, we have current string, n open parentheses and m close parentheses.
    // And we can do only 3 next steps:
    // 1. If n > 0, add 1 open parenthesis and repeat with n - 1, m + 1.
    // 2. If m > 0, add 1 close parenthesis and repeat with n, m - 1.
    // 3. If n == 0 and m == 0, add current string to result.
    //
    // Note: Trying to reduce reallocations, we use the same string and truncate
    // it back to the initial size after each step.
    //
    void generate(std::vector<std::string>& result, std::string& s, int count_open, int count_close)
    {
        // String is finished
        if (!count_open && !count_close) {
            result.push_back(s);
            return;
        }
        // Continue with open parenthesis
        const size_t str_size = s.size();
        if (count_open) {
            std::string next_str = s + "(";
            generate(result, next_str, count_open - 1, count_close + 1);
        }
        // Continue with close parenthesis
        s.resize(str_size);
        if (count_close) {
            std::string next_str = s + ")";
            generate(result, next_str, count_open, count_close - 1);
        }
    }

    // Note: for n = 0 returns {""}
    //
    std::vector<std::string> run2(int n)
    {
        std::vector<std::string> result;
        std::string s;
        generate(result, s, n, 0);
        return result;
    }

};

int main()
{
    auto methods = {&Solution::run, &Solution::run2};

    for (auto method : methods) {
        ASSERT(compare_sets((Solution().*method)(0), {""}));
        ASSERT(compare_sets((Solution().*method)(1), {"()"}));
        ASSERT(compare_sets((Solution().*method)(2), {"()()", "(())"}));
        ASSERT(compare_sets((Solution().*method)(3), {"((()))", "(()())", "(())()", "()(())", "()()()"}));
        ASSERT(compare_sets((Solution().*method)(4), {"(((())))", "((()()))", "((())())", "((()))()", "(()(()))", "(()()())", "(()())()", "(())(())", "(())()()", "()((()))", "()(()())", "()(())()", "()()(())", "()()()()"}));
    }

    return 0;
}

}
