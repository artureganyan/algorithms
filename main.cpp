#include <iostream>

#define TEST(solution)           \
    namespace solution {         \
        extern int main();       \
    }                            \
    int test() {                 \
        return solution::main(); \
    }

//TEST(longest_substring_without_repeating_characters)
TEST(zig_zag_conversion)

int main(int argc, char** argv)
{
    return test();
}
