#include <array>

#include "zserio/Types.h"

namespace zserio
{

namespace detail
{

static const std::array<uint64_t, 2> VARINT16_MAX_VALUES = {
        (UINT64_C(1) << (6U)) - 1,
        (UINT64_C(1) << (6U + 8U)) - 1,
};

static const std::array<uint64_t, 4> VARINT32_MAX_VALUES = {
        (UINT64_C(1) << (6U)) - 1,
        (UINT64_C(1) << (6U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 8U)) - 1,
};

static const std::array<uint64_t, 8> VARINT64_MAX_VALUES = {
        (UINT64_C(1) << (6U)) - 1,
        (UINT64_C(1) << (6U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U + 7U + 7U + 8U)) - 1,
};

static const std::array<uint64_t, 2> VARUINT16_MAX_VALUES = {
        (UINT64_C(1) << (7U)) - 1,
        (UINT64_C(1) << (7U + 8U)) - 1,
};

static const std::array<uint64_t, 4> VARUINT32_MAX_VALUES = {
        (UINT64_C(1) << (7U)) - 1,
        (UINT64_C(1) << (7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 8U)) - 1,
};

static const std::array<uint64_t, 8> VARUINT64_MAX_VALUES = {
        (UINT64_C(1) << (7U)) - 1,
        (UINT64_C(1) << (7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U + 7U + 7U + 8)) - 1,
};

static const std::array<uint64_t, 9> VARINT_MAX_VALUES = {
        (UINT64_C(1) << (6U)) - 1,
        (UINT64_C(1) << (6U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (6U + 7U + 7U + 7U + 7U + 7U + 7U + 7U + 8)) - 1,
};

static const std::array<uint64_t, 9> VARUINT_MAX_VALUES = {
        (UINT64_C(1) << (7U)) - 1,
        (UINT64_C(1) << (7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U + 7U + 7U + 7U + 7U)) - 1,
        UINT64_MAX,
};

static const std::array<uint64_t, 5> VARSIZE_MAX_VALUES = {
        (UINT64_C(1) << (7U)) - 1,
        (UINT64_C(1) << (7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (7U + 7U + 7U + 7U)) - 1,
        (UINT64_C(1) << (2U + 7U + 7U + 7U + 8U)) - 1,
};

template <std::size_t SIZE>
static BitSize bitSizeOfVarIntImpl(
        uint64_t value, const std::array<uint64_t, SIZE>& maxValues, const char* varIntName)
{
    BitSize byteSize = 1;
    for (uint64_t maxValue : maxValues)
    {
        if (value <= maxValue)
        {
            break;
        }
        byteSize++;
    }

    if (byteSize > maxValues.size())
    {
        throw CppRuntimeException("BitSizeOfCalculator: Value '")
                << value << "' is out of range for " << varIntName << "!";
    }

    return byteSize * 8;
}

template <typename T>
static uint64_t convertToAbsValue(typename T::ValueType value)
{
    return static_cast<uint64_t>((value < 0) ? -value : value);
}

BitSize bitSizeOf(VarInt16 value, BitSize)
{
    return bitSizeOfVarIntImpl(convertToAbsValue<VarInt16>(value), VARINT16_MAX_VALUES, "varint16");
}

BitSize bitSizeOf(VarInt32 value, BitSize)
{
    return bitSizeOfVarIntImpl(convertToAbsValue<VarInt32>(value), VARINT32_MAX_VALUES, "varint32");
}

BitSize bitSizeOf(VarInt64 value, BitSize)
{
    return bitSizeOfVarIntImpl(convertToAbsValue<VarInt64>(value), VARINT64_MAX_VALUES, "varint64");
}

BitSize bitSizeOf(VarUInt16 value, BitSize)
{
    return bitSizeOfVarIntImpl(value, VARUINT16_MAX_VALUES, "varuint16");
}

BitSize bitSizeOf(VarUInt32 value, BitSize)
{
    return bitSizeOfVarIntImpl(value, VARUINT32_MAX_VALUES, "varuint32");
}

BitSize bitSizeOf(VarUInt64 value, BitSize)
{
    return bitSizeOfVarIntImpl(value, VARUINT64_MAX_VALUES, "varuint64");
}

BitSize bitSizeOf(VarInt value, BitSize)
{
    if (value == INT64_MIN)
    {
        return 8; // INT64_MIN is stored as -0
    }

    return bitSizeOfVarIntImpl(convertToAbsValue<VarInt>(value), VARINT_MAX_VALUES, "varint");
}

BitSize bitSizeOf(VarUInt value, BitSize)
{
    return bitSizeOfVarIntImpl(value, VARUINT_MAX_VALUES, "varuint");
}

BitSize bitSizeOf(VarSize value, BitSize)
{
    return bitSizeOfVarIntImpl(value, VARSIZE_MAX_VALUES, "varsize");
}

BitSize initializeOffsets(VarInt16 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarInt32 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarInt64 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarUInt16 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarUInt32 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarUInt64 value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarInt value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarUInt value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

BitSize initializeOffsets(VarSize value, BitSize bitPosition)
{
    return bitSizeOf(value, bitPosition);
}

} // namespace detail

} // namespace zserio
