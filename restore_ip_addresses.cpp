// Problem: https://leetcode.com/problems/restore-ip-addresses/

#include <string>
#include <vector>
#include <algorithm>
#include "utils.h"

namespace restore_ip_addresses {

class Solution {
public:
    // Note: The string must contain only digit characters '0'-'9'. If it can
    // not represent a valid ip address (e.g. "256000000000" or too short/long
    // string), returns an empty result.
    //
    std::vector<std::string> run(const std::string& s)
    {
        if (!s.size() || s.size() < 4 || s.size() > 12)
            return {};

        std::vector<std::string> result;
        generateAddresses(result, s, 0, TOTAL_DOT_COUNT);
        return result;
    }

private:
    const int  TOTAL_DOT_COUNT = 3;

    inline bool isValidByte(const std::string& s, int pos, int size) const
    {
        if (size <= 0 || size > 3 || pos < 0 || pos + size > s.size())
            return false;

        if (size == 3) {
            int byte        = 0;
            int multiplier  = 1;
            for (int i = pos + size - 1; i >= pos; i--) {
                byte       += (s[i] - '0') * multiplier;
                multiplier *= 10;
            }
            return byte <= 255;
        }
        return true;
    }

    void generateAddresses(std::vector<std::string>& result, const std::string& s, int s_start, int dot_count) const
    {
        if (s_start >= s.size())
            return;

        // We are at the last byte
        if (dot_count == 0) {
            if (isValidByte(s, s_start, s.size() - s_start)) {
                // Generate the dotted string ending with the last byte: "...<last byte>"
                std::string ip(s.size() + TOTAL_DOT_COUNT, '.');
                std::copy(s.begin() + s_start, s.end(), ip.begin() + s_start + TOTAL_DOT_COUNT);
                result.push_back(ip);
            }
            return;
        }

        // Try to put dot after the first 1, 2 or 3 characters, starting from s_start
        for (int i = s_start; i < s_start + 3; i++) {
            if (!isValidByte(s, s_start, i - s_start + 1))
                continue;

            const int prev_size = result.size();
            generateAddresses(result, s, i + 1, dot_count - 1);
            for (int r = prev_size; r < result.size(); r++) {
                std::copy(s.begin() + s_start, s.begin() + i + 1,
                    result[r].begin() + s_start + (TOTAL_DOT_COUNT - dot_count));
            }
        }
    }
};


int main()
{
    ASSERT( compare_sets(Solution().run(std::string()), {}) );
    ASSERT( compare_sets(Solution().run("1"), {}) );
    ASSERT( compare_sets(Solution().run("12"), {}) );
    ASSERT( compare_sets(Solution().run("123"), {}) );

    ASSERT( compare_sets(Solution().run("0000"), {"0.0.0.0"}) );
    ASSERT( compare_sets(Solution().run("1111"), {"1.1.1.1"}) );
    ASSERT( compare_sets(Solution().run("1234"), {"1.2.3.4"}) );
    ASSERT( compare_sets(Solution().run("9999"), {"9.9.9.9"}) );

    ASSERT( compare_sets(Solution().run("11111"),
        {"1.1.1.11", "1.1.11.1", "1.11.1.1", "11.1.1.1"}) );

    ASSERT( compare_sets(Solution().run("12345"),
        {"1.2.3.45", "1.2.34.5", "1.23.4.5", "12.3.4.5"}) );

    ASSERT( compare_sets(Solution().run("99999"),
        {"9.9.9.99", "9.9.99.9", "9.99.9.9", "99.9.9.9"}) );

    ASSERT( compare_sets(Solution().run("111111"),
        {"1.1.1.111", "1.1.11.11", "1.1.111.1", "1.11.11.1", "1.11.1.11", "1.111.1.1",
         "11.1.1.11", "11.1.11.1", "11.11.1.1",
         "111.1.1.1"}) );

    ASSERT( compare_sets(Solution().run("125678"),
        {"1.2.56.78", "1.25.6.78", "1.25.67.8",
         "12.5.6.78", "12.5.67.8", "12.56.7.8",
         "125.6.7.8"}) );

    ASSERT( compare_sets(Solution().run("0000000000"),
        {"0.000.000.000",
         "00.00.000.000", "00.000.00.000", "00.000.000.00",
         "000.000.000.0", "000.000.00.00", "000.000.0.000", "000.00.00.000", "000.00.000.00", "000.0.000.000"}) );

    ASSERT( compare_sets(Solution().run("00000000000"),
        {"00.000.000.000",
         "000.00.000.000", "000.000.00.000", "000.000.000.00"}) );

    ASSERT( compare_sets(Solution().run("000000000000"), {"000.000.000.000"}) );
    ASSERT( compare_sets(Solution().run("099099099099"), {"099.099.099.099"}) );
    ASSERT( compare_sets(Solution().run("100100100100"), {"100.100.100.100"}) );
    ASSERT( compare_sets(Solution().run("123145167189"), {"123.145.167.189"}) );
    ASSERT( compare_sets(Solution().run("200200200200"), {"200.200.200.200"}) );
    ASSERT( compare_sets(Solution().run("255255255255"), {"255.255.255.255"}) );
    ASSERT( compare_sets(Solution().run("256255255255"), {}) );
    ASSERT( compare_sets(Solution().run("255256255255"), {}) );
    ASSERT( compare_sets(Solution().run("255255256255"), {}) );
    ASSERT( compare_sets(Solution().run("255255255256"), {}) );
    ASSERT( compare_sets(Solution().run("300000000000"), {}) );

    return 0;
}

}
