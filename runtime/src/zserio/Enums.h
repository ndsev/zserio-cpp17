#ifndef ZSERIO_ENUMS_H_INC
#define ZSERIO_ENUMS_H_INC

#include <algorithm>
#include <cstddef>
#include <string_view>
#include <type_traits>

#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/DeltaContext.h"
#include "zserio/ItemsAccessor.h"

namespace zserio
{

/**
 * Enum traits provides various information for Zserio enums.
 *
 * This information is provided via specializations of the EnumTraits structure.
 */
template <typename T>
struct EnumTraits
{};

/**
 * Gets ordinal number of the given enum item.
 *
 * \param value Enum item.
 *
 * \return Ordinal number of the enum item.
 */
template <typename T>
size_t enumToOrdinal(T value);

/**
 * Converts the given raw value to an appropriate enum item.
 *
 * \param rawValue Raw value of the proper underlying type.
 *
 * \return Enum item corresponding to the rawValue.
 *
 * \throw CppRuntimeException when the rawValue doesn't match to any enum item.
 */
template <typename T>
T valueToEnum(typename EnumTraits<T>::ZserioType rawValue);

/**
 * Gets the underlying raw value of the given enum item.
 *
 * \param value Enum item.
 *
 * \return Raw value.
 */
template <typename T>
constexpr typename EnumTraits<T>::ZserioType enumToValue(T value)
{
    return static_cast<typename EnumTraits<T>::ZserioType>(static_cast<std::underlying_type_t<T>>(value));
}

/**
 * Converts the given enum item name to an appropriate enum item.
 *
 * \param itemName Name of the enum item.
 *
 * \return Enum item corresponding to the itemName.
 *
 * \throw CppRuntimeException when the itemName doesn't match to any enum item.
 */
template <typename T>
T stringToEnum(std::string_view itemName)
{
    const auto foundIt = std::find_if(EnumTraits<T>::names.begin(), EnumTraits<T>::names.end(),
            [itemName](std::string_view enumItemName) {
                return itemName.compare(enumItemName) == 0;
            });
    if (foundIt == EnumTraits<T>::names.end())
    {
        throw CppRuntimeException("Enum item '")
                << itemName << "' doesn't exist in enum '" << EnumTraits<T>::enumName << "'!";
    }

    const size_t ordinal = static_cast<size_t>(std::distance(EnumTraits<T>::names.begin(), foundIt));
    return EnumTraits<T>::values[ordinal];
}

/**
 * Gets the name of the given enum item.
 *
 * \param value Enum item.
 *
 * \return Name of the enum item.
 */
template <typename T>
std::string_view enumToString(T value)
{
    return EnumTraits<T>::names[enumToOrdinal(value)];
}

namespace detail
{

template <typename T>
std::enable_if_t<std::is_enum_v<T>> validate(T value, std::string_view fieldName)
{
    validate(enumToValue(value), fieldName);
}

template <typename T>
std::enable_if_t<std::is_enum_v<T>, BitSize> bitSizeOf(T value, BitSize bitPosition = 0)
{
    return bitSizeOf(enumToValue(value), bitPosition);
}

template <typename T>
std::enable_if_t<std::is_enum_v<T>> write(BitStreamWriter& writer, T value)
{
    write(writer, enumToValue(value));
}

template <typename T>
std::enable_if_t<std::is_enum_v<T>> read(zserio::BitStreamReader& reader, T& value)
{
    typename EnumTraits<T>::ZserioType rawValue;
    read(reader, rawValue);
    value = valueToEnum<T>(rawValue);
}

template <typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
inline void initContext(DeltaContext& deltaContext, T value)
{
    deltaContext.init(enumToValue(value));
}

template <typename T>
std::enable_if_t<std::is_enum_v<T>, BitSize> bitSizeOf(DeltaContext& deltaContext, T value, BitSize = 0)
{
    return deltaContext.bitSizeOf(enumToValue(value));
}

template <typename T>
std::enable_if_t<std::is_enum_v<T>> write(DeltaContext& deltaContext, BitStreamWriter& writer, T value)
{
    deltaContext.write(writer, enumToValue(value));
}

template <typename T>
std::enable_if_t<std::is_enum_v<T>> read(DeltaContext& deltaContext, BitStreamReader& reader, T& value)
{
    typename EnumTraits<T>::ZserioType rawValue;
    deltaContext.read(reader, rawValue);
    value = valueToEnum<T>(rawValue);
}

} // namespace detail

/**
 * Appends any enumeration value to the exception's description.
 *
 * \param exception Exception to modify.
 * \param value Enumeration value to append.
 *
 * \return Reference to the exception to allow operator chaining.
 */
template <typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
CppRuntimeException& operator<<(CppRuntimeException& exception, T value)
{
    exception << enumToString(value);
    return exception;
}

} // namespace zserio

#endif // ZSERIO_ENUMS_H_INC
