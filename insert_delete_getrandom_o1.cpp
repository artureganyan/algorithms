// Problem: https://leetcode.com/problems/insert-delete-getrandom-o1/

#include <unordered_map>
#include <vector>
#include "utils.h"

namespace insert_delete_getrandom_o1 {

// Note: The first call to getRandom() prepares randomized values in O(n) time,
// which are returned in O(1) during n calls (including the first one). This
// results in O(1) average time, but has the following issues:
// 1. Values inserted after getRandom() are not returned until all the
//    previously generated values are returned.
// 2. If call getRandom() and then remove all the values and insert the new
//    ones, the next getRandom() will take O(n) time again. So such getRandom()
//    + remove() calls make getRandom() linear-time.
//
// Time:  O(1) in average for insert(), remove(), getRandom()
// Space: O(n) during the object lifetime
// n - number of elements

class RandomizedSet1 {
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
            throw std::out_of_range("RandomizedSet1::getRandom(): Empty set");

        // Remove the previously generated values that do not exist anymore
        while (values_randomized_.size()) {
            const auto v = values_randomized_.back();
            if (values_.find(v) != values_.end())
                break;
            values_randomized_.pop_back();
        }

        // If there are no values left, generate the new ones
        if (!values_randomized_.size()) {
            std::vector<int> values_vector(values_.begin(), values_.end());
            for (int i = 0; i < values_vector.size(); i++) {
                const auto index = rand() % values_vector.size();
                values_randomized_.push_back(values_vector[index]);
            }
        }
        assert(values_randomized_.size());

        // Return the next value
        const auto v = values_randomized_.back();
        values_randomized_.pop_back();
        return v;
    }

private:
    std::unordered_set<int> values_;
    std::vector<int>        values_randomized_;
};


// Note: This idea is found in the solutions of
// https://leetcode.com/problems/insert-delete-getrandom-o1/: we can remove the
// arbitrary element from the vector in O(1) time if the order of elements may
// change, as follows:
// vector[i] = vector.back();   // Removes i-th element by replacing it with the last one
// vector.pop_back();           // and removing the last one
//
// Time:  O(1) in average for insert(), remove()
//        O(1) for getRandom()
// Space: O(n) during the object lifetime
// n - number of elements

class RandomizedSet2 {
public:
    bool insert(int value)
    {
        if (values_index_.find(value) != values_index_.end())
            return false;

        values_index_[value] = values_index_.size();
        values_.push_back(value);

        return true;
    }

    bool remove(int value)
    {
        if (values_index_.find(value) == values_index_.end())
            return false;

        const int index      = values_index_[value];
        const int last_value = values_.back();

        values_[index] = last_value;
        values_.pop_back();

        values_index_[last_value] = index;
        values_index_.erase(value);

        return true;
    }

    int getRandom()
    {
        if (!values_.size())
            throw std::out_of_range("RandomizedSet2::getRandom(): Empty set");

        const int index = rand() % values_.size();
        return values_[index];
    }

private:
    std::unordered_map<int /*value*/, int /*index*/> values_index_;
    std::vector<int>                                 values_;
};


template <typename RandomizedSet>
void test()
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

    // Check the probability of the randomized values
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

int main()
{
    test<RandomizedSet1>();
    test<RandomizedSet2>();

    return 0;
}

}
