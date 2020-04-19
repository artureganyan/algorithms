// Problem: https://leetcode.com/problems/maximum-product-of-word-lengths/

#include <vector>
#include <string>
#include "utils.h"

namespace maximum_product_of_word_lengths {

class Solution {
public:
    // Note: The words may contain the following characters only: 'a'-'z'. If
    // each two words have common characters, or the words is empty, returns 0.
    // Words with zero length are allowed. The result must fit into int.
    //
    // Time: O(n^2), Space: O(n), n - number of words
    //
    int run(const std::vector<std::string>& words)
    {
        if (!words.size())
            return 0;

        // For each word, create a mask of its characters
        std::vector<int32_t> word_chars;
        word_chars.reserve(words.size());

        for (const auto& word : words) {
            int32_t mask = 0;
            for (const char c : word) {
                mask |= (1 << (c - 'a'));
            }
            word_chars.push_back(mask);
        }

        // Find the maximum product of sizes
        int max_size_product = 0;

        for (int i1 = 0; i1 < words.size(); i1++) {
            const auto mask1 = word_chars[i1];
            const int  size1 = words[i1].size();

            for (int i2 = i1 + 1; i2 < words.size(); i2++) {
                const auto mask2 = word_chars[i2];
                if (mask1 & mask2)
                    continue;

                const int size_product = size1 * words[i2].size();
                max_size_product = std::max(max_size_product, size_product);
            }
        }

        return max_size_product;
    }
};

int main()
{
    ASSERT( Solution().run({}) == 0 );

    ASSERT( Solution().run({""}) == 0 );
    ASSERT( Solution().run({"", ""}) == 0 );

    ASSERT( Solution().run({"a"}) == 0 );
    ASSERT( Solution().run({"a", ""}) == 0 );
    ASSERT( Solution().run({"a", "a"}) == 0 );
    ASSERT( Solution().run({"a", "aa"}) == 0 );
    ASSERT( Solution().run({"a", "ab"}) == 0 );

    ASSERT( Solution().run({"a", "b"}) == 1 );
    ASSERT( Solution().run({"a", "b", ""}) == 1 );
    ASSERT( Solution().run({"a", "b", "c"}) == 1 );
    ASSERT( Solution().run({"a", "b", "ab"}) == 1 );
    ASSERT( Solution().run({"a", "b", "bb"}) == 2 );

    ASSERT( Solution().run({"ab", "bc", "ac"}) == 0 );
    ASSERT( Solution().run({"ab", "bc", "cd"}) == 4 );

    ASSERT( Solution().run({"abcdefghijklmnopqrstuvwxyz"}) == 0 );
    ASSERT( Solution().run({"abcdefghijklmnopqrstuvwxyz", "abcdefghijk", "lmnopqrstuvwxyz"}) == 165 );

    ASSERT( Solution().run({"ca", "abb", "cde", "efghe"}) == 15 );

    return 0;
}

}
