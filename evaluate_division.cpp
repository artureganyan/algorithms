// Problem: https://leetcode.com/problems/evaluate-division/

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

namespace evaluate_division {

// Note: For some reason, the original problem requires to return -1 for 
// unresolved query, which leads to ambiguity. It looks more convenient to
// return 0.

class Solution {
public:
    // Note: Each equation and query must be a vector of 2 elements:
    // {<var1>, <var2>}. The equations and values must have the same size. The
    // equations must not contradict each other. If both the equations and
    // queries are empty, returns {}. If some query can't be resolved, its
    // result is -1 (including when the variable is not present in the
    // equations, or division by 0 is required).
    //
    std::vector<double> run(const std::vector<std::vector<std::string>>& equations,
        const std::vector<double>& values,
        const std::vector<std::vector<std::string>>& queries)
    {
        // Idea:
        // If we need to find a/b but do not have it in the equations (and do
        // not have b/a), try to find some v such that a/v and v/b are known
        // or can be found in the same way. Then a/b = a/v * v/b:
        //
        // f(a, b) = a/b                if the equations contain (a, b) or (b, a), or
        //           f(a, v) * f(v, b)  if f(a, v) and f(v, b) can be resolved, or
        //           -1                 otherwise
        // 
        // To avoid infinite recursion when implementing this, we can exclude
        // each variable passed to f():
        //
        // f(a, b) = f(a, v) * f(v, b), v != a, v != b, let v = c
        // f(a, c) = f(a, v) * f(v, c), v != a, v != b, v != c
        // ...
        //
        // It is possible that e.g. f(a, v) for some v can only be resolved via
        // b that already excluded. But this means we have the shorter answer
        // for a/b: if f(a, c) is resolved as f(a, d) * f(d, b) * f(b, c), then
        // f(a, b) = f(a, d) * f(d, b), so d gives the answer.

        if (!equations.size() && !queries.size()) 
            return {};

        // Create equation map
        EquationMap equation_map;
        for (int i = 0; i < equations.size(); i++) {
            const auto& e     = equations[i];
            const auto& var1  = e[0];
            const auto& var2  = e[1];
            const auto  value = values[i];
            
            equation_map[var1][var2] = value;
            if (value != 0)
                equation_map[var2][var1] = 1.0 / value;
        }

        // Resolve queries
        std::vector<double> result;
        for (int i = 0; i < queries.size(); i++) {
            const auto& q    = queries[i];
            const auto& var1 = q[0];
            const auto& var2 = q[1];
            const auto  r    = findResult(equation_map, var1, var2);

            result.push_back(r.value);
        }
        return result;
    }

private:
    typedef std::unordered_map<std::string /*var1*/, std::unordered_map<std::string /*var2*/, double /*result*/>> EquationMap;

    struct Result
    {
        double value;
        bool   found;
    };

    // Returns var1 / var2 if the result can be found in the equations,
    // otherwise returns 0. The exclude_vars is the internal parameter which
    // must be set nullptr.
    //
    Result findResult(const EquationMap& equations, const std::string& var1, const std::string& var2,
        std::unordered_set<std::string>* exclude_vars = nullptr) const
    {
        // Check if (var1, var2) is directly present in the equations
        if (const auto vi1 = equations.find(var1); vi1 != equations.end()) {
            if (var1 == var2)
                return {1, true};

            if (const auto vi2 = vi1->second.find(var2); vi2 != vi1->second.end())
                return {vi2->second, true};
        }

        // Initialize exclude_vars
        std::unordered_set<std::string> exclude_vars_;
        if (!exclude_vars) {
            exclude_vars_ = {var1, var2};
            exclude_vars  = &exclude_vars_;
        }

        // Find var_i such that (var1, var_i) and (var_i, var2) can be resolved.
        // If found, return (var1, var_i) * (var_i, var2). Otherwise, the query
        // can't be resolved.
        for (const auto e : equations) {
            const auto& var = e.first;

            if (exclude_vars->find(var) != exclude_vars->end())
                continue;
            exclude_vars->insert(var);

            if (const auto r1 = findResult(equations, var1, e.first, exclude_vars); r1.found) {
                if (const auto r2 = findResult(equations, e.first, var2, exclude_vars); r2.found) {
                    return {r1.value * r2.value, true};
                }
            }
            
            exclude_vars->erase(var);
        }
        return {-1, false};
    }
};


int main()
{
    const auto test = [](const std::vector<std::vector<std::string>>& equations,
                         const std::vector<double>& values,
                         const std::vector<std::vector<std::string>>& queries,
                         const std::vector<double>& expected)
    {
        const auto result = Solution().run(equations, values, queries);
        
        ASSERT_EX( compare_sets(result, expected),
            to_string(equations) + ", " + to_string(values)  + ", " + to_string(queries) + " -> " +
            to_string(expected) );
    };

    test({}, {}, {}, {});
    
    test({{"", ""}}, {1}, {{"", ""}}, {1});
    test({{"a", "a"}}, {1}, {{"a", "a"}}, {1});
    test({{"a", "a"}}, {1}, {{"a", "b"}}, {-1});
    test({{"a", "a"}}, {1}, {{"b", "b"}}, {-1});

    test({{"a", "b"}}, {0}, {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {1, 0, 1, -1});
    test({{"a", "b"}}, {1}, {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {1, 1, 1, 1});
    test({{"a", "b"}}, {2}, {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {1, 2, 1.0/2, 1});
    test({{"a", "b"}}, {-1}, {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {1, -1, -1, 1});
    
    test({{"a", "b"}, {"b", "c"}}, {2, 4}, {{"a", "c"}, {"c", "a"}}, {8, 1.0/8});
    test({{"a", "b"}, {"b", "c"}}, {-1, 2}, {{"a", "c"}, {"c", "a"}}, {-2, -1.0/2});
    
    test({{"a", "b"}, {"b", "c"}, {"c", "d"}}, {2, 3, 4}, 
         {{"a", "d"}, {"d", "a"}, {"b", "d"}, {"d", "b"}},
         {24, 1.0/24, 12, 1/12.0});

    return 0;
}

}
