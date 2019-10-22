// Problem: https://leetcode.com/problems/text-justification/

#include <vector>
#include <string>
#include <math.h>
#include <numeric>
#include "utils.h"

namespace text_justification {

class Solution
{
private:
    static const char CHAR_SPACE = ' ';

    enum Justification
    {
        Justification_Left,
        Justification_Full
    };

    std::string createLine(const std::vector<std::string>& words, int begin, int end, int line_width, Justification justification) const
    {
        if (!words.size() || begin >= end || line_width <= 0)
            return std::string();

        std::string line;

        if (justification == Justification_Full) {
            const int words_count = end - begin;
            if (words_count > 1) {
                int words_length = 0;
                for (int i = begin; i < end; i++)
                    words_length += words[i].size();

                int space_length = 0;
                if (words_length < line_width)
                    space_length = std::ceil(double(line_width - words_length) / (words_count - 1));

                const std::string space(space_length, CHAR_SPACE);
                for (int i = begin; i < end - 1; i++) {
                    if (line.size())
                        line += space;
                    line += words[i];
                }

                const std::string& last_word = words[end - 1];
                if (line.size() + last_word.size() < line_width)
                    line += std::string(line_width - line.size() - last_word.size(), CHAR_SPACE);
                line += last_word;
            } else {
                line = words[begin];
            }
            line.resize(line_width, CHAR_SPACE);

        } else if (justification == Justification_Left) {
            const std::string space(1, CHAR_SPACE);
            for (int i = begin; i < end; i++) {
                line += words[i];
                line += space;
            }
            line.resize(line_width, CHAR_SPACE);

        } else {
            throw std::runtime_error("Unsupported justification");
        }

        return line;
    }

public:
    // Time: O(n), Space: O(n), n - total number of characters
    std::vector<std::string> run(std::vector<std::string>& words, int line_width)
    {
        if (!words.size() || line_width <= 0)
            return {};

        std::vector<std::string> result;

        int line_start  = 0;
        int line_length = 0;

        for (int i = 0; i < words.size(); i++) {
            const std::string& word = words[i];

            line_length += (line_length ? 1 : 0) + word.size();
            if (line_length <= line_width)
                continue;

            std::string line = createLine(words, line_start, i, line_width, Justification_Full);
            if (line.size())
                result.push_back(line);

            line_length = word.size();
            line_start  = i;
        }

        std::string line = createLine(words, line_start, words.size(), line_width, Justification_Left);
        if (line.size())
            result.push_back(line);

        return result;
    }
};

int main()
{
    std::vector<std::string> v;
    ASSERT(( Solution().run(v = {"This", "is", "an", "example", "of", "text", "justification."}, 16) == std::vector<std::string>{"This    is    an", "example  of text", "justification.  "} ));
    ASSERT(( Solution().run(v = {}, 16) == std::vector<std::string>{} ));
    ASSERT(( Solution().run(v = {"test"}, -1) == std::vector<std::string>{} ));
    ASSERT(( Solution().run(v = {"test"}, 0) == std::vector<std::string>{} ));
    ASSERT(( Solution().run(v = {"test"}, 1) == std::vector<std::string>{"t"} ));
    ASSERT(( Solution().run(v = {"test"}, 4) == std::vector<std::string>{"test"} ));
    ASSERT(( Solution().run(v = {"test"}, 10) == std::vector<std::string>{"test      "} ));
    ASSERT(( Solution().run(v = {"test", "abc"}, 10) == std::vector<std::string>{"test abc  "} ));
    ASSERT(( Solution().run(v = {"test", "abc", "other"}, 10) == std::vector<std::string>{"test   abc", "other     "} ));
    ASSERT(( Solution().run(v = {"1234567890", "1234567890"}, 10) == std::vector<std::string>{"1234567890", "1234567890"} ));
    return 0;
}

}
