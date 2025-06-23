#ifndef ZSERIO_EXTENDED_H_INC
#define ZSERIO_EXTENDED_H_INC

#include "HashCodeUtil.h"

namespace zserio
{

namespace detail
{

template <typename T, typename... ARGS>
struct is_single_type : std::false_type
{};

template <typename T, typename A1>
struct is_single_type<T, A1> : std::is_same<T, std::decay_t<A1>>
{};

template <typename T, typename... ARGS>
inline constexpr bool is_single_type_v = is_single_type<T, ARGS...>::value;

} // namespace detail

/**
 * Wrapper around an extended field (defined using 'extended' keyword in Zserio schema).
 *
 * The value storage is always allocated, even though the field is not present. This allows to easily keep
 * continuous allocation and the behaviour is the same as was in the old C++11 generator.
 */
template <typename T>
class Extended
{
public:
    Extended(Extended&& other) = default;

    template <typename ALLOC>
    Extended(Extended&& other, const ALLOC& allocator) :
            m_isPresent(other.m_isPresent),
            m_value(std::move(other.m_value), allocator)
    {}

    Extended(const Extended& other) = default;

    template <typename ALLOC>
    Extended(const Extended& other, const ALLOC& allocator) :
            m_isPresent(other.m_isPresent),
            m_value(other.m_value, allocator)
    {}

    /**
     * In-place extended value constructor from T's arguments.
     */
    template <typename... ARGS,
            typename = std::enable_if_t<!detail::is_single_type_v<Extended, ARGS...> &&
                    std::is_constructible_v<T, ARGS...>>>
    explicit constexpr Extended(ARGS&&... args) :
            m_value(std::forward<ARGS>(args)...)
    {}

    Extended& operator=(Extended&& other) = default;
    Extended& operator=(const Extended& other) = default;

    ~Extended() = default;

    /**
     * Const extended value getter.
     *
     * \returns Constant reference to the value.
     */
    constexpr const T& value() const noexcept
    {
        return m_value;
    }

    /**
     * Non-const extended value getter.
     *
     * \returns Reference to the extended value.
     */
    T& value() noexcept
    {
        return m_value;
    }

    /**
     * Returns the extended value.
     *
     * \return Reference to stored value.
     */
    constexpr T& operator*() noexcept
    {
        return m_value;
    }

    /**
     * Returns the extended value.
     *
     * \return Reference to the constant extended value.
     */
    constexpr const T& operator*() const noexcept
    {
        return m_value;
    }

    /**
     * Returns the extended value.
     *
     * \return Pointer to the extended value.
     */
    T* operator->() noexcept
    {
        return &m_value;
    }

    /**
     * Returns the extended value.
     *
     * \return Pointer to the constant extended value.
     */
    const T* operator->() const noexcept
    {
        return &m_value;
    }

    /**
     * Explicit bool conversion operators - provided for convenience.
     *
     * \return True if the extended value is present, false otherwise.
     */
    constexpr explicit operator bool() const noexcept
    {
        return m_isPresent;
    }

    /**
     * Gets whether the extended value is present.
     *
     * \return True if the extended value is present, false otherwise.
     */
    bool isPresent() const noexcept
    {
        return m_isPresent;
    }

    /**
     * Sets whether the extended value is present.
     *
     * Note that setting of the presence manually can lead to an invalid zserio object, which will not pass the
     * zserio::detail::validate step. This method is designed to be used by zserio::detail::read.
     * If there is a strong need to modify the presence manually it must be ensured that:
     *    1. when this field is set to be present:
     *       * all previous extended fields in a compound type must be also present
     *    2. when this field is set to be missing:
     *       * all following extended fields in a compound type must be also missing
     *
     * \param present True when the extended value is present, false otherwise.
     */
    void setPresent(bool present) noexcept
    {
        m_isPresent = present;
    }

private:
    bool m_isPresent = true; // present by default
    T m_value;
};

template <typename T>
bool operator==(const Extended<T>& lhs, const Extended<T>& rhs)
{
    if (lhs.isPresent() && rhs.isPresent())
    {
        return lhs.value() == rhs.value();
    }
    return lhs.isPresent() == rhs.isPresent();
}

template <typename T>
bool operator!=(const Extended<T>& lhs, const Extended<T>& rhs)
{
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const Extended<T>& lhs, const Extended<T>& rhs)
{
    if (lhs.isPresent() && rhs.isPresent())
    {
        return lhs.value() < rhs.value();
    }
    return lhs.isPresent() < rhs.isPresent();
}

template <typename T>
bool operator>(const Extended<T>& lhs, const Extended<T>& rhs)
{
    if (lhs.isPresent() && rhs.isPresent())
    {
        return lhs.value() > rhs.value();
    }
    return lhs.isPresent() > rhs.isPresent();
}

template <typename T>
bool operator<=(const Extended<T>& lhs, const Extended<T>& rhs)
{
    return !(lhs > rhs);
}

template <typename T>
bool operator>=(const Extended<T>& lhs, const Extended<T>& rhs)
{
    return !(rhs > lhs);
}

/**
 * Calculates Optional hash code from given seed.
 *
 * \param seed Initial hash value.
 * \param var Optional to calculate the hash from.
 *
 * \return Calculated hash code.
 */
template <typename T>
uint32_t calcHashCode(uint32_t seed, const Extended<T>& extendedValue)
{
    uint32_t result = seed;
    if (extendedValue.isPresent())
    {
        result = calcHashCode(result, *extendedValue);
    }
    return result;
}

} // namespace zserio

namespace std
{

template <typename T>
struct hash<zserio::Extended<T>>
{
    size_t operator()(const zserio::Extended<T>& extendedValue) const
    {
        return zserio::calcHashCode(zserio::HASH_SEED, extendedValue);
    }
};

} // namespace std

#endif // ZSERIO_EXTENDED_H_INC
