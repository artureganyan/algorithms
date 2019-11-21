#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <cassert>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <vector>


// Test

#define ASSERT_EX(condition, message)                               \
    {                                                               \
        if (condition) {                                            \
            std::cout << "Passed: " << (message) << std::endl;      \
        } else {                                                    \
            std::cerr << "Failed: " << (message) << std::endl;      \
            std::abort();                                           \
        }                                                           \
    }

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

template <template <typename, typename...> class Container, typename T>
bool compare_sets(const Container<T>& set1, const Container<T>& set2)
{
    if (set1.size() != set2.size())
        return false;

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
    auto compare = [](const auto& set1, const auto& set2)
    {
        if (set1.size() != set2.size())
            return false;

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
    };

    return compare(set1, set2) && compare(set2, set1);
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
    TemporaryList(Node* node) : head_(node) {}

    ~TemporaryList()
    {
        Node* node = head_;
        while (node) {
            Node* next = node->next;
            Traits::destroy(node);
            node = next;
        }
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
void destroy_list(Node* head)
{
    Node* node = head;
    while (node) {
        Node* next = node->next;
        Traits::destroy(node);
        node = next;
    }
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
