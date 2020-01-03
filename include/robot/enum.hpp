#include <functional>
#include <set>
template <class T>
class Enum
{
private:
    // Constructors
    explicit Enum(int Value);
    // Predicate for finding the corresponding instance
    struct Enum_Predicate_Corresponds : public std::unary_function<const Enum<T> *, bool>
    {
        Enum_Predicate_Corresponds(int Value) : m_value(Value) {}
        bool operator()(const Enum<T> *E)
        {
            return E->Get_Value() == m_value;
        }

    private:
        const int m_value;
    };
    // Comparison functor for the set of instances
    struct Enum_Ptr_Less : public std::binary_function<const Enum<T> *, const Enum<T> *, bool>
    {
        bool operator()(const Enum<T> *E_1, const Enum<T> *E_2)
        {
            return E_1->Get_Value() < E_2->Get_Value();
        }
    };

public:
    // Compiler-generated copy constructor and operator= are OK.
    typedef std::set<const Enum<T> *, Enum_Ptr_Less> instances_list;
    typedef instances_list::const_iterator const_iterator;
    // Access to int value
    int Get_Value(void) const { return m_value; }
    static int Min(void) { return (*s_instances.begin())->m_value; }
    static int Max(void) { return (*s_instances.rbegin())->m_value; }
    static const Enum<T> *Corresponding_Enum(int Value)
    {
        const_iterator it = find_if(s_instances.begin(), s_instances.end(),
                                    Enum_Predicate_Corresponds(Value));
        return (it != s_instances.end()) ? *it : NULL;
    }
    static bool Is_Valid_Value(int Value) { return Corresponding_Enum(Value) != NULL; }
    // Number of elements
    static instances_list::size_type size(void)
    {
        return s_instances.size();
    }
    // Iteration
    static const_iterator begin(void) { return s_instances.begin(); }
    static const_iterator end(void) { return s_instances.end(); }

private:
    int m_value;
    static instances_list s_instances;
};
template <class T>
inline Enum<T>::Enum(int Value) : m_value(Value)
{
    s_instances.insert(this);
}