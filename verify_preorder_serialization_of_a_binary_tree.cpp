// Problem: https://leetcode.com/problems/verify-preorder-serialization-of-a-binary-tree/

#include <string>
#include "utils.h"

namespace verify_preorder_serialization_of_a_binary_tree {

// Note: The serialization string must be of the following format:
// "node1,node2,...,node_n", where node_i is the value of the i-th node or
// '#' if the node is null, and the nodes follow in preorder traversal, and
// every node has exactly 2 child nodes. An empty string or "#" are
// considered a valid empty tree. A string with the trailing garbage like
// "1,#,#,2" is considered invalid. The node values are not checked.


// Straightforward recursive solution

class Solution1 {
public:
    // Time: O(n), Space: O(h), Recursion depth <= h + 2
    // n - number of characters, h - height of the tree
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
    static const char NULL_NODE = '#';
    static const char SEPARATOR = ',';

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


// Non-recursive solution

class Solution2 {
public:
    // Time: O(n), Space: O(1), n - number of characters
    bool run(const std::string& serialization)
    {
        // Idea:
        // To verify the serialization, we read it node by node and so traverse
        // the tree. If the tree is not finished, non-null nodes can appear at
        // any place and in any count (meaning that the tree grows deeper), so
        // they can't make the serialization invalid. The number of the expected
        // null nodes is strictly determined by the previous nodes:
        // 1. At the beginning, there may be only 1 null node, meaning an empty
        //    tree.
        // 2. If we read a non-null node, it must be followed by 2 child nodes,
        //    so the number is increased by 2, but also decreased by 1 because
        //    this node is not a null we have expected before.
        // 3. If we read a null node, the number is decreased by 1.
        //
        // So we start with 1 expected null node, go through the tree and
        // update this number until it becomes 0. Then, if there are no unread
        // characters, the serialization is correct, otherwise not.

        if (!serialization.size())
            return true;

        const char NULL_NODE = '#';
        const char SEPARATOR = ',';

        int null_nodes = 1;

        for (int i = 0; i < serialization.size();) {
            // Read the node
            int node_begin = i;
            int node_end   = node_begin;

            while (node_end < serialization.size() && serialization[node_end] != SEPARATOR)
                node_end++;

            // Check the node. Note: The node value could be verified here.
            if (serialization[node_begin] == NULL_NODE) {
                null_nodes--;

                if (null_nodes == 0)
                    return node_end == serialization.size();

                if (null_nodes < 0)
                    return false;
            } else {
                null_nodes++;
            }

            i = node_end + 1;
        }

        return false;
    }
};


template <typename Solution>
void test()
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
    ASSERT( Solution().run("0,#,#") == true );
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

    ASSERT( Solution().run("1,2345,#,#,67890,#,#") == true );

    const std::string MAX = to_string(std::numeric_limits<int>::max());
    ASSERT( Solution().run(MAX + "," + MAX + ",#,#," + MAX + ",#,#") == true );
}

int main()
{
    test<Solution1>();
    test<Solution2>();

    return 0;
}

}
