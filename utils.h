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
#include <cstdlib>


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
bool compare_sets_of_sets(const std::vector<std::vector<T>>& set1, const std::vector<std::vector<T>>& set2)
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

template <typename NodePtr, typename Traits>
class BaseListIterator {
public:
    BaseListIterator(NodePtr node = nullptr) : node_(node) {}

    BaseListIterator(const BaseListIterator& other)
    {
        node_    = other.node_;
        visited_ = other.visited_;
    }

    BaseListIterator& operator =(const BaseListIterator& other)
    {
        if (this == &other)
            return *this;

        node_    = other.node_;
        visited_ = other.visited_;
        return *this;
    }

    BaseListIterator& operator++()
    {
        node_ = advance(node_);
        return *this;
    }

    BaseListIterator operator++(int /*post-increment*/)
    {
        const auto prev = *this;
        ++(*this);
        return prev;
    }

    bool atEnd() const
    {
        return node_ == nullptr;
    }

    bool atCycle() const
    {
        return visited_.find(node_) != visited_.end();
    }

    bool visitedAllNodes() const
    {
        return atEnd() || atCycle();
    }

    int index() const
    {
        const auto vi = visited_.find(node_);
        if (vi != visited_.end())
            return *vi;

        return visited_.size();
    }

    NodePtr operator ->() const
    {
        return node_;
    }

    NodePtr operator *() const
    {
        return node_;
    }

    operator bool() const
    {
        return !atEnd();
    }

    operator NodePtr() const
    {
        return node_;
    }

    bool operator ==(const BaseListIterator<NodePtr, Traits>& other) const
    {
        return node_ == other.node_;
    }

    bool operator !=(const BaseListIterator<NodePtr, Traits>& other) const
    {
        return !(*this == other);
    }

private:
    NodePtr advance(NodePtr node)
    {
        if (!node)
            return nullptr;

        if (visited_.find(node) == visited_.end())
            visited_[node] = visited_.size();

        node = Traits::next(node);
    }

    typedef std::unordered_map<NodePtr, int /*index*/> NodeMap;

    NodePtr node_;
    NodeMap visited_;
};

template <typename Node, typename Traits = DefaultListTraits<Node>>
class ListIterator : public BaseListIterator<Node*, Traits> {
public:
    using BaseListIterator<Node*, Traits>::BaseListIterator;
};

template <typename Node, typename Traits = DefaultListTraits<Node>>
class ConstListIterator : public BaseListIterator<const Node*, Traits> {
public:
    using BaseListIterator<const Node*, Traits>::BaseListIterator;
};

template <typename Node, typename Traits = DefaultListTraits<Node>>
void destroy_list(Node* head)
{
    ListIterator<Node, Traits> it(head);
    while (!it.visitedAllNodes()) {
        Traits::destroy(it++);
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

    operator Node*()
    {
        return head_;
    }

    operator const Node*() const
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

    ListIterator<Node, Traits> it(head);
    while (!it.visitedAllNodes()) {
        result.push_back(Traits::value(it));
        ++it;
    }

    return result;
}

template <typename Node, typename Traits = DefaultListTraits<Node>>
std::string list_to_string(Node* head)
{
    const char* SEPARATOR = ", ";

    std::string result = "{";
    int         count  = 0;

    ListIterator<Node, Traits> it(head);
    while (!it.visitedAllNodes()) {
        if (count++)
            result += SEPARATOR;
        result += to_string(Traits::value(it));
        ++it;
    }
    if (it.atCycle())
        result += SEPARATOR + to_string(Traits::value(it)) + SEPARATOR + "...";

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
TemporaryList<Node, Traits> create_temp_list(std::initializer_list<typename Traits::Value> values)
{
    return TemporaryList<Node, Traits>(create_list<Node, Traits>(values));
}

template <typename Node, typename Traits = DefaultListTraits<Node>>
bool compare_lists(Node* head1, Node* head2)
{
    ListIterator<Node, Traits> it1(head1);
    ListIterator<Node, Traits> it2(head2);
    while (!it1.visitedAllNodes() && !it2.visitedAllNodes()) {
        if (Traits::value(it1) != Traits::value(it2))
            return false;
        ++it1;
        ++it2;
    }
    return it1.index() == it2.index();
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

    static Node* child(const Node* node, int index)
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

    static int childCount(const Node* node)
    {
        return 2;
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
    static Node* const& getChildRef(const Node* node, int index)
    {
        if (!node)
            throw std::runtime_error("DefaultTreeTraits: Null node");

        if (index < 0 || index >= childCount(node))
            throw std::out_of_range("DefaultTreeTraits: Incorrect child index");

        if (index == 0)
            return node->left;

        if (index == 1)
            return node->right;
    }

    static Node*& getChildRef(Node* node, int index)
    {
        if (!node)
            throw std::runtime_error("DefaultTreeTraits: Null node");

        if (index < 0 || index >= childCount(node))
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

    const int child_count = Traits::childCount(root);
    for (int i = 0; i < child_count; i++)
        destroy_tree<Node, Traits>(Traits::child(root, i));

    Traits::destroy(root);
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
void get_tree_nodes(Node* tree, std::vector<Node*>& nodes)
{
    if (!tree)
        return;

    nodes.push_back(tree);

    const int child_count = Traits::childCount(tree);
    for (int i = 0; i < child_count; i++)
        get_tree_nodes<Node, Traits>(Traits::child(tree, i), nodes);
}

// Destroys trees even if they contain shared nodes (i.e. if one node is
// referenced by multiple trees)
template <typename Node, typename Traits = DefaultTreeTraits<Node>>
void destroy_trees(const std::vector<Node*>& trees)
{
    std::vector<Node*> nodes;
    for (auto tree : trees)
        get_tree_nodes<Node, Traits>(tree, nodes);

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

    Node* data() const
    {
        return root_;
    }

    operator Node*() const
    {
        return data();
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

    const int child_count1 = Traits::childCount(tree1);
    const int child_count2 = Traits::childCount(tree2);
    if (child_count1 != child_count2)
        return false;

    for (int i = 0; i < child_count1; i++) {
        if (!compare_trees<Node, Traits>(Traits::child(tree1, i), Traits::child(tree2, i)))
            return false;
    }
    return true;
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
struct compare_trees_functor
{
    bool operator ()(const Node* tree1, const Node* tree2) const
    {
        return compare_trees<Node, Traits>(tree1, tree2);
    }
};

// If the tree is a balanced search tree which values are within
// (min_value, max_value), returns its height, otherwise -1
template <typename Node, typename Traits = DefaultTreeTraits<Node>>
int is_balanced_search_tree_impl(Node* tree,
    const typename Traits::Value& min_value, const typename Traits::Value& max_value,
    bool allow_duplicates)
{
    if (!tree)
        return 0;

    const auto root_value = Traits::value(tree);
    if ((root_value <  min_value || root_value >  max_value) || (!allow_duplicates &&
        (root_value == min_value || root_value == max_value)))
        return -1;

    int height_left = is_balanced_search_tree_impl<Node, Traits>(Traits::child(tree, 0), min_value, root_value, allow_duplicates);
    if (height_left < 0)
        return -1;

    int height_right = is_balanced_search_tree_impl<Node, Traits>(Traits::child(tree, 1), root_value, max_value, allow_duplicates);
    if (height_right < 0)
        return -1;

    if (std::abs(height_left - height_right) > 1)
        return -1;

    return std::max(height_left, height_right) + 1;
}

// Note: For the null tree, returns false
template <typename Node, typename Traits = DefaultTreeTraits<Node>>
bool is_balanced_search_tree(Node* tree, bool allow_duplicates = true)
{
    if (!tree)
        return false;

    return is_balanced_search_tree_impl<Node, Traits>(tree,
        std::numeric_limits<typename Traits::Value>::min(),
        std::numeric_limits<typename Traits::Value>::max(),
        allow_duplicates) >= 0;
}

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

            result << tree_to_string_impl<Node, Traits>(tree->left,  next_indent, null, false);
            result << tree_to_string_impl<Node, Traits>(tree->right, next_indent, null, false);
        }
    } else {
        result << null << std::endl;
    }

    return result.str();
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
std::string tree_to_string_ex(Node* tree, const std::string& indent, const std::string& null)
{
    return tree_to_string_impl<Node, Traits>(tree, indent, null, true);
}

template <typename Node, typename Traits = DefaultTreeTraits<Node>>
std::string tree_to_string(Node* tree)
{
    return tree_to_string_ex<Node, Traits>(tree, "    ", "null");
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
