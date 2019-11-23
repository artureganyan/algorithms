// Problem: https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/

#include <vector>
#include "utils.h"

namespace remove_duplicates_from_sorted_array_2 {

class Solution {
public:
    // Removes elements that appear more than 2 times from the sorted array,
    // in-place (the first 2 duplicates are not removed). Returns the new
    // array size. The array itself is not resized.
    //
    // Note: The array must be sorted.
    //
    // Time: O(n), Space: O(1)
    //
    int run(std::vector<int>& array)
    {
        if (!array.size())
            return 0;

        int count_remove     = 0;
        int count_duplicates = 0;
        int prev_value       = array[0];

        for (int i = 1; i < array.size(); i++) {
            const int value = array[i];
            if (value == prev_value) {
                count_duplicates++;
            } else {
                count_duplicates = 0;
                prev_value = value;
            }
            if (count_duplicates > 1) {
                count_remove++;
                continue;
            }
            if (count_remove)
                array[i - count_remove] = value;
        }

        return array.size() - count_remove;
    }
};


void test(const std::vector<int>& values, const std::vector<int>& expected)
{
    std::vector<int> values_parsed = values;
    const int result = Solution().run(values_parsed);
    values_parsed.resize(result);

    ASSERT_EX( result == expected.size() && values_parsed == expected,
               to_string(values) + " -> " + to_string(expected) );
}

int main()
{
    std::vector<int> v;

    test({}, {});
    test({0}, {0});
    test({0, 0}, {0, 0});
    test({0, 0, 0}, {0, 0});
    test({0, 0, 0, 0}, {0, 0});

    test({0, 1}, {0, 1});

    test({0, 1, 2}, {0, 1, 2});

    test({0, 0, 1, 2}, {0, 0, 1, 2});
    test({0, 0, 1, 2, 2}, {0, 0, 1, 2, 2});
    test({0, 0, 1, 1, 2}, {0, 0, 1, 1, 2});
    test({0, 0, 1, 1, 2, 2}, {0, 0, 1, 1, 2, 2});
    test({0, 0, 0, 1, 1, 2, 2}, {0, 0, 1, 1, 2, 2});
    test({0, 0, 0, 1, 1, 1, 2, 2}, {0, 0, 1, 1, 2, 2});
    test({0, 0, 0, 1, 1, 1, 2, 2, 2}, {0, 0, 1, 1, 2, 2});
    test({0, 0, 0, 1, 2, 2}, {0, 0, 1, 2, 2});
    test({0, 0, 0, 1, 2}, {0, 0, 1, 2});

    test({0, 0, 1, 1, 1, 2, 2, 2}, {0, 0, 1, 1, 2, 2});
    test({0, 0, 1, 1, 2, 2, 2}, {0, 0, 1, 1, 2, 2});
    test({0, 0, 1, 2, 2, 2}, {0, 0, 1, 2, 2});
    test({0, 1, 1, 2, 2, 2}, {0, 1, 1, 2, 2});
    test({0, 1, 2, 2, 2}, {0, 1, 2, 2});
    test({0, 1, 2, 2}, {0, 1, 2, 2});

    test({0, 0, 1, 1, 1, 2, 2}, {0, 0, 1, 1, 2, 2});
    test({0, 0, 1, 1, 1, 2}, {0, 0, 1, 1, 2});
    test({0, 1, 1, 1, 2, 2}, {0, 1, 1, 2, 2});
    test({0, 1, 1, 1, 2}, {0, 1, 1, 2});
    test({0, 1, 1, 2}, {0, 1, 1, 2});

    return 0;
}

}
