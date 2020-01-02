// Problem: https://leetcode.com/problems/palindrome-partitioning/

#include <string>
#include <vector>
#include "utils.h"

namespace palindrome_partitioning {

class Solution {
public:
    std::vector<std::vector<std::string>> run(const std::string& s)
    {
        if (!s.size())
            return {};

        std::vector<std::vector<std::string>> result;
        generatePalindromes(result, s, 0, 0);
        return result;
    }

private:
    void generatePalindromes(std::vector<std::vector<std::string>>& result,
        const std::string& s, int s_start, int partitions_count) const
    {
        if (s_start >= s.size()) {
            result.push_back(std::vector<std::string>(partitions_count));
            return;
        }

        for (int s_end = s_start + 1; s_end <= s.size(); s_end++) {
            if (!isPalindrome(s, s_start, s_end))
                continue;

            const size_t prev_size = result.size();
            generatePalindromes(result, s, s_end, partitions_count + 1);
            for (int r = prev_size; r < result.size(); r++) {
                result[r][partitions_count] = s.substr(s_start, s_end - s_start);
            }
        }
    }

    bool isPalindrome(const std::string& s, int s_start, int s_end) const
    {
        for (int i1 = s_start, i2 = s_end - 1; i1 < i2; i1++, i2--) {
            if (s[i1] != s[i2])
                return false;
        }
        return true;
    }
};

int main()
{
    ASSERT(compare_sets(Solution().run(std::string()), {}));
    ASSERT(compare_sets(Solution().run("a"), {{"a"}}));
    ASSERT(compare_sets(Solution().run("aa"), {{"a", "a"}, {"aa"}}));
    ASSERT(compare_sets(Solution().run("aaa"), {{"a", "a", "a"}, {"aa", "a"}, {"a", "aa"}, {"aaa"}}));
    ASSERT(compare_sets(Solution().run("aab"), {{"a", "a", "b"}, {"aa", "b"}}));
    ASSERT(compare_sets(Solution().run("ab"), {{"a", "b"}}));
    ASSERT(compare_sets(Solution().run("aab"), {{"a", "a", "b"}, {"aa", "b"}}));
    ASSERT(compare_sets(Solution().run("baa"), {{"b", "a", "a"}, {"b", "aa"}}));
    ASSERT(compare_sets(Solution().run("baab"), {{"b", "a", "a", "b"}, {"b", "aa", "b"}, {"baab"}}));
    ASSERT(compare_sets(Solution().run("1aa1bbb"),
        {{"1", "a", "a", "1", "b", "b", "b"}, {"1", "a", "a", "1", "bb", "b"}, {"1", "a", "a", "1", "b", "bb"}, {"1", "a", "a", "1", "bbb"},
         {"1", "aa", "1", "b", "b", "b"}, {"1", "aa", "1", "bb", "b"}, {"1", "aa", "1", "b", "bb"}, {"1", "aa", "1", "bbb"},
         {"1aa1", "b", "b", "b"}, {"1aa1", "bb", "b"}, {"1aa1", "b", "bb"}, {"1aa1", "bbb"}}
        ));

    return 0;
}

}
