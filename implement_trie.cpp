// Problem: https://leetcode.com/problems/implement-trie-prefix-tree/

#include <string>
#include <unordered_map>
#include <memory>
#include "utils.h"

namespace implement_trie {

class Trie {
public:
    Trie();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool startsWith(const std::string& prefix) const;

private:
    struct Node
    {
        Node() : has_word(false) {}

        std::unordered_map<char, std::unique_ptr<Node>> children;
        bool has_word;
    };

    struct Prefix
    {
        Prefix() : node(nullptr), char_index(-1) {}
        Prefix(Node* n, int i) : node(n), char_index(i) {}

        Node* node;
        int   char_index;
    };

    Prefix findPrefix(const std::string& word) const;
    bool   findString(const std::string& s, bool whole_word) const;

    std::unique_ptr<Node> root_;
};


Trie::Trie()
{
    root_.reset(new Node);
}

// Time: O(n), Space: O(n), n - number of characters
void Trie::insert(const std::string& word)
{
    if (!word.size())
        return;

    const Prefix prefix = findPrefix(word);

    Node* node = prefix.node;
    for (int i = prefix.char_index + 1; i < word.size(); i++) {
        Node* child = new Node;
        node->children[word[i]].reset(child);
        node = child;
    }
    node->has_word = true;
}

// Time: O(n), Space: O(n), n - number of characters
bool Trie::search(const std::string& word) const
{
    return findString(word, true);
}

// Time: O(n), Space: O(n), n - number of characters
bool Trie::startsWith(const std::string& prefix) const
{
    return findString(prefix, false);
}

// Finds the longest prefix of the string
Trie::Prefix Trie::findPrefix(const std::string& s) const
{
    if (!s.size())
        return {};

    Node* node = root_.get();
    int   i    = 0;
    for (; i < s.size(); i++) {
        const auto ni = node->children.find(s[i]);
        if (ni == node->children.end())
            break;
        node = ni->second.get();
    }
    return {node, i - 1};
}

// Finds the string as a prefix or as the whole word
bool Trie::findString(const std::string& s, bool whole_word) const
{
    if (!s.size())
        return false;

    const Prefix prefix = findPrefix(s);
    if (prefix.char_index != s.size() - 1)
        return false;

    if (whole_word)
        return prefix.node->has_word;

    return true;
}


int main()
{
    Trie t;

    t.insert("");
    ASSERT( t.search("") == false );
    ASSERT( t.startsWith("") == false );

    for (int i = 0; i < 2; i++) {
        t.insert("a");
        ASSERT( t.search("a") == true );
        ASSERT( t.search("aa") == false );
        ASSERT( t.startsWith("a") == true );
        ASSERT( t.startsWith("aa") == false );
    }

    t.insert("ab");
    ASSERT( t.search("") == false );
    ASSERT( t.search("a") == true );
    ASSERT( t.search("ab") == true );
    ASSERT( t.startsWith("") == false );
    ASSERT( t.startsWith("a") == true );
    ASSERT( t.startsWith("b") == false );
    ASSERT( t.startsWith("ab") == true );
    ASSERT( t.startsWith("aba") == false );

    t.insert("b");
    ASSERT( t.search("b") == true );
    ASSERT( t.search("ba") == false );
    ASSERT( t.startsWith("b") == true );

    t.insert("cd");
    ASSERT( t.search("c") == false );
    ASSERT( t.search("cd") == true );

    t.insert("zz");
    ASSERT( t.search("z") == false );
    ASSERT( t.search("zz") == true );
    ASSERT( t.search("zzz") == false );
    ASSERT( t.startsWith("z") == true );
    ASSERT( t.startsWith("zz") == true );
    ASSERT( t.startsWith("zzz") == false );

    return 0;
}

}
