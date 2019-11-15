// Problem: https://leetcode.com/problems/simplify-path/

#include <string>
#include "utils.h"

namespace simplify_path {

class Solution {
public:
    // Time: O(n), Space: O(n), n - number of characters
    //
    // Note: The path can contain any characters except 0. It should start
    // with "/", otherwise the function returns an empty string.
    //
    std::string run(const std::string& path)
    {
        if (!path.size() || path[0] != '/')
            return {};

        std::string result;
        result.reserve(path.size());

        int prev_slash = 0;

        for (int i = 0; i <= path.size(); i++) {
            const char c = i < path.size() ? path[i] : 0;

            // Check what is between slashes
            if (c == '/' || c == 0) {
                const int part_size = result.size() - prev_slash + 1;

                // "//"
                if (part_size == 2) {
                    continue;
                // "/./"
                } else if (part_size == 3 && result[prev_slash + 1] == '.') {
                    result.resize(prev_slash + 1);
                    continue;
                // "/../"
                } else if (part_size == 4 && result[prev_slash + 1] == '.' && result[prev_slash + 2] == '.') {
                    int r = prev_slash - 1;
                    while (r >= 0 && result[r] != '/')
                        r--;
                    result.resize(std::max(r + 1, 1));
                    prev_slash = result.size() - 1;
                    continue;
                // "/name/"
                } else {
                    prev_slash = result.size();
                }
            }

            // Add character to result
            if (c != 0)
                result.push_back(c);
        }

        if (result.size() > 1 && result.back() == '/')
            result.pop_back();

        return result;
    }
};


int main()
{
    ASSERT( Solution().run("") == std::string() );
    ASSERT( Solution().run("abc") == std::string() );
    ASSERT( Solution().run(".") == std::string() );
    ASSERT( Solution().run("..") == std::string() );
    ASSERT( Solution().run(" /") == std::string() );

    ASSERT( Solution().run("/") == "/" );
    ASSERT( Solution().run("//") == "/" );
    ASSERT( Solution().run("///") == "/" );
    ASSERT( Solution().run("/.") == "/" );
    ASSERT( Solution().run("/./") == "/" );
    ASSERT( Solution().run("/././") == "/" );
    ASSERT( Solution().run("/..") == "/" );
    ASSERT( Solution().run("/../") == "/" );
    ASSERT( Solution().run("/../../") == "/" );
    ASSERT( Solution().run("/.././") == "/" );
    ASSERT( Solution().run("/./../") == "/" );
    ASSERT( Solution().run("//.//..//") == "/" );

    ASSERT( Solution().run("/ ") == "/ " );
    ASSERT( Solution().run("/  ") == "/  " );
    ASSERT( Solution().run("/. ") == "/. " );
    ASSERT( Solution().run("/.. ") == "/.. " );
    ASSERT( Solution().run("/...") == "/..." );
    ASSERT( Solution().run("/....") == "/...." );
    ASSERT( Solution().run("/\\") == "/\\" );
    ASSERT( Solution().run("/\\/") == "/\\" );

    ASSERT( Solution().run("/path") == "/path" );
    ASSERT( Solution().run("/path/") == "/path" );
    ASSERT( Solution().run("/path1/path2/") == "/path1/path2" );
    ASSERT( Solution().run("/path1/./path2/") == "/path1/path2" );
    ASSERT( Solution().run("/path1/../path2/") == "/path2" );
    ASSERT( Solution().run("/path1/../path2/../") == "/" );
    ASSERT( Solution().run("//path1//..//path2//./path3/") == "/path2/path3" );

    ASSERT( Solution().run("/..//a/../bcdef/./gh//1234/.") == "/bcdef/gh/1234" );

    return 0;
}

}
