#ifndef ZSERIO_BITMASKS_H_INC
#define ZSERIO_BITMASKS_H_INC

#include <string_view>
#include <type_traits>

#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/DeltaContext.h"
#include "zserio/ItemsAccessor.h"
#include "zserio/Traits.h"

namespace zserio
{

namespace detail
{

template <typename T>
std::enable_if_t<is_bitmask_v<T>> validate(T value, std::string_view fieldName)
{
    validate(value.getValue(), fieldName);
}

template <typename T>
std::enable_if_t<is_bitmask_v<T>, BitSize> bitSizeOf(T value, BitSize bitPosition = 0)
{
    return bitSizeOf(value.getValue(), bitPosition);
}

template <typename T>
std::enable_if_t<is_bitmask_v<T>> write(BitStreamWriter& writer, T value)
{
    write(writer, value.getValue());
}

template <typename T>
std::enable_if_t<is_bitmask_v<T>> read(BitStreamReader& reader, T& value)
{
    typename T::ZserioType rawValue;
    read(reader, rawValue);
    value = T(rawValue);
}

template <typename T>
std::enable_if_t<zserio::is_bitmask_v<T>> initContext(DeltaContext& deltaContext, T value)
{
    deltaContext.init(value.getValue());
}

template <typename T>
std::enable_if_t<zserio::is_bitmask_v<T>, BitSize> bitSizeOf(DeltaContext& deltaContext, T value, BitSize = 0)
{
    return deltaContext.bitSizeOf(value.getValue());
}

template <typename T>
std::enable_if_t<zserio::is_bitmask_v<T>> write(DeltaContext& deltaContext, BitStreamWriter& writer, T value)
{
    deltaContext.write(writer, value.getValue());
}

template <typename T>
std::enable_if_t<zserio::is_bitmask_v<T>> read(DeltaContext& deltaContext, BitStreamReader& reader, T& value)
{
    typename T::ZserioType rawValue;
    deltaContext.read(reader, rawValue);
    value = T(rawValue);
}

} // namespace detail

/**
 * Appends any bitmask value to the exception's description.
 *
 * \param exception Exception to modify.
 * \param value Bitmask value to append.
 *
 * \return Reference to the exception to allow operator chaining.
 */
template <typename T, typename std::enable_if<is_bitmask<T>::value, int>::type = 0>
CppRuntimeException& operator<<(CppRuntimeException& exception, T value)
{
    exception << value.getValue(); // note that toString() allocates and exception cannot allocate!
    return exception;
}

} // namespace zserio

#endif // ZSERIO_BITMASKS_H_INC
