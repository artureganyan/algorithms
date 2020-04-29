// Problem: https://leetcode.com/problems/verify-preorder-serialization-of-a-binary-tree/

#include <string>
#include "utils.h"

namespace verify_preorder_serialization_of_a_binary_tree {

class Solution {
public:
    // Note: The serialization string must be of the following format:
    // "node1,node2,...,node_n", where node_i is the value of the i-th node or
    // '#' if the node is null, and the nodes follow in preorder traversal, and
    // every node has exactly 2 child nodes. An empty string or "#" are
    // considered a valid empty tree. A string with the trailing garbage like
    // "1,#,#,2" is considered invalid. The node values are not checked.
    //
    // Time: O(n), Space: O(h), Recursion depth <= h + 2
    // n - number of nodes, h - height of the tree
    //
    bool run(const std::string& serialization)
    {
        if (!serialization.size())
            return true;

        int start = 0;
        if (!isValid(serialization, start))
            return false;

        // Check for trailing characters, e.g. "1,#,#,2"
        return start == serialization.size();
    }

private:
    const char NULL_NODE = '#';
    const char SEPARATOR = ',';

    // Returns true if s[start, end) begins with the valid tree, and advances
    // the start right after the last character of this tree. Otherwise returns
    // false, with the undefined start. If start >= end, also returns false.
    //
    bool isValid(const std::string& s, int& start) const
    {
        if (start >= s.size())
            return false;

        const bool is_null_root = (s[start] == NULL_NODE);

        // Skip current token
        while (start < s.size() && s[start++] != SEPARATOR);

        if (is_null_root)
            return true;

        // Left subtree
        if (!isValid(s, start))
            return false;

        // Right subtree
        if (!isValid(s, start))
            return false;

        return true;
    }
};

int main()
{
    ASSERT( Solution().run(std::string()) == true );

    ASSERT( Solution().run("0") == false );
    ASSERT( Solution().run("1") == false );
    ASSERT( Solution().run("123") == false );

    ASSERT( Solution().run("#") == true );
    ASSERT( Solution().run("#,#") == false );
    ASSERT( Solution().run("#,#,#") == false );
    ASSERT( Solution().run("#,1") == false );
    ASSERT( Solution().run("#,1,1") == false );
    ASSERT( Solution().run("#,1,#") == false );

    ASSERT( Solution().run("1,#") == false );
    ASSERT( Solution().run("1,#,#") == true );
    ASSERT( Solution().run("123,#,#") == true );

    ASSERT( Solution().run("1,1") == false );
    ASSERT( Solution().run("1,1,1") == false );
    ASSERT( Solution().run("1,1,1,#") == false );
    ASSERT( Solution().run("1,1,1,#,#") == false );
    ASSERT( Solution().run("1,1,#") == false );
    ASSERT( Solution().run("1,1,#,#") == false );
    ASSERT( Solution().run("1,1,#,#,#") == true );
    ASSERT( Solution().run("1,1,#,#,1") == false );
    ASSERT( Solution().run("1,1,#,#,1,#") == false );
    ASSERT( Solution().run("1,1,#,#,1,#,#") == true );

    ASSERT( Solution().run("1,2,#,#,#") == true );
    ASSERT( Solution().run("1,#,2,#,#") == true );
    ASSERT( Solution().run("1,2,#,#,3,#,#") == true );
    ASSERT( Solution().run("1,2,3,#,#,#,#") == true );
    ASSERT( Solution().run("1,#,2,#,3,#,#") == true );
    ASSERT( Solution().run("1,2,#,3,#,#,#") == true );
    ASSERT( Solution().run("1,#,2,3,#,#,#") == true );
    ASSERT( Solution().run("1,2,3,#,#,4,#,#,5,6,#,#,7,#,#") == true );

    return 0;
}

}
