// Problem: https://leetcode.com/problems/group-anagrams/

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "utils.h"

namespace group_anagrams {

#define LOWERCASE_ALPHABET_ONLY

class Solution {
public:
    // If only lowercase characters a-z are supported:
    // Time: O(n * s), Space: O(n * s), n - number of strings, s - average string size
    //
    // If any characters are supported:
    // Time: O(n * s_max * log(s_max))), Space: O(n * s_avg),
    // n - number of strings, s_max - maximum string size, s_avg - average string size
    // Note: The real time is like n1 * s1 * log(s1) + ... + nk * sk * log(sk) + n * s_avg * 3,
    // where the sum of logarithms describes sortings, the last addition describes
    // copyings, ni - a number of strings in the group i, si - a size of each string
    // in the group i, n - total number of strings, s_avg - average string size.
    //
    std::vector<std::vector<std::string>> run(const std::vector<std::string>& strings)
    {
        if (!strings.size())
            return {};

        typedef std::unordered_map<std::string /*chars*/, std::vector<std::string> /*anagrams*/> AnagramsMap;
        AnagramsMap anagrams_map;

        for (int i = 0; i < strings.size(); i++) {
            const std::string& string = strings[i];

            std::string chars = string;
        #ifdef LOWERCASE_ALPHABET_ONLY
            sort_linear(chars);
        #else
            std::sort(chars.begin(), chars.end());
        #endif

            anagrams_map[chars].push_back(string);
        }

        std::vector<std::vector<std::string>> result;
        for (const AnagramsMap::value_type& e : anagrams_map)
            result.push_back(e.second);

        return result;
    }

private:
    // Sorts the string s inplace, in linear time.
    //
    // Time: O(n), Space: O(1), n = s.size()
    //
    // Note: Only lowercase characters a-z are supported.
    //
    void sort_linear(std::string& s) const
    {
        if (!s.size())
            return;

        const char CHAR_FIRST = 'a';
        const char CHAR_LAST  = 'z';
        const int  CHAR_COUNT = CHAR_LAST - CHAR_FIRST + 1;

        std::array<int /*count*/, CHAR_COUNT> char_count = {}; // Zero-initialized
        for (int i = 0; i < s.size(); i++) {
            const char c = s[i];
            if (c < CHAR_FIRST || c > CHAR_LAST)
                throw std::runtime_error(std::string("Unsupported character: ") + c);
            char_count[c - CHAR_FIRST]++;
        }

        for (int i = 0, p = 0; i < char_count.size(); i++) {
            const char c = CHAR_FIRST + i;
            int count = char_count[i];
            while (count--)
                s[p++] = c;
        }
    }
};


int main()
{
    ASSERT( compare_sets(Solution().run({}), {}) );
    ASSERT( compare_sets(Solution().run({""}), {{""}}) );
    ASSERT( compare_sets(Solution().run({"a"}), {{"a"}}) );
    ASSERT( compare_sets(Solution().run({"a", "b", "c"}), {{"a"}, {"b"}, {"c"}}) );
    ASSERT( compare_sets(Solution().run({"abz", "abz", "bza", "abza", "azb", "aa", "cb", "bc", "", "d", ""}), {{"abz", "abz", "bza", "azb"}, {"cb", "bc"}, {"abza"}, {"aa"}, {"d"}, {"", ""}}) );

    // Test performance
    std::cout << "Test performance" << std::endl;

    std::vector<std::string> strings;
    for (size_t string_size = 1; string_size <= 1024; string_size *= 2) {
        for (int i = 0; i < 10000; i++) {
            std::string s(string_size, 0);
            for (int c = 0; c < string_size; c++) {
                s[c] = 'a' + rand() % 26;
            }
            strings.push_back(s);
        }
    }
    std::random_shuffle(strings.begin(), strings.end());

    std::cout << "String count: " << strings.size() << std::endl;
    clock_t start = clock();

    Solution().run(strings);

    double elapsed_secs = double(clock() - start) / CLOCKS_PER_SEC;
    std::cout << "Elapsed: " << elapsed_secs << " seconds" << std::endl;
}

}
