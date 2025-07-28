#ifndef ZSERIO_HASH_CODE_UTIL_H_INC
#define ZSERIO_HASH_CODE_UTIL_H_INC

#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "zserio/Bytes.h"
#include "zserio/FloatUtil.h"
#include "zserio/Traits.h"
#include "zserio/Types.h"
#include "zserio/Vector.h"

namespace zserio
{

/** Prime number for hash calculation. */
static const uint32_t HASH_PRIME_NUMBER = 37;
/** Initial seed for hash calculation. */
static const uint32_t HASH_SEED = 23;

/**
 * Gets initial hash code calculated from the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 *
 * \return Hash code.
 */
inline uint32_t calcHashCodeFirstTerm(uint32_t seedValue)
{
    return HASH_PRIME_NUMBER * seedValue;
}

/**
 * Calculates hash code of the given integral value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename T>
inline typename std::enable_if<std::is_integral<T>::value && (sizeof(T) <= 4), uint32_t>::type calcHashCode(
        uint32_t seedValue, T value)
{
    return calcHashCodeFirstTerm(seedValue) + static_cast<uint32_t>(value);
}

/**
 * Calculates hash code of the given integral value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename T>
inline typename std::enable_if<std::is_integral<T>::value && (sizeof(T) > 4), uint32_t>::type calcHashCode(
        uint32_t seedValue, T value)
{
    const auto unsignedValue = static_cast<typename std::make_unsigned<T>::type>(value);
    return calcHashCodeFirstTerm(seedValue) + static_cast<uint32_t>(unsignedValue ^ (unsignedValue >> 32U));
}

/**
 * Calculates hash code of the given float value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
inline uint32_t calcHashCode(uint32_t seedValue, float value)
{
    return calcHashCode(seedValue, convertFloatToUInt32(value));
}

/**
 * Calculates hash code of the given double value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
inline uint32_t calcHashCode(uint32_t seedValue, double value)
{
    return calcHashCode(seedValue, convertDoubleToUInt64(value));
}

/**
 * Calculates hash code of the given bool wrapper value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
inline uint32_t calcHashCode(uint32_t seedValue, Bool value)
{
    return calcHashCode(seedValue, static_cast<Bool::ValueType>(value));
}

/**
 * Calculates hash code of the given int wrapper value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <BitSize BIT_SIZE, bool IS_SIGNED>
inline uint32_t calcHashCode(uint32_t seedValue, detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED> value)
{
    using ValueType = typename detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>::ValueType;
    return calcHashCode(seedValue, static_cast<ValueType>(value));
}

/**
 * Calculates hash code of the given dynamic int wrapper value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename VALUE_TYPE>
inline uint32_t calcHashCode(uint32_t seedValue, detail::DynIntWrapper<VALUE_TYPE> value)
{
    return calcHashCode(seedValue, static_cast<VALUE_TYPE>(value));
}

/**
 * Calculates hash code of the given variable int wrapper value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename VALUE_TYPE, detail::VarIntType VAR_TYPE>
inline uint32_t calcHashCode(uint32_t seedValue, detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE> value)
{
    return calcHashCode(seedValue, static_cast<VALUE_TYPE>(value));
}

/**
 * Calculates hash code of the given floating-point wrapper value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename VALUE_TYPE, detail::FloatType FLOAT_TYPE>
inline uint32_t calcHashCode(uint32_t seedValue, detail::FloatWrapper<VALUE_TYPE, FLOAT_TYPE> value)
{
    return calcHashCode(seedValue, static_cast<VALUE_TYPE>(value));
}

// TODO[Mi-L@]: support all zserio wrappers

/**
 * Calculates hash code of the given string value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param stringValue Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename ALLOC>
inline uint32_t calcHashCode(uint32_t seedValue, const BasicString<ALLOC>& stringValue)
{
    uint32_t result = seedValue;
    for (auto element : stringValue)
    {
        result = calcHashCode(result, element);
    }

    return result;
}

/**
 * Calculates hash code of the given string value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param stringValue Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
inline uint32_t calcHashCode(uint32_t seedValue, std::string_view stringValue)
{
    uint32_t result = seedValue;
    for (auto element : stringValue)
    {
        result = calcHashCode(result, element);
    }

    return result;
}

/**
 * Calculates hash code of the given enum item or bitmask value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Enum item or bitmask value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename T>
inline std::enable_if_t<std::is_enum_v<T> || is_bitmask_v<T>, uint32_t> calcHashCode(
        uint32_t seedValue, T value)
{
    return calcHashCode(seedValue, std::hash<T>()(value));
}

/**
 * Calculates hash code of the given Zserio object (structure, choice, ...) using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param object Object for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename OBJECT>
inline std::enable_if_t<!std::is_enum_v<OBJECT> && !is_bitmask_v<OBJECT> && !std::is_integral_v<OBJECT>,
        uint32_t>
calcHashCode(uint32_t seedValue, const OBJECT& object)
{
    return calcHashCode(seedValue, std::hash<OBJECT>()(object));
}

// TODO[Mi-L@]: implement for a generic span?
/**
 * Calculates hash code of the given BytesView type using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param array Array for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
inline uint32_t calcHashCode(uint32_t seedValue, const BytesView& bytes)
{
    uint32_t result = seedValue;
    for (auto byte : bytes)
    {
        result = calcHashCode(result, byte);
    }

    return result;
}

/**
 * Calculates hash code of the given Zserio array using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param array Array for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename ARRAY_ELEMENT, typename ALLOC>
inline uint32_t calcHashCode(uint32_t seedValue, const Vector<ARRAY_ELEMENT, ALLOC>& array)
{
    uint32_t result = seedValue;
    for (const ARRAY_ELEMENT& element : array)
    {
        result = calcHashCode(result, element);
    }

    return result;
}

/**
 * Calculates the hash code of std::monostate which is often used in variants so the hash code is not
 * implementation defined.
 *
 * \param seedValue Seed value (current hash code)
 *
 * \return Calculated hash code.
 */
inline uint32_t calcHashCode(uint32_t seedValue, std::monostate)
{
    return calcHashCode(seedValue, 1729);
}

} // namespace zserio

#endif // ZSERIO_HASH_CODE_UTIL_H_INC
