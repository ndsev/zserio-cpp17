/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_CREATOR_BITMASK_H
#define TEST_OBJECT_STD_ALLOCATOR_CREATOR_BITMASK_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <memory>
#include <zserio/Bitmasks.h>
#include <zserio/String.h>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectableData.h>
#include <zserio/IIntrospectableView.h>
#include <zserio/Types.h>

namespace test_object
{
namespace std_allocator
{

class CreatorBitmask
{
public:
    /** Definition for bitmask zserio type. */
    using ZserioType = ::zserio::UInt8;

    /** Enumeration of all bitmask values. */
    enum class Values : ZserioType::ValueType
    {
        READ = UINT8_C(1),
        WRITE = UINT8_C(2)
    };

    /** Default constructor. */
    constexpr CreatorBitmask() noexcept :
            m_value(0)
    {}

    /**
     * Bitmask value constructor.
     *
     * \param value Bitmask value to construct from.
     */
    constexpr CreatorBitmask(Values value) noexcept :
            m_value(static_cast<ZserioType::ValueType>(value))
    {}

    /**
     * Raw bitmask value constructor.
     *
     * \param value Raw bitmask value to construct from.
     */
    constexpr explicit CreatorBitmask(ZserioType value) noexcept :
            m_value(value)
    {}

    /** Default destructor. */
    ~CreatorBitmask() = default;

    /** Default copy constructor. */
    CreatorBitmask(const CreatorBitmask&) = default;

    /** Default assignment operator. */
    CreatorBitmask& operator=(const CreatorBitmask&) = default;

    /** Default move constructor. */
    CreatorBitmask(CreatorBitmask&&) = default;
    /** Default move assignment operator. */
    CreatorBitmask& operator=(CreatorBitmask&&) = default;

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
 * Defines operator '==' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is equal to the rhs, otherwise false.
 */
inline constexpr bool operator==(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return lhs.getValue() == rhs.getValue();
}

/**
 * Defines operator '!=' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is not equal to the rhs, otherwise false.
 */
inline constexpr bool operator!=(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return lhs.getValue() != rhs.getValue();
}

/**
 * Defines operator '<' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than the rhs, otherwise false.
 */
inline constexpr bool operator<(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return lhs.getValue() < rhs.getValue();
}

/**
 * Defines operator '>' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than the rhs, otherwise false.
 */
inline constexpr bool operator>(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return lhs.getValue() > rhs.getValue();
}

/**
 * Defines operator '<=' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator<=(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return lhs.getValue() <= rhs.getValue();
}

/**
 * Defines operator '>=' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator>=(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return lhs.getValue() >= rhs.getValue();
}

/**
 * Defines operator '|' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr CreatorBitmask operator|(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return CreatorBitmask(lhs.getValue() | rhs.getValue());
}

/**
 * Defines operator '|' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr CreatorBitmask operator|(CreatorBitmask::Values lhs, CreatorBitmask::Values rhs)
{
    return CreatorBitmask(lhs) | CreatorBitmask(rhs);
}

/**
 * Defines operator '&' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr CreatorBitmask operator&(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return CreatorBitmask(lhs.getValue() & rhs.getValue());
}

/**
 * Defines operator '&' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr CreatorBitmask operator&(CreatorBitmask::Values lhs, CreatorBitmask::Values rhs)
{
    return CreatorBitmask(lhs) & CreatorBitmask(rhs);
}

/**
 * Defines operator '^' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr CreatorBitmask operator^(const CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    return CreatorBitmask(lhs.getValue() ^ rhs.getValue());
}

/**
 * Defines operator '^' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr CreatorBitmask operator^(CreatorBitmask::Values lhs, CreatorBitmask::Values rhs)
{
    return CreatorBitmask(lhs) ^ CreatorBitmask(rhs);
}

/**
 * Defines operator '~' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Const reference to the left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr CreatorBitmask operator~(const CreatorBitmask& lhs)
{
    return CreatorBitmask(~lhs.getValue());
}

/**
 * Defines operator '~' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr CreatorBitmask operator~(CreatorBitmask::Values lhs)
{
    return ~CreatorBitmask(lhs);
}

/**
 * Defines operator '|=' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|=' on given operands.
 */
inline CreatorBitmask operator|=(CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    lhs = CreatorBitmask(lhs.getValue() | rhs.getValue());
    return lhs;
}

/**
 * Defines operator '&=' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&=' on given operands.
 */
inline CreatorBitmask operator&=(CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    lhs = CreatorBitmask(lhs.getValue() & rhs.getValue());
    return lhs;
}

/**
 * Defines operator '^=' for the bitmask 'CreatorBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^=' on given operands.
 */
inline CreatorBitmask operator^=(CreatorBitmask& lhs, const CreatorBitmask& rhs)
{
    lhs = CreatorBitmask(lhs.getValue() ^ rhs.getValue());
    return lhs;
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct TypeInfo<::test_object::std_allocator::CreatorBitmask, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::CreatorBitmask value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IIntrospectableViewConstPtr introspectable(::test_object::std_allocator::CreatorBitmask value, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::CreatorBitmask>
{
    size_t operator()(const ::test_object::std_allocator::CreatorBitmask& value) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_CREATOR_BITMASK_H
