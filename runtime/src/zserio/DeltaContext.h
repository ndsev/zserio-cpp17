#ifndef ZSERIO_DELTA_CONTEXT_H_INC
#define ZSERIO_DELTA_CONTEXT_H_INC

#include <cstdint>
#include <type_traits>

#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Traits.h"
#include "zserio/View.h"

namespace zserio
{

namespace detail
{

// calculates bit length on delta provided as an absolute number
inline uint8_t absDeltaBitLength(uint64_t absDelta)
{
    uint8_t result = 0;
    while (absDelta > 0)
    {
        result++;
        absDelta >>= 1U;
    }

    return result;
}

// calculates bit length, emulates Python bit_length to keep same logic
template <typename T>
uint8_t calcBitLength(T lhs, T rhs)
{
    const uint64_t absDelta = lhs > rhs
            ? static_cast<uint64_t>(lhs) - static_cast<uint64_t>(rhs)
            : static_cast<uint64_t>(rhs) - static_cast<uint64_t>(lhs);

    return absDeltaBitLength(absDelta);
}

// calculates delta, doesn't check for possible int64_t overflow since it's used only in cases where it's
// already known that overflow cannot occur
template <typename T>
int64_t calcUncheckedDelta(T lhs, uint64_t rhs)
{
    return static_cast<int64_t>(static_cast<uint64_t>(lhs) - rhs);
}

} // namespace detail

/**
 * Context for delta packing created for each packable field.
 *
 * Contexts are always newly created for each array operation (bitSizeOfPacked, readPacked, writePacked).
 * They must be initialized at first via calling the init method for each packable element present in the array.
 * After the full initialization, only a single method (bitSizeOf, read, write) can be repeatedly called for
 * exactly the same sequence of packable elements.
 */
class DeltaContext
{
public:
    /**
     * Method generated by default.
     * \{
     */
    DeltaContext() = default;
    ~DeltaContext() = default;

    DeltaContext(DeltaContext&& other) = default;
    DeltaContext& operator=(DeltaContext&& other) = default;
    DeltaContext(const DeltaContext& other) = default;
    DeltaContext& operator=(const DeltaContext& other) = default;
    /**
     * \}
     */

    /**
     * Calls the initialization step for a single element.
     *
     * \param element Current element.
     */
    template <typename T>
    void init(T element)
    {
        m_numElements++;
        m_unpackedBitSize += detail::bitSizeOf(element);

        if (!isFlagSet(INIT_STARTED_FLAG))
        {
            setFlag(INIT_STARTED_FLAG);
            m_previousElement = static_cast<uint64_t>(element);
            m_firstElementBitSize = static_cast<uint8_t>(m_unpackedBitSize);
        }
        else
        {
            if (m_maxBitNumber <= MAX_BIT_NUMBER_LIMIT)
            {
                setFlag(IS_PACKED_FLAG);
                const auto previousElement = static_cast<typename T::ValueType>(m_previousElement);
                const uint8_t maxBitNumber =
                        detail::calcBitLength(static_cast<typename T::ValueType>(element), previousElement);
                if (maxBitNumber > m_maxBitNumber)
                {
                    m_maxBitNumber = maxBitNumber;
                    if (m_maxBitNumber > MAX_BIT_NUMBER_LIMIT)
                    {
                        resetFlag(IS_PACKED_FLAG);
                    }
                }
                m_previousElement = static_cast<uint64_t>(element);
            }
        }
    }

    /**
     * Returns length of the packed element stored in the bit stream in bits.
     *
     * \param element Value of the current element.
     *
     * \return Length of the packed element stored in the bit stream in bits.
     */
    template <typename T>
    BitSize bitSizeOf(T element)
    {
        if (!isFlagSet(PROCESSING_STARTED_FLAG))
        {
            setFlag(PROCESSING_STARTED_FLAG);
            finishInit();

            return bitSizeOfDescriptor() + detail::bitSizeOf(element);
        }
        else if (!isFlagSet(IS_PACKED_FLAG))
        {
            return detail::bitSizeOf(element);
        }
        else
        {
            return static_cast<BitSize>(m_maxBitNumber + (m_maxBitNumber > 0 ? 1 : 0));
        }
    }

    /**
     * Reads a packed element from the bit stream.
     *
     * \param reader Bit stream reader.
     * \param element Element to read.
     * \param args Arguments, currently it can be only uint8_t bitSize for dynamic bit fields.
     */
    template <typename T, typename... ARGS>
    void read(BitStreamReader& reader, T& element, ARGS&&... args)
    {
        if (!isFlagSet(PROCESSING_STARTED_FLAG))
        {
            setFlag(PROCESSING_STARTED_FLAG);
            readDescriptor(reader);

            readUnpacked(reader, element, std::forward<ARGS>(args)...);
        }
        else if (!isFlagSet(IS_PACKED_FLAG))
        {
            readUnpacked(reader, element, std::forward<ARGS>(args)...);
        }
        else
        {
            if (m_maxBitNumber > 0)
            {
                const int64_t delta = reader.readSignedBits64(m_maxBitNumber + 1);
                const T readElement =
                        static_cast<typename T::ValueType>(m_previousElement + static_cast<uint64_t>(delta));
                m_previousElement = static_cast<uint64_t>(readElement);
            }

            element = static_cast<typename T::ValueType>(m_previousElement);
        }
    }

    /**
     * Writes the packed element to the bit stream.
     *
     * \param writer Bit stream writer.
     * \param element Value of the current element.
     */
    template <typename T>
    void write(BitStreamWriter& writer, T element)
    {
        if (!isFlagSet(PROCESSING_STARTED_FLAG))
        {
            setFlag(PROCESSING_STARTED_FLAG);
            finishInit();
            writeDescriptor(writer);

            writeUnpacked(writer, element);
        }
        else if (!isFlagSet(IS_PACKED_FLAG))
        {
            writeUnpacked(writer, element);
        }
        else
        {
            if (m_maxBitNumber > 0)
            {
                // it's already checked in the init phase that the delta will fit into int64_t
                const int64_t delta = detail::calcUncheckedDelta(
                        static_cast<typename T::ValueType>(element), m_previousElement);
                writer.writeSignedBits64(delta, m_maxBitNumber + 1);
                m_previousElement = static_cast<uint64_t>(element);
            }
        }
    }

private:
    void finishInit()
    {
        if (isFlagSet(IS_PACKED_FLAG))
        {
            const BitSize deltaBitSize = static_cast<BitSize>(m_maxBitNumber + (m_maxBitNumber > 0 ? 1 : 0));
            const BitSize packedBitSizeWithDescriptor = 1 + MAX_BIT_NUMBER_BITS + // descriptor
                    m_firstElementBitSize + (m_numElements - 1) * deltaBitSize;
            const BitSize unpackedBitSizeWithDescriptor = static_cast<BitSize>(1 + m_unpackedBitSize);
            if (packedBitSizeWithDescriptor >= unpackedBitSizeWithDescriptor)
            {
                resetFlag(IS_PACKED_FLAG);
            }
        }
    }

    BitSize bitSizeOfDescriptor() const
    {
        if (isFlagSet(IS_PACKED_FLAG))
        {
            return 1 + MAX_BIT_NUMBER_BITS;
        }
        else
        {
            return 1;
        }
    }

    void readDescriptor(BitStreamReader& in)
    {
        if (in.readBool())
        {
            setFlag(IS_PACKED_FLAG);
            m_maxBitNumber = static_cast<uint8_t>(in.readUnsignedBits32(MAX_BIT_NUMBER_BITS));
        }
        else
        {
            resetFlag(IS_PACKED_FLAG);
        }
    }

    template <typename T, typename... ARGS>
    void readUnpacked(BitStreamReader& reader, T& element, ARGS&&... args)
    {
        detail::read(reader, element, std::forward<ARGS>(args)...);
        m_previousElement = static_cast<uint64_t>(element);
    }

    void writeDescriptor(BitStreamWriter& writer) const
    {
        const bool isPacked = isFlagSet(IS_PACKED_FLAG);
        writer.writeBool(isPacked);
        if (isPacked)
        {
            writer.writeUnsignedBits32(m_maxBitNumber, MAX_BIT_NUMBER_BITS);
        }
    }

    template <typename T>
    void writeUnpacked(BitStreamWriter& writer, T element)
    {
        m_previousElement = static_cast<uint64_t>(element);
        detail::write(writer, element);
    }

    void setFlag(uint8_t flagMask)
    {
        m_flags |= flagMask;
    }

    void resetFlag(uint8_t flagMask)
    {
        m_flags &= static_cast<uint8_t>(~flagMask);
    }

    bool isFlagSet(uint8_t flagMask) const
    {
        return ((m_flags & flagMask) != 0);
    }

    static const uint8_t MAX_BIT_NUMBER_BITS = 6;
    static const uint8_t MAX_BIT_NUMBER_LIMIT = 62;

    static const uint8_t INIT_STARTED_FLAG = 0x01;
    static const uint8_t IS_PACKED_FLAG = 0x02;
    static const uint8_t PROCESSING_STARTED_FLAG = 0x04;

    uint64_t m_previousElement = 0;
    uint8_t m_maxBitNumber = 0;
    uint8_t m_flags = 0x00;

    uint8_t m_firstElementBitSize = 0;
    uint32_t m_numElements = 0;
    size_t m_unpackedBitSize = 0;
};

namespace detail
{

template <typename T>
struct PackingContext;

// TODO[Mi-L@]: comments?!
template <typename T>
void initContext(PackingContext<T>& packingContext, const View<T>& view);

template <typename T>
BitSize bitSizeOf(PackingContext<T>& packingContext, const View<T>& view, BitSize bitPosition);

template <typename T>
void write(PackingContext<T>& packingContext, BitStreamWriter& writer, const View<T>& view);

template <typename T, typename... ARGS>
void read(PackingContext<T>& packingContext, BitStreamReader& reader, T& data, ARGS...);

inline void initContext(DeltaContext& deltaContext, Bool value)
{
    deltaContext.init(value);
}

inline BitSize bitSizeOf(DeltaContext& deltaContext, Bool value, BitSize = 0)
{
    return deltaContext.bitSizeOf(value);
}

inline void write(DeltaContext& deltaContext, BitStreamWriter& writer, Bool value)
{
    deltaContext.write(writer, value);
}

inline void read(DeltaContext& deltaContext, BitStreamReader& reader, Bool& value)
{
    deltaContext.read(reader, value);
}

template <typename T, BitSize BIT_SIZE>
void initContext(DeltaContext& deltaContext, IntWrapper<T, BIT_SIZE> value)
{
    static_assert(BIT_SIZE != 0, "Variable dynamic bit fields not allowed here!");
    deltaContext.init(value);
}

template <typename T, BitSize BIT_SIZE>
BitSize bitSizeOf(DeltaContext& deltaContext, IntWrapper<T, BIT_SIZE> value, BitSize = 0)
{
    static_assert(BIT_SIZE != 0, "Variable dynamic bit fields not allowed here!");
    return deltaContext.bitSizeOf(value);
}

template <typename T, BitSize BIT_SIZE>
void write(DeltaContext& deltaContext, BitStreamWriter& writer, IntWrapper<T, BIT_SIZE> value)
{
    static_assert(BIT_SIZE != 0, "Variable dynamic bit fields not allowed here!");
    deltaContext.write(writer, value);
}

template <typename T, BitSize BIT_SIZE>
void read(DeltaContext& deltaContext, BitStreamReader& reader, IntWrapper<T, BIT_SIZE>& value)
{
    static_assert(BIT_SIZE != 0, "Variable dynamic bit fields not allowed here!");
    deltaContext.read(reader, value);
}

template <typename T, VarIntType VAR_TYPE>
void initContext(DeltaContext& deltaContext, VarIntWrapper<T, VAR_TYPE> value)
{
    deltaContext.init(value);
}

template <typename T, VarIntType VAR_TYPE>
BitSize bitSizeOf(DeltaContext& deltaContext, VarIntWrapper<T, VAR_TYPE> value, BitSize = 0)
{
    return deltaContext.bitSizeOf(value);
}

template <typename T, VarIntType VAR_TYPE>
void write(DeltaContext& deltaContext, BitStreamWriter& writer, VarIntWrapper<T, VAR_TYPE> value)
{
    deltaContext.write(writer, value);
}

template <typename T, VarIntType VAR_TYPE>
void read(DeltaContext& deltaContext, BitStreamReader& reader, VarIntWrapper<T, VAR_TYPE>& value)
{
    deltaContext.read(reader, value);
}

template <typename T>
void initContext(DeltaContext& deltaContext, View<DynIntWrapper<T, 0>> view)
{
    deltaContext.init(view);
}

template <typename T>
BitSize bitSizeOf(DeltaContext& deltaContext, View<DynIntWrapper<T, 0>> view, BitSize = 0)
{
    return deltaContext.bitSizeOf(view);
}

template <typename T>
void write(DeltaContext& deltaContext, BitStreamWriter& writer, View<DynIntWrapper<T, 0>> view)
{
    deltaContext.write(writer, view);
}

template <typename T>
void read(DeltaContext& deltaContext, BitStreamReader& reader, DynIntWrapper<T, 0>& value, uint8_t bitSize)
{
    deltaContext.read(reader, value, bitSize);
}

} // namespace detail

} // namespace zserio

#endif // ZSERIO_DELTA_CONTEXT_H_INC
