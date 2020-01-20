// Problem: https://leetcode.com/problems/compare-version-numbers/

#include <cstdlib>
#include <string>
#include "utils.h"

namespace compare_version_numbers {

class Solution {
public:
    // Note: Any part of the version must not exceed the maximum int value.
    //
    // Time: O(n), Space: O(1), n = version1.size() + version2.size()
    //
    int run(const std::string& version1, const std::string& version2)
    {
        const char* s1 = version1.data();
        const char* s2 = version2.data();

        while (*s1 && *s2) {
            const int v1 = extract_version(s1);
            const int v2 = extract_version(s2);
            if (v1 < v2) {
                return -1;
            } else if (v1 > v2) {
                return 1;
            }
        }
        while (*s1) {
            if (extract_version(s1) > 0)
                return 1;
        }
        while (*s2) {
            if (extract_version(s2) > 0)
                return -1;
        }
        return 0;
    }

private:
    static int extract_version(const char*& s)
    {
        if (!s)
            return 0;
#if 1
        int result = 0;
        while (const char c = *s) {
            if (c == '.') {
                s++;
                break;
            } else if (!std::isdigit(c)) {
                throw std::runtime_error("Incorrect version character");
            }
            result = (result * 10) + (c - '0');
            s++;
        }
        return result;
#else
        // The simpler way using std::strtol()
        char*     s_end  = nullptr;
        const int result = std::strtol(s, &s_end, 10);

        s = s_end;
        if (s && *s) {
            if (*s != '.')
                throw std::runtime_error("Incorrect version character");
            s++;
        }

        return result;
#endif
    }
};


void test(const std::string& version1, const std::string& version2, int expected)
{
    ASSERT_EX( Solution().run(version1, version2) == expected,
        version1 + ", " + version2 + " -> " + to_string(expected) );
}

void test_symmetric(const std::string& version1, const std::string& version2, int expected)
{
    test(version1, version2, expected);
    test(version2, version1, -expected);
}

int main()
{
    test          (std::string(), std::string(), 0);
    test_symmetric("", "0", 0);
    test          ("0", "0", 0);
    test_symmetric("00", "0", 0);
    test          ("00", "00", 0);

    test          ("0", "1", -1);
    test_symmetric("00", "1", -1);
    test_symmetric("0", "01", -1);
    test          ("1", "1", 0);
    test_symmetric("01", "1", 0);
    test_symmetric("2", "1", 1);
    test_symmetric("02", "1", 1);
    test_symmetric("2", "01", 1);
    test_symmetric("9", "10", -1);
    test_symmetric("10", "11", -1);
    test_symmetric("10", "90", -1);
    test_symmetric("100", "90", 1);
    test_symmetric("100", "99", 1);
    test_symmetric("100", "101", -1);

    test          ("0.0", "0.0", 0);
    test          ("0.1", "0.1", 0);
    test_symmetric("0.2", "0.1", 1);
    test_symmetric("0.02", "0.1", 1);
    test_symmetric("0.02", "0.01", 1);
    test_symmetric("0.10", "0.1", 1);
    test_symmetric("0.1", "1.0", -1);
    test_symmetric("0.1", "01.0", -1);
    test_symmetric("00.1", "01.0", -1);
    test_symmetric("1.0", "1.0", 0);
    test_symmetric("1.0", "1.1", -1);
    test_symmetric("1.0", "1.0.1", -1);
    test_symmetric("1.0", "1.0.0", 0);
    test_symmetric("1.0", "1.0.01", -1);
    test_symmetric("1.0", "1.0.0.0", 0);
    test_symmetric("1.0", "1.0.0.1", -1);

    test          ("001.023.456.078.9",  "001.023.456.0079.9", -1);

    return 0;
}

}
