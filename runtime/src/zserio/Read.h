#ifndef ZSERIO_READ_H_INC
#define ZSERIO_READ_H_INC

#include "zserio/BitStreamReader.h"

namespace zserio
{

namespace detail
{

// logic for objects provided via specializations
template <typename T>
std::enable_if_t<!std::is_enum_v<T>> read(BitStreamReader& reader, T& value);

inline void read(BitStreamReader& reader, Bool& value)
{
    value = reader.readBool();
}

template <typename T, BitSize BIT_SIZE>
void read(BitStreamReader& reader, IntWrapper<T, BIT_SIZE>& value)
{
    // TODO[Mi-L@]: implement on the reader to get rid of the numBits check
    if constexpr (sizeof(T) <= 4)
    {
        if constexpr (std::is_signed_v<T>)
        {
            value = static_cast<T>(reader.readSignedBits32(BIT_SIZE));
        }
        else
        {
            value = static_cast<T>(reader.readUnsignedBits32(BIT_SIZE));
        }
    }
    else
    {
        if constexpr (std::is_signed_v<T>)
        {
            value = static_cast<T>(reader.readSignedBits64(BIT_SIZE));
        }
        else
        {
            value = static_cast<T>(reader.readUnsignedBits64(BIT_SIZE));
        }
    }
}

inline void read(BitStreamReader& reader, VarInt16& value)
{
    value = reader.readVarInt16();
}

inline void read(BitStreamReader& reader, VarInt32& value)
{
    value = reader.readVarInt32();
}

inline void read(BitStreamReader& reader, VarInt64& value)
{
    value = reader.readVarInt64();
}

inline void read(BitStreamReader& reader, VarInt& value)
{
    value = reader.readVarInt();
}

inline void read(BitStreamReader& reader, VarUInt16& value)
{
    value = reader.readVarUInt16();
}

inline void read(BitStreamReader& reader, VarUInt32& value)
{
    value = reader.readVarUInt32();
}

inline void read(BitStreamReader& reader, VarUInt64& value)
{
    value = reader.readVarUInt64();
}

inline void read(BitStreamReader& reader, VarUInt& value)
{
    value = reader.readVarUInt();
}

inline void read(BitStreamReader& reader, VarSize& value)
{
    value = reader.readVarSize();
}

inline void read(BitStreamReader& reader, Float16& value)
{
    value = reader.readFloat16();
}

inline void read(BitStreamReader& reader, Float32& value)
{
    value = reader.readFloat32();
}

inline void read(BitStreamReader& reader, Float64& value)
{
    value = reader.readFloat64();
}

template <typename ALLOC>
inline void read(BitStreamReader& reader, std::vector<uint8_t, ALLOC>& value)
{
    value = reader.readBytes(value.get_allocator());
}

template <typename ALLOC>
inline void read(BitStreamReader& reader, std::basic_string<char, std::char_traits<char>, ALLOC>& value)
{
    value = reader.readString(value.get_allocator());
}

template <typename ALLOC>
inline void read(BitStreamReader& reader, BasicBitBuffer<ALLOC>& value)
{
    value = reader.readBitBuffer(value.get_allocator());
}

} // namespace detail

} // namespace zserio

#endif // ZSERIO_READ_H_INC
