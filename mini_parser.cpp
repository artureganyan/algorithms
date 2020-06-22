// Problem: https://leetcode.com/problems/mini-parser/

#include <vector>
#include <string>
#include "utils.h"

namespace mini_parser {

// Dummy implementation of NestedInteger, for tests

class NestedInteger {
public:
    NestedInteger(): is_integer_(false) {}

    NestedInteger(int value)
        : is_integer_(true), integer_(value) {}

    NestedInteger(const std::vector<NestedInteger>& list)
        : is_integer_(false), integer_(0), list_(list) {}

    bool isInteger() const
    {
        return is_integer_;
    }

    void setInteger(int value)
    {
        integer_ = value;
        list_.clear();
        is_integer_ = true;
    }

    int getInteger() const
    {
        return integer_;
    }

    void add(const NestedInteger& integer)
    {
        list_.push_back(integer);
        is_integer_ = false;
    }

    const std::vector<NestedInteger>& getList() const
    {
        return list_;
    }

private:
    std::vector<NestedInteger> list_;
    int                        integer_;
    bool                       is_integer_;
};


class Solution {
public:
    // Note: The string must contain the following characters only: '0'-'9',
    // '-', '[', ']', ',' (whitespaces are not allowed). It must represent
    // either an integer or a list: "-123", "[-123,45]". The list can be empty
    // "[]", and can contain nested lists: "[1,[2]]". If the string is empty,
    // returns a default-constructed nested integer.
    //
    // Time:  O(n)
    // Space: O(d_max)
    // n     - number of characters
    // d_max - maximum depth of the integer (1 + the maximum number of nestings),
    //
    NestedInteger run(const std::string& s)
    {
        if (!s.size())
            return {};

        const std::runtime_error incorrect_input("Incorrect input");

        std::vector<NestedInteger> result = {};

        for (int i = 0; i < s.size(); i++) {
            const char c = s[i];
            // List begin
            if (c == '[') {
                result.push_back({});
            // List end
            } else if (c == ']') {
                if (result.size() >= 2) {
                    result[result.size() - 2].add(result.back());
                    result.pop_back();
                }
            // Separator
            } else if (c == ',') {
                if (!result.size() || result.back().isInteger())
                    throw incorrect_input;
            // Integer
            } else if ('0' <= c && c <= '9' || c == '-') {
                const int value = readInteger(s, i);
                if (result.size())
                    result.back().add(value); else
                    result.push_back(value);
                i--;
            // Unexpected character
            } else {
                throw incorrect_input;
            }
        }

        assert(result.size() <= 1);
        if (result.size() != 1)
            throw incorrect_input;

        return result.back();
    }

private:
    // Reads the integer from s[pos, ...] and advances the pos at the first
    // character after the integer. Throws the same as std::stoi().
    //
    int readInteger(const std::string& s, int& pos) const
    {
        std::string value;
        for (; pos < s.size(); pos++) {
            const char c = s[pos];
            if (!std::isdigit(c) && c != '-')
                break;
            value += c;
        }
        return std::stoi(value);
    }
};


bool compare(const NestedInteger& i1, const NestedInteger& i2)
{
    if (i1.isInteger() != i2.isInteger())
        return false;

    if (i1.isInteger())
        return i1.getInteger() == i2.getInteger();

    const auto& list1 = i1.getList();
    const auto& list2 = i2.getList();
    if (list1.size() != list2.size())
        return false;

    for (int i = 0; i < list1.size(); i++) {
        if (!compare(list1[i], list2[i]))
            return false;
    }
    return true;
}

void test(const std::string& s, const NestedInteger& expected)
{
    const auto result = Solution().run(s);
    
    ASSERT_EX( compare(result, expected), s );
}

int main()
{
    typedef std::vector<NestedInteger> List;

    test(std::string(), {});

    test("0", 0);
    test("-0", 0);
    test("123", 123);
    test("-123", -123);

    test("[]", List{} );
    test("[[]]", List{List{}} );
    test("[[[]]]", {List{List{List{}}}} );
    test("[[],[]]", {List{List{}, List{}}} );

    test("[1]", List{1});
    test("[1,2]", List{1, 2});
    test("[[1]]", List{List{1}});
    test("[[1,2]]", List{List{1, 2}});
    test("[[1],2]", List{List{1}, 2});
    test("[1,[2]]", List{1, List{2}});
    test("[1,[2],3]", List{1, List{2}, 3});
    test("[[1],[2]]", List{List{1}, List{2}});
    test("[[1],2,[3]]", List{List{1}, 2, List{3}});
    test("[[[1]]]", List{List{List{1}}});
    test("[[[1]],[2]]", List{List{List{1}}, List{2}});
    test("[[[1],2],[3],4]", List{List{List{1},2}, List{3}, 4});

    test("[[],1]", List{List{}, 1} );
    test("[1,[]]", List{1, List{}});
    test("[1,[],2]", List{1, List{}, 2});

    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    test("[[],[-1,[]],0,[1,[23,[],[456],[],78],9,[0,1]]," + to_string(MIN) + ",[" + to_string(MAX) + "],[]]",
         List{List{}, List{-1, List{}}, 0, List{1, List{23, List{}, List{456}, List{}, 78}, 9, List{0, 1}}, MIN, List{MAX}, List{}});

    return 0;
}

}
