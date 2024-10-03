#ifndef ZSERIO_WRITE_H_INC
#define ZSERIO_WRITE_H_INC

#include "zserio/BitStreamWriter.h"
#include "zserio/Traits.h"

namespace zserio
{

namespace detail
{

// logic for objects provided via specializations
template <typename T>
std::enable_if_t<!std::is_enum_v<T> && !is_bitmask_v<T>> write(BitStreamWriter& writer, const T& value);

inline void write(BitStreamWriter& writer, Bool value)
{
    writer.writeBool(value);
}

template <typename T, BitSize BIT_SIZE>
void write(BitStreamWriter& writer, IntWrapper<T, BIT_SIZE> value)
{
    if constexpr (sizeof(T) <= 4)
    {
        if constexpr (std::is_signed_v<T>)
        {
            writer.writeSignedBits32(value, BIT_SIZE);
        }
        else
        {
            writer.writeUnsignedBits32(value, BIT_SIZE);
        }
    }
    else
    {
        if constexpr (std::is_signed_v<T>)
        {
            writer.writeSignedBits64(value, BIT_SIZE);
        }
        else
        {
            writer.writeUnsignedBits64(value, BIT_SIZE);
        }
    }
}

inline void write(BitStreamWriter& writer, VarInt16 value)
{
    writer.writeVarInt16(value);
}

inline void write(BitStreamWriter& writer, VarInt32 value)
{
    writer.writeVarInt32(value);
}

inline void write(BitStreamWriter& writer, VarInt64 value)
{
    writer.writeVarInt64(value);
}

inline void write(BitStreamWriter& writer, VarInt value)
{
    writer.writeVarInt(value);
}

inline void write(BitStreamWriter& writer, VarUInt16 value)
{
    writer.writeVarUInt16(value);
}

inline void write(BitStreamWriter& writer, VarUInt32 value)
{
    writer.writeVarUInt32(value);
}

inline void write(BitStreamWriter& writer, VarUInt64 value)
{
    writer.writeVarUInt64(value);
}

inline void write(BitStreamWriter& writer, VarUInt value)
{
    writer.writeVarUInt(value);
}

inline void write(BitStreamWriter& writer, VarSize value)
{
    writer.writeVarSize(value);
}

inline void write(BitStreamWriter& writer, Float16 value)
{
    writer.writeFloat16(value);
}

inline void write(BitStreamWriter& writer, Float32 value)
{
    writer.writeFloat32(value);
}

inline void write(BitStreamWriter& writer, Float64 value)
{
    writer.writeFloat64(value);
}

template <typename ALLOC>
inline void write(BitStreamWriter& writer, const std::vector<uint8_t, ALLOC>& value)
{
    writer.writeBytes(value);
}

template <typename ALLOC>
inline void write(BitStreamWriter& writer, const std::basic_string<char, std::char_traits<char>, ALLOC>& value)
{
    writer.writeString(value);
}

template <typename ALLOC>
inline void write(BitStreamWriter& writer, const BasicBitBuffer<ALLOC>& value)
{
    writer.writeBitBuffer(value);
}

} // namespace detail

} // namespace zserio

#endif // ZSERIO_WRITE_H_INC
