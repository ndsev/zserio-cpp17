#ifndef ZSERIO_VARIANT_H_INC
#define ZSERIO_VARIANT_H_INC

#include "zserio/AllocatorHolder.h"
#include "zserio/Any.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/HashCodeUtil.h"
#include "zserio/Traits.h"

namespace zserio
{

namespace detail
{

template <size_t I, typename... T>
struct type_at
{
    using type = std::tuple_element_t<I, std::tuple<T...>>;
};

template <auto I, typename... T>
using type_at_t = typename type_at<static_cast<size_t>(I), T...>::type;

} // namespace detail

/**
 * Exception thrown when variant index doesn't match or variant is in valueless state.
 */
class BadVariantAccess : public CppRuntimeException
{
public:
    using CppRuntimeException::CppRuntimeException;
};

template <auto I>
struct in_place_index_t
{};

/**
 * Like std::in_place_index but works with any enum index type.
 */
template <auto I>
constexpr in_place_index_t<I> in_place_index{};

/**
 * Implementation of variant type using BasicAny which allocates memory when selected
 * type doesn't fit under 3*sizeof(void*)
 *
 * Largely compatible with std::variant with following differences:
 * - access only through index whose type has to be specified (by design)
 * - supplied INDEX type needs to be convertible to/from size_t and std::variant_npos
 * - holds_alternative() omitted
 * - get/get_if() exist as member functions as well
 */
template <typename ALLOC, typename INDEX, typename... T>
class BasicVariant
{
    using AllocTraits = std::allocator_traits<ALLOC>;

public:
    using allocator_type = ALLOC;
    using IndexType = INDEX;

    /**
     * Empty constructor.
     */
    BasicVariant() :
            BasicVariant(ALLOC())
    {}

    /**
     * Constructor from given allocator.
     *
     * \param allocator Allocator to be used.
     *
     * \throw can throw any exception thrown by T[0]
     */
    explicit BasicVariant(const ALLOC& allocator) :
            m_data(allocator)
    {
        emplace<INDEX{}>(); // enforce no empty state like std::variant
    }

    /**
     * Constructor with initial emplacement.
     *
     * \param in_place_index Index of the active element.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T[I]
     */
    template <INDEX I, typename... ARGS, std::enable_if_t<!is_first_allocator_v<ARGS...>>* = nullptr>
    explicit BasicVariant(in_place_index_t<I>, ARGS&&... args)
    {
        emplace<I>(std::forward<ARGS>(args)...);
    }

    /**
     * Constructor with allocator and initial emplacement.
     *
     * \param in_place_index Index of the active element.
     * \param allocator Allocator to be used.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T[I]
     */
    template <INDEX I, typename... ARGS>
    BasicVariant(in_place_index_t<I>, const ALLOC& allocator, ARGS&&... args) :
            m_data(allocator)
    {
        emplace<I>(std::forward<ARGS>(args)...);
    }

    /**
     * Copy constructor.
     *
     * \param other Variant to copy.
     */
    BasicVariant(const BasicVariant& other) :
            m_data(other.m_data),
            m_index(other.m_index)
    {}

    /**
     * Allocator-extended copy constructor.
     *
     * \param other Variant to copy.
     * \param allocator Allocator to be used for dynamic memory allocations.
     *
     * \throw can throw any exception thrown by the active element in other
     */
    BasicVariant(const BasicVariant& other, const ALLOC& allocator) :
            m_data(other.m_data, allocator),
            m_index(other.m_index)
    {}

    /**
     * Copy assignment operator.
     *
     * \param other Variant to copy.
     *
     * \return Reference to this.
     *
     * \throw can throw any exception thrown by the active element in other
     */
    BasicVariant& operator=(const BasicVariant& other)
    {
        m_data = other.m_data;
        m_index = other.m_index;
        return *this;
    }

    /**
     * Move constructor.
     *
     * \param other Variant to move from.
     *
     * \throw can throw any exception thrown by the active element in other
     */
    BasicVariant(BasicVariant&& other) :
            m_data(std::move(other.m_data)),
            m_index(other.m_index)
    {}

    /**
     * Allocator-extended move constructor.
     *
     * \param other Variant to move from.
     * \param allocator Allocator to be used for dynamic memory allocations.
     *
     * \throw can throw any exception thrown by the active element in other
     */
    BasicVariant(BasicVariant&& other, const ALLOC& allocator) :
            m_data(std::move(other.m_data), allocator),
            m_index(other.m_index)
    {}

    /**
     * Desctructor.
     */
    ~BasicVariant() = default;

    /**
     * Move assignment operator.
     *
     * \param other Variant to move from.
     *
     * \return Reference to this.
     */
    BasicVariant& operator=(BasicVariant&& other)
    {
        if (this != &other)
        {
            m_data = std::move(other.m_data);
            m_index = other.m_index;
        }

        return *this;
    }

    /**
     * Reports if variant is in nonstandard empty state.
     * Note: which operations lead to an empty state is not specified by ISO C++
     *
     * As an zserio extension, the valueless state can arise after move. This happens only When the held
     * alternative is stored on the heap, and the used allocator is the same in both variants. In that case,
     * the underlying pointer is stolen and the moved-out variant remains in the valueless state.
     */
    bool valueless_by_exception() const noexcept
    {
        return static_cast<size_t>(index()) == std::variant_npos || !m_data.hasValue();
    }

    /**
     * Constructs a value of type U at index I with given arguments.
     *
     * \param args Arguments to be forwarded for element construction.
     */
    template <INDEX I, typename... ARGS>
    decltype(auto) emplace(ARGS&&... args)
    {
        using U = detail::type_at_t<I, T...>;
        auto& ret = m_data.template emplace<U>(std::forward<ARGS>(args)...);
        m_index = I;
        return ret;
    }

    /**
     * Returns an index of active element.
     */
    INDEX index() const noexcept
    {
        return m_data.hasValue() ? m_index : static_cast<INDEX>(std::variant_npos);
    }

    /**
     * Returns a pointer to an element at index I or nullptr if index doesn't match.
     */
    template <INDEX I, typename U = detail::type_at_t<I, T...>>
    U* get_if() noexcept
    {
        if (I != index())
        {
            return nullptr;
        }
        return m_data.template get_if<U>();
    }

    /**
     * Returns a pointer to an element at index I or nullptr if index doesn't match.
     */
    template <INDEX I, typename U = detail::type_at_t<I, T...>>
    const U* get_if() const noexcept
    {
        if (I != index())
        {
            return nullptr;
        }
        return m_data.template get_if<U>();
    }

    /**
     * Returns element at index I. Throws if index doesn't match
     *
     * \throw BadVariantAccess if the requested index doesn't match.
     */
    template <INDEX I, typename U = detail::type_at_t<I, T...>>
    U& get()
    {
        auto ptr = get_if<I>();
        if (!ptr)
        {
            throw BadVariantAccess("Variant: Attempt to retrieve an inactive element at index ")
                    << static_cast<size_t>(I) << ". Active element index is " << static_cast<size_t>(index());
        }
        return *ptr;
    }

    /**
     * Returns element at index I. Throws if index doesn't match
     *
     * \throw BadVariantAccess if the requested index doesn't match.
     */
    template <INDEX I, typename U = detail::type_at_t<I, T...>>
    const U& get() const
    {
        auto ptr = get_if<I>();
        if (!ptr)
        {
            throw BadVariantAccess("Variant: Attempt to retrieve an inactive element at index ")
                    << static_cast<size_t>(I) << ". Active element index is " << static_cast<size_t>(index());
        }
        return *ptr;
    }

    /**
     * Swaps content of variant with other.
     *
     * \param other Other variant to swap content with.
     */
    void swap(BasicVariant& other)
    {
        m_data.swap(other.m_data);
        std::swap(m_index, other.m_index);
    }

    /**
     * Calls a given functor with active element.
     *
     * \param fun Functor to be called on the active element.
     *
     * \throw BadVariantAccess if variant is in valueless state.
     */
    template <typename F>
    auto visit(F&& fun) -> decltype(fun(std::declval<detail::type_at_t<0, T...>>()))
    {
        if (valueless_by_exception())
        {
            throw BadVariantAccess("Variant: Cannot visit variant in valueless state");
        }
        using R = decltype(fun(std::declval<detail::type_at_t<0, T...>>()));
        if constexpr (std::is_same_v<R, void>)
        {
            std::nullptr_t dummy;
            visitSeq(std::forward<F>(fun), dummy, std::make_index_sequence<sizeof...(T)>());
        }
        else
        {
            R ret;
            visitSeq(std::forward<F>(fun), ret, std::make_index_sequence<sizeof...(T)>());
            return ret;
        }
    }

    /**
     * Calls a given functor with active element.
     *
     * \param fun Functor to be called on the active element.
     *
     * \throw BadVariantAccess if variant is in valueless state.
     */
    template <typename F>
    auto visit(F&& fun) const -> decltype(fun(std::declval<detail::type_at_t<0, T...>>()))
    {
        if (valueless_by_exception())
        {
            throw BadVariantAccess("Variant: Cannot visit variant in valueless state");
        }
        using R = decltype(fun(std::declval<detail::type_at_t<0, T...>>()));
        if constexpr (std::is_same_v<R, void>)
        {
            std::nullptr_t dummy;
            visitSeq(std::forward<F>(fun), dummy, std::make_index_sequence<sizeof...(T)>());
        }
        else
        {
            R ret;
            visitSeq(std::forward<F>(fun), ret, std::make_index_sequence<sizeof...(T)>());
            return ret;
        }
    }

    /**
     * Variant equality test.
     *
     * \param other Variant to compare with.
     */
    bool operator==(const BasicVariant& other) const
    {
        if (index() != other.index())
        {
            return false;
        }
        return equalSeq(other, std::make_index_sequence<sizeof...(T)>());
    }

    /**
     * Variant inequality test.
     *
     * \param other Variant to compare with.
     */
    bool operator!=(const BasicVariant& other) const
    {
        return !(*this == other);
    }

    /**
     * Variant less-than test.
     *
     * \param other Variant to compare with.
     */
    bool operator<(const BasicVariant& other) const
    {
        if (index() != other.index())
        {
            return index() < other.index();
        }
        return lessSeq(other, std::make_index_sequence<sizeof...(T)>());
    }

    /**
     * Variant greater-than test.
     *
     * \param other Variant to compare with.
     */
    bool operator>(const BasicVariant& other) const
    {
        return other < *this;
    }

    /**
     * Variant less-or-equal test.
     *
     * \param other Variant to compare with.
     */
    bool operator<=(const BasicVariant& other) const
    {
        return !(other < *this);
    }

    /**
     * Variant greater-or-equal test.
     *
     * \param other Variant to compare with.
     */
    bool operator>=(const BasicVariant& other) const
    {
        return !(other > *this);
    }

private:
    template <size_t... I, typename F, typename R>
    void visitSeq(F&& fun, R& returnValue, std::index_sequence<I...>)
    {
        (visit<I>(fun, returnValue), ...);
    }

    template <size_t... I, typename F, typename R>
    void visitSeq(F&& fun, R& returnValue, std::index_sequence<I...>) const
    {
        (visit<I>(fun, returnValue), ...);
    }

    template <size_t I, typename F, typename R>
    void visit(F&& fun, R& returnValue)
    {
        if (I != static_cast<size_t>(index()))
        {
            return;
        }
        if constexpr (std::is_same_v<R, std::nullptr_t>)
        {
            std::forward<F>(fun)(get<static_cast<INDEX>(I)>());
        }
        else
        {
            returnValue = std::forward<F>(fun)(get<static_cast<INDEX>(I)>());
        }
    }

    template <size_t I, typename F, typename R>
    void visit(F&& fun, R& returnValue) const
    {
        if (I != static_cast<size_t>(index()))
        {
            return;
        }
        if constexpr (std::is_same_v<R, std::nullptr_t>)
        {
            std::forward<F>(fun)(get<static_cast<INDEX>(I)>());
        }
        else
        {
            returnValue = std::forward<F>(fun)(get<static_cast<INDEX>(I)>());
        }
    }

    template <size_t... I>
    bool equalSeq(const BasicVariant& other, std::index_sequence<I...>) const
    {
        return (equal<I>(other) && ...);
    }

    template <size_t I>
    bool equal(const BasicVariant& other) const
    {
        if (I != static_cast<size_t>(index()))
        {
            return true;
        }
        return get<static_cast<INDEX>(I)>() == other.get<static_cast<INDEX>(I)>();
    }

    template <size_t... I>
    bool lessSeq(const BasicVariant& other, std::index_sequence<I...>) const
    {
        return (less<I>(other) && ...);
    }

    template <size_t I>
    bool less(const BasicVariant& other) const
    {
        if (I != static_cast<size_t>(index()))
        {
            return true;
        }
        return get<static_cast<INDEX>(I)>() < other.get<static_cast<INDEX>(I)>();
    }

    BasicAny<ALLOC> m_data;
    INDEX m_index;
};

// Using declarations

template <typename INDEX, typename... T>
using Variant = BasicVariant<std::allocator<uint8_t>, INDEX, T...>;

/**
 * Gets value of an element at given index.
 *
 * \param var Variant to be accessed.
 *
 * \throw CppRuntimeException if the requested type doesn't match to the stored value.
 */
template <auto I, typename ALLOC, typename INDEX, typename... T>
decltype(auto) get(BasicVariant<ALLOC, INDEX, T...>& var)
{
    static_assert(std::is_same_v<decltype(I), INDEX>, "Index has a wrong type");
    return var.template get<I>();
}

/**
 * Gets value of an element at given index.
 *
 * \param var Variant to be accessed.
 *
 * \throw CppRuntimeException if the requested type doesn't match to the stored value.
 */
template <auto I, typename ALLOC, typename INDEX, typename... T>
decltype(auto) get(const BasicVariant<ALLOC, INDEX, T...>& var)
{
    static_assert(std::is_same_v<decltype(I), INDEX>, "Index has a wrong type");
    return var.template get<I>();
}

/**
 *
 * \param var Variant to be accessed.
 * Returns a pointer to an element at index I or nullptr if index doesn't match.
 */
template <auto I, typename ALLOC, typename INDEX, typename... T>
decltype(auto) get_if(BasicVariant<ALLOC, INDEX, T...>* var)
{
    static_assert(std::is_same_v<decltype(I), INDEX>, "Index has a wrong type");
    return var->template get_if<I>();
}

/**
 * Returns a pointer to an element at index I or nullptr if index doesn't match.
 *
 * \param var Variant to be accessed.
 */
template <auto I, typename ALLOC, typename INDEX, typename... T>
decltype(auto) get_if(const BasicVariant<ALLOC, INDEX, T...>* var)
{
    static_assert(std::is_same_v<decltype(I), INDEX>, "Index has a wrong type");
    return var->template get_if<I>();
}

/**
 * Calls a given functor with active element.
 *
 * \param fun Functor to be called with the active element.
 * \param var Variant to be accessed.
 *
 * \throw BadVariantAccess if variant is in valueless state.
 */
template <typename F, typename ALLOC, typename INDEX, typename... T>
decltype(auto) visit(F&& fun, BasicVariant<ALLOC, INDEX, T...>& var)
{
    return var.visit(std::forward<F>(fun));
}

/**
 * Calls a given functor with active element.
 *
 * \param fun Functor to be called with the active element.
 * \param var Variant to be accessed.
 *
 * \throw BadVariantAccess if variant is in valueless state.
 */
template <typename F, typename ALLOC, typename INDEX, typename... T>
decltype(auto) visit(F&& fun, const BasicVariant<ALLOC, INDEX, T...>& var)
{
    return var.visit(std::forward<F>(fun));
}

/**
 * Calculates variant hash code from given seed.
 *
 * \param seed Initial hash value.
 * \param var Variant to calculate the hash from.
 */
template <typename ALLOC, typename INDEX, typename... T>
uint32_t calcHashCode(uint32_t seed, const BasicVariant<ALLOC, INDEX, T...>& var)
{
    uint32_t result = seed;
    result = calcHashCode(result, static_cast<size_t>(var.index()));
    var.visit([&result](const auto& value) {
        result = calcHashCode(result, value);
    });
    return result;
}

} // namespace zserio

namespace std
{

template <typename ALLOC, typename INDEX, typename... T>
struct hash<zserio::BasicVariant<ALLOC, INDEX, T...>>
{
    size_t operator()(const zserio::BasicVariant<ALLOC, INDEX, T...>& var) const
    {
        return zserio::calcHashCode(zserio::HASH_SEED, var);
    }
};

} // namespace std

#endif // ZSERIO_VARIANT_H_INC
