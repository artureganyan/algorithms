#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cassert>

#define ASSERT(condition)                                           \
    {                                                               \
        if (condition) {                                            \
            std::cout << "Passed: " << #condition << std::endl;     \
        } else {                                                    \
            std::cerr << "Failed: " << #condition << std::endl;     \
            std::abort();                                           \
        }                                                           \
    }

#endif
