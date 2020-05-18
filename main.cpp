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
//ADD_TEST(flatten_binary_tree_to_linked_list);
//ADD_TEST(populating_next_right_pointers_in_each_node);
//ADD_TEST(triangle);
//ADD_TEST(word_ladder);
//ADD_TEST(sum_root_to_leaf_numbers);
//ADD_TEST(surrounded_regions);
//ADD_TEST(palindrome_partitioning);
//ADD_TEST(clone_graph);
//ADD_TEST(gas_station);
//ADD_TEST(copy_list_with_random_pointer);
//ADD_TEST(word_break);
//ADD_TEST(linked_list_cycle_2);
//ADD_TEST(reorder_list);
//ADD_TEST(binary_tree_preorder_traversal);
//ADD_TEST(insertion_sort_list);
//ADD_TEST(sort_list);
//ADD_TEST(evaluate_reverse_polish_notation);
//ADD_TEST(reverse_words_in_a_string);
//ADD_TEST(maximum_product_subarray);
//ADD_TEST(find_minimum_in_rotated_sorted_array);
//ADD_TEST(compare_version_numbers);
//ADD_TEST(fraction_to_recurring_decimal);
//ADD_TEST(binary_search_tree_iterator);
//ADD_TEST(largest_number);
//ADD_TEST(repeated_dna_sequences);
//ADD_TEST(binary_tree_right_side_view);
//ADD_TEST(number_of_islands);
//ADD_TEST(bitwise_and_of_numbers_range);
//ADD_TEST(course_schedule);
//ADD_TEST(implement_trie);
//ADD_TEST(minimum_size_subarray_sum);
//ADD_TEST(maximal_square);
//ADD_TEST(count_complete_tree_nodes);
//ADD_TEST(rectangle_area);
//ADD_TEST(basic_calculator_2);
//ADD_TEST(summary_ranges);
//ADD_TEST(find_peak_element);
//ADD_TEST(kth_smallest_element_in_a_bst);
//ADD_TEST(lowest_common_ancestor_of_a_binary_tree);
//ADD_TEST(product_of_array_except_self);
//ADD_TEST(different_ways_to_add_parentheses);
//ADD_TEST(h_index);
//ADD_TEST(h_index_2);
//ADD_TEST(peeking_iterator);
//ADD_TEST(game_of_life);
//ADD_TEST(longest_increasing_subsequence);
//ADD_TEST(range_sum_query_2d_immutable);
//ADD_TEST(is_additive_number);
//ADD_TEST(range_sum_query_mutable);
//ADD_TEST(best_time_to_buy_and_sell_stock_with_cooldown);
//ADD_TEST(minimum_height_trees);
//ADD_TEST(maximum_product_of_word_lengths);
//ADD_TEST(bulb_switcher);
//ADD_TEST(coin_change);
//ADD_TEST(wiggle_sort_2);
//ADD_TEST(odd_even_linked_list);
//ADD_TEST(verify_preorder_serialization_of_a_binary_tree);
//ADD_TEST(reconstruct_itinerary);
//ADD_TEST(increasing_triplet_subsequence);
//ADD_TEST(counting_bits);
//ADD_TEST(flatten_nested_list_iterator);
//ADD_TEST(integer_break);
//ADD_TEST(top_k_frequent_elements);
ADD_TEST(count_numbers_with_unique_digits);

RUN_TESTS();
