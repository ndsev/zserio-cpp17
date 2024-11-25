#ifndef ZSERIO_TYPES_H_INC
#define ZSERIO_TYPES_H_INC

#include <cstdint>
#include <limits>
#include <string_view>
#include <type_traits>

#include "zserio/BitSize.h"
#include "zserio/OutOfRangeException.h"

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
        NumericTypeWrapper old;
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
        NumericTypeWrapper old;
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

template <typename VALUE_TYPE, BitSize BIT_SIZE = 8 * sizeof(VALUE_TYPE)>
class IntWrapper : public NumericTypeWrapper<VALUE_TYPE>
{
private:
    static_assert((BIT_SIZE + 7) / 8 <= sizeof(VALUE_TYPE), "BitSize doesn't fit to the VALUE_TYPE!");

public:
    using NumericTypeWrapper<VALUE_TYPE>::NumericTypeWrapper;
};

template <typename VALUE_TYPE, BitSize BIT_SIZE = 0>
class DynIntWrapper : public IntWrapper<VALUE_TYPE, BIT_SIZE>
{
public:
    using IntWrapper<VALUE_TYPE, BIT_SIZE>::IntWrapper;
};

template <typename VALUE_TYPE>
class DynIntWrapper<VALUE_TYPE, 0> : public IntWrapper<VALUE_TYPE>
{
public:
    using IntWrapper<VALUE_TYPE>::IntWrapper;
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

template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct needs_range_check<IntWrapper<VALUE_TYPE, BIT_SIZE>>
{
    static constexpr bool value = sizeof(VALUE_TYPE) * 8 != BIT_SIZE ||
            (BIT_SIZE != 8 && BIT_SIZE != 16 && BIT_SIZE != 32 && BIT_SIZE != 64);
};

template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct needs_range_check<DynIntWrapper<VALUE_TYPE, BIT_SIZE>>
        : needs_range_check<IntWrapper<VALUE_TYPE, BIT_SIZE>>
{};

template <typename VALUE_TYPE>
struct needs_range_check<DynIntWrapper<VALUE_TYPE, 0>> : std::true_type
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
struct RangeChecker<DynIntWrapper<VALUE_TYPE, 0>>
{
    static constexpr void check(VALUE_TYPE value, BitSize numBits, std::string_view fieldName = "")
    {
        std::string_view fieldNamePrefix = (fieldName.empty()) ? "" : " for field ";
        if constexpr (std::is_signed_v<VALUE_TYPE>)
        {
            if (value < NumericLimits<DynIntWrapper<VALUE_TYPE, 0>>::min(numBits) ||
                    value > NumericLimits<DynIntWrapper<VALUE_TYPE, 0>>::max(numBits))
            {
                throw OutOfRangeException("Value '")
                        << value << "' out of range '<"
                        << NumericLimits<DynIntWrapper<VALUE_TYPE, 0>>::min(numBits) << ", "
                        << NumericLimits<DynIntWrapper<VALUE_TYPE, 0>>::max(numBits) << ">'" << fieldNamePrefix
                        << fieldName << "!";
            }
        }
        else
        {
            if (value > NumericLimits<DynIntWrapper<VALUE_TYPE, 0>>::max(numBits))
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
template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct NumericLimits<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>
{
    static constexpr detail::IntWrapper<VALUE_TYPE, BIT_SIZE> min() noexcept
    {
        if constexpr (!detail::needs_range_check_v<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>)
        {
            return std::numeric_limits<VALUE_TYPE>::min();
        }
        else if constexpr (std::is_signed_v<VALUE_TYPE>)
        {
            return static_cast<VALUE_TYPE>(-static_cast<VALUE_TYPE>(1ULL << (BIT_SIZE - 1U)));
        }
        else
        {
            return 0;
        }
    }

    static constexpr detail::IntWrapper<VALUE_TYPE, BIT_SIZE> max() noexcept
    {
        if constexpr (!detail::needs_range_check_v<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>)
        {
            return std::numeric_limits<VALUE_TYPE>::max();
        }
        else if constexpr (std::is_signed_v<VALUE_TYPE>)
        {
            return static_cast<VALUE_TYPE>((1ULL << (BIT_SIZE - 1U)) - 1U);
        }
        else
        {
            return static_cast<VALUE_TYPE>((1ULL << BIT_SIZE) - 1U);
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
 * Specialization of NumericLimits template for dynamic length integral types wrapper which has known bit size.
 */
template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct NumericLimits<detail::DynIntWrapper<VALUE_TYPE, BIT_SIZE>>
        : NumericLimits<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>
{};

/**
 * Specialization of NumericLimits template for dynamic length integral types wrapper with unknown bit size.
 */
template <typename VALUE_TYPE>
struct NumericLimits<detail::DynIntWrapper<VALUE_TYPE, 0>>
{
    static constexpr detail::DynIntWrapper<VALUE_TYPE, 0> min(BitSize numBits)
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

    static constexpr detail::DynIntWrapper<VALUE_TYPE, 0> max(BitSize numBits)
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
template <typename T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::ValueType>, T>, int> = 0>
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
template <typename T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::ValueType>, T>, int> = 0>
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
template <typename T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::ValueType>, T>, int> = 0>
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
template <typename T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::ValueType>, T>, int> = 0>
constexpr typename T::ValueType toCheckedValue(T wrapper, BitSize numBits)
{
    detail::RangeChecker<T>::check(wrapper, numBits);

    return wrapper;
}

/** Typedef for a zserio type. */
/** \{ */

using Bool = detail::BoolWrapper;

using Int8 = detail::IntWrapper<int8_t>;
using Int16 = detail::IntWrapper<int16_t>;
using Int32 = detail::IntWrapper<int32_t>;
using Int64 = detail::IntWrapper<int64_t>;

using UInt8 = detail::IntWrapper<uint8_t>;
using UInt16 = detail::IntWrapper<uint16_t>;
using UInt32 = detail::IntWrapper<uint32_t>;
using UInt64 = detail::IntWrapper<uint64_t>;

using Int1 = detail::IntWrapper<int8_t, 1>;
using Int2 = detail::IntWrapper<int8_t, 2>;
using Int3 = detail::IntWrapper<int8_t, 3>;
using Int4 = detail::IntWrapper<int8_t, 4>;
using Int5 = detail::IntWrapper<int8_t, 5>;
using Int6 = detail::IntWrapper<int8_t, 6>;
using Int7 = detail::IntWrapper<int8_t, 7>;
using Int9 = detail::IntWrapper<int16_t, 9>;
using Int10 = detail::IntWrapper<int16_t, 10>;
using Int11 = detail::IntWrapper<int16_t, 11>;
using Int12 = detail::IntWrapper<int16_t, 12>;
using Int13 = detail::IntWrapper<int16_t, 13>;
using Int14 = detail::IntWrapper<int16_t, 14>;
using Int15 = detail::IntWrapper<int16_t, 15>;
using Int17 = detail::IntWrapper<int32_t, 17>;
using Int18 = detail::IntWrapper<int32_t, 18>;
using Int19 = detail::IntWrapper<int32_t, 19>;
using Int20 = detail::IntWrapper<int32_t, 20>;
using Int21 = detail::IntWrapper<int32_t, 21>;
using Int22 = detail::IntWrapper<int32_t, 22>;
using Int23 = detail::IntWrapper<int32_t, 23>;
using Int24 = detail::IntWrapper<int32_t, 24>;
using Int25 = detail::IntWrapper<int32_t, 25>;
using Int26 = detail::IntWrapper<int32_t, 26>;
using Int27 = detail::IntWrapper<int32_t, 27>;
using Int28 = detail::IntWrapper<int32_t, 28>;
using Int29 = detail::IntWrapper<int32_t, 29>;
using Int30 = detail::IntWrapper<int32_t, 30>;
using Int31 = detail::IntWrapper<int32_t, 31>;
using Int33 = detail::IntWrapper<int64_t, 33>;
using Int34 = detail::IntWrapper<int64_t, 34>;
using Int35 = detail::IntWrapper<int64_t, 35>;
using Int36 = detail::IntWrapper<int64_t, 36>;
using Int37 = detail::IntWrapper<int64_t, 37>;
using Int38 = detail::IntWrapper<int64_t, 38>;
using Int39 = detail::IntWrapper<int64_t, 39>;
using Int40 = detail::IntWrapper<int64_t, 40>;
using Int41 = detail::IntWrapper<int64_t, 41>;
using Int42 = detail::IntWrapper<int64_t, 42>;
using Int43 = detail::IntWrapper<int64_t, 43>;
using Int44 = detail::IntWrapper<int64_t, 44>;
using Int45 = detail::IntWrapper<int64_t, 45>;
using Int46 = detail::IntWrapper<int64_t, 46>;
using Int47 = detail::IntWrapper<int64_t, 47>;
using Int48 = detail::IntWrapper<int64_t, 48>;
using Int49 = detail::IntWrapper<int64_t, 49>;
using Int50 = detail::IntWrapper<int64_t, 50>;
using Int51 = detail::IntWrapper<int64_t, 51>;
using Int52 = detail::IntWrapper<int64_t, 52>;
using Int53 = detail::IntWrapper<int64_t, 53>;
using Int54 = detail::IntWrapper<int64_t, 54>;
using Int55 = detail::IntWrapper<int64_t, 55>;
using Int56 = detail::IntWrapper<int64_t, 56>;
using Int57 = detail::IntWrapper<int64_t, 57>;
using Int58 = detail::IntWrapper<int64_t, 58>;
using Int59 = detail::IntWrapper<int64_t, 59>;
using Int60 = detail::IntWrapper<int64_t, 60>;
using Int61 = detail::IntWrapper<int64_t, 61>;
using Int62 = detail::IntWrapper<int64_t, 62>;
using Int63 = detail::IntWrapper<int64_t, 63>;

using UInt1 = detail::IntWrapper<uint8_t, 1>;
using UInt2 = detail::IntWrapper<uint8_t, 2>;
using UInt3 = detail::IntWrapper<uint8_t, 3>;
using UInt4 = detail::IntWrapper<uint8_t, 4>;
using UInt5 = detail::IntWrapper<uint8_t, 5>;
using UInt6 = detail::IntWrapper<uint8_t, 6>;
using UInt7 = detail::IntWrapper<uint8_t, 7>;
using UInt9 = detail::IntWrapper<uint16_t, 9>;
using UInt10 = detail::IntWrapper<uint16_t, 10>;
using UInt11 = detail::IntWrapper<uint16_t, 11>;
using UInt12 = detail::IntWrapper<uint16_t, 12>;
using UInt13 = detail::IntWrapper<uint16_t, 13>;
using UInt14 = detail::IntWrapper<uint16_t, 14>;
using UInt15 = detail::IntWrapper<uint16_t, 15>;
using UInt17 = detail::IntWrapper<uint32_t, 17>;
using UInt18 = detail::IntWrapper<uint32_t, 18>;
using UInt19 = detail::IntWrapper<uint32_t, 19>;
using UInt20 = detail::IntWrapper<uint32_t, 20>;
using UInt21 = detail::IntWrapper<uint32_t, 21>;
using UInt22 = detail::IntWrapper<uint32_t, 22>;
using UInt23 = detail::IntWrapper<uint32_t, 23>;
using UInt24 = detail::IntWrapper<uint32_t, 24>;
using UInt25 = detail::IntWrapper<uint32_t, 25>;
using UInt26 = detail::IntWrapper<uint32_t, 26>;
using UInt27 = detail::IntWrapper<uint32_t, 27>;
using UInt28 = detail::IntWrapper<uint32_t, 28>;
using UInt29 = detail::IntWrapper<uint32_t, 29>;
using UInt30 = detail::IntWrapper<uint32_t, 30>;
using UInt31 = detail::IntWrapper<uint32_t, 31>;
using UInt33 = detail::IntWrapper<uint64_t, 33>;
using UInt34 = detail::IntWrapper<uint64_t, 34>;
using UInt35 = detail::IntWrapper<uint64_t, 35>;
using UInt36 = detail::IntWrapper<uint64_t, 36>;
using UInt37 = detail::IntWrapper<uint64_t, 37>;
using UInt38 = detail::IntWrapper<uint64_t, 38>;
using UInt39 = detail::IntWrapper<uint64_t, 39>;
using UInt40 = detail::IntWrapper<uint64_t, 40>;
using UInt41 = detail::IntWrapper<uint64_t, 41>;
using UInt42 = detail::IntWrapper<uint64_t, 42>;
using UInt43 = detail::IntWrapper<uint64_t, 43>;
using UInt44 = detail::IntWrapper<uint64_t, 44>;
using UInt45 = detail::IntWrapper<uint64_t, 45>;
using UInt46 = detail::IntWrapper<uint64_t, 46>;
using UInt47 = detail::IntWrapper<uint64_t, 47>;
using UInt48 = detail::IntWrapper<uint64_t, 48>;
using UInt49 = detail::IntWrapper<uint64_t, 49>;
using UInt50 = detail::IntWrapper<uint64_t, 50>;
using UInt51 = detail::IntWrapper<uint64_t, 51>;
using UInt52 = detail::IntWrapper<uint64_t, 52>;
using UInt53 = detail::IntWrapper<uint64_t, 53>;
using UInt54 = detail::IntWrapper<uint64_t, 54>;
using UInt55 = detail::IntWrapper<uint64_t, 55>;
using UInt56 = detail::IntWrapper<uint64_t, 56>;
using UInt57 = detail::IntWrapper<uint64_t, 57>;
using UInt58 = detail::IntWrapper<uint64_t, 58>;
using UInt59 = detail::IntWrapper<uint64_t, 59>;
using UInt60 = detail::IntWrapper<uint64_t, 60>;
using UInt61 = detail::IntWrapper<uint64_t, 61>;
using UInt62 = detail::IntWrapper<uint64_t, 62>;
using UInt63 = detail::IntWrapper<uint64_t, 63>;

template <BitSize BIT_SIZE = 0>
using DynInt8 = detail::DynIntWrapper<int8_t, BIT_SIZE>;
template <BitSize BIT_SIZE = 0>
using DynInt16 = detail::DynIntWrapper<int16_t, BIT_SIZE>;
template <BitSize BIT_SIZE = 0>
using DynInt32 = detail::DynIntWrapper<int32_t, BIT_SIZE>;
template <BitSize BIT_SIZE = 0>
using DynInt64 = detail::DynIntWrapper<int64_t, BIT_SIZE>;

template <BitSize BIT_SIZE = 0>
using DynUInt8 = detail::DynIntWrapper<uint8_t, BIT_SIZE>;
template <BitSize BIT_SIZE = 0>
using DynUInt16 = detail::DynIntWrapper<uint16_t, BIT_SIZE>;
template <BitSize BIT_SIZE = 0>
using DynUInt32 = detail::DynIntWrapper<uint32_t, BIT_SIZE>;
template <BitSize BIT_SIZE = 0>
using DynUInt64 = detail::DynIntWrapper<uint64_t, BIT_SIZE>;

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

template <typename T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::ValueType>, T>, int> = 0>
void validate(T wrapper, std::string_view fieldName) noexcept(!detail::needs_range_check_v<T>)
{
    if constexpr (detail::needs_range_check_v<T>)
    {
        detail::RangeChecker<T>::check(wrapper, fieldName);
    }
}

template <typename T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::ValueType>, T>, int> = 0>
void validate(T wrapper, BitSize numBits, std::string_view fieldName)
{
    detail::RangeChecker<T>::check(wrapper, numBits, fieldName);
}

inline BitSize bitSizeOf(BoolWrapper, BitSize = 0)
{
    return 1;
}

template <typename T, BitSize BIT_SIZE>
BitSize bitSizeOf(IntWrapper<T, BIT_SIZE>, BitSize = 0)
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

} // namespace detail

} // namespace zserio

#endif // ifndef ZSERIO_TYPES_H_INC
