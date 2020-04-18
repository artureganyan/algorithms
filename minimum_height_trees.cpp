// Problem: https://leetcode.com/problems/minimum-height-trees/

#include <vector>
#include <unordered_set>
#include "utils.h"

namespace minimum_height_trees {

class Solution {
public:
    // Note: The nodes_count must be >= 0. Each edge must be an unique vector
    // {n1, n2}, where n1 != n2 and both are within [0, nodes_count - 1]. The
    // graph must be a tree. If the nodes_count <= 0 or the edges are empty,
    // returns empty result. If the nodes_count == 1, returns {0}.
    //
    // Time: O(n), Space: O(n), n - number of nodes
    //
    std::vector<int> run(int nodes_count, const std::vector<std::vector<int>>& edges)
    {
        if (nodes_count == 1)
            return {0};

        if (nodes_count <= 0 || !edges.size())
            return {};

        // Create adjacency matrix
        std::vector<std::vector<int>> adjacency_matrix(nodes_count);
        for (const auto& edge : edges) {
            adjacency_matrix[edge[0]].push_back(edge[1]);
            adjacency_matrix[edge[1]].push_back(edge[0]);
        }

        // Find any leaf node
        int leaf_node = -1;
        for (int n = 0; n < adjacency_matrix.size(); n++) {
            if (adjacency_matrix[n].size() == 1) {
                leaf_node = n;
                break;
            }
        }
        if (leaf_node < 0)
            throw std::runtime_error("Incorrect graph");

        // Find the longest pathes starting at the leaf, and add their middle
        // nodes to the result. Since the graph is a tree, these nodes will be
        // the roots of the minimum height trees.
        int                     max_path_size = std::numeric_limits<int>::min();
        std::vector<int>        path;
        std::vector<int>        next_nodes = {leaf_node};
        std::unordered_set<int> visited_nodes = {leaf_node};
        std::unordered_set<int> root_nodes;

        while (next_nodes.size()) {
            // Take the next node and add it to the path
            const int node = next_nodes.back();
            next_nodes.pop_back();
            path.push_back(node);

            // Update the next nodes
            const int next_nodes_prev_size = next_nodes.size();
            for (const int n : adjacency_matrix[node]) {
                if (visited_nodes.find(n) == visited_nodes.end()) {
                    visited_nodes.insert(n);
                    next_nodes.push_back(n);
                }
            }

            // This is not the end of the path, so continue
            if (next_nodes.size() != next_nodes_prev_size)
                continue;

            // Check if the path is the longest one
            const int path_size = path.size();
            if (path_size >= max_path_size) {
                // It's longer than the previous, so discard the current result
                if (path_size > max_path_size) {
                    max_path_size = path_size;
                    root_nodes.clear();
                }
                // Add the middle node to the result
                root_nodes.insert(path[path_size / 2]);
                if (path_size % 2 == 0)
                    root_nodes.insert(path[path_size / 2 - 1]);
            }

            // Return to the previous path node
            path.pop_back();
        }

        std::vector<int> result(root_nodes.begin(), root_nodes.end());
        return result;
    }
};

int main()
{
    ASSERT( compare_sets(Solution().run(0, {}), {}) );

    ASSERT( compare_sets(Solution().run(1, {}), {0}) );

    ASSERT( compare_sets(Solution().run(2, {{0, 1}}), {0, 1}) );
    ASSERT( compare_sets(Solution().run(2, {{1, 0}}), {0, 1}) );

    ASSERT( compare_sets(Solution().run(3, {{0, 1}, {0, 2}}), {0}) );
    ASSERT( compare_sets(Solution().run(3, {{0, 1}, {1, 2}}), {1}) );

    ASSERT( compare_sets(Solution().run(4, {{0, 1}, {1, 2}, {1, 3}}), {1}) );
    ASSERT( compare_sets(Solution().run(4, {{2, 1}, {3, 1}, {1, 0}}), {1}) );
    ASSERT( compare_sets(Solution().run(4, {{0, 1}, {1, 2}, {0, 3}}), {0, 1}) );
    ASSERT( compare_sets(Solution().run(4, {{0, 1}, {1, 2}, {2, 3}}), {1, 2}) );
    ASSERT( compare_sets(Solution().run(4, {{0, 1}, {0, 2}, {0, 3}}), {0}) );

    ASSERT( compare_sets(Solution().run(5, {{0, 1}, {1, 3}, {0, 2}, {2, 4}}), {0}) );
    ASSERT( compare_sets(Solution().run(5, {{0, 1}, {1, 2}, {2, 3}, {0, 4}}), {1}) );
    ASSERT( compare_sets(Solution().run(5, {{0, 1}, {1, 2}, {1, 3}, {0, 4}}), {0, 1}) );

    return 0;
}

}
