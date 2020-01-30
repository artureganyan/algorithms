// Problem: https://leetcode.com/problems/course-schedule/

#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

namespace course_schedule {

class Solution {
public:
    // Note: The num_courses parameter is not used in this implementation. The
    // prerequisites are not checked for non-existing courses (i.e. for values
    // < 0 or >= num_courses - 1).
    //
    // Time: O(c * p), Space: O(p)
    // c - number of unique courses specified as prerequisites; e.g. for
    //     prerequisites = {{3, 5}, {4, 5}, {4, 6}} these courses are {5, 6}, so c = 2
    // p - number of prerequisites
    //
    bool run(int num_courses, const std::vector<std::vector<int>>& prerequisites)
    {
        return !graphHasLoops(prerequisites);
    }

private:
    typedef std::unordered_map<int /*source*/, std::unordered_set<int> /*targets*/> AdjacencyMap;

    bool graphHasLoops(const std::vector<std::vector<int>>& edges) const
    {
        // Create adjacency map
        AdjacencyMap adjacency_map;
        for (const auto& edge : edges) {
            const int from = edge[1];
            const int to   = edge[0];
            adjacency_map[from].insert(to);
        }

        // Check for loops
        for (const auto entry : adjacency_map) {
            const int source = entry.first;
            if (graphHasPath(adjacency_map, source, source))
                return true;
        }
        return false;
    }

    bool graphHasPath(const AdjacencyMap& adjacency_map, int source, int target) const
    {
        std::deque<int>         vertices     = {source};
        std::unordered_set<int> vertices_set = {source};

        while (vertices.size()) {
            const int vertice = vertices.front();
            vertices.pop_front();

            const auto iv = adjacency_map.find(vertice);
            if (iv != adjacency_map.end()) {
                for (int v : iv->second) {
                    if (v == target)
                        return true;

                    if (vertices_set.count(v))
                        continue;

                    vertices.push_back(v);
                    vertices_set.insert(v);
                }
            }
        }
        return false;
    }
};

int main()
{
    ASSERT( Solution().run(0, {}) == true );

    ASSERT( Solution().run(1, {}) == true );
    ASSERT( Solution().run(1, {{0, 0}}) == false );

    ASSERT( Solution().run(2, {}) == true );
    ASSERT( Solution().run(2, {{0, 1}}) == true );
    ASSERT( Solution().run(2, {{0, 1}, {1, 0}}) == false );

    ASSERT( Solution().run(3, {{0, 1}, {0, 2}}) == true );
    ASSERT( Solution().run(3, {{0, 1}, {1, 2}}) == true );
    ASSERT( Solution().run(3, {{0, 1}, {1, 2}, {0, 2}}) == true );
    ASSERT( Solution().run(3, {{0, 1}, {1, 2}, {2, 0}}) == false );

    ASSERT( Solution().run(4, {{0, 1}, {2, 3}}) == true );
    ASSERT( Solution().run(4, {{0, 1}, {2, 3}, {3, 0}, {1, 3}}) == false );

    return 0;
}

}
