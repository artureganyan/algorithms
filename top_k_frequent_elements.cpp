// Problem: https://leetcode.com/problems/top-k-frequent-elements/

#include <vector>
#include <unordered_map>
#include <map>
#include "utils.h"

namespace top_k_frequent_elements {

class Solution {
public:
    // Note: If the nums is empty or k <= 0, returns empty result. If k is
    // greater than the number of unique elements, all the unique elements
    // are returned. If the answer is ambiguous, one of the possible results
    // is returned. The returned elements are sorted by their frequency, in
    // the ascending order (not required in the original problem).
    //
    // Time: O(u * log(k)), Space: O(u),
    // u - number of unique elements in the source array,
    // k - number of elements to return
    //
    std::vector<int> run(const std::vector<int>& nums, int k)
    {
        if (!nums.size() || k <= 0)
            return {};

        // Calculate count of each element
        std::unordered_map<int /*value*/, int /*count*/> count_map;
        for (const int n : nums)
            count_map[n]++;

        // Create the map of the first k frequent elements
        // Note: std::multimap is required to not miss the elements with the
        // same count, e.g. for nums={1, 2, 3} and k=3 the answer is {1, 2, 3}.
        // If there may be multiple results, one of them remains in the map:
        // for the nums above and k=2 it may be {1, 2}, {1, 3} or {2, 3}.
        std::multimap<int /*count*/, int /*value*/> value_map;
        for (const auto i : count_map) {
            value_map.insert(std::make_pair(i.second, i.first));
            if (value_map.size() > k)
                value_map.erase(value_map.begin());
        }

        // Convert the map to the result
        std::vector<int> result;
        result.reserve(k);
        for (const auto i : value_map)
            result.push_back(i.second);

        return result;
    }
};

int main()
{
    auto test = [](const std::vector<int>& nums, int k, const std::vector<int>& expected)
    {
        const auto result = Solution().run(nums, k);
        ASSERT_EX( compare_sets(result, expected),
            to_string(nums) + ", " + to_string(k) + " -> " + to_string(expected) );
    };

    test({}, 1, {});

    test({0},  1, {0});
    test({-1}, 1, {-1});
    test({1},  1, {1});
    test({1},  2, {1});

    test({1, 1}, 1, {1});
    test({1, 1}, 2, {1});
    test({1, 1}, 3, {1});

    test({1, 2}, 2, {1, 2});
    test({2, 1}, 2, {1, 2});

    test({1, 1, 2}, 1, {1});
    test({1, 2, 1}, 1, {1});
    test({2, 1, 1}, 1, {1});
    test({1, 1, 2}, 2, {1, 2});

    test({1, 2, 3}, 3, {1, 2, 3});
    test({1, 2, 1, 2}, 2, {1, 2});

    test({1, 1, 1, 1, 2, 2, 2, 3, 3}, 3, {1, 2, 3});
    test({1, 2, 3, 1, 2, 3, 1, 2, 1}, 3, {1, 2, 3});
    test({1, 3, 2, 1, 3, 2, 1, 2, 1}, 3, {1, 2, 3});
    test({1, 2, 2, 1, 3, 3, 1, 2, 1}, 3, {1, 2, 3});
    test({1, 1, 2, 2, 3, 3, 1, 2, 1}, 3, {1, 2, 3});
    test({2, 2, 3, 3, 1, 1, 2, 1, 1}, 3, {1, 2, 3});
    test({3, 3, 2, 2, 1, 1, 2, 1, 1}, 3, {1, 2, 3});

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    // Shuffled {1, 1, 1, 1, 1, MIN, MIN, MIN, MIN, MAX, MAX, MAX, -1, -1, 0, 4, 5, 6, 100}
    test({1, 0, MIN, 100, MIN, 6, MAX, -1, MIN, 1, MIN, MAX, 1, 5, 1, 4, -1, MAX, 1}, 4,
        {1, MIN, MAX, -1});

    return 0;
}

}
