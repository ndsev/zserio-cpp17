/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_BITMASK_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_BITMASK_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <zserio/pmr/PropagatingPolymorphicAllocator.h>
#include <zserio/Bitmasks.h>
#include <zserio/pmr/String.h>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/pmr/IIntrospectableView.h>
#include <zserio/Types.h>

namespace test_object
{
namespace polymorphic_allocator
{

class ReflectableBitmask
{
public:
    /** Definition for bitmask zserio type. */
    using ZserioType = ::zserio::UInt8;

    /** Enumeration of all bitmask values. */
    enum class Values : ZserioType::ValueType
    {
        CREATE = UINT8_C(1),
        READ = UINT8_C(2),
        WRITE = UINT8_C(4)
    };

    /** Default constructor. */
    constexpr ReflectableBitmask() noexcept :
            m_value(0)
    {}

    /**
     * Bitmask value constructor.
     *
     * \param value Bitmask value to construct from.
     */
    constexpr ReflectableBitmask(Values value) noexcept :
            m_value(static_cast<ZserioType::ValueType>(value))
    {}

    /**
     * Raw bitmask value constructor.
     *
     * \param value Raw bitmask value to construct from.
     */
    constexpr explicit ReflectableBitmask(ZserioType value) noexcept :
            m_value(value)
    {}

    /** Default destructor. */
    ~ReflectableBitmask() = default;

    /** Default copy constructor. */
    ReflectableBitmask(const ReflectableBitmask&) = default;

    /** Default assignment operator. */
    ReflectableBitmask& operator=(const ReflectableBitmask&) = default;

    /** Default move constructor. */
    ReflectableBitmask(ReflectableBitmask&&) = default;
    /** Default move assignment operator. */
    ReflectableBitmask& operator=(ReflectableBitmask&&) = default;

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
 * Defines operator '==' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is equal to the rhs, otherwise false.
 */
inline constexpr bool operator==(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return lhs.getValue() == rhs.getValue();
}

/**
 * Defines operator '!=' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is not equal to the rhs, otherwise false.
 */
inline constexpr bool operator!=(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return lhs.getValue() != rhs.getValue();
}

/**
 * Defines operator '<' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than the rhs, otherwise false.
 */
inline constexpr bool operator<(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return lhs.getValue() < rhs.getValue();
}

/**
 * Defines operator '>' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than the rhs, otherwise false.
 */
inline constexpr bool operator>(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return lhs.getValue() > rhs.getValue();
}

/**
 * Defines operator '<=' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is less than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator<=(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return lhs.getValue() <= rhs.getValue();
}

/**
 * Defines operator '>=' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return True if lhs is greater than or equal to the rhs, otherwise false.
 */
inline constexpr bool operator>=(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return lhs.getValue() >= rhs.getValue();
}

/**
 * Defines operator '|' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr ReflectableBitmask operator|(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return ReflectableBitmask(lhs.getValue() | rhs.getValue());
}

/**
 * Defines operator '|' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|' on given operands.
 */
inline constexpr ReflectableBitmask operator|(ReflectableBitmask::Values lhs, ReflectableBitmask::Values rhs)
{
    return ReflectableBitmask(lhs) | ReflectableBitmask(rhs);
}

/**
 * Defines operator '&' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr ReflectableBitmask operator&(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return ReflectableBitmask(lhs.getValue() & rhs.getValue());
}

/**
 * Defines operator '&' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&' on given operands.
 */
inline constexpr ReflectableBitmask operator&(ReflectableBitmask::Values lhs, ReflectableBitmask::Values rhs)
{
    return ReflectableBitmask(lhs) & ReflectableBitmask(rhs);
}

/**
 * Defines operator '^' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Const reference to the left operand.
 * \param rhs Const reference to the right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr ReflectableBitmask operator^(const ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    return ReflectableBitmask(lhs.getValue() ^ rhs.getValue());
}

/**
 * Defines operator '^' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^' on given operands.
 */
inline constexpr ReflectableBitmask operator^(ReflectableBitmask::Values lhs, ReflectableBitmask::Values rhs)
{
    return ReflectableBitmask(lhs) ^ ReflectableBitmask(rhs);
}

/**
 * Defines operator '~' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Const reference to the left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr ReflectableBitmask operator~(const ReflectableBitmask& lhs)
{
    return ReflectableBitmask(~lhs.getValue());
}

/**
 * Defines operator '~' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 *
 * \return Bitmask which contains result after applying the operator '~' on given operand.
 */
inline constexpr ReflectableBitmask operator~(ReflectableBitmask::Values lhs)
{
    return ~ReflectableBitmask(lhs);
}

/**
 * Defines operator '|=' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '|=' on given operands.
 */
inline ReflectableBitmask operator|=(ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    lhs = ReflectableBitmask(lhs.getValue() | rhs.getValue());
    return lhs;
}

/**
 * Defines operator '&=' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '&=' on given operands.
 */
inline ReflectableBitmask operator&=(ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    lhs = ReflectableBitmask(lhs.getValue() & rhs.getValue());
    return lhs;
}

/**
 * Defines operator '^=' for the bitmask 'ReflectableBitmask'.
 *
 * \param lhs Left operand.
 * \param rhs Right operand.
 *
 * \return Bitmask which contains result after applying the operator '^=' on given operands.
 */
inline ReflectableBitmask operator^=(ReflectableBitmask& lhs, const ReflectableBitmask& rhs)
{
    lhs = ReflectableBitmask(lhs.getValue() ^ rhs.getValue());
    return lhs;
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct TypeInfo<::test_object::polymorphic_allocator::ReflectableBitmask, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableBitmask value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IIntrospectableViewConstPtr introspectable(::test_object::polymorphic_allocator::ReflectableBitmask value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::ReflectableBitmask>
{
    size_t operator()(const ::test_object::polymorphic_allocator::ReflectableBitmask& value) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_BITMASK_H
