#include "utils.h"
#include <iostream>
#include <vector>

typedef int TestEntryPoint();

static std::vector<TestEntryPoint*> tests;

#define ADD_TEST(solution)                          \
    namespace solution {                            \
        extern int main();                          \
    }                                               \
    static bool solution##_inited =                 \
        (tests.push_back(solution::main), true);

#define RUN_TESTS()                                 \
    int main(int, char**)                           \
    {                                               \
        int result = EXIT_SUCCESS;                  \
        for (auto test : tests) {                   \
            if (!test())                            \
                result = EXIT_FAILURE;              \
        }                                           \
        return result;                              \
    }

//ADD_TEST(test_utils);
//ADD_TEST(longest_substring_without_repeating_characters);
//ADD_TEST(zig_zag_conversion);
//ADD_TEST(max_increase_to_keep_city_skyline);
//ADD_TEST(string_to_integer);
//ADD_TEST(three_sum);
//ADD_TEST(three_sum_closest);
//ADD_TEST(integer_to_roman);
//ADD_TEST(letter_combinations_of_phone_number);
//ADD_TEST(four_sum);
//ADD_TEST(remove_nth_node_from_end_of_list);
//ADD_TEST(generate_parentheses);
//ADD_TEST(swap_nodes_in_pairs);
//ADD_TEST(jump_game);
//ADD_TEST(text_justification);
//ADD_TEST(container_with_most_water);
//ADD_TEST(next_permutation);
//ADD_TEST(search_in_rotated_array);
//ADD_TEST(find_first_and_last_position_of_element_in_sorted_array);
//ADD_TEST(valid_sudoku);
//ADD_TEST(combination_sum);
//ADD_TEST(multiply_strings);
//ADD_TEST(permutations);
//ADD_TEST(rotate_image);
//ADD_TEST(group_anagrams);
//ADD_TEST(pow_x_n);
//ADD_TEST(spiral_matrix);
//ADD_TEST(merge_intervals);
//ADD_TEST(spiral_matrix_2);
//ADD_TEST(permutation_sequence);
//ADD_TEST(rotate_list);
//ADD_TEST(unique_paths);
//ADD_TEST(unique_paths_2);
//ADD_TEST(minimum_path_sum);
//ADD_TEST(simplify_path);
//ADD_TEST(set_matrix_zeros);
//ADD_TEST(search_in_2d_matrix);
//ADD_TEST(sort_colors);
//ADD_TEST(combinations);
//ADD_TEST(subsets);
//ADD_TEST(word_search);
//ADD_TEST(remove_duplicates_from_sorted_array_2);
//ADD_TEST(search_in_rotated_array_2);
//ADD_TEST(remove_duplicates_from_sorted_list_2);
//ADD_TEST(partition_list);
//ADD_TEST(gray_code);
//ADD_TEST(subsets_2);
//ADD_TEST(decode_ways);
//ADD_TEST(add_two_numbers);
//ADD_TEST(reverse_linked_list_2);
//ADD_TEST(restore_ip_addresses);
//ADD_TEST(binary_tree_inorder_traversal);
//ADD_TEST(unique_binary_search_trees_2);
//ADD_TEST(unique_binary_search_trees);
//ADD_TEST(validate_binary_search_tree);
//ADD_TEST(binary_tree_level_order_traversal);
//ADD_TEST(binary_tree_zigzag_level_order_traversal);
//ADD_TEST(construct_binary_tree_from_preorder_and_inorder_traversal);
//ADD_TEST(construct_binary_tree_from_inorder_and_postorder_traversal);
//ADD_TEST(convert_sorted_list_to_binary_search_tree);
//ADD_TEST(path_sum_2);
ADD_TEST(flatten_binary_tree_to_linked_list);

RUN_TESTS();
