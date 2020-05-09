// Problem: https://leetcode.com/problems/flatten-nested-list-iterator/

#include <vector>
#include "utils.h"

namespace flatten_nested_list_iterator {

// Dummy implementation of NestedInteger, for tests

class NestedInteger {
public:
    NestedInteger(int value)
        : is_integer_(true), integer_(value) {}

    NestedInteger(const std::vector<NestedInteger>& list)
        : is_integer_(false), integer_(0), list_(list) {}

    bool isInteger() const
    {
        return is_integer_;
    }

    int getInteger() const
    {
        return integer_;
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


// NestedIterator
//
// Note: This solution allows empty lists, including nested ones. But it's not
// clear whether they are allowed in the original problem.
//
// Time:  O(1) in average and O(n) in the worst case for next(),
//        O(1) for hasNext(),
//        O(d_0) for the constructor
// Space: O(d_avg) in average during the iterator lifetime,
//        O(d_max) in the worst case
//
// n - total number of elements (integers and lists, including nested ones)
// d - depth of the element (1 + the maximum number of nestings),
// d_0 - depth of the first element,
// d_avg - average depth,
// d_max - maximum depth

class NestedIterator {
public:
    NestedIterator(std::vector<NestedInteger>& nested_list)
    {
        positions_.push_back({&nested_list, -1});
        advance();
    }

    int next()
    {
        if (!hasNext())
            throw std::out_of_range("NestedIterator is out of range");

        const int result = positions_.back().item().getInteger();
        advance();
        return result;
    }

    bool hasNext() const
    {
        return positions_.size();
    }

private:
    void advance()
    {
        if (!hasNext())
            return;

        Position* p = &positions_.back();
        do {
            // Advance and then step out until the next item (list or integer):
            // { {1, {2}}, { {} }, 3 }
            //        |---->
            while (p->index++, !p->hasItem()) {
                positions_.pop_back();
                if (!positions_.size())
                    return;
                p = &positions_.back();
            }

            // Step in until the integer or empty list
            // { {1, {2}}, { {} }, 3 }
            //             |->
            while (p->hasItem() && !p->item().isInteger()) {
                Position next{&p->item().getList(), 0};
                positions_.push_back(next);
                p = &positions_.back();
            }

            // If we are at empty list, repeat
        } while (!p->hasItem());
    }

    struct Position
    {
        const std::vector<NestedInteger>* list;
        int                               index;

        const NestedInteger& item() const
        {
            return (*list)[index];
        }

        bool hasItem() const
        {
            return 0 <= index && index < list->size();
        }
    };

    std::vector<Position> positions_;
};


// Test

std::string to_string(const std::vector<NestedInteger>& nested_list)
{
    std::string result = "{";
    for (int i = 0; i < nested_list.size(); i++) {
        if (i > 0)
            result += ", ";

        const auto& e = nested_list[i];
        if (e.isInteger())
            result += ::to_string(e.getInteger()); else
            result += to_string(e.getList());
    }
    result += "}";
    return result;
}

void test(std::vector<NestedInteger> list, const std::vector<int>& expected)
{
    NestedIterator it(list);

    std::vector<int> result;
    while (it.hasNext())
        result.push_back(it.next());

    ASSERT_EX( result == expected, to_string(list) + " -> " + ::to_string(expected) );
}

int main()
{
    typedef std::vector<NestedInteger> List;

    test( {}, {} );
    test( {List{}}, {} );
    test( {List{List{}}}, {} );
    test( {List{List{List{}}}}, {} );
    test( {List{List{}, List{}}}, {} );
    test( {List{}, List{}, List{}}, {} );

    test( {0}, {0} );
    test( {0, -1}, {0, -1} );
    test( {0, -1, 2}, {0, -1, 2} );

    test( {List{1}}, {1} );
    test( {List{1, 2}}, {1, 2} );
    test( {List{List{1}}}, {1} );
    test( {List{List{1, 2}}}, {1, 2} );
    test( {List{List{1}, 2}}, {1, 2} );
    test( {List{1, List{2}}}, {1, 2} );
    test( {List{List{1}, List{2}}}, {1, 2} );
    test( {List{List{List{1}}}}, {1} );

    test( {List{}, 1}, {1} );
    test( {1, List{}}, {1} );
    test( {1, List{}, 2}, {1, 2} );

    test( {List{0}, 1}, {0, 1} );
    test( {0, List{1}}, {0, 1} );
    test( {0, List{1}, 2}, {0, 1, 2} );
    test( {List{0}, 1, 2}, {0, 1, 2} );
    test( {List{0, 1}, 2}, {0, 1, 2} );
    test( {0, List{1, 2}}, {0, 1, 2} );
    test( {0, 1, List{2}}, {0, 1, 2} );

    test( {List{}, List{-1, List{}}, 0, List{1, List{2, List{}, List{3}, List{}, 4}, 5, List{6, 7}}, List{8}, List{}},
        {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8} );

    return 0;
}

}
