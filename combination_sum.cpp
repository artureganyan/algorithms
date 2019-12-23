// Problem: https://leetcode.com/problems/combination-sum/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace combination_sum {

class Solution1 {
public:
    std::vector<std::vector<int>> run(const std::vector<int>& candidates, int target)
    {
        std::vector<int> correct_candidates = candidates;
        prepareCandidates(correct_candidates, target);
        return generateCombinations(correct_candidates, target);
    }

private:
    // Remove incorrect and duplicated values from the candidates
    //
    // Note: As a side effect, current implementation sorts the candidates,
    // but this is not necessary.
    //
    void prepareCandidates(std::vector<int>& candidates, int target)
    {
        auto is_wrong_candidate = [&](int c) { return c < 0 || c > target; };
        candidates.erase(std::remove_if(candidates.begin(), candidates.end(), is_wrong_candidate), candidates.end());
        std::sort(candidates.begin(), candidates.end());
        candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());
    }

    // Straightforward recursive way: for each candidate, remove it from the
    // target and repeat the same for the new target.
    //
    std::vector<std::vector<int>> generateCombinations(const std::vector<int>& candidates, int target) const
    {
        // Important: return 1 empty result for target == 0, but no result for the wrong input
        if (target == 0)
            return {{}};

        if (!candidates.size() || target <= 0)
            return {};

        // Generate combinations
        std::vector<std::vector<int>> result;

        for (int i = 0; i < candidates.size(); i++) {
            const int value = candidates[i];
            std::vector<std::vector<int>> combinations = generateCombinations(candidates, target - value);
            for (std::vector<int>& combination : combinations) {
                combination.push_back(value);
                result.emplace_back(combination);
            }
        }

        // Remove duplicates
        for (std::vector<int>& combination : result) {
            std::sort(combination.begin(), combination.end());
        }
        std::sort(result.begin(), result.end());
        result.erase(std::unique(result.begin(), result.end()), result.end());

        return result;
    }
};


class Solution2 {
public:
    std::vector<std::vector<int>> run(const std::vector<int>& candidates, int target)
    {
        std::vector<int> correct_candidates = candidates;
        prepareCandidates(correct_candidates, target);
        return generateCombinations(correct_candidates, 0, target);
    }

private:
    // Remove incorrect and duplicated values from the candidates
    //
    // Note: As a side effect, current implementation sorts the candidates,
    // but this is not necessary.
    //
    void prepareCandidates(std::vector<int>& candidates, int target)
    {
        auto is_wrong_candidate = [&](int value) { return value < 0 || value > target; };
        candidates.erase(std::remove_if(candidates.begin(), candidates.end(), is_wrong_candidate), candidates.end());
        std::sort(candidates.begin(), candidates.end());
        candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());
    }

    // This solution is optimized to not generate duplicates:
    // 1. For the first candidate, calculate how much it's included in the target:
    //    count = target / candidate
    // 2. For each c = 0, 1, 2, ..., count, do step 1 with
    //    target = target - candidate * c and
    //    candidates excluding the current one
    //
    // Note: Candidates must have no duplicated values, otherwise it may generate
    // duplicated results.
    //
    std::vector<std::vector<int>> generateCombinations(const std::vector<int>& candidates, int begin, int target) const
    {
        // Important: return 1 empty result for target == 0, but no result for the wrong input
        if (target == 0)
            return {{}};

        if (!candidates.size() || begin >= candidates.size() || target < 0)
            return {};

        // Generate combinations
        std::vector<std::vector<int>> result;

        const int value = candidates[begin];
        const int count = target / value;

        for (int c = 0; c <= count; c++) {
            std::vector<int> value_repeated(c, value);
            std::vector<std::vector<int>> combinations = generateCombinations(candidates, begin + 1, target - value * c);
            for (std::vector<int>& combination : combinations) {
                combination.insert(combination.end(), value_repeated.begin(), value_repeated.end());
                result.emplace_back(combination);
            }
        }

        return result;
    }
};


template <typename Solution>
void test()
{
    ASSERT(compare_sets_of_sets(Solution().run({}, 1), {}));
    ASSERT(compare_sets_of_sets(Solution().run({-1}, 1), {}));
    ASSERT(compare_sets_of_sets(Solution().run({2}, 1), {}));
    ASSERT(compare_sets_of_sets(Solution().run({-1, 2}, 1), {}));
    ASSERT(compare_sets_of_sets(Solution().run({1}, 1), {{1}}));
    ASSERT(compare_sets_of_sets(Solution().run({1, 1}, 1), {{1}}));
    ASSERT(compare_sets_of_sets(Solution().run({1}, 2), {{1, 1}}));
    ASSERT(compare_sets_of_sets(Solution().run({2}, 7), {}));
    ASSERT(compare_sets_of_sets(Solution().run({1, 2}, 2), {{1, 1}, {2}}));
    ASSERT(compare_sets_of_sets(Solution().run({1, 2, 3}, 3), {{1, 1, 1}, {1, 2}, {3}}));
    ASSERT(compare_sets_of_sets(Solution().run({3, 5}, 15), {{3, 3, 3, 3, 3}, {5, 5, 5}}));
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
