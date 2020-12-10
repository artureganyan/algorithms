// Problem: https://leetcode.com/problems/queue-reconstruction-by-height/

#include "utils.h"
#include <vector>
#include <algorithm>

namespace queue_reconstruction_by_height {

class Solution {
public:
    // Note: The people must contain only the following elements: {h, p}, where
    // h >= 0 is the person's height and p >= 0 is the number of persons with
    // the same or greater height which should stay before that person. If the
    // people is empty, or if there is no correct ordering, returns empty
    // result.
    //
    // Note: The original problem limits the height by 10^6. This solution does
    // not require such a limit.
    //
    // Time: O(n^2), Space: O(n), n - number of elements
    //
    std::vector<std::vector<int>> run(const std::vector<std::vector<int>>& people)
    {
        // Idea:
        // The element with the minimum height, let's call it (h, p), must be
        // placed at position p+1, because we need p elements with the same or
        // greater heights standing before it. If there are multiple elements
        // with the same minimum height (h, p1), (h, p2), ..., (h, pk), this is
        // true for each of them. When all these elements are placed at their
        // positions p1+1, p2+1, ..., pk+1, let's consider these elements and
        // positions as "used". Now we can repeat this for the remaining
        // elements and positions: take the elements with the next minimum
        // height and place them at _unused_ positions p1+1, p2+1, ..., pk+1.
        // Repeating so until all the elements are placed, we can construct the
        // result. Since all the positions are forced, this result is the only
        // possible. And if there is no free position for some element, the
        // result can't be constructed.
        //
        // Note: If, for each element (h, p), we would need p elements with the
        // greater (but not the same) height standing before it, then the
        // duplicated heights should be handled differently. First, we should
        // consider the position as "used" right after the element is placed,
        // because this position would not count for the same heights anymore.
        // Second, when we have (h, p1), (h, p2), ..., (h, pk), we should place
        // them by ascending p so that unused positions before the placed
        // element (which are reserved for the greater heights) could not be
        // taken by the same height.
        //
        // Note: It may be simplier to find out the algorithm if first consider
        // that all the heights are unique. Then, after finding the idea, we
        // can think about duplicated heights and see that they are either
        // handled in the same way (if both equal and greater heights are
        // counted) or handled by ascending p (if only the greater heights are
        // counted).

        if (!people.size())
            return {};

        // Sort
        auto people_sorted = people;

        std::sort(people_sorted.begin(), people_sorted.end(), [](const auto& e1, const auto& e2)
            {
                return e1[0] < e2[0];
//              return e1[0] < e2[0] || (e1[0] == e2[0] && e1[1] < e2[1]); // For the greater heights only
            }
        );

        // Reorder
        auto result = std::vector<std::vector<int>>(people.size(), {-1, -1});

        for (const auto& p : people_sorted) {
            const int p_height     = p[0]; 
            const int p_taller     = p[1];
            int       taller_found = 0;
            
            for (int i = 0; i < result.size(); i++) {
                auto&      r           = result[i];
                const int  r_height    = r[0];
                const bool r_is_free   = r_height < 0;
                const bool r_is_taller = r_is_free || r_height == p_height;
//              const bool r_is_taller = r_is_free; // For the greater heights only
                
                if (r_is_taller && taller_found++ == p_taller) {
                    if (r_is_free) {
                        r = p;
                    } else {
                        taller_found = 0;
                    }
                    break;
                }
            }

            // We have not found the required taller positions, the result
            // can't be constructed
            if (taller_found != p_taller + 1)
                return {};
        }

        return result;
    }
};

int main()
{
    const auto test = [](const std::vector<std::vector<int>>& people, const std::vector<std::vector<int>>& expected)
    {
        ASSERT_EX( Solution().run(people) == expected,
            to_string(people) + " -> " + to_string(expected) );
    };

    test({}, {});

    test({{0, 0}}, {{0, 0}});
    test({{1, 0}}, {{1, 0}});
    test({{2, 0}}, {{2, 0}});

    test({{0, 0}, {0, 1}}, {{0, 0}, {0, 1}});
    test({{0, 1}, {0, 0}}, {{0, 0}, {0, 1}});
    test({{1, 0}, {1, 1}}, {{1, 0}, {1, 1}});
    test({{1, 1}, {1, 0}}, {{1, 0}, {1, 1}});

    test({{0, 0}, {1, 0}}, {{0, 0}, {1, 0}});
    test({{1, 0}, {0, 0}}, {{0, 0}, {1, 0}});
    
    test({{0, 1}, {1, 0}}, {{1, 0}, {0, 1}});
    test({{1, 0}, {0, 1}}, {{1, 0}, {0, 1}});

    test({{2, 0}, {1, 1}}, {{2, 0}, {1, 1}});

    test({{1, 0}, {2, 0}, {3, 0}}, {{1, 0}, {2, 0}, {3, 0}});
    test({{1, 1}, {2, 0}, {3, 0}}, {{2, 0}, {1, 1}, {3, 0}});
    test({{1, 2}, {2, 0}, {3, 0}}, {{2, 0}, {3, 0}, {1, 2}});
    test({{1, 1}, {2, 1}, {3, 0}}, {{3, 0}, {1, 1}, {2, 1}});

    // Note: The original problem limits the height by 10^6
    const int MAX = std::numeric_limits<int>::max();

    test({{MAX, 0}}, {{MAX, 0}});
    test({{MAX, 0}, {MAX, 1}}, {{MAX, 0}, {MAX, 1}});
    test({{MAX, 0}, {MAX-1, 1}}, {{MAX, 0}, {MAX-1, 1}});

    test({{2, 2}, {0, 4}, {0, 0}, {MAX, 0}, {4, 1}, {MAX, 1}},
         {{0, 0}, {MAX, 0}, {4, 1}, {2, 2}, {0, 4}, {MAX, 1}});

    return 0;
}

}
