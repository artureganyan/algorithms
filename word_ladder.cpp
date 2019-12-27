// Problem: https://leetcode.com/problems/word-ladder/

#include <vector>
#include <list>
#include "utils.h"

namespace word_ladder {

class Solution {
public:
    // Note: If begin_word can't be transformed to end_word, returns 0. If
    // begin_word == end_word, and end_word is present in the word list,
    // returns 1, otherwise also returns 0.
    //
    // Time: O(n^2 * w), Space: O(n), n - number of words, w - word length
    //
    // Note: O(w) time is required to compare two words.
    //
    int run(const std::string& begin_word, const std::string& end_word, const std::vector<std::string>& words)
    {
        // Idea:
        // If think about the transformations as a graph where we can move
        // from word A to word B if A differs from B in 1 character, then we
        // need to find the shortest path in that graph. This can be done
        // e.g. by the breadth-first search (BSF).

        if (!begin_word.size() || !end_word.size() || begin_word.size() != end_word.size() || !words.size())
            return 0;

        // Prepare the list of word indices and check if the end word exists
        std::list<int> words_left;

        bool end_word_found = false;
        for (int i = 0; i < words.size(); i++) {
            words_left.push_back(i);
            if (words[i] == end_word)
                end_word_found = true;
        }
        if (!end_word_found)
            return 0;

        // To avoid returning 2 for the loop path "word -> word", return 1
        if (begin_word == end_word)
            return 1;

        // Find the shortest transformation path
        std::vector<int> words_visited  = {-1};
        int              words_to_check = 1;
        int              steps_count    = 1;

        for (int i1 = 0; i1 < words_visited.size(); i1++) {
            const int          word_index   = words_visited[i1];
            const std::string& word_visited = word_index >= 0 ? words[word_index] : begin_word;

            for (auto i2 = words_left.begin(); i2 != words_left.end();) {
                const std::string& word = words[*i2];

                if (canTransform(word_visited, word)) {
                    if (word == end_word)
                        return steps_count + 1;

                    words_visited.push_back(*i2);
                    i2 = words_left.erase(i2);
                } else {
                    i2++;
                }
            }

            if (--words_to_check == 0) {
                words_to_check = words_visited.size() - i1 - 1;
                steps_count++;
            }
        }

        return 0;
    }

private:
    bool canTransform(const std::string& from, const std::string& to) const
    {
        if (from.size() != to.size())
            return false;

        int diff_count = 0;
        for (int i = 0; i < from.size(); i++) {
            if (from[i] != to[i]) {
                if (++diff_count > 1)
                    return false;
            }
        }
        return true;
    }
};


int main()
{
    ASSERT( Solution().run({}, {}, {}) == 0 );
    ASSERT( Solution().run({"a"}, {}, {}) == 0 );
    ASSERT( Solution().run({}, {"a"}, {}) == 0 );
    ASSERT( Solution().run({}, {}, {"a"}) == 0 );
    ASSERT( Solution().run({"a"}, {}, {"a"}) == 0 );
    ASSERT( Solution().run({}, {"a"}, {"a"}) == 0 );

    ASSERT( Solution().run({"a"}, {"a"}, {"a"}) == 1 );

    ASSERT( Solution().run({"a"}, {"b"}, {"b"}) == 2 );
    ASSERT( Solution().run({"a"}, {"b"}, {"c", "b"}) == 2 );
    ASSERT( Solution().run({"a"}, {"b"}, {"b", "c"}) == 2 );

    ASSERT( Solution().run({"aaa"}, {"bbb"}, {"aab", "abb", "bbb"}) == 4 );
    ASSERT( Solution().run({"aaa"}, {"bbb"}, {"aab", "bab", "bbb"}) == 4 );
    ASSERT( Solution().run({"aaa"}, {"bbb"}, {"aab", "aba", "bbb"}) == 0 );
    ASSERT( Solution().run({"aaa"}, {"bbb"}, {"aac", "abc", "bbc", "bbb"}) == 5 );
    ASSERT( Solution().run({"aaa"}, {"bbb"}, {"bbb", "abc", "aac", "bbc"}) == 5 );

    ASSERT( Solution().run({"aaaa"}, {"bbbb"}, {"aaac", "aacc", "abcc", "bbcc", "bbbc", "bbbb"}) == 7 );

    return 0;
}

}
