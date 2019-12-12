// Problem: https://leetcode.com/problems/unique-binary-search-trees/

#include "utils.h"

namespace unique_binary_search_trees {

class Solution {
public:
    int run(int n)
    {
        if (n < 1)
            return 0;

        return count(n);
    }

private:
    int count(int n) const
    {
        // Idea:
        // Let count(n) be the total number of search trees containing all and
        // only values 1, 2, ..., n. We can choose any of these values as a
        // root node. If we choose i as a root, there are count(i-1) left
        // subtrees with values 1, ..., i-1, and count(n-i) right subtrees with
        // values i+1, ..., n, resulting in count(i-1) * count(n-i) trees with
        // root i. Finally, let count(0) = 1 to make this work (actually it
        // represents an empty subtree).

        if (n <= 1)
            return 1;

        int result = 0;
        for (int i = 1; i <= n; i++) {
            result += count(i - 1) * count(n - i);
        }

        return result;
    }
};

int main()
{
    ASSERT( Solution().run(0) == 0 );
    ASSERT( Solution().run(1) == 1 );
    ASSERT( Solution().run(2) == 2 );
    ASSERT( Solution().run(3) == 5 );
    ASSERT( Solution().run(4) == (5 + 1*2 + 2*1 + 5) );
    ASSERT( Solution().run(5) == (14 + 1*5 + 2*2 + 5*1 + 14) );

    return 0;
}

}
