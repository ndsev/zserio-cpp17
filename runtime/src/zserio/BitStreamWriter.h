#ifndef ZSERIO_BIT_STREAM_WRITER_H_INC
#define ZSERIO_BIT_STREAM_WRITER_H_INC

#include <algorithm>
#include <cstddef>
#include <string_view>
#include <type_traits>

#include "zserio/BitBuffer.h"
#include "zserio/Bytes.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/SizeConvertUtil.h"
#include "zserio/String.h"
#include "zserio/Types.h"

namespace zserio
{

/**
 * Writer class which allows to write various data to the bit stream.
 */
class BitStreamWriter
{
public:
    /** Exception throw in case of insufficient capacity of the given buffer. */
    class InsufficientCapacityException : public CppRuntimeException
    {
    public:
        using CppRuntimeException::CppRuntimeException;
    };

    /** Type for bit position. */
    using BitPosType = size_t;

    /**
     * Constructor from externally allocated byte buffer.
     *
     * \param buffer External byte buffer to create from.
     * \param bufferBitSize Size of the buffer in bits.
     */
    explicit BitStreamWriter(uint8_t* buffer, size_t bufferBitSize, BitsTag);

    /**
     * Constructor from externally allocated byte buffer.
     *
     * \param buffer External byte buffer to create from.
     * \param bufferByteSize Size of the buffer in bytes.
     */
    explicit BitStreamWriter(uint8_t* buffer, size_t bufferByteSize);

    /**
     * Constructor from externally allocated byte buffer.
     *
     * \param buffer External buffer to create from as a Span.
     */
    explicit BitStreamWriter(Span<uint8_t> buffer);

    /**
     * Constructor from externally allocated byte buffer with exact bit size.
     *
     * \param buffer External buffer to create from as a Span.
     * \param bufferBitSize Size of the buffer in bits.
     */
    explicit BitStreamWriter(Span<uint8_t> buffer, size_t bufferBitSize);

    /**
     * Constructor from externally allocated bit buffer.
     *
     * \param bitBuffer External bit buffer to create from.
     */
    template <typename ALLOC>
    explicit BitStreamWriter(BasicBitBuffer<ALLOC>& bitBuffer) :
            BitStreamWriter(bitBuffer.getData(), bitBuffer.getBitSize())
    {}

    /**
     * Destructor.
     */
    ~BitStreamWriter() = default;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    BitStreamWriter(const BitStreamWriter&) = delete;
    BitStreamWriter& operator=(const BitStreamWriter&) = delete;

    BitStreamWriter(const BitStreamWriter&&) = delete;
    BitStreamWriter& operator=(BitStreamWriter&&) = delete;
    /**
     * \}
     */

    /**
     * Writes unsigned bits up to 32 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeUnsignedBits32(uint32_t data, uint8_t numBits = 32);

    /**
     * Writes unsigned bits up to 64 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeUnsignedBits64(uint64_t data, uint8_t numBits = 64);

    /**
     * Writes signed bits up to 32 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeSignedBits32(int32_t data, uint8_t numBits = 32);

    /**
     * Writes signed bits up to 64 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeSignedBits64(int64_t data, uint8_t numBits = 64);

    /**
     * Writes bool as a single bit.
     *
     * \param data Bool to write.
     */
    void writeBool(Bool data);

    /**
     * Writes signed variable integer up to 16 bits.
     *
     * \param data Varint16 to write.
     */
    void writeVarInt16(VarInt16 data);

    /**
     * Writes signed variable integer up to 32 bits.
     *
     * \param data Varint32 to write.
     */
    void writeVarInt32(VarInt32 data);

    /**
     * Writes signed variable integer up to 64 bits.
     *
     * \param data Varint64 to write.
     */
    void writeVarInt64(VarInt64 data);

    /**
     * Writes signed variable integer up to 72 bits.
     *
     * \param data Varuint64 to write.
     */
    void writeVarInt(VarInt data);

    /**
     * Writes unsigned variable integer up to 16 bits.
     *
     * \param data Varuint16 to write.
     */
    void writeVarUInt16(VarUInt16 data);

    /**
     * Writes unsigned variable integer up to 32 bits.
     *
     * \param data Varuint32 to write.
     */
    void writeVarUInt32(VarUInt32 data);

    /**
     * Writes unsigned variable integer up to 64 bits.
     *
     * \param data Varuint64 to write.
     */
    void writeVarUInt64(VarUInt64 data);

    /**
     * Writes signed variable integer up to 72 bits.
     *
     * \param data Varuint64 to write.
     */
    void writeVarUInt(VarUInt data);

    /**
     * Writes variable size integer up to 40 bits.
     *
     * \param data Varsize to write.
     */
    void writeVarSize(VarSize data);

    /**
     * Writes 16-bit float.
     *
     * \param data Float16 to write.
     */
    void writeFloat16(Float16 data);

    /**
     * Writes 32-bit float.
     *
     * \param data Float32 to write.
     */
    void writeFloat32(Float32 data);

    /**
     * Writes 64-bit float.
     *
     * \param data Float64 to write.
     */
    void writeFloat64(Float64 data);

    /**
     * Writes bytes.
     *
     * \param data Bytes to write.
     */
    void writeBytes(BytesView data);

    /**
     * Writes UTF-8 string.
     *
     * \param data String view to write.
     */
    void writeString(std::string_view data);

    /**
     * Writes bit buffer.
     *
     * \param bitBuffer Bit buffer to write.
     */
    template <typename ALLOC>
    void writeBitBuffer(const BasicBitBuffer<ALLOC>& bitBuffer)
    {
        const VarSize bitSize = fromCheckedValue<VarSize>(convertSizeToUInt32(bitBuffer.getBitSize()));
        writeVarSize(bitSize);

        Span<const uint8_t> buffer = bitBuffer.getData();
        size_t numBytesToWrite = bitSize / 8;
        const uint8_t numRestBits = static_cast<uint8_t>(bitSize - numBytesToWrite * 8);
        const BitPosType beginBitPosition = getBitPosition();
        const Span<const uint8_t>::iterator itEnd = buffer.begin() + numBytesToWrite;
        if ((beginBitPosition & 0x07U) != 0)
        {
            // we are not aligned to byte
            for (Span<const uint8_t>::iterator it = buffer.begin(); it != itEnd; ++it)
            {
                writeUnsignedBits32Impl(*it, 8);
            }
        }
        else
        {
            // we are aligned to byte
            setBitPosition(beginBitPosition + numBytesToWrite * 8);
            if (hasWriteBuffer())
            {
                (void)std::copy(buffer.begin(), buffer.begin() + numBytesToWrite,
                        m_buffer.data() + beginBitPosition / 8);
            }
        }

        if (numRestBits > 0)
        {
            writeUnsignedBits32Impl(static_cast<uint32_t>(*itEnd) >> (8U - numRestBits), numRestBits);
        }
    }

    /**
     * Gets current bit position.
     *
     * \return Current bit position.
     */
    BitPosType getBitPosition() const
    {
        return m_bitIndex;
    }

    /**
     * Sets current bit position. Use with caution!
     *
     * \param position New bit position.
     */
    void setBitPosition(BitPosType position);

    /**
     * Moves current bit position to perform the requested bit alignment.
     *
     * \param alignment Size of the alignment in bits.
     */
    void alignTo(size_t alignment);

    /**
     * Gets whether the writer has assigned a write buffer.
     *
     * \return True when a buffer is assigned. False otherwise.
     */
    bool hasWriteBuffer() const
    {
        return m_buffer.data() != nullptr;
    }

    /**
     * Gets the write buffer.
     *
     * \return Pointer to the beginning of write buffer.
     */
    const uint8_t* getWriteBuffer() const;

    /**
     * Gets the write buffer as span.
     *
     * \return Span which represents the write buffer.
     */
    Span<const uint8_t> getBuffer() const;

    /**
     * Gets size of the underlying buffer in bits.
     *
     * \return Buffer bit size.
     */
    size_t getBufferBitSize() const
    {
        return m_bufferBitSize;
    }

private:
    void writeUnsignedBits32Impl(uint32_t data, uint8_t numBits);
    void writeUnsignedBits64Impl(uint64_t data, uint8_t numBits);
    void writeSignedVarNum(int64_t value, size_t maxVarBytes, size_t numVarBytes);
    void writeUnsignedVarNum(uint64_t value, size_t maxVarBytes, size_t numVarBytes);
    void writeVarNum(uint64_t value, bool hasSign, bool isNegative, size_t maxVarBytes, size_t numVarBytes);

    void checkCapacity(size_t bitSize) const;
    void throwInsufficientCapacityException() const;

    Span<uint8_t> m_buffer;
    size_t m_bitIndex;
    size_t m_bufferBitSize;
};

namespace detail
{

inline void write(BitStreamWriter& writer, Bool value)
{
    writer.writeBool(value);
}

template <BitSize BIT_SIZE, bool IS_SIGNED>
void write(BitStreamWriter& writer, FixedIntWrapper<BIT_SIZE, IS_SIGNED> value)
{
    using ValueType = typename FixedIntWrapper<BIT_SIZE, IS_SIGNED>::ValueType;

    static_assert(BIT_SIZE != 0, "Variable dynamic bit fields not allowed here!");
    if constexpr (sizeof(ValueType) <= 4)
    {
        if constexpr (std::is_signed_v<ValueType>)
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
        if constexpr (std::is_signed_v<ValueType>)
        {
            writer.writeSignedBits64(value, BIT_SIZE);
        }
        else
        {
            writer.writeUnsignedBits64(value, BIT_SIZE);
        }
    }
}

template <typename T>
void write(BitStreamWriter& writer, DynIntWrapper<T> value, uint8_t numBits)
{
    if constexpr (sizeof(T) <= 4)
    {
        if constexpr (std::is_signed_v<T>)
        {
            writer.writeSignedBits32(value, numBits);
        }
        else
        {
            writer.writeUnsignedBits32(value, numBits);
        }
    }
    else
    {
        if constexpr (std::is_signed_v<T>)
        {
            writer.writeSignedBits64(value, numBits);
        }
        else
        {
            writer.writeUnsignedBits64(value, numBits);
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

inline void write(BitStreamWriter& writer, BytesView value)
{
    writer.writeBytes(value);
}

inline void write(BitStreamWriter& writer, std::string_view value)
{
    writer.writeString(value);
}

template <typename ALLOC>
inline void write(BitStreamWriter& writer, const BasicBitBufferView<ALLOC>& value)
{
    writer.writeBitBuffer(value.get());
}

} // namespace detail

} // namespace zserio

#endif // ifndef ZSERIO_BIT_STREAM_WRITER_H_INC
