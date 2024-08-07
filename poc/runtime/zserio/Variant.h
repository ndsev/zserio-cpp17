#ifndef ZSERIO_VARIANT_H_INC
#define ZSERIO_VARIANT_H_INC

#include <variant>

namespace zserio
{

template <typename... T>
class Variant
{
public:
    template <size_t I, typename U>
    static Variant create(U&& val)
    {
        Variant<T...> var;
        var.template emplace<I>(std::forward<U>(val));
        return var;
    }

    template <size_t I>
    void emplace()
    {
        value.template emplace<I>();
    }
    template <size_t I, typename U>
    void emplace(U&& val)
    {
        value.template emplace<I>(std::forward<U>(val));
    }
    size_t index() const
    {
        return value.index();
    }
    template <size_t I>
    decltype(auto) get()
    {
        return std::get<I>(value);
    }
    template <size_t I>
    decltype(auto) get() const
    {
        return std::get<I>(value);
    }

    template <typename V>
    void visit(V&& visitor)
    {
        std::visit(std::forward<V>(visitor), value);
    }

    bool operator==(const Variant& v) const
    {
        return value == v.value;
    }
    bool operator!=(const Variant& v) const
    {
        return value != v.value;
    }
    bool operator<(const Variant& v) const
    {
        return value < v.value;
    }

private:
    friend ::std::hash<Variant>;
    std::variant<T...> value;
};

} // namespace zserio

namespace std
{

template <typename... T>
struct hash<::zserio::Variant<T...>>
{
    size_t operator()(const ::zserio::Variant<T...>& value) const
    {
        return ::std::hash<std::variant<T...>>()(value.value);
    }
};

} // namespace std

#endif