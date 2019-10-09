#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cassert>
#include <unordered_set>
#include <vector>

#define ASSERT(condition)                                           \
    {                                                               \
        if (condition) {                                            \
            std::cout << "Passed: " << #condition << std::endl;     \
        } else {                                                    \
            std::cerr << "Failed: " << #condition << std::endl;     \
            std::abort();                                           \
        }                                                           \
    }

template <template <typename, typename...> class Container, typename T>
bool compare_sets(const Container<T>& set1, const Container<T>& set2)
{
    std::unordered_set<T> s1(set1.begin(), set1.end());
    std::unordered_set<T> s2(set2.begin(), set2.end());
    return s1 == s2;
}

template <typename T>
inline bool compare_sets(const std::unordered_set<T>& set1, const std::unordered_set<T>& set2)
{
    return set1 == set2;
}

template <typename T>
bool compare_sets(const std::vector<std::vector<T>>& set1, const std::vector<std::vector<T>>& set2)
{
    for (const auto& s1 : set1) {
        bool found = false;
        for (const auto& s2 : set2) {
            if (compare_sets(s1, s2)) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

#endif
