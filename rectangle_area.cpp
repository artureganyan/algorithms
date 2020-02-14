// Problem: https://leetcode.com/problems/rectangle-area/

#include <cmath>
#include "utils.h"

namespace rectangle_area {

class Solution {
public:
    // Time: O(1), Space: O(1)
    int run(int l1, int b1, int r1, int t1, int l2, int b2, int r2, int t2)
    {
        const int w = getIntersectionOfSegments(l1, r1, l2, r2);
        const int h = getIntersectionOfSegments(b1, t1, b2, t2);
        return w * h;
    }

private:
    int getIntersectionOfSegments(int l1, int r1, int l2, int r2) const
    {
        if (r1 < l1) std::swap(l1, r1);
        if (r2 < l2) std::swap(l2, r2);

        const bool l1_inside = (l2 <= l1 && l1 <= r2);
        const bool r1_inside = (l2 <= r1 && r1 <= r2);

        if (l1_inside) {
            if (r1_inside)
                return r1 - l1;
            return r2 - l1;
        }
        if (r1_inside)
            return r1 - l2;

        const bool l2_inside = (l1 <= l2 && l2 <= r1);
        if (l2_inside)
            return r2 - l2;

        return 0;
    }
};


struct Rect
{
    int l;
    int t;
    int w;
    int h;
};

std::string rect_to_string(const Rect& rect)
{
    std::stringstream s;
    s << "(" << rect.l << "," << rect.t << " " << rect.w << "x" << rect.h << ")";
    return s.str();
}

void test(const Rect& rect1, const Rect& rect2, int expected)
{
    const int result = Solution().run(rect1.l, rect1.t - rect1.h, rect1.l + rect1.w, rect1.t,
                                      rect2.l, rect2.t - rect2.h, rect2.l + rect2.w, rect2.t);

    ASSERT_EX( result == expected,
        rect_to_string(rect1) + ", " + rect_to_string(rect2) + " -> " + to_string(expected) );
}

int main()
{
    // Empty
    test({0, 0, 0, 0}, {0, 0, 0, 0}, 0);

    // Trivial
    test({0, 1, 1, 1}, {0, 1, 1, 1}, 1);
    test({0, 1, 1, 1}, {1, 1, 1, 1}, 0);

    // Intersection of different sizes
    {
        // Move the left rect 3x4 through the right one 2x3
        //
        // 5         * * *
        // 4 * * * * *   *
        // 3 *     * *   *
        // 2 *     * * * *
        // 1 *     *
        // 0 * * * *
        //   0 1 2 3 4 5 6 7

        const int results[] = {0, 0, 2, 4, 4, 2, 0, 0};
        for (int x = 0, i = 0; x <= 7; x++, i++) {
            test({x, 4, 3, 4}, {4, 5, 2, 3}, results[i]);
        }
    }

    // One inside another
    test({0, 4, 4, 4}, {1, 3, 2, 2}, 4);
    test({1, 3, 2, 2}, {0, 4, 4, 4}, 4);

    // Negative coordinates
    test({-2, -1, 4, 4}, {-1, -2, 2, 2}, 4);

    // Maximum/minimum values
    const int MAX = std::numeric_limits<int>::max();
    const int MIN = std::numeric_limits<int>::min();

    test({MAX - 1, MAX, 1, 1}, {0, MAX, MAX, MAX}, 1);
    test({MIN, MIN + 1, 1, 1}, {MIN, 0, MAX, -MIN}, 1);

    const int MAX_sqrt = std::sqrt(MAX);
    test({0, MAX, MAX_sqrt, MAX_sqrt}, {0, MAX, MAX_sqrt, MAX_sqrt}, MAX_sqrt * MAX_sqrt);

    return 0;
}

}
