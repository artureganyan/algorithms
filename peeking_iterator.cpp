// Problem: https://leetcode.com/problems/peeking-iterator/

#include <vector>
#include "utils.h"

namespace peeking_iterator {

// Note: next() and hasNext() are not virtual in the original problem, but
// here they are for the solution which reimplements the next() method.
//
// Note: peek() and next() for the end iterator are considered undefined
// behavior.


// Dummy implementation of the base class

class Iterator {
public:
    Iterator(const std::vector<int>& nums)
        : it_(nums.begin()), end_(nums.end()) {}

    Iterator(const Iterator& other)
        : it_(other.it_), end_(other.end_) {}

    virtual ~Iterator() {}

    virtual int  next()          { return *(it_++);    }
    virtual bool hasNext() const { return it_ != end_; }

private:
    std::vector<int>::const_iterator it_;
    std::vector<int>::const_iterator end_;
};


// Straightforward solution, constructing a copy of the iterator on each peek()

class PeekingIterator1 : public Iterator {
public:
    PeekingIterator1(const std::vector<int>& nums) : Iterator(nums) {}

    int peek()
    {
        Iterator peek_it = *this;
        if (peek_it.hasNext())
            return peek_it.next();
        return 0;
    }
};


// More efficient solution with reimplemented next(). If this method is not
// virtual, this solution is error-prone.

class PeekingIterator2 : public Iterator {
public:
    PeekingIterator2(const std::vector<int>& nums) : Iterator(nums), peek_it_(nums)
    {
        nextPeekValue();
    }

    int peek()
    {
        return peek_value_;
    }

    int next()
    {
        nextPeekValue();
        return Iterator::next();
    }

private:
    void nextPeekValue()
    {
        if (peek_it_.hasNext())
            peek_value_ = peek_it_.next(); else
            peek_value_ = 0;
    }

    Iterator peek_it_;
    int      peek_value_;
};


template <typename PeekIterator>
void test()
{
    {
        std::vector<int> v = {};
        PeekIterator     it(v);

        ASSERT( it.hasNext() == false );
    }

    {
        std::vector<int> v = {1, 2, 3};
        PeekIterator     it(v);

        for (int i = 1; i <= v.size(); i++) {
            ASSERT( it.hasNext() == true );
            ASSERT( it.peek() == i );
            ASSERT( it.next() == i );
        }
        ASSERT( it.hasNext() == false );
    }
}

int main()
{
    test<PeekingIterator1>();
    test<PeekingIterator2>();

    return 0;
}

}
