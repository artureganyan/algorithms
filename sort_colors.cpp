// Problem: https://leetcode.com/problems/sort-colors/

#include <vector>
#include <algorithm>
#include "utils.h"

namespace sort_colors {

// Uses counting sort (2 passes)

class Solution1 {
public:
    // Time: O(n), Space: O(1), n - number of elements
    //
    // Note: The nums must contain only the following values: 0, 1, 2.
    //
    void run(std::vector<int>& nums)
    {
        if (!nums.size())
            return;

        // Count values
        const int VALUE_COUNT = 3;

        int nums_count[VALUE_COUNT] = {};
        for (int i = 0; i < nums.size(); i++)
            nums_count[nums[i]]++;

        // Fill the array
        for (int v = 0, i = 0; v < VALUE_COUNT; v++) {
            const int count = nums_count[v];
            for (int c = 0; c < count; c++, i++)
                nums[i] = v;
        }
    }
};


// Uses 1-pass sort

class Solution2 {
public:
    // Time: O(n), Space: O(1), n - number of elements
    //
    // Note: The nums must contain only the following values: 0, 1, 2.
    //
    void run(std::vector<int>& nums)
    {
        // Idea:
        // This is the 3-way partitioning (Dutch national flag algorithm).
        // Iterate from the start to the end and swap elements, building
        // 3 groups of values:
        //
        // 0 ... 0 1 ... 1 ? ? ? 2 ... 2
        // -------->------->   <--------
        //         i0      i1  i2
        //
        // i0, i1 and i2 are positions for the next 0, 1 and 2 respectively.
        // i1 is also the iterator, increasing until it reaches i2. If nums[i1]
        // is 0, swap it with nums[i0], which is 1 or 0 (if there were no 1s);
        // if it's 1, just keep going; if it's 2, swap it with nums[i2] and
        // don't advance i1. i0/i2 is increased/decreased after each swap.

        if (!nums.size())
            return;

        int i0 = 0;
        int i1 = 0;
        int i2 = nums.size() - 1;

        while (i1 <= i2) {
            int& e = nums[i1];
            if (e == 0) {
                std::swap(nums[i0], e);
                i0++;
                i1++;
            } else if (e == 1) {
                i1++;
            } else if (e == 2) {
                std::swap(e, nums[i2]);
                i2--;
            }
        }
    }
};


template <typename Solution>
void test()
{
    std::vector<int> v;

    ASSERT(( Solution().run(v = {}),  v == std::vector<int>{} ));

    ASSERT(( Solution().run(v = {0}), v == std::vector<int>{0} ));
    ASSERT(( Solution().run(v = {1}), v == std::vector<int>{1} ));
    ASSERT(( Solution().run(v = {2}), v == std::vector<int>{2} ));

    ASSERT(( Solution().run(v = {0, 0, 0}), v == std::vector<int>{0, 0, 0} ));
    ASSERT(( Solution().run(v = {1, 1, 1}), v == std::vector<int>{1, 1, 1} ));
    ASSERT(( Solution().run(v = {2, 2, 2}), v == std::vector<int>{2, 2, 2} ));

    ASSERT(( Solution().run(v = {0, 1, 2}), v == std::vector<int>{0, 1, 2} ));
    ASSERT(( Solution().run(v = {0, 2, 1}), v == std::vector<int>{0, 1, 2} ));
    ASSERT(( Solution().run(v = {1, 2, 0}), v == std::vector<int>{0, 1, 2} ));
    ASSERT(( Solution().run(v = {1, 0, 2}), v == std::vector<int>{0, 1, 2} ));
    ASSERT(( Solution().run(v = {2, 0, 1}), v == std::vector<int>{0, 1, 2} ));
    ASSERT(( Solution().run(v = {2, 1, 0}), v == std::vector<int>{0, 1, 2} ));

    ASSERT(( Solution().run(v = {0, 0, 1, 1, 2, 2}), v == std::vector<int>{0, 0, 1, 1, 2, 2} ));
    ASSERT(( Solution().run(v = {1, 1, 2, 2, 0, 0}), v == std::vector<int>{0, 0, 1, 1, 2, 2} ));
    ASSERT(( Solution().run(v = {0, 1, 2, 0, 1, 2}), v == std::vector<int>{0, 0, 1, 1, 2, 2} ));
    ASSERT(( Solution().run(v = {2, 1, 0, 2, 1, 0}), v == std::vector<int>{0, 0, 1, 1, 2, 2} ));
    ASSERT(( Solution().run(v = {0, 1, 2, 2, 1, 0}), v == std::vector<int>{0, 0, 1, 1, 2, 2} ));
    ASSERT(( Solution().run(v = {2, 1, 0, 0, 1, 2}), v == std::vector<int>{0, 0, 1, 1, 2, 2} ));
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
