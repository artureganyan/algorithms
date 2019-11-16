#include <iostream>

#define TEST(solution)              \
    namespace solution {            \
        extern int main();          \
    }                               \
    int main(int argc, char** argv) \
    {                               \
        return solution::main();    \
    }

//TEST(longest_substring_without_repeating_characters)
//TEST(zig_zag_conversion)
//TEST(max_increase_to_keep_city_skyline)
//TEST(string_to_integer)
//TEST(three_sum);
//TEST(three_sum_closest);
//TEST(integer_to_roman);
//TEST(letter_combinations_of_phone_number);
//TEST(four_sum);
//TEST(remove_nth_node_from_end_of_list);
//TEST(generate_parentheses);
//TEST(swap_nodes_in_pairs);
//TEST(jump_game);
//TEST(text_justification);
//TEST(container_with_most_water);
//TEST(next_permutation);
//TEST(search_in_rotated_array);
//TEST(find_first_and_last_position_of_element_in_sorted_array);
//TEST(valid_sudoku);
//TEST(combination_sum);
//TEST(multiply_strings);
//TEST(permutations);
//TEST(rotate_image);
//TEST(group_anagrams);
//TEST(pow_x_n);
//TEST(spiral_matrix);
//TEST(merge_intervals);
//TEST(spiral_matrix_2);
//TEST(permutation_sequence);
//TEST(rotate_list);
//TEST(unique_paths);
//TEST(unique_paths_2);
//TEST(minimum_path_sum);
//TEST(simplify_path);
//TEST(set_matrix_zeros);
TEST(search_in_2d_matrix);
