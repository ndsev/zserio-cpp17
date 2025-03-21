/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UTIL_BITMASK_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UTIL_BITMASK_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <zserio/pmr/PropagatingPolymorphicAllocator.h>
#include <zserio/Bitmasks.h>
#include <zserio/pmr/String.h>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/Types.h>

namespace test_object
{
namespace polymorphic_allocator
{

class ReflectableUtilBitmask
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
    constexpr ReflectableUtilBitmask() noexcept :
            m_value(0)
    {}

    /**
     * Bitmask value constructor.
     *
     * \param value Bitmask value to construct from.
     */
    constexpr ReflectableUtilBitmask(Values value) noexcept :
            m_value(static_cast<ZserioType::ValueType>(value))
    {}

    /**
     * Raw bitmask value constructor.
     *
     * \param value Raw bitmask value to construct from.
     */
    constexpr explicit ReflectableUtilBitmask(ZserioType value) noexcept :
            m_value(value)
    {}

    /** Default destructor. */
    ~ReflectableUtilBitmask() = default;

    /** Default copy constructor. */
    ReflectableUtilBitmask(const ReflectableUtilBitmask&) = default;

    /** Default assignment operator. */
    ReflectableUtilBitmask& operator=(const ReflectableUtilBitmask&) = default;

    /** Default move constructor. */
    ReflectableUtilBitmask(ReflectableUtilBitmask&&) = default;
    /** Default move assignment operator. */
    ReflectableUtilBitmask& operator=(ReflectableUtilBitmask&&) = default;

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
    ::zserio::pmr::String toString(const ::zserio::pmr::String::allocator_type& allocator =
            ::zserio::pmr::String::allocator_type()) const;

private:
    ZserioType m_value;
};

/**
 * Defines operator '==' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is equal to the rhs, otherwise false.
 */
inline constexpr bool operator==(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return lhs.getValue() == rhs.getValue();
}

/**
 * Defines operator '!=' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is not equal to the rhs, otherwise false.
 */
inline constexpr bool operator!=(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return lhs.getValue() != rhs.getValue();
}

/**
 * Defines operator '<' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than the rhs, otherwise false.
 */
inline constexpr bool operator<(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return lhs.getValue() < rhs.getValue();
}

/**
 * Defines operator '>' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than the rhs, otherwise false.
 */
inline constexpr bool operator>(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return lhs.getValue() > rhs.getValue();
}

/**
 * Defines operator '<=' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator<=(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return lhs.getValue() <= rhs.getValue();
}

/**
 * Defines operator '>=' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator>=(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return lhs.getValue() >= rhs.getValue();
}

/**
 * Defines operator '|' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr ReflectableUtilBitmask operator|(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return ReflectableUtilBitmask(lhs.getValue() | rhs.getValue());
}

/**
 * Defines operator '|' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr ReflectableUtilBitmask operator|(ReflectableUtilBitmask::Values lhs, ReflectableUtilBitmask::Values rhs)
{
    return ReflectableUtilBitmask(lhs) | ReflectableUtilBitmask(rhs);
}

/**
 * Defines operator '&' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr ReflectableUtilBitmask operator&(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return ReflectableUtilBitmask(lhs.getValue() & rhs.getValue());
}

/**
 * Defines operator '&' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr ReflectableUtilBitmask operator&(ReflectableUtilBitmask::Values lhs, ReflectableUtilBitmask::Values rhs)
{
    return ReflectableUtilBitmask(lhs) & ReflectableUtilBitmask(rhs);
}

/**
 * Defines operator '^' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr ReflectableUtilBitmask operator^(const ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    return ReflectableUtilBitmask(lhs.getValue() ^ rhs.getValue());
}

/**
 * Defines operator '^' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr ReflectableUtilBitmask operator^(ReflectableUtilBitmask::Values lhs, ReflectableUtilBitmask::Values rhs)
{
    return ReflectableUtilBitmask(lhs) ^ ReflectableUtilBitmask(rhs);
}

/**
 * Defines operator '~' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Const reference to the left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr ReflectableUtilBitmask operator~(const ReflectableUtilBitmask& lhs)
{
    return ReflectableUtilBitmask(~lhs.getValue());
}

/**
 * Defines operator '~' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr ReflectableUtilBitmask operator~(ReflectableUtilBitmask::Values lhs)
{
    return ~ReflectableUtilBitmask(lhs);
}

/**
 * Defines operator '|=' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|=' on given operands.
 */
inline ReflectableUtilBitmask operator|=(ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    lhs = ReflectableUtilBitmask(lhs.getValue() | rhs.getValue());
    return lhs;
}

/**
 * Defines operator '&=' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&=' on given operands.
 */
inline ReflectableUtilBitmask operator&=(ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    lhs = ReflectableUtilBitmask(lhs.getValue() & rhs.getValue());
    return lhs;
}

/**
 * Defines operator '^=' for the bitmask 'ReflectableUtilBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^=' on given operands.
 */
inline ReflectableUtilBitmask operator^=(ReflectableUtilBitmask& lhs, const ReflectableUtilBitmask& rhs)
{
    lhs = ReflectableUtilBitmask(lhs.getValue() ^ rhs.getValue());
    return lhs;
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct TypeInfo<::test_object::polymorphic_allocator::ReflectableUtilBitmask, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableUtilBitmask value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::ReflectableUtilBitmask>
{
    size_t operator()(const ::test_object::polymorphic_allocator::ReflectableUtilBitmask& value) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UTIL_BITMASK_H
