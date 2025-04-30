#ifndef ZSERIO_VARIANT_H_INC
#define ZSERIO_VARIANT_H_INC

#include <variant>

#include "zserio/AllocatorHolder.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/HashCodeUtil.h"
#include "zserio/Traits.h"

namespace zserio
{

namespace detail
{

template <typename T, bool BIG = (sizeof(T) > 3 * sizeof(void*))>
struct variant_element
{
    using type = T;
    using is_heap_allocated = std::false_type;
};

template <typename T>
struct variant_element<T, true>
{
    using type = T*;
    using is_heap_allocated = std::true_type;
};

template <size_t I, typename... T>
struct type_at
{
    using type = std::tuple_element_t<I, std::tuple<T...>>;
};

template <auto I, typename... T>
using type_at_t = typename type_at<static_cast<size_t>(I), T...>::type;

template <auto I, typename... T>
struct is_variant_heap_allocated : variant_element<type_at_t<I, T...>>::is_heap_allocated
{};

template <auto I, class... T>
constexpr bool is_variant_heap_allocated_v = is_variant_heap_allocated<I, T...>::value;

template <typename... T>
struct any_variant_heap_allocated : std::false_type
{};

template <typename T, typename... ARGS>
struct any_variant_heap_allocated<T, ARGS...>
        : std::bool_constant<variant_element<T>::is_heap_allocated::value ||
                  any_variant_heap_allocated<ARGS...>::value>
{};

template <typename... ARGS>
inline constexpr bool any_variant_heap_allocated_v = any_variant_heap_allocated<ARGS...>::value;

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
 * Implementation of variant type which allocates memory when selected type
 * doesn't fit under 3*sizeof(void*)
 *
 * Largely compatible with std::variant with following differences:
 * - access only through index whose type has to be specified
 * - supplied INDEX type needs to be convertible to/from size_t
 * - holds_alternative() omitted
 * - variant_npos omitted
 * - get/get_if() exists as member functions as well
 */
template <typename ALLOC, typename INDEX, typename... T>
class BasicVariant : public AllocatorHolder<ALLOC>
{
    using AllocTraits = std::allocator_traits<ALLOC>;
    using AllocatorHolder<ALLOC>::get_allocator_ref;
    using AllocatorHolder<ALLOC>::set_allocator;

public:
    using AllocatorHolder<ALLOC>::get_allocator;
    using allocator_type = ALLOC;
    using IndexType = INDEX;
    using VariantType = std::variant<typename detail::variant_element<T>::type...>;

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
            AllocatorHolder<ALLOC>(allocator)
    {
        if constexpr (detail::is_variant_heap_allocated_v<0, T...>)
        {
            emplace<INDEX{}>(); // enforce no empty state like std::variant
        }
    }

    /**
     * Constructor with initial emplacement.
     *
     * \param in_place_index Index of the active element.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T[I]
     */
    template <INDEX I, typename... ARGS,
            std::enable_if_t<!detail::is_variant_heap_allocated_v<I, T...>>* = nullptr,
            std::enable_if_t<!is_first_allocator_v<ARGS...>>* = nullptr>
    explicit BasicVariant(in_place_index_t<I>, ARGS&&... args) :
            m_data(std::in_place_index<static_cast<size_t>(I)>, std::forward<ARGS>(args)...)
    {}

    /**
     * Constructor with allocator and initial emplacement.
     *
     * \param in_place_index Index of the active element.
     * \param allocator Allocator to be used.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T[I]
     */
    template <INDEX I, typename... ARGS,
            std::enable_if_t<!detail::is_variant_heap_allocated_v<I, T...>>* = nullptr>
    BasicVariant(in_place_index_t<I>, const ALLOC& allocator, ARGS&&... args) :
            AllocatorHolder<ALLOC>(allocator),
            m_data(std::in_place_index<static_cast<size_t>(I)>, std::forward<ARGS>(args)...)
    {}

    /**
     * Constructor with allocator and initial emplacement.
     *
     * \param in_place_index Index of the active element.
     * \param allocator Allocator to be used.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T[I]
     */
    template <INDEX I, typename... ARGS, typename U = detail::type_at_t<static_cast<size_t>(I), T...>,
            std::enable_if_t<detail::is_variant_heap_allocated_v<I, T...>>* = nullptr>
    BasicVariant(in_place_index_t<I>, const ALLOC& allocator, ARGS&&... args) :
            AllocatorHolder<ALLOC>(allocator),
            m_data(std::in_place_index<static_cast<size_t>(I)>, allocateValue<U>(std::forward<ARGS>(args)...))
    {}

    /**
     * Constructor with initial emplacement.
     *
     * \param in_place_index Index of the active element.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T[I]
     */
    template <INDEX I, typename... ARGS, typename U = detail::type_at_t<static_cast<size_t>(I), T...>,
            std::enable_if_t<detail::is_variant_heap_allocated_v<I, T...>>* = nullptr,
            std::enable_if_t<!is_first_allocator_v<ARGS...>>* = nullptr>
    explicit BasicVariant(in_place_index_t<I>, ARGS&&... args) :
            m_data(std::in_place_index<static_cast<size_t>(I)>, allocateValue<U>(std::forward<ARGS>(args)...))
    {}

    /**
     * Destructor.
     */
    ~BasicVariant()
    {
        clear();
    }

    /**
     * Copy constructor.
     *
     * \param other Variant to copy.
     */
    BasicVariant(const BasicVariant& other) :
            AllocatorHolder<ALLOC>(
                    AllocTraits::select_on_container_copy_construction(other.get_allocator_ref()))
    {
        copy(other);
    }

    /**
     * Allocator-extended copy constructor.
     *
     * \param other Variant to copy.
     * \param allocator Allocator to be used for dynamic memory allocations.
     *
     * \throw can throw any exception thrown by the active element in other
     */
    BasicVariant(const BasicVariant& other, const ALLOC& allocator) :
            AllocatorHolder<ALLOC>(allocator)
    {
        copy(other);
    }

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
        if (this != &other)
        {
            clear();
            if constexpr (AllocTraits::propagate_on_container_copy_assignment::value)
            {
                set_allocator(other.get_allocator_ref());
            }
            copy(other);
        }

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
            AllocatorHolder<ALLOC>(std::move(other.get_allocator_ref()))
    {
        move(std::move(other));
    }

    /**
     * Allocator-extended move constructor.
     *
     * \param other Variant to move from.
     * \param allocator Allocator to be used for dynamic memory allocations.
     *
     * \throw can throw any exception thrown by the active element in other
     */
    BasicVariant(BasicVariant&& other, const ALLOC& allocator) :
            AllocatorHolder<ALLOC>(allocator)
    {
        move(std::move(other));
    }

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
            clear();
            if constexpr (AllocTraits::propagate_on_container_move_assignment::value)
            {
                set_allocator(std::move(other.get_allocator_ref()));
            }
            move(std::move(other));
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
        if constexpr (detail::any_variant_heap_allocated_v<T...>)
        {
            return m_data.valueless_by_exception() ||
                    valuelessByMoveSeq(std::make_index_sequence<sizeof...(T)>());
        }
        else
        {
            return m_data.valueless_by_exception();
        }
    }

    /**
     * Sets any value to variant.
     *
     * \param args Arguments to be forwarded for element construction.
     */
    template <INDEX I, typename... ARGS>
    decltype(auto) emplace(ARGS&&... args)
    {
        clear();
        if constexpr (detail::is_variant_heap_allocated_v<I, T...>)
        {
            using U = detail::type_at_t<I, T...>;
            U* ptr = allocateValue<U>(std::forward<ARGS>(args)...);
            return *m_data.template emplace<static_cast<size_t>(I)>(ptr);
        }
        else
        {
            return m_data.template emplace<static_cast<size_t>(I)>(std::forward<ARGS>(args)...);
        }
    }

    /**
     * Returns an index of active element.
     */
    INDEX index() const noexcept
    {
        if constexpr (detail::any_variant_heap_allocated_v<T...>)
        {
            return valuelessByMoveSeq(std::make_index_sequence<sizeof...(T)>())
                    ? static_cast<INDEX>(std::variant_npos)
                    : static_cast<INDEX>(m_data.index());
        }
        else
        {
            return static_cast<INDEX>(m_data.index());
        }
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
        if constexpr (detail::is_variant_heap_allocated_v<I, T...>)
        {
            return *std::get_if<static_cast<size_t>(I)>(&m_data);
        }
        else
        {
            return std::get_if<static_cast<size_t>(I)>(&m_data);
        }
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
        if constexpr (detail::is_variant_heap_allocated_v<I, T...>)
        {
            return *std::get_if<static_cast<size_t>(I)>(&m_data);
        }
        else
        {
            return std::get_if<static_cast<size_t>(I)>(&m_data);
        }
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
        if constexpr (AllocTraits::propagate_on_container_swap::value)
        {
            using std::swap;
            swap(get_allocator_ref(), other.get_allocator_ref());
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
    template <size_t... I>
    bool valuelessByMoveSeq(std::index_sequence<I...>) const
    {
        return (valuelessByMove<I>() || ...);
    }

    template <size_t I>
    bool valuelessByMove() const
    {
        if constexpr (detail::is_variant_heap_allocated_v<I, T...>)
        {
            if (I != m_data.index())
            {
                return false;
            }
            return *std::get_if<I>(&m_data) == nullptr;
        }
        else
        {
            return false;
        }
    }

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

    template <typename U, typename... ARGS>
    U* allocateValue(ARGS&&... args)
    {
        using AllocType = RebindAlloc<ALLOC, U>;
        using ConcreteAllocTraits = std::allocator_traits<AllocType>;
        AllocType typedAlloc = get_allocator_ref();
        auto ptr = ConcreteAllocTraits::allocate(typedAlloc, 1);
        try
        {
            ConcreteAllocTraits::construct(typedAlloc, std::addressof(*ptr), std::forward<ARGS>(args)...);
        }
        catch (...)
        {
            ConcreteAllocTraits::deallocate(typedAlloc, ptr, 1);
            throw;
        }
        return ptr;
    }

    // same as for std::variant throwing dtors are not supported see
    // https://stackoverflow.com/questions/78602733/should-stdvariant-be-nothrow-destructible-when-its-alternative-has-potentially
    template <typename U>
    void destroyValue(U* ptr)
    {
        if (ptr)
        {
            using AllocType = RebindAlloc<ALLOC, U>;
            using ConcreteAllocTraits = std::allocator_traits<AllocType>;
            AllocType typedAlloc = get_allocator_ref();
            ConcreteAllocTraits::destroy(typedAlloc, ptr);
            ConcreteAllocTraits::deallocate(typedAlloc, ptr, 1);
        }
    }

    void copy(const BasicVariant& other)
    {
        // assumes this holder is cleared

        copySeq(other, std::make_index_sequence<sizeof...(T)>());
    }

    template <size_t... I>
    void copySeq(const BasicVariant& other, std::index_sequence<I...>)
    {
        (copy<I>(other), ...);
    }

    template <size_t I>
    void copy(const BasicVariant& other)
    {
        if (I != static_cast<size_t>(other.index()))
        {
            return;
        }
        emplace<static_cast<INDEX>(I)>(other.get<static_cast<INDEX>(I)>());
    }

    void move(BasicVariant&& other)
    {
        // assumes this holder is cleared

        moveSeq(std::move(other), std::make_index_sequence<sizeof...(T)>());
    }

    template <size_t... I>
    void moveSeq(BasicVariant&& other, std::index_sequence<I...>)
    {
        (move<I>(std::move(other)), ...);
    }

    template <size_t I>
    void move(BasicVariant&& other)
    {
        if (I != static_cast<size_t>(other.index()))
        {
            return;
        }
        if constexpr (detail::is_variant_heap_allocated_v<I, T...>)
        {
            if (get_allocator_ref() == other.get_allocator_ref())
            {
                auto& ptr = *std::get_if<I>(&other.m_data);
                m_data.template emplace<I>(ptr);
                ptr = nullptr;
            }
            else
            {
                using U = detail::type_at_t<I, T...>;
                U* ptr = allocateValue<U>(std::move(**std::get_if<I>(&other.m_data)));
                m_data.template emplace<I>(ptr);
            }
        }
        else
        {
            auto& value = *std::get_if<I>(&other.m_data);
            m_data.template emplace<I>(std::move(value));
        }
    }

    void clear()
    {
        clearSeq(std::make_index_sequence<sizeof...(T)>());
    }

    template <size_t... I>
    void clearSeq(std::index_sequence<I...>)
    {
        (clear<I>(), ...);
    }

    template <size_t I>
    void clear()
    {
        if (I != static_cast<size_t>(index()))
        {
            return;
        }
        if constexpr (detail::is_variant_heap_allocated_v<I, T...>)
        {
            auto& ptr = *std::get_if<I>(&m_data);
            destroyValue(ptr);
            ptr = nullptr;
        }
    }

    VariantType m_data;
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
