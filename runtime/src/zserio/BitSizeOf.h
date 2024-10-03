#ifndef ZSERIO_BITSIZEOF_H_INC
#define ZSERIO_BITSIZEOF_H_INC

#include "zserio/BitBuffer.h"
#include "zserio/BitSize.h"
#include "zserio/Traits.h"
#include "zserio/Types.h"

namespace zserio
{

namespace detail
{

// logic for objects provided via specializations
template <typename T>
std::enable_if_t<!std::is_enum_v<T> && !is_bitmask_v<T>, BitSize> bitSizeOf(T value);

inline BitSize bitSizeOf(BoolWrapper)
{
    return 1;
}

template <typename T, BitSize BIT_SIZE>
BitSize bitSizeOf(IntWrapper<T, BIT_SIZE>)
{
    return BIT_SIZE;
}

BitSize bitSizeOf(VarInt16 value);

BitSize bitSizeOf(VarInt32 value);

BitSize bitSizeOf(VarInt64 value);

BitSize bitSizeOf(VarUInt16 value);

BitSize bitSizeOf(VarUInt32 value);

BitSize bitSizeOf(VarUInt64 value);

BitSize bitSizeOf(VarInt value);

BitSize bitSizeOf(VarUInt value);

BitSize bitSizeOf(VarSize value);

inline BitSize bitSizeOf(Float16)
{
    return 16;
}

inline BitSize bitSizeOf(Float32)
{
    return 32;
}

inline BitSize bitSizeOf(Float64)
{
    return 64;
}

BitSize bitSizeOf(Span<const uint8_t> bytesValue);

BitSize bitSizeOf(std::string_view stringValue);

template <typename ALLOC>
BitSize bitSizeOf(const std::basic_string<char, std::char_traits<char>, ALLOC>& value)
{
    return bitSizeOf(std::string_view{value});
}

template <typename ALLOC>
BitSize bitSizeOf(const BasicBitBuffer<ALLOC>& bitBuffer)
{
    const VarSize bitBufferSize =
            fromCheckedValue<::zserio::VarSize>(convertSizeToUInt32(bitBuffer.getBitSize()));

    // bit buffer consists of varsize for bit size followed by the bits
    return bitSizeOf(bitBufferSize) + bitBufferSize;
}

} // namespace detail

} // namespace zserio

#endif // ifndef ZSERIO_BITSIZEOF_H_INC
