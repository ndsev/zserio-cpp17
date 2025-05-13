#ifndef ZSERIO_OPTIONAL_H_INC
#define ZSERIO_OPTIONAL_H_INC

#include <optional>

#include "zserio/AllocatorHolder.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/HashCodeUtil.h"
#include "zserio/Traits.h"
#include "zserio/View.h"

namespace zserio
{

template <typename ALLOC, typename T>
class BasicOptional;

namespace detail
{

template <typename T, bool = (sizeof(T) > 3 * sizeof(void*))>
struct is_optional_big : std::false_type
{};

template <typename T>
struct is_optional_big<T, true> : std::true_type
{};

template <typename T, typename = void>
struct is_recursive : std::false_type
{};

template <typename T>
struct is_recursive<T, std::void_t<typename T::IS_RECURSIVE>> : std::true_type
{};

template <typename T, bool = is_recursive<T>::value>
struct is_optional_heap_allocated_impl : is_optional_big<T>::type
{};

template <typename T>
struct is_optional_heap_allocated_impl<T, true> : std::true_type
{};

template <typename T>
struct is_optional_heap_allocated : is_optional_heap_allocated_impl<T>
{};

template <typename T>
struct is_optional_heap_allocated<View<T>> : std::false_type
{};

template <typename T>
constexpr bool is_optional_heap_allocated_v = is_optional_heap_allocated<T>::value;

template <typename T, bool heap = is_optional_heap_allocated_v<T>>
struct optional_element
{
    using type = T;
};

template <typename T>
struct optional_element<T, true>
{
    using type = T*;
};

template <typename T>
struct is_optional : std::false_type
{};

template <typename A, typename T>
struct is_optional<BasicOptional<A, T>> : std::true_type
{};

template <typename T>
constexpr bool is_optional_v = is_optional<T>::value;

} // namespace detail

/**
 * Exception thrown when accessed optional is empty.
 */
class BadOptionalAccess : public CppRuntimeException
{
public:
    using CppRuntimeException::CppRuntimeException;
};

/**
 * Implementation of optional type which allocates memory when type
 * 1. the type is explicitly tagged with IS_RECURSIVE type OR
 * 2. doesn't fit under 3*sizeof(void*)
 *
 * Largely compatible with std::optional with following differences:
 * + BasicOptional(U&&) implemented only with T because the required is_constructible constrain can't be used on
 * incomplete types
 * + operator=(U&&) implemented without is_constructible constrain because that can't be used on incomplete
 * types
 * + BasicOptional(std::in_place_t, std::initializer_list) not implemented
 * + Monadic operations from C++23 not implemented
 * + Conditional noexcept and conditional explicit-ness on certain members not implemented
 */
template <typename ALLOC, typename T>
class BasicOptional : public AllocatorHolder<ALLOC>
{
    using AllocTraits = std::allocator_traits<ALLOC>;
    using AllocatorHolder<ALLOC>::set_allocator;

public:
    using AllocatorHolder<ALLOC>::get_allocator_ref;
    using allocator_type = ALLOC;
    using OptionalType = std::optional<typename detail::optional_element<T>::type>;

    /**
     * Default constructor.
     */
    constexpr BasicOptional() noexcept :
            BasicOptional(ALLOC())
    {}

    /**
     * Nullopt constructor (implicit).
     */
    constexpr BasicOptional(std::nullopt_t, const ALLOC& allocator = {}) noexcept :
            BasicOptional(allocator)
    {}

    /**
     * Constructor from given value (implicit).
     *
     * \param value Value to be initialized with.
     * \param allocator Optional allocator to be used.
     */
    constexpr BasicOptional(const T& value, const ALLOC& allocator = {}) :
            BasicOptional(std::in_place, allocator, value)
    {}

    /**
     * Constructor from given value and allocator (implicit).
     *
     * \param value Value to be initialized with.
     * \param allocator Optional allocator to be used.
     */
    constexpr BasicOptional(T&& value, const ALLOC& allocator = {}) :
            BasicOptional(std::in_place, allocator, std::move(value))
    {}

    /**
     * Constructor from given allocator.
     *
     * \param allocator Allocator to be used.
     */
    explicit constexpr BasicOptional(const ALLOC& allocator) :
            AllocatorHolder<ALLOC>(allocator)
    {}

    /**
     * Constructor which constructs the value.
     *
     * \param in_place tag to request this constructor.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T.
     */
    template <typename... ARGS, typename TT = T,
            std::enable_if_t<!detail::is_optional_heap_allocated_v<TT>>* = nullptr,
            std::enable_if_t<!is_first_allocator<ARGS...>::value>* = nullptr>
    explicit constexpr BasicOptional(std::in_place_t, ARGS&&... args) :
            m_data(std::in_place, std::forward<ARGS>(args)...)
    {}

    /**
     * Constructor which constructs the value.
     *
     * \param in_place tag to request this constructor.
     * \param allocator Allocator to be used.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T.
     */
    template <typename... ARGS, typename TT = T,
            std::enable_if_t<!detail::is_optional_heap_allocated_v<TT>>* = nullptr>
    constexpr BasicOptional(std::in_place_t, const ALLOC& allocator, ARGS&&... args) :
            AllocatorHolder<ALLOC>(allocator),
            m_data(std::in_place, std::forward<ARGS>(args)...)
    {}

    /**
     * Constructor which constructs the value.
     *
     * \param in_place tag to request this constructor.
     * \param allocator Allocator to be used.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T.
     */
    template <typename... ARGS, typename TT = T,
            std::enable_if_t<detail::is_optional_heap_allocated_v<TT>>* = nullptr>
    constexpr BasicOptional(std::in_place_t, const ALLOC& allocator, ARGS&&... args) :
            AllocatorHolder<ALLOC>(allocator),
            m_data(std::in_place, allocateValue(std::forward<ARGS>(args)...))
    {}

    /**
     * Constructor which constructs the value.
     *
     * \param in_place tag to request this constructor.
     * \param args Arguments to be forwarded for element construction.
     *
     * \throw can throw any exception thrown by T.
     */
    template <typename... ARGS, typename TT = T,
            std::enable_if_t<detail::is_optional_heap_allocated_v<TT>>* = nullptr,
            std::enable_if_t<!is_first_allocator<ARGS...>::value>* = nullptr>
    explicit constexpr BasicOptional(std::in_place_t, ARGS&&... args) :
            m_data(std::in_place, allocateValue(std::forward<ARGS>(args)...))
    {}

    /**
     * Destructor.
     */
    ~BasicOptional()
    {
        clear();
    }

    /**
     * Copy constructor.
     *
     * \param other Optional to copy.
     */
    constexpr BasicOptional(const BasicOptional& other) :
            AllocatorHolder<ALLOC>(
                    AllocTraits::select_on_container_copy_construction(other.get_allocator_ref()))
    {
        copy(other);
    }

    /**
     * Conversion copy constructor.
     *
     * \param other Optional to copy.
     */
    template <typename U>
    constexpr BasicOptional(const BasicOptional<ALLOC, U>& other) :
            AllocatorHolder<ALLOC>(
                    AllocTraits::select_on_container_copy_construction(other.get_allocator_ref()))
    {
        copy(other);
    }

    /**
     * Allocator-extended copy constructor.
     *
     * \param other Optional to copy.
     * \param allocator Allocator to be used for dynamic memory allocations.
     *
     * \throw can throw any exception thrown by the active element in other.
     */
    constexpr BasicOptional(const BasicOptional& other, const ALLOC& allocator) :
            AllocatorHolder<ALLOC>(allocator)
    {
        copy(other);
    }

    /**
     * Allocator-extended conversion copy constructor.
     *
     * \param other Optional to copy.
     * \param allocator Allocator to be used for dynamic memory allocations.
     *
     * \throw can throw any exception thrown by the active element in other.
     */
    template <typename A, typename U>
    BasicOptional(const BasicOptional<A, U>& other, const ALLOC& allocator) :
            AllocatorHolder<ALLOC>(allocator)
    {
        copy(other);
    }

    /**
     * Copy assignment operator.
     *
     * \param other Optional to copy.
     *
     * \return Reference to this.
     *
     * \throw can throw any exception thrown by the active element in other.
     */
    constexpr BasicOptional& operator=(const BasicOptional& other)
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
     * Conversion copy assignment operator.
     *
     * \param other Optional to copy.
     *
     * \return Reference to this.
     *
     * \throw can throw any exception thrown by the active element in other.
     */
    template <typename A, typename U>
    BasicOptional& operator=(const BasicOptional<A, U>& other)
    {
        clear();
        if constexpr (AllocTraits::propagate_on_container_copy_assignment::value)
        {
            set_allocator(other.get_allocator_ref());
        }
        copy(other);

        return *this;
    }

    /**
     * Resetting assignment operator.
     *
     * \return Reference to this.
     */
    BasicOptional& operator=(std::nullopt_t)
    {
        reset();
        return *this;
    }

    /**
     * Value assignment operator.
     *
     * \param value The value to be emplaced.
     *
     * \return Reference to this.
     */
    template <typename U = T,
            std::enable_if_t<!detail::is_optional_v<std::decay_t<U>> &&
                    !(std::is_same_v<T, std::decay_t<U>> && std::is_scalar_v<U>)>* = nullptr>
    BasicOptional& operator=(U&& value)
    {
        emplace(std::forward<U>(value));
        return *this;
    }

    /**
     * Move constructor.
     *
     * \param other Optional to move from.
     *
     * \throw can throw any exception thrown by the value in other.
     */
    BasicOptional(BasicOptional&& other) :
            AllocatorHolder<ALLOC>(std::move(other.get_allocator_ref()))
    {
        move(std::move(other));
    }

    /**
     * Allocator-extended move constructor.
     *
     * \param other Optional to move from.
     * \param allocator Allocator to be used for dynamic memory allocations.
     *
     * \throw can throw any exception thrown by the value in other.
     */
    BasicOptional(BasicOptional&& other, const ALLOC& allocator) :
            AllocatorHolder<ALLOC>(allocator)
    {
        move(std::move(other));
    }

    /**
     * Move assignment operator.
     *
     * \param other Optional to move from.
     *
     * \return Reference to this.
     */
    BasicOptional& operator=(BasicOptional&& other)
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
     * Reports if Optional has stored value.
     *
     * \return True if value is set.
     */
    constexpr bool has_value() const noexcept
    {
        if (!m_data.has_value())
        {
            return false;
        }
        if constexpr (detail::is_optional_heap_allocated_v<T>)
        {
            if (!m_data.value())
            {
                return false;
            }
        }
        return true;
    }

    /**
     * Resets optional to empty state.
     */
    void reset()
    {
        clear();
        m_data.reset();
    }

    /**
     * Swaps content with other optional.
     *
     * \param other Other optional to swap with.
     */
    void swap(BasicOptional& other)
    {
        m_data.swap(other.m_data);
        if constexpr (AllocTraits::propagate_on_container_swap::value)
        {
            using std::swap;
            swap(get_allocator_ref(), other.get_allocator_ref());
        }
    }

    /**
     * Reports if Optional has stored value.
     *
     * \return True if value is set.
     */
    constexpr explicit operator bool() const noexcept
    {
        return has_value();
    }

    /**
     * Constructs a value in the Optional.
     *
     * \param args Arguments to be forwarded for value construction.
     *
     * \return Reference to inserted value.
     */
    template <typename... ARGS>
    T& emplace(ARGS&&... args)
    {
        clear();
        if constexpr (detail::is_optional_heap_allocated_v<T>)
        {
            T* ptr = allocateValue(std::forward<ARGS>(args)...);
            return *m_data.emplace(ptr);
        }
        else
        {
            return m_data.emplace(std::forward<ARGS>(args)...);
        }
    }

    /**
     * Returns contained value.
     *
     * \return Reference to stored value.
     *
     * \throws BadOptionalAccess if optional is empty.
     */
    constexpr T& value()
    {
        if (!has_value())
        {
            throw BadOptionalAccess("Optional is empty");
        }
        if constexpr (detail::is_optional_heap_allocated_v<T>)
        {
            return *m_data.value();
        }
        else
        {
            return m_data.value();
        }
    }

    /**
     * Returns contained value.
     *
     * \return Reference to stored value.
     *
     * \throws BadOptionalAccess if optional is empty.
     */
    constexpr const T& value() const
    {
        if (!has_value())
        {
            throw BadOptionalAccess("Optional is empty");
        }
        if constexpr (detail::is_optional_heap_allocated_v<T>)
        {
            return *m_data.value();
        }
        else
        {
            return m_data.value();
        }
    }

    /**
     * Returns contained value.
     *
     * \return Reference to stored value.
     *
     * \throws BadOptionalAccess if optional is empty.
     */
    T& operator*()
    {
        return value();
    }

    /**
     * Returns contained value.
     *
     * \return Reference to stored value.
     *
     * \throws BadOptionalAccess if optional is empty.
     */
    const T& operator*() const
    {
        return value();
    }

    /**
     * Returns contained value.
     *
     * \return Pointer to stored value.
     *
     * \throws BadOptionalAccess if optional is empty.
     */
    T* operator->()
    {
        return &value();
    }

    /**
     * Returns contained value.
     *
     * \return Pointer to stored value.
     *
     * \throws BadOptionalAccess if optional is empty.
     */
    const T* operator->() const
    {
        return &value();
    }

    /**
     * Returns contained value or default value if optional is empty.
     *
     * \return Stored value or argument if empty.
     *
     * \param def default return value.
     */
    template <typename U>
    T value_or(U&& def)
    {
        if (!has_value())
        {
            return static_cast<T>(std::forward<U>(def));
        }
        return value();
    }

    /**
     * Returns contained value or default value if optional is empty.
     *
     * \return Stored value or argument if empty.
     *
     * \param def default return value.
     */
    template <typename U>
    T value_or(U&& def) const
    {
        if (!has_value())
        {
            return static_cast<T>(std::forward<U>(def));
        }
        return value();
    }

private:
    template <typename... ARGS>
    T* allocateValue(ARGS&&... args)
    {
        using MyAllocType = RebindAlloc<ALLOC, T>;
        using MyAllocTraits = std::allocator_traits<MyAllocType>;
        MyAllocType typedAlloc = get_allocator_ref();
        auto ptr = MyAllocTraits::allocate(typedAlloc, 1);
        try
        {
            MyAllocTraits::construct(typedAlloc, std::addressof(*ptr), std::forward<ARGS>(args)...);
        }
        catch (...)
        {
            MyAllocTraits::deallocate(typedAlloc, ptr, 1);
            throw;
        }
        return ptr;
    }

    // same as for std::Optional throwing dtors are not supported see
    // https://stackoverflow.com/questions/78602733/should-stdOptional-be-nothrow-destructible-when-its-alternative-has-potentially
    void destroyValue(T* ptr)
    {
        if (ptr)
        {
            using MyAllocType = RebindAlloc<ALLOC, T>;
            using MyAllocTraits = std::allocator_traits<MyAllocType>;
            MyAllocType typedAlloc = get_allocator_ref();
            MyAllocTraits::destroy(typedAlloc, ptr);
            MyAllocTraits::deallocate(typedAlloc, ptr, 1);
        }
    }

    template <typename A, typename U>
    void copy(const BasicOptional<A, U>& other)
    {
        // assumes this holder is cleared

        if (!other.has_value())
        {
            reset();
        }
        else
        {
            emplace(other.value());
        }
    }

    void move(BasicOptional&& other)
    {
        // assumes this holder is cleared

        if (!other.has_value())
        {
            reset();
        }
        else
        {
            if constexpr (detail::is_optional_heap_allocated_v<T>)
            {
                if (get_allocator_ref() == other.get_allocator_ref())
                {
                    // non-standard optimization to reuse memory from the same resource
                    // note that other will be unset after move in this case
                    auto& ptr = other.m_data.value();
                    m_data.emplace(ptr);
                    ptr = nullptr;
                }
                else
                {
                    auto& value = *other.m_data.value();
                    T* ptr = allocateValue(std::move(value));
                    m_data.emplace(ptr);
                }
            }
            else
            {
                auto& value = *other.m_data;
                m_data.emplace(std::move(value));
            }
        }
    }

    void clear()
    {
        if (!has_value())
        {
            return;
        }
        if constexpr (detail::is_optional_heap_allocated_v<T>)
        {
            auto& ptr = m_data.value();
            destroyValue(ptr);
            ptr = nullptr;
        }
    }

    OptionalType m_data;
};

// Using declarations

template <typename T>
using Optional = BasicOptional<std::allocator<uint8_t>, T>;

/**
 * Calculates Optional hash code from given seed.
 *
 * \param seed Initial hash value.
 * \param opt Optional to calculate the hash from.
 *
 * \return Calculated hash code.
 */
template <typename ALLOC, typename T>
uint32_t calcHashCode(uint32_t seed, const BasicOptional<ALLOC, T>& opt)
{
    uint32_t result = seed;
    if (opt)
    {
        result = calcHashCode(result, *opt);
    }
    return result;
}

/**
 * Calls Optional constructor with value.
 *
 * \param value Value to copy.
 *
 * \return Optional with value set.
 */
template <typename T>
constexpr auto make_optional(T&& value)
{
    return Optional<std::decay_t<T>>(std::forward<T>(value));
}

/**
 * Calls Optional<T> constructor with arguments for T.
 *
 * \param args Variadic arguments for T construction.
 *
 * \return Optional with value set.
 */
template <typename T, typename... ARGS>
constexpr Optional<T> make_optional(ARGS&&... args)
{
    return Optional<T>(std::in_place, std::forward<ARGS>(args)...);
}

/**
 * Optional equality test.
 *
 * \param first First optional to compare.
 * \param second Second optional to compare.
 *
 * \return first and second are either both empty or having equal value.
 */
template <typename A1, typename T, typename A2, typename U>
constexpr bool operator==(const BasicOptional<A1, T>& first, const BasicOptional<A2, U>& second)
{
    if (first.has_value() && second.has_value())
    {
        return first.value() == second.value();
    }
    return first.has_value() == second.has_value();
}

/**
 * Optional equality test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return opt is empty.
 */
template <typename A, typename T>
constexpr bool operator==(const BasicOptional<A, T>& opt, std::nullopt_t)
{
    return !opt.has_value();
}

/**
 * Optional equality test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return opt is empty.
 */
template <typename A, typename T>
constexpr bool operator==(std::nullopt_t, const BasicOptional<A, T>& opt)
{
    return !opt.has_value();
}

/**
 * Optional equality test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return Optional's value equals to value.
 */
template <typename A, typename T, typename U>
constexpr bool operator==(const BasicOptional<A, T>& opt, const U& value)
{
    return opt.has_value() && opt.value() == value;
}

/**
 * Optional equality test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return Optional's value equals to value.
 */
template <typename A, typename T, typename U>
constexpr bool operator==(const U& value, const BasicOptional<A, T>& opt)
{
    return opt.has_value() && opt.value() == value;
}

/**
 * Optional inequality test.
 *
 * \param first First optional to compare.
 * \param second Second optional to compare.
 *
 * \return first and second are not equal.
 */
template <typename A1, typename T, typename A2, typename U>
constexpr bool operator!=(const BasicOptional<A1, T>& first, const BasicOptional<A2, U>& second)
{
    return !(first == second);
}

/**
 * Optional inequality test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return Optional is not empty.
 */
template <typename A, typename T>
constexpr bool operator!=(const BasicOptional<A, T>& opt, std::nullopt_t)
{
    return !(opt == std::nullopt);
}

/**
 * Optional inequality test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return Optional is not empty.
 */
template <typename A, typename T>
constexpr bool operator!=(std::nullopt_t, const BasicOptional<A, T>& opt)
{
    return !(std::nullopt == opt);
}

/**
 * Optional inequality test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return opt's value is not equal to value.
 */
template <typename A, typename T, typename U>
constexpr bool operator!=(const BasicOptional<A, T>& opt, const U& value)
{
    return !(opt == value);
}

/**
 * Optional inequality test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return opt's value is not equal to value.
 */
template <typename A, typename T, typename U>
constexpr bool operator!=(const U& value, const BasicOptional<A, T>& opt)
{
    return !(value == opt);
}

/**
 * Optional less-than test.
 *
 * \param first First optional to compare.
 * \param second Second optional to compare.
 *
 * \return first is less than second.
 */
template <typename A1, typename T, typename A2, typename U>
constexpr bool operator<(const BasicOptional<A1, T>& first, const BasicOptional<A2, U>& second)
{
    if (first.has_value() && second.has_value())
    {
        return first.value() < second.value();
    }
    return first.has_value() < second.has_value();
}

/**
 * Optional less-than test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return false.
 */
template <typename A, typename T>
constexpr bool operator<(const BasicOptional<A, T>&, std::nullopt_t)
{
    return false;
}

/**
 * Optional less-than test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return nullopt is less than opt.
 */
template <typename A, typename T>
constexpr bool operator<(std::nullopt_t, const BasicOptional<A, T>& opt)
{
    return opt.has_value();
}

/**
 * Optional less-than test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return opt is less than value.
 */
template <typename A, typename T, typename U>
constexpr bool operator<(const BasicOptional<A, T>& opt, const U& value)
{
    return !opt.has_value() || opt.value() < value;
}

/**
 * Optional less-than test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return value is less than opt.
 */
template <typename A, typename T, typename U>
constexpr bool operator<(const U& value, const BasicOptional<A, T>& opt)
{
    return opt.has_value() && value < opt.value();
}

/**
 * Optional greater-than test.
 *
 * \param first First optional to compare.
 * \param second Second optional to compare.
 *
 * \return first is greater than second
 */
template <typename A1, typename T, typename A2, typename U>
constexpr bool operator>(const BasicOptional<A1, T>& first, const BasicOptional<A2, U>& second)
{
    if (first.has_value() && second.has_value())
    {
        return first.value() > second.value();
    }
    return first.has_value() > second.has_value();
}

/**
 * Optional greater-than test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return opt is greater than nullopt.
 */
template <typename A, typename T>
constexpr bool operator>(const BasicOptional<A, T>& opt, std::nullopt_t)
{
    return opt.has_value();
}

/**
 * Optional greater-than test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return nullopt is greater than opt.
 */
template <typename A, typename T>
constexpr bool operator>(std::nullopt_t, const BasicOptional<A, T>&)
{
    return false;
}

/**
 * Optional greater-than test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return opt is greater than value.
 */
template <typename A, typename T, typename U>
constexpr bool operator>(const BasicOptional<A, T>& opt, const U& value)
{
    return opt.has_value() && opt.value() > value;
}

/**
 * Optional greater-than test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return value is greater than opt.
 */
template <typename A, typename T, typename U>
constexpr bool operator>(const U& value, const BasicOptional<A, T>& opt)
{
    return !opt.has_value() || value > opt.value();
}

/**
 * Optional less-or-equal test.
 *
 * \param first First optional to compare.
 * \param second Second optional to compare.
 *
 * \return first is less or equal to second.
 */
template <typename A1, typename T, typename A2, typename U>
constexpr bool operator<=(const BasicOptional<A1, T>& first, const BasicOptional<A2, U>& second)
{
    return !(first > second);
}

/**
 * Optional less-or-equal test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return opt is less or equal to nullopt.
 */
template <typename A, typename T>
constexpr bool operator<=(const BasicOptional<A, T>& opt, std::nullopt_t)
{
    return !(opt > std::nullopt);
}

/**
 * Optional less-or-equal test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return nullopt is less or equal to opt.
 */
template <typename A, typename T>
constexpr bool operator<=(std::nullopt_t, const BasicOptional<A, T>& opt)
{
    return !(std::nullopt > opt);
}

/**
 * Optional less-or-equal test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return opt is less or equal to value.
 */
template <typename A, typename T, typename U>
constexpr bool operator<=(const BasicOptional<A, T>& opt, const U& value)
{
    return !(opt > value);
}

/**
 * Optional less-or-equal test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return value is less or equal to opt.
 */
template <typename A, typename T, typename U>
constexpr bool operator<=(const U& value, const BasicOptional<A, T>& opt)
{
    return !(value > opt);
}

/**
 * Optional greater-or-equal test.
 *
 * \param first First optional to compare.
 * \param second Second optional to compare.
 *
 * \return first is greater or equal to second.
 */
template <typename A1, typename T, typename A2, typename U>
constexpr bool operator>=(const BasicOptional<A1, T>& first, const BasicOptional<A2, U>& second)
{
    return !(first < second);
}

/**
 * Optional greater-or-equal test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return opt is greater or equal to nullopt.
 */
template <typename A, typename T>
constexpr bool operator>=(const BasicOptional<A, T>& opt, std::nullopt_t)
{
    return !(opt < std::nullopt);
}

/**
 * Optional greater-or-equal test with nullopt.
 *
 * \param opt The optional to compare.
 *
 * \return nullopt is greater or equal to opt.
 */
template <typename A, typename T>
constexpr bool operator>=(std::nullopt_t, const BasicOptional<A, T>& opt)
{
    return !(std::nullopt < opt);
}

/**
 * Optional greater-or-equal test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return opt is greater or equal to value.
 */
template <typename A, typename T, typename U>
constexpr bool operator>=(const BasicOptional<A, T>& opt, const U& value)
{
    return !(opt < value);
}

/**
 * Optional greater-or-equal test with value.
 *
 * \param opt The optional to compare.
 * \param value Value to compare.
 *
 * \return value is greater or equal to opt.
 */
template <typename A, typename T, typename U>
constexpr bool operator>=(const U& value, const BasicOptional<A, T>& opt)
{
    return !(value < opt);
}

} // namespace zserio

namespace std
{

template <typename ALLOC, typename T>
struct hash<zserio::BasicOptional<ALLOC, T>>
{
    size_t operator()(const zserio::BasicOptional<ALLOC, T>& opt) const
    {
        return zserio::calcHashCode(zserio::HASH_SEED, opt);
    }
};

} // namespace std

#endif // ZSERIO_OPTIONAL_H_INC
