// Problem: https://leetcode.com/problems/repeated-dna-sequences/

#include <vector>
#include <string>
#include <unordered_map>
#include "utils.h"

namespace repeated_dna_sequences {

class Solution {
public:
    // Note: The string must contain only the following characters: 'A', 'C',
    // 'G', 'T'. Overlapped sequences are not considered as a repetition (e.g.
    // 11 characters "AAAAAAAAAAA" contain 2 overlapped sequences).
    //
    // Time: O(n), Space: O(n), n - number of characters
    //
    std::vector<std::string> run(const std::string& s)
    {
        const int SEQUENCE_SIZE = 10;
        if (s.size() < SEQUENCE_SIZE * 2)
            return {};

        // Prepare character codes
        typedef std::unordered_map<char, int /*code*/> CharCodeMap;

        const int         code_size  = 2;
        const CharCodeMap char_codes = {
            {'A', 0},
            {'C', 1},
            {'G', 2},
            {'T', 3}
        };

        // Check the string
        typedef uint32_t Sequence;
        typedef std::unordered_map<Sequence, int /*pos*/> SequenceMap;

        SequenceMap              sequences;
        const Sequence           sequence_mask    = 0xFFFFF;
        Sequence                 current_sequence = 0;
        std::vector<std::string> result;

        for (int i = 0; i < s.size(); i++) {
            // Remove the first character and append the current one
            current_sequence  = (current_sequence << code_size) & sequence_mask;
            current_sequence |= char_codes.at(s[i]);

            // Wait until the first sequence is ready
            if (i < SEQUENCE_SIZE - 1)
                continue;

            // Check if such a sequence is already found and does not overlap with the current one
            auto si = sequences.find(current_sequence);
            if (si != sequences.end()) {
                auto& sequence_pos = si->second;
                if (sequence_pos != -1 && (i - sequence_pos + 1 >= SEQUENCE_SIZE * 2)) {
                    result.push_back(s.substr(sequence_pos, SEQUENCE_SIZE));
                    si->second = -1;
                }
            } else {
                sequences[current_sequence] = i - SEQUENCE_SIZE + 1;
            }
        }

        return result;
    }
};

int main()
{
    ASSERT( compare_sets(Solution().run(std::string()), {}) );
    ASSERT( compare_sets(Solution().run("A"), {}) );
    ASSERT( compare_sets(Solution().run("AA"), {}) );
    ASSERT( compare_sets(Solution().run("AAA"), {}) );
    ASSERT( compare_sets(Solution().run("AAAAAAAAAA"), {}) );
    ASSERT( compare_sets(Solution().run("AAAAAAAAAAA"), {}) );
    ASSERT( compare_sets(Solution().run("AAAAAAAAAAAAAAAAAAA"), {}) );

    ASSERT( compare_sets(Solution().run("AAAAAAAAAAAAAAAAAAAA"), {"AAAAAAAAAA"}) );
    ASSERT( compare_sets(Solution().run("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"), {"AAAAAAAAAA"}) );
    ASSERT( compare_sets(Solution().run("ACGTACGTACACGTACGTAC"), {"ACGTACGTAC"}) );

    ASSERT( compare_sets(Solution().run("AACCGGTTAAAACCGGTTAAAACC"),
        {"AACCGGTTAA", "ACCGGTTAAA", "CCGGTTAAAA", "CGGTTAAAAC", "GGTTAAAACC"}) );

    return 0;
}

}
