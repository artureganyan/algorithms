// Problem: https://leetcode.com/problems/longest-absolute-file-path/

#include <vector>
#include "utils.h"

namespace longest_absolute_file_path {

class Solution {
public:
    // Note: The filesystem must be of the following format:
    // "<item>\n[\t]<item>\n[\t]...<item>". Each item can contain any
    // characters (including escape sequences) except '\n', '\t', '\0'; if it
    // contains '.', this item is a file, otherwise it's a directory; the
    // first '.' must be followed by the file extension starting with at least
    // 1 non-dot character. Each item except the last one must be followed by
    // '\n' and, optionally, by one or more '\t'. '\n' separates items. '\t'
    // nests the successive item relative to the previous top-level directory;
    // the number of \t must be <= the same number for the previous directory
    // plus 1 (e.g. "dir1\n\tdir2" is correct but "dir1\n\t\tdir2" is not).
    //
    // Time: O(n), Space: O(p_max)
    // n - number of characters, p_max - maximum absolute path length
    //
    int run(const std::string& filesystem)
    {
        if (!filesystem.size())
            return 0;

        // Helpers
        const char ITEM_SEPARATOR      = '\n';
        const char NESTING_SEPARATOR   = '\t';
        const char EXTENSION_SEPARATOR = '.';

        const auto throw_error = [&](const std::string& message, int pos = -1)
        {
            throw std::invalid_argument(message + (pos >= 0 ?
                ", detected at " + to_string(pos) + " in " + filesystem : ""));
        };

        struct Item
        {
            std::string name;
            bool        is_file     = false;
            int         path_length = 0;
        };

        // Parse
        std::vector<Item> current_path;
        Item              current_item;
        bool              current_item_ready = false;
        int               current_item_level = 0;
        int               max_path_length    = 0; // In characters

        for (int i = 0; i <= filesystem.size(); i++) {
            const char c = i < filesystem.size() ? filesystem[i] : 0;

            if (c == ITEM_SEPARATOR || c == 0) {
                if (current_item_level > current_path.size())
                    throw_error("Incorrect nesting level: " + current_item.name, i);

                if (current_item.name.size() == 0) {
                    if (i < filesystem.size())
                        throw_error("Empty item name", i); else
                        throw_error("String can not end with item separator", i);
                }
    
                if (const size_t p = current_item.name.find(EXTENSION_SEPARATOR);
                    p != std::string::npos) {
                    if (p == i - 1 || current_item.name[p + 1] == EXTENSION_SEPARATOR)
                        throw_error("Incorrect file name: " + current_item.name, i);

                    current_item.is_file = true;
                }

                current_path.resize(current_item_level);
                if (current_path.size()) {
                    const auto& parent_item = current_path.back();
                    if (parent_item.is_file)
                        throw_error("File can't be a parent item: " + parent_item.name, i);
                    
                    current_item.path_length = parent_item.path_length + 1 /*"/"*/ + current_item.name.size();
                } else {
                    current_item.path_length = current_item.name.size();
                }
                current_path.push_back(current_item);
                
                if (current_item.is_file)
                    max_path_length = std::max(max_path_length, current_item.path_length);

                current_item       = Item();
                current_item_ready = false;
                current_item_level = 0;

            } else if (c == NESTING_SEPARATOR) {
                if (current_item.name.size())
                    throw_error("Missing item separator", i);

                current_item_level++;
            
            } else {
                current_item.name += c;
            }
        }

        return max_path_length;
    }
};

int main()
{
    ASSERT( Solution().run(std::string()) == 0 );

    ASSERT( Solution().run("a") == 0 );

    ASSERT( Solution().run(".a") == 2 );
    ASSERT( Solution().run(".a.") == 3 );
    ASSERT( Solution().run("a.b") == 3 );
    ASSERT( Solution().run("a.b.") == 4 );

    ASSERT( Solution().run("a\nb") == 0 );
    ASSERT( Solution().run("a\nbc") == 0 );
    ASSERT( Solution().run("a\nb\nc") == 0 );

    ASSERT( Solution().run("d\n.f") == 2 );
    ASSERT( Solution().run("d\nf.f") == 3 );
    ASSERT( Solution().run("f.f\nd") == 3 );

    ASSERT( Solution().run("d\n\tf.f") == 5 );
    ASSERT( Solution().run("d\n\tf.f\n\tf.f") == 5 );
    ASSERT( Solution().run("d\n\tf.f\n\tdddd") == 5 );
    ASSERT( Solution().run("d\n\tdddd\n\tf.f") == 5 );
    ASSERT( Solution().run("d\n\tf.f\n\tf.ff") == 6 );
    ASSERT( Solution().run("d\n\tf.ff\n\tf.f") == 6 );

    ASSERT( Solution().run("d\n\td\n\t\tf.f") == 7 );
    ASSERT( Solution().run("d\n\td\n\t\tf.f\n\tf.ffff") == 8 );
    ASSERT( Solution().run("d\n\tf.ffff\n\td\n\t\tf.f") == 8 );
    ASSERT( Solution().run("d\n\td\n\t\tf.f\n\td\n\t\tf.ff") == 8 );
    ASSERT( Solution().run("d\n\td\n\t\tf.ff\n\td\n\t\tf.f") == 8 );
    ASSERT( Solution().run("d\n\td\n\t\td\n\t\t\tf.f") == 9 );

    // root/dir1/dir2/file1.ext
    ASSERT( Solution().run("root\n\tdir1\n\t\tdir2\n\t\t\tfile1.ext") == 24 );
    // root/dir1/dir2/file1.ext root/dir1/file12345678.ext root/file123456789123.ext
    ASSERT( Solution().run("root\n\tdir1\n\t\tdir2\n\t\t\tfile1.ext\n\t\tfile12345678.ext\n\tfile123456789123.ext") == 26 );

    ASSERT_EXCEPTION( Solution().run("."), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run(".."), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a."), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("\n"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("\t"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a\n"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a\n\t\tb"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a\n\nb"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("a\tb"), std::invalid_argument );
    ASSERT_EXCEPTION( Solution().run("file.ext\n\tdir"), std::invalid_argument );

    return 0;
}

}
