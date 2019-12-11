#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <cassert>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>


// Test

#define ASSERT_EX(condition, message)                               \
    {                                                               \
        if (condition) {                                            \
            std::cout << "Passed: " << (message) << std::endl;      \
        } else {                                                    \
            std::cerr << "Failed: " << (message) << std::endl;      \
            std::abort();                                           \
        }                                                           \
    }                                                               \
    ((void)"Semicolon is required")

#define ASSERT(condition) \
    ASSERT_EX(condition, #condition)


// Strings

template <typename T>
std::string to_string(const T& value)
{
    std::stringstream s;
    s << value;
    return s.str();
}

template <typename T, template <typename, typename...> typename Container>
std::string to_string(const Container<T>& container)
{
    std::stringstream s;

    s << "{";
    int i = 0;
    for (const T& value : container) {
        if (i)
            s << ", ";
        s << to_string(value);
        i++;
    }
    s << "}";

    return s.str();
}

inline std::string to_string(bool value)
{
    return value ? "true" : "false";
}


// Set

template <typename Container, typename Compare = std::equal_to<typename Container::value_type>>
bool compare_sets(const Container& set1, const Container& set2)
{
    auto compare = [](const auto& set1, const auto& set2)
    {
        if (set1.size() != set2.size())
            return false;

        for (const auto& s1 : set1) {
            bool found = false;
            for (const auto& s2 : set2) {
                if (Compare()(s1, s2)) {
                    found = true;
                    break;
                }
            }
            if (!found)
                return false;
        }
        return true;
    };

    return compare(set1, set2) && compare(set2, set1);
}

template <typename Container, typename Compare = std::equal_to<typename Container::value_type>>
struct compare_sets_functor
{
    bool operator ()(const Container& set1, const Container& set2) const
    {
        return compare_sets<Container, Compare>(set1, set2);
    }
};

template <typename T, typename Compare = std::equal_to<T>>
bool compare_sets(const std::vector<std::vector<T>>& set1, const std::vector<std::vector<T>>& set2)
{
    return compare_sets<decltype(set1), compare_sets_functor<std::vector<T>, Compare>>(set1, set2);
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

    static void setNext(Node* node, Node* next)
    {
        if (node)
            node->next = next;
    }

    static Value value(const Node* node)
    {
        return node ? node->val : 0;
    }

    static void setValue(Node* node, const Value& value)
    {
        if (node)
            node->val = value;
    }

    static Node* create(const Value& value)
    {
        return new Node{value};
    }

    static void destroy(const Node* node)
    {
        if (node)
            delete node;
    }
};

template <typename Node, typename Traits = DefaultListTraits<Node>>
void destroy_list(Node* head)
{
    Node* node = head;
    while (node) {
        Node* next = Traits::next(node);
        Traits::destroy(node);
        node = next;
    }
}

template <typename Node, typename Traits = DefaultListTraits<Node>>
class TemporaryList {
public:
    TemporaryList(Node* node) : head_(node) {}

    ~TemporaryList()
    {
        destroy_list<Node, Traits>(head_);
    }

    operator Node*() const
    {
        return head_;
    }

private:
    Node* head_;
};

template <typename Node, typename Traits = DefaultListTraits<Node>>
std::vector<typename Traits::Value> list_to_vector(Node* head)
{
    std::vector<typename Traits::Value> result;

    Node* node = head;
    while (node) {
        result.push_back(Traits::value(node));
        node = Traits::next(node);
    }

    return result;
}

template <typename Node, typename Traits = DefaultListTraits<Node>>
std::string list_to_string(Node* head)
{
    std::string result = "{";

    Node* node = head;
    while (node) {
        if (result.size())
            result += ", ";
        result += to_string(Traits::value(node));
        node = Traits::next(node);
    }

    result += "}";
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
            Traits::setNext(prev, node);
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

template <typename Node, typename Traits = DefaultListTraits<Node>>
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


// Tree

template <typename Node>
class DefaultTreeTraits {
public:
    typedef decltype(Node::val) Value;

    enum {
        ChildCount = 2
    };

    static Node* child(Node* node, int index)
    {
        if (!node)
            return nullptr;

        return getChildRef(node, index);
    }

    static void setChild(Node* node, int index, Node* child)
    {
        if (node)
            getChildRef(node, index) = child;
    }

    static Value value(const Node* node)
    {
        return node ? node->val : 0;
    }

    static void setValue(Node* node, const Value& value)
    {
        if (node)
            node->val = value;
    }

    static Node* create(const Value& value)
    {
        return new Node{value};
    }

    static void destroy(const Node* node)
    {
        if (node)
            delete node;
    }

private:
    static Node*& getChildRef(Node* node, int index)
    {
        if (!node)
            throw std::runtime_error("DefaultTreeTraits: Null node");

        if (index < 0 || index >= ChildCount)
            throw std::out_of_range("DefaultTreeTraits: Incorrect child index");

        if (index == 0)
            return node->left;

        if (index == 1)
            return node->right;
    }
};

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
void destroy_tree(Node* root)
{
    if (!root)
        return;

    for (int i = 0; i < Traits::ChildCount; i++)
        destroy_tree(Traits::child(root, i));

    Traits::destroy(root);
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
void get_tree_nodes(Node* tree, std::vector<Node*>& nodes)
{
    if (!tree)
        return;

    nodes.push_back(tree);

    for (int i = 0; i < Traits::ChildCount; i++)
        get_tree_nodes(Traits::child(tree, i), nodes);
}

// Destroys trees even if they contain shared nodes (i.e. if one node is
// referenced by multiple trees)
template <typename Node, typename Traits = DefaultTreeTraits<Node>>
void destroy_trees(const std::vector<Node*>& trees)
{
    std::vector<Node*> nodes;
    for (auto tree : trees)
        get_tree_nodes(tree, nodes);

    std::sort(nodes.begin(), nodes.end());
    nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());

    for (auto node : nodes)
        Traits::destroy(node);
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
class TemporaryTree {
public:
    TemporaryTree(Node* root) : root_(root) {}

    ~TemporaryTree()
    {
        destroy_tree<Node, Traits>(root_);
    }

    operator Node*() const
    {
        return root_;
    }

private:
    Node* root_;
};

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
bool compare_trees(const Node* tree1, const Node* tree2)
{
    if (!tree1 || !tree2)
        return tree1 == tree2;

    if (tree1->val != tree2->val)
        return false;

    return compare_trees(tree1->left,  tree2->left) &&
           compare_trees(tree1->right, tree2->right);
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
struct compare_trees_functor
{
    bool operator ()(const Node* tree1, const Node* tree2) const
    {
        return compare_trees(tree1, tree2);
    }
};

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
std::string tree_to_string_impl(Node* tree, const std::string& indent, const std::string& null, bool is_root)
{
    std::stringstream result;

    if (!is_root)
        result << indent;

    if (tree) {
        result << to_string(tree->val) << std::endl;

        if (tree->left || tree->right) {
            std::string next_indent = indent;
            if (!is_root)
                next_indent += indent;

            result << tree_to_string_impl(tree->left,  next_indent, null, false);
            result << tree_to_string_impl(tree->right, next_indent, null, false);
        }
    } else {
        result << null << std::endl;
    }

    return result.str();
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
std::string tree_to_string_ex(Node* tree, const std::string& indent, const std::string& null)
{
    return tree_to_string_impl(tree, indent, null, true);
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
std::string tree_to_string(Node* tree)
{
    return tree_to_string_ex(tree, "    ", "null");
}


// Matrix

// The positive times means clockwise, the negative means counterclockwise
template <typename T>
std::vector<std::vector<T>> rotate(const std::vector<std::vector<T>>& matrix, int times = 1)
{
    if (!matrix.size() || !matrix[0].size())
        return matrix;

    times = times % 4;
    if (!times)
        return matrix;

    if (times < 0)
        times = 4 + times;

    const int rows_count = matrix.size();
    const int cols_count = matrix[0].size();

    std::vector<std::vector<T>> result;

    if (times == 1) {
        result.resize(cols_count, std::vector<T>(rows_count, 0));
        for (int r = 0; r < rows_count; r++)
            for (int c = 0; c < cols_count; c++)
                result[c][rows_count - r - 1] = matrix[r][c];

    } else if (times == 2) {
         result.resize(rows_count, std::vector<T>(cols_count, 0));
         for (int r = 0; r < rows_count; r++)
            for (int c = 0; c < cols_count; c++)
                result[rows_count - r - 1][cols_count - c - 1] = matrix[r][c];

    } else if (times == 3) {
        result.resize(cols_count, std::vector<T>(rows_count, 0));
        for (int r = 0; r < rows_count; r++)
            for (int c = 0; c < cols_count; c++)
                result[cols_count - c - 1][r] = matrix[r][c];
    }

    return result;
}


// OperationCounter
//
// This class helps to count different operations. Usage:
//
// OperationCounter counter;
//
// void function(int arg)
// {
//     OperationCounter::Call call = counter.addCall();
//
//     for (int i = 0; i < arg; i++) {
//         std::vector<int> v1 = some_data;
//         std::vector<int> v2 = v1;
//         counter.addOperation("copy", v1.size() + v2.size());
//     }
//     counter.addOperation("iteration", arg);
//
//     if (arg > 0)
//         function(arg - 1);
// }
//
// function(5);
//
// std::cout << counter.toString() << std::endl;
//
// Note: The class is not thread-safe.

class OperationCounter {
public:
    class Call {
        friend class OperationCounter;

    public:
        Call(Call&& other)
        {
            c_ = other.c_;
            other.c_ = nullptr;
        }

        ~Call()
        {
            if (c_)
                c_->call_depth_--;
        }

    private:
        Call(OperationCounter* counter)
        {
            c_ = counter;
            if (!c_)
                return;

            c_->call_count_++;
            c_->call_depth_++;
            c_->call_depth_max_ = std::max(c_->call_depth_max_, c_->call_depth_);
        }

        OperationCounter* c_;
    };

    struct Count
    {
        Count() :
            current(0),
            min(std::numeric_limits<int>::max()),
            max(std::numeric_limits<int>::min()) {}

        int current;
        int min;
        int max;
    };

    typedef std::unordered_map<std::string /*name*/, Count> OperationMap;

public:
    OperationCounter()
    {
        reset();
    }

    void addOperation(const std::string& name, int count = 1)
    {
        setOperation(name, operation_count_[name].current + count);
    }

    void setOperation(const std::string& name, int count)
    {
        Count& c  = operation_count_[name];
        c.current = count;
        c.min     = std::min(c.min, c.current);
        c.max     = std::max(c.max, c.current);
    }

    Count operationCount(const std::string& name) const
    {
        return operation_count_.at(name);
    }

    OperationMap allOperationCount() const
    {
        return operation_count_;
    }

    // Should be used as "Call call = counter.addCall()" at the beginning
    // of the function, within its body scope.
    Call addCall()
    {
        return Call(this);
    }

    int callCount() const
    {
        return call_count_;
    }

    int callDepth() const
    {
        return call_depth_;
    }

    int maxCallDepth() const
    {
        return call_depth_max_;
    }

    void reset()
    {
        operation_count_.clear();
        call_count_     = 0;
        call_depth_     = 0;
        call_depth_max_ = 0;
    }

    std::string toString() const
    {
        std::stringstream s;

        if (call_count_) {
            s << "Call count: "     << call_count_ << std::endl;
            s << "Max call depth: " << call_depth_max_ << std::endl;
        }
        for (auto item : operation_count_) {
            const std::string& name  = item.first;
            const Count&       count = item.second;
            s << name << ": "
              << count.current
              << " (min=" << count.min
              <<  " max=" << count.max
              << ")" << std::endl;
        }

        return s.str();
    }

private:
    OperationMap    operation_count_;
    int             call_count_;
    int             call_depth_;
    int             call_depth_max_;
};

#endif
