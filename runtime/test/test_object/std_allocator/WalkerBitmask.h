/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_WALKER_BITMASK_H
#define TEST_OBJECT_STD_ALLOCATOR_WALKER_BITMASK_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>
#include <zserio/Bitmasks.h>
#include <zserio/String.h>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectable.h>
#include <zserio/Types.h>

namespace test_object
{
namespace std_allocator
{

class WalkerBitmask
{
public:
    /** Definition for bitmask zserio type. */
    using ZserioType = ::zserio::UInt32;

    /** Enumeration of all bitmask values. */
    enum class Values : ZserioType::ValueType
    {
        ZERO = UINT32_C(1)
    };

    /** Default constructor. */
    constexpr WalkerBitmask() noexcept :
            m_value(0)
    {}

    /**
     * Bitmask value constructor.
     *
     * \param value Bitmask value to construct from.
     */
    constexpr WalkerBitmask(Values value) noexcept :
            m_value(static_cast<ZserioType::ValueType>(value))
    {}

    /**
     * Raw bitmask value constructor.
     *
     * \param value Raw bitmask value to construct from.
     */
    constexpr explicit WalkerBitmask(ZserioType value) noexcept :
            m_value(value)
    {}

    /** Default destructor. */
    ~WalkerBitmask() = default;

    /** Default copy constructor. */
    WalkerBitmask(const WalkerBitmask&) = default;

    /** Default assignment operator. */
    WalkerBitmask& operator=(const WalkerBitmask&) = default;

    /** Default move constructor. */
    WalkerBitmask(WalkerBitmask&&) = default;
    /** Default move assignment operator. */
    WalkerBitmask& operator=(WalkerBitmask&&) = default;

    /**
     * Defines conversion to bitmask underlying type.
     *
     * \return Raw value which holds this bitmask.
     */
    constexpr explicit operator ZserioType() const
    {
        return m_value;
    }

    /**
     * Gets the bitmask raw value.
     *
     * \return Raw value which holds this bitmask.
     */
    constexpr ZserioType getValue() const
    {
        return m_value;
    }

    /**
     * Converts the bitmask value to the string.
     *
     * \param allocator Allocator to use for all dynamic memory allocations.
     *
     * \return String which represents this bitmask.
     */
    ::zserio::String toString(const ::zserio::String::allocator_type& allocator =
            ::zserio::String::allocator_type()) const;

private:
    ZserioType m_value;
};

/**
 * Defines operator '==' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is equal to the rhs, otherwise false.
 */
inline constexpr bool operator==(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return lhs.getValue() == rhs.getValue();
}

/**
 * Defines operator '!=' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is not equal to the rhs, otherwise false.
 */
inline constexpr bool operator!=(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return lhs.getValue() != rhs.getValue();
}

/**
 * Defines operator '<' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than the rhs, otherwise false.
 */
inline constexpr bool operator<(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return lhs.getValue() < rhs.getValue();
}

/**
 * Defines operator '>' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than the rhs, otherwise false.
 */
inline constexpr bool operator>(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return lhs.getValue() > rhs.getValue();
}

/**
 * Defines operator '<=' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator<=(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return lhs.getValue() <= rhs.getValue();
}

/**
 * Defines operator '>=' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator>=(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return lhs.getValue() >= rhs.getValue();
}

/**
 * Defines operator '|' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr WalkerBitmask operator|(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return WalkerBitmask(lhs.getValue() | rhs.getValue());
}

/**
 * Defines operator '|' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr WalkerBitmask operator|(WalkerBitmask::Values lhs, WalkerBitmask::Values rhs)
{
    return WalkerBitmask(lhs) | WalkerBitmask(rhs);
}

/**
 * Defines operator '&' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr WalkerBitmask operator&(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return WalkerBitmask(lhs.getValue() & rhs.getValue());
}

/**
 * Defines operator '&' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr WalkerBitmask operator&(WalkerBitmask::Values lhs, WalkerBitmask::Values rhs)
{
    return WalkerBitmask(lhs) & WalkerBitmask(rhs);
}

/**
 * Defines operator '^' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr WalkerBitmask operator^(const WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    return WalkerBitmask(lhs.getValue() ^ rhs.getValue());
}

/**
 * Defines operator '^' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr WalkerBitmask operator^(WalkerBitmask::Values lhs, WalkerBitmask::Values rhs)
{
    return WalkerBitmask(lhs) ^ WalkerBitmask(rhs);
}

/**
 * Defines operator '~' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Const reference to the left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr WalkerBitmask operator~(const WalkerBitmask& lhs)
{
    return WalkerBitmask(~lhs.getValue());
}

/**
 * Defines operator '~' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr WalkerBitmask operator~(WalkerBitmask::Values lhs)
{
    return ~WalkerBitmask(lhs);
}

/**
 * Defines operator '|=' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|=' on given operands.
 */
inline WalkerBitmask operator|=(WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    lhs = WalkerBitmask(lhs.getValue() | rhs.getValue());
    return lhs;
}

/**
 * Defines operator '&=' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&=' on given operands.
 */
inline WalkerBitmask operator&=(WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    lhs = WalkerBitmask(lhs.getValue() & rhs.getValue());
    return lhs;
}

/**
 * Defines operator '^=' for the bitmask 'WalkerBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^=' on given operands.
 */
inline WalkerBitmask operator^=(WalkerBitmask& lhs, const WalkerBitmask& rhs)
{
    lhs = WalkerBitmask(lhs.getValue() ^ rhs.getValue());
    return lhs;
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct TypeInfo<::test_object::std_allocator::WalkerBitmask, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectablePtr reflectable(::test_object::std_allocator::WalkerBitmask value, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::WalkerBitmask>
{
    size_t operator()(const ::test_object::std_allocator::WalkerBitmask& value) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_WALKER_BITMASK_H
