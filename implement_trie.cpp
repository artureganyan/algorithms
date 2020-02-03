// Problem: https://leetcode.com/problems/implement-trie-prefix-tree/

#include <string>
#include <unordered_map>
#include <memory>
#include "utils.h"

namespace implement_trie {

// Straightforward implementation
//
// Time:  O(n) for all operations
// Space: O(d^n_max) for the tree in the worst case, when each possible word of
//        the length n_max is present in the tree
//
// n - length of the word, n_max - maximum length of the word,
// d - size of the alphabet

class Trie1 {
public:
    Trie1();

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

Trie1::Trie1()
{
    root_.reset(new Node);
}

void Trie1::insert(const std::string& word)
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

bool Trie1::search(const std::string& word) const
{
    return findString(word, true);
}

bool Trie1::startsWith(const std::string& prefix) const
{
    return findString(prefix, false);
}

// Finds the longest prefix of the string
Trie1::Prefix Trie1::findPrefix(const std::string& s) const
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
bool Trie1::findString(const std::string& s, bool whole_word) const
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


// Optimized for space
//
// Time:  O(n) for all operations
// Space: O(n_max) for the tree
//
// n - length of the word, n_max - maximum length of the word

class Trie2 {
public:
    Trie2();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool startsWith(const std::string& prefix) const;

private:
    enum {
        Char_Min   = 'a',
        Char_Max   = 'z',
        Char_Count = Char_Max - Char_Min + 1
    };

    class Node {
    public:
        void setChildren(char c, bool enabled);
        bool hasChildren(char c) const;

        void setWord(char c, bool enabled);
        bool hasWord(char c) const;

    private:
        int bit(char c) const;

        uint64_t children_ = 0;
    };

    struct Level
    {
        Node nodes[Char_Count] = {};
    };

    Node* getNode(int level, char c) const;
    bool  findString(const std::string& s, bool whole_word) const;

    std::vector<Level> levels_;
};

Trie2::Trie2()
{
    levels_.push_back({});
}

void Trie2::insert(const std::string& word)
{
    if (!word.size())
        return;

    Node* node = &levels_[0].nodes[0];
    for (int i = 0; i < word.size(); i++) {
        const char c = word[i];

        node->setChildren(c, true);
        if (i == word.size() - 1)
            node->setWord(c, true);

        if (levels_.size() <= i + 1)
            levels_.push_back({});

        node = &levels_[i + 1].nodes[c - Char_Min];
    }
}

bool Trie2::search(const std::string& word) const
{
    return findString(word, true);
}

bool Trie2::startsWith(const std::string& prefix) const
{
    return findString(prefix, false);
}

bool Trie2::findString(const std::string& s, bool whole_word) const
{
    if (!s.size())
        return false;

    const Node* node = &levels_[0].nodes[0];
    for (int i = 0; i < s.size(); i++) {
        const char c = s[i];

        if (!node->hasChildren(c))
            return false;

        if (i == s.size() - 1 && whole_word && !node->hasWord(c))
            return false;

        node = &levels_[i + 1].nodes[c - Char_Min];
    }
    return true;
}

inline void Trie2::Node::setChildren(char c, bool enabled)
{
    children_ |= 1 << bit(c);
}

inline bool Trie2::Node::hasChildren(char c) const
{
    return children_ & (1 << bit(c));
}

inline void Trie2::Node::setWord(char c, bool enabled)
{
    children_ |= 2 << bit(c);
}

inline bool Trie2::Node::hasWord(char c) const
{
    return children_ & (2 << bit(c));
}

inline int Trie2::Node::bit(char c) const
{
    return (c - Char_Min) * 2;
}


template <typename Trie>
void test()
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
}

int main()
{
    test<Trie1>();
    test<Trie2>();

    return 0;
}

}
