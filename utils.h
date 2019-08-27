#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cassert>

#define ASSERT(condition)                                           \
    {                                                               \
        if (condition) {                                            \
            std::cout << "Passed: " << #condition << std::endl;     \
        } else {                                                    \
            std::cout << "Failed: " << #condition << std::endl;     \
            assert(condition);                                      \
        }                                                           \
    }

#endif
