// Problem: https://leetcode.com/problems/evaluate-division/

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

namespace evaluate_division {

// Straightforward recursive solution

class Solution {
public:
    // Note: Each equation and query must be a vector of 2 elements:
    // {"var1", "var2"}. The equations and values must have the same size. The
    // equations must not contradict each other. If both the equations and
    // queries are empty, returns {}. If some query can't be resolved, its
    // result is -1.
    //
    std::vector<double> run(const std::vector<std::vector<std::string>>& equations,
        const std::vector<double>& values,
        const std::vector<std::vector<std::string>>& queries)
    {
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
            
            std::unordered_set<std::string> exclude_vars = {var1, var2};
            result.push_back(findResult(equation_map, var1, var2, exclude_vars));
        }
        return result;
    }

private:
    typedef std::unordered_map<std::string /*var1*/, std::unordered_map<std::string /*var2*/, double /*result*/>> EquationMap;

    double findResult(const EquationMap& equations, const std::string& var1, const std::string& var2,
        std::unordered_set<std::string>& exclude_vars) const
    {
        // Check if (var1, var2) is directly presented in the equations
        if (const auto vi1 = equations.find(var1); vi1 != equations.end()) {
            if (var1 == var2)
                return 1;

            if (const auto vi2 = vi1->second.find(var2); vi2 != vi1->second.end())
                return vi2->second;
        }

        // Find var_i such that (var1, var_i) and (var_i, var2) can be resolved.
        // If it exists, return (var1, var_i) * (var_i, var2).
        for (const auto e : equations) {
            const auto& var = e.first;

            if (exclude_vars.find(var) != exclude_vars.end())
                continue;
            exclude_vars.insert(var);

            const auto r1 = findResult(equations, var1, e.first, exclude_vars);
            if (r1 != -1) {
                const auto r2 = findResult(equations, e.first, var2, exclude_vars);
                if (r2 != -1)
                    return r1 * r2;
            }
            
            exclude_vars.erase(var);
        }
        return -1;
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
    
    test({{"a", "b"}, {"b", "c"}}, {2, 4}, {{"a", "c"}, {"c", "a"}}, {8, 1.0/8});
    
    test({{"a", "b"}, {"b", "c"}, {"c", "d"}}, {2, 3, 4}, 
         {{"a", "d"}, {"d", "a"}, {"b", "d"}, {"d", "b"}},
         {24, 1.0/24, 12, 1/12.0});

    return 0;
}

}
