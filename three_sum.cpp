// Problem: https://leetcode.com/problems/3sum/

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

namespace three_sum {

class Solution {
private:
    struct Triple
    {
        int n1;
        int n2;
        int n3;

        bool operator ==(const Triple& other) const
        {
            return (n1 == other.n1 && n2 == other.n2 && n3 == other.n3) ||
                   (n1 == other.n1 && n2 == other.n3 && n3 == other.n2) ||
                   (n1 == other.n2 && n2 == other.n1 && n3 == other.n3) ||
                   (n1 == other.n2 && n2 == other.n3 && n3 == other.n1) ||
                   (n1 == other.n3 && n2 == other.n2 && n3 == other.n1) ||
                   (n1 == other.n3 && n2 == other.n1 && n3 == other.n2);
        }
    };

    struct triple_hash
    {
        bool operator ()(const Triple& v) const
        {
            return v.n1 ^ v.n2 ^ v.n3;
        }
    };

public:
    // Time: O(n^2), Space: O(n)
    std::vector<std::vector<int>> run(const std::vector<int>& nums)
    {
        if (!nums.size())
            return std::vector<std::vector<int>>();

        // Prepare map of numbers' positions
        typedef std::unordered_multimap<int /*number*/, int /*pos*/> NumsMap;
        NumsMap nums_map;

        for (size_t i = 0; i < nums.size(); i++)
            nums_map.insert({nums[i], i});

        // Take each two numbers and check the map for the third one
        typedef std::unordered_set<Triple, triple_hash> ResultSet;
        ResultSet result_set;

        for (size_t i1 = 0; i1 < nums.size(); i1++) {
            const int n1 = nums[i1];

            for (size_t i2 = i1 + 1; i2 < nums.size(); i2++) {
                const int n2 = nums[i2];
                const int n3 = -(n1 + n2);

                bool n3_found = false;
                NumsMap::const_iterator ni = nums_map.find(n3);
                while (ni != nums_map.end() && ni->first == n3) {
                    if (ni->second != i1 && ni->second != i2) {
                        n3_found = true;
                        break;
                    }
                    ni++;
                }
                if (n3_found)
                    result_set.insert({n1, n2, n3});
            }
        }

        // Convert result set to vector
        std::vector<std::vector<int>> result;
        result.reserve(result_set.size());
        for (ResultSet::const_iterator i = result_set.begin(); i != result_set.end(); i++)
            result.push_back({i->n1, i->n2, i->n3});

        return result;
    }
};

int main()
{
    ASSERT(compare_sets_of_sets(Solution().run({-1, 0, 1, 2, -1, -4}), {{-1, 0, 1}, {-1, -1, 2}}));
    ASSERT(compare_sets_of_sets(Solution().run({}), {}));
    ASSERT(compare_sets_of_sets(Solution().run({1}), {}));
    ASSERT(compare_sets_of_sets(Solution().run({0, 0, 0, 0}), {{0, 0, 0}}));
    ASSERT(compare_sets_of_sets(Solution().run({2, -2, 1}), {}));
    ASSERT(compare_sets_of_sets(Solution().run({1, 1, -2, 1, 1, -2}), {{1, 1, -2}}) );

    return 0;
}

}
