// Problem: https://leetcode.com/problems/clone-graph/

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

namespace clone_graph {

struct Node {
    int val;
    std::vector<Node*> neighbors;
};


class Solution {
public:
    // Note: The original problem supposes only undirected graphs without loops
    // and repeated edges, but this solution does not have such restrictions.
    //
    // Time: O(n + e), Space: O(n + e), Recursion depth <= n + 1
    // n - number of nodes, e - number of edges
    //
    // Note: Recursion depth depends on the maximum number of connected nodes,
    // which is <= the total number of nodes.
    //
    Node* run(Node* node)
    {
        CopyMap copies;
        return copy(node, copies);
    }

private:
    typedef std::unordered_map<const Node* /*original*/, Node* /*copy*/> CopyMap;

    Node* copy(const Node* node, CopyMap& copies) const
    {
        if (!node)
            return nullptr;

        Node*& node_copy = copies[node];
        if (node_copy)
            return node_copy;

        node_copy = new Node;
        node_copy->val = node->val;
        for (Node* neighbor : node->neighbors)
            node_copy->neighbors.push_back(copy(neighbor, copies));

        return node_copy;
    }
};


// Graph utils

bool compare_graphs_impl(const Node* node1, const Node* node2, std::unordered_set<const Node*>& handled)
{
    if (handled.count(node1))
        return true;

    if (!node1 || !node2)
        return node1 == node2;

    if (node1->val != node2->val)
        return false;

    const int neighbors_count1 = node1->neighbors.size();
    const int neighbors_count2 = node2->neighbors.size();
    if (neighbors_count1 != neighbors_count2)
        return false;

    handled.insert(node1);

    for (int i = 0; i < neighbors_count1; i++) {
        if (!compare_graphs_impl(node1->neighbors[i], node2->neighbors[i], handled))
            return false;
    }
    return true;
}

bool compare_graphs(const Node* node1, const Node* node2)
{
    std::unordered_set<const Node*> handled;
    return compare_graphs_impl(node1, node2, handled);
}

void destroy_graph_impl(const Node* node, std::unordered_set<const Node*>& handled)
{
    if (!node || handled.count(node))
        return;

    handled.insert(node);

    const int neighbors_count = node->neighbors.size();
    for (int i = 0; i < neighbors_count; i++)
        destroy_graph_impl(node->neighbors[i], handled);

    delete node;
}

void destroy_graph(const Node* node)
{
    std::unordered_set<const Node*> handled;
    destroy_graph_impl(node, handled);
}

std::string graph_to_string_impl(Node* node, std::unordered_set<const Node*>& handled)
{
    if (!node || handled.count(node))
        return {};

    handled.insert(node);

    std::stringstream result;
    result << node->val;

    if (node->neighbors.size()) {
        result << ": [";
        for (int i = 0; i < node->neighbors.size(); i++) {
            if (i)
                result << ", ";
            result << node->neighbors[i]->val;
        }
        result << "]" << std::endl;

        for (int i = 0; i < node->neighbors.size(); i++) {
            result << graph_to_string_impl(node->neighbors[i], handled);
        }
    } else {
        result << std::endl;
    }

    return result.str();
}

std::string graph_to_string(Node* node)
{
    std::unordered_set<const Node*> handled;
    return graph_to_string_impl(node, handled);
}


// Test

void test(Node* source)
{
    Node* result = Solution().run(source);

    ASSERT_EX( compare_graphs(source, result), graph_to_string(source) );

    destroy_graph(source);
    destroy_graph(result);
}

int main()
{
    ASSERT( Solution().run(nullptr) == nullptr );

    // Undirected graphs
    test( new Node{1} );

    {
        Node* n1 = new Node{1};
        Node* n2 = new Node{2};
        n1->neighbors = {n2};
        n2->neighbors = {n1};
        test(n1);
    }

    {
        Node* n1 = new Node{1};
        Node* n2 = new Node{2};
        Node* n3 = new Node{3};
        n1->neighbors = {n2, n3};
        n2->neighbors = {n1, n3};
        n3->neighbors = {n1, n2};
        test(n1);
    }

    // Directed graphs
    test( new Node{1, {new Node{2}}} );
    test( new Node{1, {new Node{2}, new Node{3}}} );
    test( new Node{1, {new Node{2, {new Node{3}}}}} );

    {
        Node* n1 = new Node{1};
        Node* n2 = new Node{2};
        Node* n3 = new Node{3};
        n1->neighbors = {n2};
        n2->neighbors = {n3};
        n3->neighbors = {n1};
        test(n1);
    }

    // Loop
    {
        Node* n1 = new Node{1};
        n1->neighbors = {n1};
        test(n1);
    }

    // Repeated edge
    {
        Node* n1 = new Node{1};
        Node* n2 = new Node{2};
        n1->neighbors = {n2, n2};
        n2->neighbors = {n1};
        test(n1);
    }

    return 0;
}

}
