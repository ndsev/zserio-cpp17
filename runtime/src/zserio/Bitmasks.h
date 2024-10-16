#ifndef ZSERIO_BITMASKS_H_INC
#define ZSERIO_BITMASKS_H_INC

#include <type_traits>

#include "zserio/BitSize.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

namespace zserio
{
namespace detail
{

template <typename T>
std::enable_if_t<is_bitmask_v<T>, BitSize> bitSizeOf(T value)
{
    return bitSizeOf(value.getValue());
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

} // namespace detail
} // namespace zserio

#endif // ZSERIO_BITMASKS_H_INC
