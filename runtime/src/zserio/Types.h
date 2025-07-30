#ifndef ZSERIO_TYPES_H_INC
#define ZSERIO_TYPES_H_INC

#include <cstdint>
#include <limits>
#include <string_view>
#include <type_traits>

#include "zserio/BitSize.h"
#include "zserio/OutOfRangeException.h"
#include "zserio/StringConvertUtil.h"
#include "zserio/Traits.h"

namespace zserio
{

/**
 * NumericLimits template provides a standardized way to query various properties of
 * zserio numerical type wrappers. Inspired by std::numeric_limits.
 *
 * This information is provided via specializations of the NumericLimits template. All types provide information
 * about their minimum and maximum value.
 */
template <typename T>
struct NumericLimits
{};

namespace detail
{

/**
 * Base type wrapper for all numeric types.
 *
 * Implements all necessary conversions and operators needed to allow to use particular type wrappers
 * in all expressions as if they were built-in types.
 *
 * Limitations:
 *
 * * Implicit assignment between different wrapper types is not allowed.
 */
template <typename VALUE_TYPE>
class NumericTypeWrapper
{
public:
    /** Typedef for the underlying value type. */
    using ValueType = VALUE_TYPE;

    /**
     * Empty constructor.
     */
    constexpr explicit NumericTypeWrapper() noexcept :
            m_value()
    {}

    /**
     * Implicit constructor from underlying value type.
     *
     * \param value Value to construct from.
     */
    constexpr NumericTypeWrapper(ValueType value) noexcept :
            m_value(value)
    {}

    /**
     * Implicit conversion to underlying value type.
     */
    constexpr operator ValueType() const noexcept
    {
        return m_value;
    }

    /**
     * Operator overload needed to allow to use particular type wrappers in all expressions as if they
     * were built-in types.
     *
     * Note that overload of operator ValueType&() could be used instead, but it fires a warning on gcc.
     */
    /** \{ */

    constexpr NumericTypeWrapper& operator++() noexcept
    {
        ++m_value;
        return *this;
    }

    constexpr NumericTypeWrapper operator++(int) noexcept
    {
        NumericTypeWrapper old = *this;
        operator++();
        return old;
    }

    constexpr NumericTypeWrapper& operator--() noexcept
    {
        --m_value;
        return *this;
    }

    constexpr NumericTypeWrapper operator--(int) noexcept
    {
        NumericTypeWrapper old = *this;
        operator--();
        return old;
    }

    constexpr NumericTypeWrapper& operator+=(ValueType value) noexcept
    {
        m_value += value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator-=(ValueType value) noexcept
    {
        m_value -= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator*=(ValueType value) noexcept
    {
        m_value *= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator/=(ValueType value) noexcept
    {
        m_value /= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator%=(ValueType value) noexcept
    {
        m_value %= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator&=(ValueType value) noexcept
    {
        m_value &= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator|=(ValueType value) noexcept
    {
        m_value |= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator^=(ValueType value) noexcept
    {
        m_value ^= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator<<=(ValueType value) noexcept
    {
        m_value <<= value;
        return *this;
    }

    constexpr NumericTypeWrapper& operator>>=(ValueType value) noexcept
    {
        m_value >>= value;
        return *this;
    }

    /** \} */

private:
    ValueType m_value;
};

class BoolWrapper : public NumericTypeWrapper<bool>
{
public:
    using NumericTypeWrapper<bool>::NumericTypeWrapper;
};

constexpr bool operator<(const BoolWrapper& lhs, const BoolWrapper& rhs)
{
    return static_cast<int>(lhs) < static_cast<int>(rhs);
}

constexpr bool operator>(const BoolWrapper& lhs, const BoolWrapper& rhs)
{
    return rhs < lhs;
}

constexpr bool operator<=(const BoolWrapper& lhs, const BoolWrapper& rhs)
{
    return !(rhs < lhs);
}

constexpr bool operator>=(const BoolWrapper& lhs, const BoolWrapper& rhs)
{
    return !(lhs < rhs);
}

template <BitSize BIT_SIZE, bool IS_SIGNED, typename = void>
struct fixed_int_value_type;

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, true, std::enable_if_t<(BIT_SIZE > 0 && BIT_SIZE <= 8)>>
{
    using type = int8_t;
};

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, true, std::enable_if_t<(BIT_SIZE > 8 && BIT_SIZE <= 16)>>
{
    using type = int16_t;
};

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, true, std::enable_if_t<(BIT_SIZE > 16 && BIT_SIZE <= 32)>>
{
    using type = int32_t;
};

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, true, std::enable_if_t<(BIT_SIZE > 32 && BIT_SIZE <= 64)>>
{
    using type = int64_t;
};

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, false, std::enable_if_t<(BIT_SIZE > 0 && BIT_SIZE <= 8)>>
{
    using type = uint8_t;
};

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, false, std::enable_if_t<(BIT_SIZE > 8 && BIT_SIZE <= 16)>>
{
    using type = uint16_t;
};

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, false, std::enable_if_t<(BIT_SIZE > 16 && BIT_SIZE <= 32)>>
{
    using type = uint32_t;
};

template <BitSize BIT_SIZE>
struct fixed_int_value_type<BIT_SIZE, false, std::enable_if_t<(BIT_SIZE > 32 && BIT_SIZE <= 64)>>
{
    using type = uint64_t;
};

template <BitSize BIT_SIZE, bool IS_SIGNED>
using fixed_int_value_type_t = typename fixed_int_value_type<BIT_SIZE, IS_SIGNED>::type;

template <BitSize BIT_SIZE, bool IS_SIGNED>
class FixedIntWrapper : public NumericTypeWrapper<fixed_int_value_type_t<BIT_SIZE, IS_SIGNED>>
{
public:
    using ValueType = fixed_int_value_type_t<BIT_SIZE, IS_SIGNED>;
    using NumericTypeWrapper<ValueType>::NumericTypeWrapper;

    static_assert(BIT_SIZE > 0, "BitSize cannot be 0!");
    static_assert((BIT_SIZE + 7) / 8 <= sizeof(ValueType), "BitSize doesn't fit to the VALUE_TYPE!");
};

template <typename VALUE_TYPE>
class DynIntWrapper : public NumericTypeWrapper<VALUE_TYPE>
{
public:
    using NumericTypeWrapper<VALUE_TYPE>::NumericTypeWrapper;
};

enum class VarIntType : uint8_t
{
    VAR16,
    VAR32,
    VAR64,
    VAR,
    VARSIZE
};

template <typename VALUE_TYPE, VarIntType VAR_TYPE>
class VarIntWrapper : public NumericTypeWrapper<VALUE_TYPE>
{
    static_assert(
            VAR_TYPE != VarIntType::VARSIZE || std::is_unsigned_v<VALUE_TYPE>, "VARSIZE must be unsigned!");

public:
    using NumericTypeWrapper<VALUE_TYPE>::NumericTypeWrapper;
};

enum class FloatType : uint8_t
{
    FLOAT16,
    FLOAT32,
    FLOAT64
};

template <typename VALUE_TYPE, FloatType FLOAT_TYPE>
class FloatWrapper : public NumericTypeWrapper<VALUE_TYPE>
{
public:
    using NumericTypeWrapper<VALUE_TYPE>::NumericTypeWrapper;
};

// helper traits to check if range checking is needed (used in checkedCast)
template <typename T>
struct needs_range_check
{};

template <typename T>
inline constexpr bool needs_range_check_v = needs_range_check<T>::value;

template <>
struct needs_range_check<BoolWrapper> : std::false_type
{};

template <BitSize BIT_SIZE, bool IS_SIGNED>
struct needs_range_check<FixedIntWrapper<BIT_SIZE, IS_SIGNED>>
{
    using ValueType = typename FixedIntWrapper<BIT_SIZE, IS_SIGNED>::ValueType;

    static constexpr bool value = sizeof(ValueType) * 8 != BIT_SIZE ||
            (BIT_SIZE != 8 && BIT_SIZE != 16 && BIT_SIZE != 32 && BIT_SIZE != 64);
};

template <typename VALUE_TYPE>
struct needs_range_check<DynIntWrapper<VALUE_TYPE>> : std::true_type
{};

template <typename VALUE_TYPE, VarIntType VAR_TYPE>
struct needs_range_check<VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
{
    static constexpr bool value = VAR_TYPE != VarIntType::VAR;
};

template <typename VALUE_TYPE, FloatType FLOAT_TYPE>
struct needs_range_check<FloatWrapper<VALUE_TYPE, FLOAT_TYPE>> : std::false_type
{
    // TODO[mikir]: Does it have sense to have range checking for Float16?
};

// range checker used by checkedCast, specialized for particular types when needed
template <typename T>
struct RangeChecker
{
    static constexpr void check(typename T::ValueType value, std::string_view fieldName = "")
    {
        std::string_view fieldNamePrefix = (fieldName.empty()) ? "" : " for field ";
        if constexpr (std::is_signed_v<typename T::ValueType>)
        {
            if (value < NumericLimits<T>::min() || value > NumericLimits<T>::max())
            {
                throw OutOfRangeException("Value '")
                        << value << "' out of range '<" << NumericLimits<T>::min() << ", "
                        << NumericLimits<T>::max() << ">'" << fieldNamePrefix << fieldName << "!";
            }
        }
        else
        {
            if (value > NumericLimits<T>::max())
            {
                throw OutOfRangeException("Value '")
                        << value << "' out of bounds '" << NumericLimits<T>::max() << "'" << fieldNamePrefix
                        << fieldName << "!";
            }
        }
    }
};

template <typename VALUE_TYPE>
struct RangeChecker<DynIntWrapper<VALUE_TYPE>>
{
    static constexpr void check(VALUE_TYPE value, BitSize numBits, std::string_view fieldName = "")
    {
        std::string_view fieldNamePrefix = (fieldName.empty()) ? "" : " for field ";
        if constexpr (std::is_signed_v<VALUE_TYPE>)
        {
            if (value < NumericLimits<DynIntWrapper<VALUE_TYPE>>::min(numBits) ||
                    value > NumericLimits<DynIntWrapper<VALUE_TYPE>>::max(numBits))
            {
                throw OutOfRangeException("Value '")
                        << value << "' out of range '<"
                        << NumericLimits<DynIntWrapper<VALUE_TYPE>>::min(numBits) << ", "
                        << NumericLimits<DynIntWrapper<VALUE_TYPE>>::max(numBits) << ">'" << fieldNamePrefix
                        << fieldName << "!";
            }
        }
        else
        {
            if (value > NumericLimits<DynIntWrapper<VALUE_TYPE>>::max(numBits))
            {
                throw OutOfRangeException("Value '")
                        << value << "' out of bounds '"
                        << NumericLimits<DynIntWrapper<VALUE_TYPE>>::max(numBits) << "'" << fieldNamePrefix
                        << fieldName << "!";
            }
        }
    }
};

} // namespace detail

/**
 * Specialization of NumericLimits template for bool type wrapper.
 */
template <>
struct NumericLimits<detail::BoolWrapper>
{
    static constexpr detail::BoolWrapper min() noexcept
    {
        return false;
    }

    static constexpr detail::BoolWrapper max() noexcept
    {
        return true;
    }
};

/**
 * Specialization of NumericLimits template for integral types wrapper.
 */
template <BitSize BIT_SIZE, bool IS_SIGNED>
struct NumericLimits<detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>>
{
    using ValueType = typename detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>::ValueType;

    static constexpr detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED> min() noexcept
    {
        if constexpr (!detail::needs_range_check_v<detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>>)
        {
            return std::numeric_limits<ValueType>::min();
        }
        else if constexpr (std::is_signed_v<ValueType>)
        {
            return static_cast<ValueType>(-static_cast<ValueType>(1ULL << (BIT_SIZE - 1U)));
        }
        else
        {
            return 0;
        }
    }

    static constexpr detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED> max() noexcept
    {
        if constexpr (!detail::needs_range_check_v<detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>>)
        {
            return std::numeric_limits<ValueType>::max();
        }
        else if constexpr (std::is_signed_v<ValueType>)
        {
            return static_cast<ValueType>((1ULL << (BIT_SIZE - 1U)) - 1U);
        }
        else
        {
            return static_cast<ValueType>((1ULL << BIT_SIZE) - 1U);
        }
    }
};

/**
 * Specialization of NumericLimits template for variable length integral types wrapper.
 */
template <typename VALUE_TYPE, detail::VarIntType VAR_TYPE>
struct NumericLimits<detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
{
    static constexpr detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE> min() noexcept
    {
        if constexpr (std::is_signed_v<VALUE_TYPE>)
        {
            if constexpr (VAR_TYPE == detail::VarIntType::VAR)
            {
                return std::numeric_limits<VALUE_TYPE>::min();
            }
            else
            {
                return -max();
            }
        }
        else
        {
            return 0;
        }
    }

    static constexpr detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE> max() noexcept
    {
        if constexpr (VAR_TYPE == detail::VarIntType::VAR16)
        {
            return static_cast<VALUE_TYPE>((UINT64_C(1) << (FIRST_BYTE_BITS + 8)) - 1);
        }
        else if constexpr (VAR_TYPE == detail::VarIntType::VAR32)
        {
            return static_cast<VALUE_TYPE>((UINT64_C(1) << (FIRST_BYTE_BITS + 7 + 7 + 8)) - 1);
        }
        else if constexpr (VAR_TYPE == detail::VarIntType::VAR64)
        {
            return static_cast<VALUE_TYPE>((UINT64_C(1) << (FIRST_BYTE_BITS + 7 + 7 + 7 + 7 + 7 + 7 + 8)) - 1);
        }
        else if constexpr (VAR_TYPE == detail::VarIntType::VAR)
        {
            return std::numeric_limits<VALUE_TYPE>::max();
        }
        else // VARSIZE
        {
            return (UINT64_C(1) << (2U + 7U + 7U + 7U + 8U)) - 1;
        }
    }

private:
    static constexpr uint64_t FIRST_BYTE_BITS = std::is_signed_v<VALUE_TYPE> ? 6 : 7;
};

/**
 * Specialization of NumericLimits template for dynamic length integral types wrapper with unknown bit size.
 */
template <typename VALUE_TYPE>
struct NumericLimits<detail::DynIntWrapper<VALUE_TYPE>>
{
    static constexpr detail::DynIntWrapper<VALUE_TYPE> min(BitSize numBits)
    {
        if constexpr (std::is_signed_v<VALUE_TYPE>)
        {
            checkNumBits(numBits);

            return minSignedValue(numBits);
        }
        else
        {
            return 0;
        }
    }

    static constexpr detail::DynIntWrapper<VALUE_TYPE> max(BitSize numBits)
    {
        checkNumBits(numBits);

        if constexpr (std::is_signed_v<VALUE_TYPE>)
        {
            return maxSignedValue(numBits);
        }
        else
        {
            return maxUnsignedValue(numBits);
        }
    }

private:
    static constexpr void checkNumBits(BitSize numBits)
    {
        if (numBits < 1 || numBits > sizeof(VALUE_TYPE) * 8)
        {
            throw OutOfRangeException("Dynamic bit field numBits '")
                    << numBits << "' out of range '<1, " << sizeof(VALUE_TYPE) * 8 << ">'!";
        }
    }

    static constexpr VALUE_TYPE minSignedValue(BitSize numBits)
    {
        if constexpr (std::is_same_v<VALUE_TYPE, int64_t>)
        {
            if (numBits == 64)
            {
                return INT64_MIN;
            }
        }

        return static_cast<VALUE_TYPE>(-static_cast<int64_t>(1ULL << (numBits - 1U)));
    }

    static constexpr VALUE_TYPE maxSignedValue(BitSize numBits)
    {
        if constexpr (std::is_same_v<VALUE_TYPE, int64_t>)
        {
            if (numBits == 64)
            {
                return INT64_MAX;
            }
        }

        return static_cast<VALUE_TYPE>((1ULL << (numBits - 1U)) - 1U);
    }

    static constexpr VALUE_TYPE maxUnsignedValue(BitSize numBits)
    {
        if constexpr (std::is_same_v<VALUE_TYPE, uint64_t>)
        {
            if (numBits == 64)
            {
                return UINT64_MAX;
            }
        }

        return static_cast<VALUE_TYPE>((1ULL << numBits) - 1U);
    }
};

/**
 * Specialization of NumericLimits template for float16 type wrapper.
 *
 * Hard-coded constants are used to allow the limits to be constexpr.
 */
template <>
struct NumericLimits<detail::FloatWrapper<float, detail::FloatType::FLOAT16>>
{
    static constexpr detail::FloatWrapper<float, detail::FloatType::FLOAT16> min() noexcept
    {
        return 6.103515625e-05F;
    }

    static constexpr detail::FloatWrapper<float, detail::FloatType::FLOAT16> max() noexcept
    {
        return 65504.F;
    }
};

/**
 * Specialization of NumericLimits template for float32 type wrapper.
 */
template <>
struct NumericLimits<detail::FloatWrapper<float, detail::FloatType::FLOAT32>>
{
    static constexpr detail::FloatWrapper<float, detail::FloatType::FLOAT32> min() noexcept
    {
        return std::numeric_limits<float>::min();
    }

    static constexpr detail::FloatWrapper<float, detail::FloatType::FLOAT32> max() noexcept
    {
        return std::numeric_limits<float>::max();
    }
};

/**
 * Specialization of NumericLimits template for float64 type wrapper.
 */
template <>
struct NumericLimits<detail::FloatWrapper<double, detail::FloatType::FLOAT64>>
{
    static constexpr detail::FloatWrapper<double, detail::FloatType::FLOAT64> min() noexcept
    {
        return std::numeric_limits<double>::min();
    }

    static constexpr detail::FloatWrapper<double, detail::FloatType::FLOAT64> max() noexcept
    {
        return std::numeric_limits<double>::max();
    }
};

/**
 * Utility to safely construct zserio numeric type wrapper from its underlying value with range checking.
 *
 * \param value Underlying value to convert to the type wrapper.
 *
 * \return Numeric type wrapper constructed after range checking (if needed).
 *
 * \throw OutOfRangeException when the underlying value is out of range of the zserio numeric type.
 */
template <typename T, std::enable_if_t<is_numeric_wrapper_v<T>, int> = 0>
constexpr T fromCheckedValue(typename T::ValueType value) noexcept(!detail::needs_range_check_v<T>)
{
    if constexpr (detail::needs_range_check_v<T>)
    {
        detail::RangeChecker<T>::check(value);
    }

    return T(value);
}

/**
 * Utility to safely construct zserio numeric type wrapper from its underlying value with range checking.
 *
 * Overload for dynamic length types wrappers.
 *
 * \param value Underlying value to convert to the type wrapper.
 * \param numBits Number of bits as a length of a dynamic length numeric type.
 *
 * \return Numeric type wrapper constructed after range checking (if needed).
 *
 * \throw OutOfRangeException when the underlying value is out of range of the zserio numeric type.
 */
template <typename T, std::enable_if_t<is_numeric_wrapper_v<T>, int> = 0>
constexpr T fromCheckedValue(typename T::ValueType value, BitSize numBits)
{
    detail::RangeChecker<T>::check(value, numBits);

    return T(value);
}

/**
 * Utility to get checked underlying value from zserio numeric type wrapper.
 *
 * \param wrapper Numeric type wrapper to use.
 *
 * \return Built-in numeric type casted from numeric type wrapper after range checking (if needed).
 *
 * \throw OutOfRangeException when the underlying value is out of range of the zserio numeric type.
 */
template <typename T, std::enable_if_t<is_numeric_wrapper_v<T>, int> = 0>
constexpr typename T::ValueType toCheckedValue(T wrapper) noexcept(!detail::needs_range_check_v<T>)
{
    if constexpr (detail::needs_range_check_v<T>)
    {
        detail::RangeChecker<T>::check(wrapper);
    }

    return wrapper;
}

/**
 * Utility to get checked underlying value from zserio numeric type wrapper.
 *
 * Overload for dynamic length types wrappers.
 *
 * \param wrapper Numeric type wrapper to use.
 * \param numBits Number of bits as a length of a dynamic length numeric type.
 *
 * \return Built-in numeric type casted from numeric type wrapper after range checking (if needed).
 *
 * \throw OutOfRangeException when the underlying value is out of range of the zserio numeric type.
 */
template <typename T, std::enable_if_t<is_numeric_wrapper_v<T>, int> = 0>
constexpr typename T::ValueType toCheckedValue(T wrapper, BitSize numBits)
{
    detail::RangeChecker<T>::check(wrapper, numBits);

    return wrapper;
}

/** Typedef for a zserio type. */
/** \{ */

using Bool = detail::BoolWrapper;

using Int1 = detail::FixedIntWrapper<1, true>;
using Int2 = detail::FixedIntWrapper<2, true>;
using Int3 = detail::FixedIntWrapper<3, true>;
using Int4 = detail::FixedIntWrapper<4, true>;
using Int5 = detail::FixedIntWrapper<5, true>;
using Int6 = detail::FixedIntWrapper<6, true>;
using Int7 = detail::FixedIntWrapper<7, true>;
using Int8 = detail::FixedIntWrapper<8, true>;
using Int9 = detail::FixedIntWrapper<9, true>;
using Int10 = detail::FixedIntWrapper<10, true>;
using Int11 = detail::FixedIntWrapper<11, true>;
using Int12 = detail::FixedIntWrapper<12, true>;
using Int13 = detail::FixedIntWrapper<13, true>;
using Int14 = detail::FixedIntWrapper<14, true>;
using Int15 = detail::FixedIntWrapper<15, true>;
using Int16 = detail::FixedIntWrapper<16, true>;
using Int17 = detail::FixedIntWrapper<17, true>;
using Int18 = detail::FixedIntWrapper<18, true>;
using Int19 = detail::FixedIntWrapper<19, true>;
using Int20 = detail::FixedIntWrapper<20, true>;
using Int21 = detail::FixedIntWrapper<21, true>;
using Int22 = detail::FixedIntWrapper<22, true>;
using Int23 = detail::FixedIntWrapper<23, true>;
using Int24 = detail::FixedIntWrapper<24, true>;
using Int25 = detail::FixedIntWrapper<25, true>;
using Int26 = detail::FixedIntWrapper<26, true>;
using Int27 = detail::FixedIntWrapper<27, true>;
using Int28 = detail::FixedIntWrapper<28, true>;
using Int29 = detail::FixedIntWrapper<29, true>;
using Int30 = detail::FixedIntWrapper<30, true>;
using Int31 = detail::FixedIntWrapper<31, true>;
using Int32 = detail::FixedIntWrapper<32, true>;
using Int33 = detail::FixedIntWrapper<33, true>;
using Int34 = detail::FixedIntWrapper<34, true>;
using Int35 = detail::FixedIntWrapper<35, true>;
using Int36 = detail::FixedIntWrapper<36, true>;
using Int37 = detail::FixedIntWrapper<37, true>;
using Int38 = detail::FixedIntWrapper<38, true>;
using Int39 = detail::FixedIntWrapper<39, true>;
using Int40 = detail::FixedIntWrapper<40, true>;
using Int41 = detail::FixedIntWrapper<41, true>;
using Int42 = detail::FixedIntWrapper<42, true>;
using Int43 = detail::FixedIntWrapper<43, true>;
using Int44 = detail::FixedIntWrapper<44, true>;
using Int45 = detail::FixedIntWrapper<45, true>;
using Int46 = detail::FixedIntWrapper<46, true>;
using Int47 = detail::FixedIntWrapper<47, true>;
using Int48 = detail::FixedIntWrapper<48, true>;
using Int49 = detail::FixedIntWrapper<49, true>;
using Int50 = detail::FixedIntWrapper<50, true>;
using Int51 = detail::FixedIntWrapper<51, true>;
using Int52 = detail::FixedIntWrapper<52, true>;
using Int53 = detail::FixedIntWrapper<53, true>;
using Int54 = detail::FixedIntWrapper<54, true>;
using Int55 = detail::FixedIntWrapper<55, true>;
using Int56 = detail::FixedIntWrapper<56, true>;
using Int57 = detail::FixedIntWrapper<57, true>;
using Int58 = detail::FixedIntWrapper<58, true>;
using Int59 = detail::FixedIntWrapper<59, true>;
using Int60 = detail::FixedIntWrapper<60, true>;
using Int61 = detail::FixedIntWrapper<61, true>;
using Int62 = detail::FixedIntWrapper<62, true>;
using Int63 = detail::FixedIntWrapper<63, true>;
using Int64 = detail::FixedIntWrapper<64, true>;

using UInt1 = detail::FixedIntWrapper<1, false>;
using UInt2 = detail::FixedIntWrapper<2, false>;
using UInt3 = detail::FixedIntWrapper<3, false>;
using UInt4 = detail::FixedIntWrapper<4, false>;
using UInt5 = detail::FixedIntWrapper<5, false>;
using UInt6 = detail::FixedIntWrapper<6, false>;
using UInt7 = detail::FixedIntWrapper<7, false>;
using UInt8 = detail::FixedIntWrapper<8, false>;
using UInt9 = detail::FixedIntWrapper<9, false>;
using UInt10 = detail::FixedIntWrapper<10, false>;
using UInt11 = detail::FixedIntWrapper<11, false>;
using UInt12 = detail::FixedIntWrapper<12, false>;
using UInt13 = detail::FixedIntWrapper<13, false>;
using UInt14 = detail::FixedIntWrapper<14, false>;
using UInt15 = detail::FixedIntWrapper<15, false>;
using UInt16 = detail::FixedIntWrapper<16, false>;
using UInt17 = detail::FixedIntWrapper<17, false>;
using UInt18 = detail::FixedIntWrapper<18, false>;
using UInt19 = detail::FixedIntWrapper<19, false>;
using UInt20 = detail::FixedIntWrapper<20, false>;
using UInt21 = detail::FixedIntWrapper<21, false>;
using UInt22 = detail::FixedIntWrapper<22, false>;
using UInt23 = detail::FixedIntWrapper<23, false>;
using UInt24 = detail::FixedIntWrapper<24, false>;
using UInt25 = detail::FixedIntWrapper<25, false>;
using UInt26 = detail::FixedIntWrapper<26, false>;
using UInt27 = detail::FixedIntWrapper<27, false>;
using UInt28 = detail::FixedIntWrapper<28, false>;
using UInt29 = detail::FixedIntWrapper<29, false>;
using UInt30 = detail::FixedIntWrapper<30, false>;
using UInt31 = detail::FixedIntWrapper<31, false>;
using UInt32 = detail::FixedIntWrapper<32, false>;
using UInt33 = detail::FixedIntWrapper<33, false>;
using UInt34 = detail::FixedIntWrapper<34, false>;
using UInt35 = detail::FixedIntWrapper<35, false>;
using UInt36 = detail::FixedIntWrapper<36, false>;
using UInt37 = detail::FixedIntWrapper<37, false>;
using UInt38 = detail::FixedIntWrapper<38, false>;
using UInt39 = detail::FixedIntWrapper<39, false>;
using UInt40 = detail::FixedIntWrapper<40, false>;
using UInt41 = detail::FixedIntWrapper<41, false>;
using UInt42 = detail::FixedIntWrapper<42, false>;
using UInt43 = detail::FixedIntWrapper<43, false>;
using UInt44 = detail::FixedIntWrapper<44, false>;
using UInt45 = detail::FixedIntWrapper<45, false>;
using UInt46 = detail::FixedIntWrapper<46, false>;
using UInt47 = detail::FixedIntWrapper<47, false>;
using UInt48 = detail::FixedIntWrapper<48, false>;
using UInt49 = detail::FixedIntWrapper<49, false>;
using UInt50 = detail::FixedIntWrapper<50, false>;
using UInt51 = detail::FixedIntWrapper<51, false>;
using UInt52 = detail::FixedIntWrapper<52, false>;
using UInt53 = detail::FixedIntWrapper<53, false>;
using UInt54 = detail::FixedIntWrapper<54, false>;
using UInt55 = detail::FixedIntWrapper<55, false>;
using UInt56 = detail::FixedIntWrapper<56, false>;
using UInt57 = detail::FixedIntWrapper<57, false>;
using UInt58 = detail::FixedIntWrapper<58, false>;
using UInt59 = detail::FixedIntWrapper<59, false>;
using UInt60 = detail::FixedIntWrapper<60, false>;
using UInt61 = detail::FixedIntWrapper<61, false>;
using UInt62 = detail::FixedIntWrapper<62, false>;
using UInt63 = detail::FixedIntWrapper<63, false>;
using UInt64 = detail::FixedIntWrapper<64, false>;

template <size_t BIT_SIZE>
using Int = detail::FixedIntWrapper<BIT_SIZE, true>;

template <size_t BIT_SIZE>
using UInt = detail::FixedIntWrapper<BIT_SIZE, false>;

using DynInt8 = detail::DynIntWrapper<int8_t>;
using DynInt16 = detail::DynIntWrapper<int16_t>;
using DynInt32 = detail::DynIntWrapper<int32_t>;
using DynInt64 = detail::DynIntWrapper<int64_t>;

using DynUInt8 = detail::DynIntWrapper<uint8_t>;
using DynUInt16 = detail::DynIntWrapper<uint16_t>;
using DynUInt32 = detail::DynIntWrapper<uint32_t>;
using DynUInt64 = detail::DynIntWrapper<uint64_t>;

using VarInt16 = detail::VarIntWrapper<int16_t, detail::VarIntType::VAR16>;
using VarInt32 = detail::VarIntWrapper<int32_t, detail::VarIntType::VAR32>;
using VarInt64 = detail::VarIntWrapper<int64_t, detail::VarIntType::VAR64>;
using VarInt = detail::VarIntWrapper<int64_t, detail::VarIntType::VAR>;

using VarUInt16 = detail::VarIntWrapper<uint16_t, detail::VarIntType::VAR16>;
using VarUInt32 = detail::VarIntWrapper<uint32_t, detail::VarIntType::VAR32>;
using VarUInt64 = detail::VarIntWrapper<uint64_t, detail::VarIntType::VAR64>;
using VarUInt = detail::VarIntWrapper<uint64_t, detail::VarIntType::VAR>;

using VarSize = detail::VarIntWrapper<uint32_t, detail::VarIntType::VARSIZE>;

using Float16 = detail::FloatWrapper<float, detail::FloatType::FLOAT16>;
using Float32 = detail::FloatWrapper<float, detail::FloatType::FLOAT32>;
using Float64 = detail::FloatWrapper<double, detail::FloatType::FLOAT64>;

/** \} */

/**
 * Allows to append NumericTypeWrapper to CppRuntimeException.
 *
 * \param exception Exception to modify.
 * \param bitBuffer Bit buffer value.
 *
 * \return Reference to the exception to allow operator chaining.
 */
template <typename VALUE_TYPE>
CppRuntimeException& operator<<(CppRuntimeException& exception, detail::NumericTypeWrapper<VALUE_TYPE> value)
{
    return exception << static_cast<VALUE_TYPE>(value);
}

namespace detail
{

template <typename T, std::enable_if_t<is_numeric_wrapper_v<T>, int> = 0>
void validate(T wrapper, std::string_view fieldName) noexcept(!detail::needs_range_check_v<T>)
{
    if constexpr (detail::needs_range_check_v<T>)
    {
        detail::RangeChecker<T>::check(wrapper, fieldName);
    }
}

template <typename T, std::enable_if_t<is_numeric_wrapper_v<T>, int> = 0>
void validate(T wrapper, BitSize numBits, std::string_view fieldName)
{
    detail::RangeChecker<T>::check(wrapper, numBits, fieldName);
}

inline BitSize bitSizeOf(BoolWrapper, BitSize = 0)
{
    return 1;
}

template <BitSize BIT_SIZE, bool IS_SIGNED>
BitSize bitSizeOf(FixedIntWrapper<BIT_SIZE, IS_SIGNED>, BitSize = 0)
{
    static_assert(BIT_SIZE != 0, "Variable dynamic bit fields not allowed here!");
    return BIT_SIZE;
}

BitSize bitSizeOf(VarInt16 value, BitSize = 0);

BitSize bitSizeOf(VarInt32 value, BitSize = 0);

BitSize bitSizeOf(VarInt64 value, BitSize = 0);

BitSize bitSizeOf(VarUInt16 value, BitSize = 0);

BitSize bitSizeOf(VarUInt32 value, BitSize = 0);

BitSize bitSizeOf(VarUInt64 value, BitSize = 0);

BitSize bitSizeOf(VarInt value, BitSize = 0);

BitSize bitSizeOf(VarUInt value, BitSize = 0);

BitSize bitSizeOf(VarSize value, BitSize = 0);

inline BitSize bitSizeOf(Float16, BitSize = 0)
{
    return 16;
}

inline BitSize bitSizeOf(Float32, BitSize = 0)
{
    return 32;
}

inline BitSize bitSizeOf(Float64, BitSize = 0)
{
    return 64;
}

inline BitSize initializeOffsets(BoolWrapper value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

template <BitSize BIT_SIZE, bool IS_SIGNED>
BitSize initializeOffsets(FixedIntWrapper<BIT_SIZE, IS_SIGNED> value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarInt16 value, BitSize = 0);

BitSize initializeOffsets(VarInt32 value, BitSize = 0);

BitSize initializeOffsets(VarInt64 value, BitSize = 0);

BitSize initializeOffsets(VarUInt16 value, BitSize = 0);

BitSize initializeOffsets(VarUInt32 value, BitSize = 0);

BitSize initializeOffsets(VarUInt64 value, BitSize = 0);

BitSize initializeOffsets(VarInt value, BitSize = 0);

BitSize initializeOffsets(VarUInt value, BitSize = 0);

BitSize initializeOffsets(VarSize value, BitSize = 0);

inline BitSize initializeOffsets(Float16 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

inline BitSize initializeOffsets(Float32 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

inline BitSize initializeOffsets(Float64 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

} // namespace detail

} // namespace zserio

#endif // ifndef ZSERIO_TYPES_H_INC
