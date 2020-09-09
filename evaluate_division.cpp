// Problem: https://leetcode.com/problems/evaluate-division/

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

namespace evaluate_division {

// Note: For some reason, the original problem requires to return -1 for 
// unresolved query, which leads to ambiguity. It looks more convenient to
// return 0.
//
// Note: Each equation and query must be a vector of 2 elements:
// {<var1>, <var2>}. The equations and values must have the same size. The
// equations must not contradict each other. If both the equations and
// queries are empty, empty result is returned. If some query can't be
// resolved, its result is -1 (including when the variable is not present
// in the equations, or division by 0 is required).


// Recursive solution

class Solution1 {
public:
    std::vector<double> run(const std::vector<std::vector<std::string>>& equations,
        const std::vector<double>& values,
        const std::vector<std::vector<std::string>>& queries)
    {
        // Idea:
        // If we need to find a/b but do not have it in the equations (and do
        // not have b/a), try to find some v such that a/v and v/b are known
        // or can be found in the same way. Then a/b = a/v * v/b:
        //
        // f(a, b) = a/b                if the equations contain a/b or b/a, or
        //           f(a, v) * f(v, b)  if f(a, v) and f(v, b) can be resolved, or
        //           -1                 otherwise
        // 
        // To avoid infinite recursion when implementing this, we can exclude
        // each variable passed to f():
        //
        // f(a, b) = f(a, v) * f(v, b), v != a and v != b, e.g. let v = c
        // f(a, c) = f(a, v) * f(v, c), v != a and v != b and v != c
        // ...
        //
        // It is possible that e.g. f(a, v) for some v can only be resolved via
        // b that was already excluded. But this means we have the shorter answer
        // for a/b: if f(a, c) is resolved as f(a, d) * f(d, b) * f(b, c), then
        // f(a, b) = f(a, d) * f(d, b), so d gives the answer.

        if (!equations.size() && !queries.size()) 
            return {};

        // Create equation map
        EquationMap equation_map;
        for (int i = 0; i < equations.size(); i++) {
            const auto& e    = equations[i];
            const auto& var1 = e[0];
            const auto& var2 = e[1];
            const auto  v    = values[i];
            
            equation_map[var1][var2] = v;
            if (v != 0) {
                equation_map[var2][var1] = 1.0 / v;
                equation_map[var1][var1] = 1.0;
            }
            equation_map[var2][var2] = 1.0;
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
    typedef std::unordered_map<std::string /*var1*/,
            std::unordered_map<std::string /*var2*/, double /*result*/>> EquationMap;

    struct Result
    {
        double value;
        bool   valid;
    };

    Result findResult(const EquationMap& equations, const std::string& var1, const std::string& var2,
        std::unordered_set<std::string>* vars = nullptr) const
    {
        // Check if var1/var2 is directly present in the equations
        if (const auto vi1 = equations.find(var1); vi1 != equations.end()) {
            const auto& var1_equations = vi1->second;
            if (const auto vi2 = var1_equations.find(var2); vi2 != var1_equations.end())
                return {vi2->second, true};
        }

        // Initialize vars if needed
        std::unordered_set<std::string> vars_;
        if (!vars) {
            for (const auto e : equations)
                vars_.insert(e.first);    
            vars_.erase(var1);
            vars_.erase(var2);
            vars = &vars_;
        }

        // Find var_i such that var1/var_i and var_i/var2 can be resolved. If
        // found, return var1/var_i * var_i/var2. Otherwise, the query can't
        // be resolved.
        for (const auto v : *vars) {
            vars->erase(v);
            if (const auto r1 = findResult(equations, var1, v, vars); r1.valid) {
                if (const auto r2 = findResult(equations, v, var2, vars); r2.valid)
                    return {r1.value * r2.value, true};
            }
            vars->insert(v);
        }
        return {-1, false};
    }
};


// Search on graph

class Solution2 {
public:
    std::vector<double> run(const std::vector<std::vector<std::string>>& equations,
        const std::vector<double>& values,
        const std::vector<std::vector<std::string>>& queries)
    {
        if (!equations.size() || !queries.size())
            return {};

        // Create equation map
        EquationMap equation_map;
        for (int i = 0; i < equations.size(); i++) {
            const auto& e    = equations[i];
            const auto& var1 = e[0];
            const auto& var2 = e[1];
            const auto  v    = values[i];
            
            equation_map[var1][var2] = v;
            if (v != 0) {
                equation_map[var2][var1] = 1.0 / v;
                equation_map[var1][var1] = 1.0;
            }
            equation_map[var2][var2] = 1.0;
        }

        // Create query map
        QueryMap query_map;
        for (int i = 0; i < queries.size(); i++) {
            const auto& q    = queries[i];
            const auto& var1 = q[0];
            const auto& var2 = q[1];

            query_map[var1].insert(var2);
        }

        // Resolve queries
        for (const auto& q : query_map)
            findResult(equation_map, query_map, q.first);

        // Collect result
        std::vector<double> result;
        for (int i = 0; i < queries.size(); i++) {
            const auto& q    = queries[i];
            const auto& var1 = q[0];
            const auto& var2 = q[1];

            double value = -1;
            if (const auto vi1 = equation_map.find(var1); vi1 != equation_map.end()) {
                if (const auto vi2 = vi1->second.find(var2); vi2 != vi1->second.end())
                    value = vi2->second;
            }
            result.push_back(value);
        }
        return result;
    }

private:
    typedef std::unordered_map<std::string /*var1*/,
            std::unordered_map<std::string /*var2*/, double /*result*/>> EquationMap;

    typedef std::unordered_map<std::string /*var1*/,
            std::unordered_set<std::string /*var2*/>> QueryMap;

    // Resolves the queries which have the source_var as a dividend, using the
    // equations and writing results into it. For each v1 / v2, the equations
    // must contain v2 / v2 == 1, and, if v1 != 0, v1 / v1 == 1 and v2 / v1. If
    // some query can't be resolved, its result is not written.
    //
    void findResult(EquationMap& equations, const QueryMap& queries, const std::string& source_var) const
    {
        typedef std::pair<std::string /*var*/, double /*source_var / var*/> NextVar;

        std::deque<NextVar>             next_vars;
        std::unordered_set<std::string> visited_vars;
        std::unordered_set<std::string> resolved_queries;

        // source_var / source_var is 1 if source_var != 0, otherwise it's -1
        if (const auto si = equations.find(source_var); si != equations.end()) {
            if (const auto ri = si->second.find(source_var); ri != si->second.end())
                next_vars.push_back({source_var, ri->second}); else
                next_vars.push_back({source_var, -1});
        } else {
            return;
        }

        while (next_vars.size()) {
            const auto         next    = next_vars.front();
            const std::string& var1    = next.first;
            const double       result1 = next.second;

            next_vars.pop_front();
            visited_vars.insert(var1);

            for (const auto vi : equations[var1]) {
                const std::string& var2 = vi.first;
                if (visited_vars.find(var2) != visited_vars.end())
                    continue;

                const double result2 = vi.second;
                next_vars.push_back({var2, result1 * result2});
            }

            if (queries.find(var1) == queries.end())
                continue;

            equations[source_var][var1] = result1;

            resolved_queries.insert(var1);
            if (resolved_queries.size() == queries.size())
                return;
        }
    }
};


template <typename Solution>
void test()
{
    const auto test = [](const std::vector<std::vector<std::string>>& equations,
                         const std::vector<double>& values,
                         const std::vector<std::vector<std::string>>& queries,
                         const std::vector<double>& expected)
    {
        const auto result = Solution().run(equations, values, queries);
        
        ASSERT_EX( result == expected,
            to_string(equations) + ", " + to_string(values)  + ", " + to_string(queries) + " -> " +
            to_string(expected) );
    };

    test({}, {}, {}, {});
    
    test({{"", ""}}, {1}, {{"", ""}}, {1});
    
    test({{"a", "a"}}, {1}, {{"a", "a"}}, {1});
    test({{"a", "a"}}, {1}, {{"a", "b"}}, {-1});
    test({{"a", "a"}}, {1}, {{"b", "a"}}, {-1});
    test({{"a", "a"}}, {1}, {{"b", "b"}}, {-1});

    test({{"a", "b"}}, {0},  {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {-1, 0, -1, 1});
    test({{"a", "b"}}, {1},  {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {1, 1, 1, 1});
    test({{"a", "b"}}, {2},  {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {1, 2, 1.0/2, 1});
    test({{"a", "b"}}, {-1}, {{"a", "a"}, {"a", "b"}, {"b", "a"}, {"b", "b"}}, {1, -1, -1, 1});
    
    test({{"a", "b"}, {"b", "c"}}, {2, 4}, {{"a", "c"}, {"c", "a"}}, {8, 1.0/8});
    test({{"a", "b"}, {"b", "c"}}, {-1, 2}, {{"a", "c"}, {"c", "a"}}, {-2, -1.0/2});
    
    test({{"a", "b"}, {"b", "c"}, {"c", "d"}}, {2, 3, 4}, 
         {{"a", "d"}, {"d", "a"}, {"b", "d"}, {"d", "b"}},
         {24, 1.0/24, 12, 1/12.0});
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
