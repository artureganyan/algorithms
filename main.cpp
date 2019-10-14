#include <iostream>

#define TEST(solution)           \
    namespace solution {         \
        extern int main();       \
    }                            \
    int test() {                 \
        return solution::main(); \
    }

//TEST(longest_substring_without_repeating_characters)
//TEST(zig_zag_conversion)
//TEST(max_increase_to_keep_city_skyline)
//TEST(string_to_integer)
//TEST(three_sum);
//TEST(three_sum_closest);
//TEST(integer_to_roman);
TEST(letter_combinations_of_phone_number);

int main(int argc, char** argv)
{
    return test();
}
