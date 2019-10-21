#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cassert>
#include <unordered_set>
#include <vector>


// Test

#define ASSERT(condition)                                           \
    {                                                               \
        if (condition) {                                            \
            std::cout << "Passed: " << #condition << std::endl;     \
        } else {                                                    \
            std::cerr << "Failed: " << #condition << std::endl;     \
            std::abort();                                           \
        }                                                           \
    }


// Set

template <template <typename, typename...> class Container, typename T>
bool compare_sets(const Container<T>& set1, const Container<T>& set2)
{
    std::unordered_set<T> s1(set1.begin(), set1.end());
    std::unordered_set<T> s2(set2.begin(), set2.end());
    return s1 == s2;
}

template <typename T>
inline bool compare_sets(const std::unordered_set<T>& set1, const std::unordered_set<T>& set2)
{
    return set1 == set2;
}

template <typename T>
bool compare_sets(const std::vector<std::vector<T>>& set1, const std::vector<std::vector<T>>& set2)
{
    for (const auto& s1 : set1) {
        bool found = false;
        for (const auto& s2 : set2) {
            if (compare_sets(s1, s2)) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}


// List

template <typename Node>
class DefaultListTraits {
public:
    typedef decltype(Node::val) Value;

    static Node* next(Node* node)
    {
        return node ? node->next : nullptr;
    }

    static int value(Node* node)
    {
        return node ? node->val : 0;
    }

    static Node* create(const Value& value)
    {
        return new Node{value};
    }

    static void destroy(Node* node)
    {
        if (node)
            delete node;
    }
};

template <typename Node, typename Traits = DefaultListTraits<Node>>
class TemporaryList {
public:
    TemporaryList(Node* node) : node_(node) {}

    ~TemporaryList()
    {
        Node* node = node_;
        while (node) {
            Node* next = node->next;
            Traits::destroy(node);
            node = next;
        }
    }

    operator Node*() const
    {
        return node_;
    }

private:
    Node* node_;
};

template <typename Node, typename Traits = DefaultListTraits<Node>>
std::vector<typename Traits::Value> list_to_vector(Node* head)
{
    std::vector<typename Traits::Value> result;

    Node* node = head;
    while (node) {
        result.push_back(Traits::value(node));
        node = node->next;
    }

    return result;
}

template <typename Node, typename Traits = DefaultListTraits<Node>>
Node* create_list(std::initializer_list<typename Traits::Value> values)
{
    Node* head = nullptr;
    Node* prev = nullptr;
    for (const typename Traits::Value& value : values) {
        Node* node = Traits::create(value);
        if (prev)
            prev->next = node;
        prev = node;
        if (!head)
            head = node;
    }
    return head;
}

template <typename Node, typename Traits = DefaultListTraits<Node>>
TemporaryList<Node> create_temp_list(std::initializer_list<typename Traits::Value> values)
{
    return TemporaryList<Node>(create_list<Node, Traits>(values));
}

template <typename Node, typename Traits>
bool compare_lists(Node* head1, Node* head2)
{
    while (head1 && head2) {
        if (Traits::value(head1) != Traits::value(head2))
            return false;
        head1 = Traits::next(head1);
        head2 = Traits::next(head2);
    }
    return head1 == nullptr && head2 == nullptr;
}

template <typename Node, typename Container, typename Traits = DefaultListTraits<Node>>
bool compare_lists(Node* head, const Container& values)
{
    return list_to_vector<Node, Traits>(head) == std::vector<typename Traits::Value>(values.begin(), values.end());
}

template <typename Node, typename Traits = ::DefaultListTraits<Node>>
bool compare_lists(Node* head, std::initializer_list<typename Traits::Value> values)
{
    return list_to_vector<Node, Traits>(head) == std::vector<typename Traits::Value>(values.begin(), values.end());
}

#endif
