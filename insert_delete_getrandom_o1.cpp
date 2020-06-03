// Problem: https://leetcode.com/problems/insert-delete-getrandom-o1/

#include <unordered_map>
#include <vector>
#include "utils.h"

namespace insert_delete_getrandom_o1 {

// Time:  O(1) for insert() and remove()
//        O(1) in average (O(n) per each n calls) for getRandom()
// Space: O(n) during the object lifetime
// n - number of elements
//
// Note: The first call of getRandom() prepares randomized values in O(n) time,
// and then returns them in O(1) during n calls, resulting in O(1) average
// time. This leads to the following issues:
// 1. New values inserted after getRandom() are not returned until all the
//    previously generated values are returned.
// 2. If we call getRandom() and then remove all n values and insert n new
//    ones, the next getRandom() will take O(n) time again. So such
//    getRandom() + n remove() calls make getRandom() linear-time.

class RandomizedSet {
public:
    bool insert(int value)
    {
        if (values_.find(value) != values_.end())
            return false;

        values_.insert(value);
        return true;
    }

    bool remove(int value)
    {
        if (values_.find(value) == values_.end())
            return false;

        values_.erase(value);
        return true;
    }

    int getRandom()
    {
        if (!values_.size())
            throw std::out_of_range("getRandom(): Empty set");

        while (true) {
            while (values_randomized_.size()) {
                const auto v = values_randomized_.back();
                values_randomized_.pop_back();
                if (values_.find(v) == values_.end())
                    continue;
                return v;
            }

            std::vector<int> values_vector(values_.begin(), values_.end());
            for (int i = 0; i < values_vector.size(); i++) {
                const auto index = rand() % values_vector.size();
                values_randomized_.push_back(values_vector[index]);
            }
        }

        throw std::runtime_error("getRandom(): Unexpected error");
    }

private:
    std::unordered_set<int> values_;
    std::vector<int>        values_randomized_;
};


int main()
{
    RandomizedSet s;

    ASSERT( s.remove(1) == false );
    ASSERT( s.insert(1) == true );
    ASSERT( s.remove(1) == true );
    ASSERT( s.remove(1) == false );

    ASSERT( s.insert(1) == true );
    ASSERT( s.getRandom() == 1 );
    ASSERT( s.getRandom() == 1 );
    ASSERT( s.getRandom() == 1 );

    ASSERT( s.insert(2) == true );
    ASSERT( is_any_of(s.getRandom(), {1, 2}) );
    ASSERT( s.remove(1) == true );
    ASSERT( s.getRandom() == 2 );
    ASSERT( s.remove(2) == true );

    // Check probability of the randomized values
    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    const std::vector<int> values = {-1, MAX, 0, 1000, 1, 123, -2, 100000, 10, MIN};
    std::unordered_map<int /*value*/, int /*count*/> values_count;
    const int runs_count = values.size() * 1000;

    for (const int v : values)
        s.insert(v);

    for (int i = 0; i < runs_count; i++) {
        const int v = s.getRandom();
        values_count[v]++;
        s.remove(v);
        s.insert(v);
    }

    for (const int v : values) {
        const double frequency  = values_count[v] / double(runs_count);
        const double expected   = 1.0 / values.size();
        const double expected_e = expected / 10.0;
        ASSERT_EX( expected - expected_e <= frequency && frequency <= expected + expected_e,
            to_string(v) + ", " + to_string(frequency) + " ~ " + to_string(expected) );
    }
}

}
