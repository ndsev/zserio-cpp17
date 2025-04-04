#ifndef ZSERIO_BIT_BUFFER_H_INC
#define ZSERIO_BIT_BUFFER_H_INC

#include <cstddef>
#include <cstring>
#include <functional>
#include <string_view>
#include <type_traits>
#include <vector>

#include "zserio/BitSize.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/HashCodeUtil.h"
#include "zserio/SizeConvertUtil.h"
#include "zserio/Span.h"

namespace zserio
{

/**
 * Bits helper structure used as a tag in BitStreamReader and BitStreamWriter constructor to pass number of bits
 * instead of number of bytes.
 */
struct BitsTag
{};

/**
 * Class which holds any bit sequence.
 *
 * Because bit buffer size does not have to be byte aligned (divisible by 8), it's possible that not all bits
 * of the last byte are used. In this case, only most significant bits of the corresponded size are used.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class BasicBitBuffer
{
public:
    static_assert(std::is_same<uint8_t, typename ALLOC::value_type>::value,
            "Allocator with uint8_t value_type is required!");

    using allocator_type = ALLOC;

    /**
     * Get copy of the allocator used for dynamic memory allocations.
     *
     * \return Allocator used for dynamic memory allocations.
     */
    ALLOC get_allocator() const
    {
        return m_buffer.get_allocator();
    }

    /**
     * Empty constructor.
     *
     * \param allocator Allocator to use for internal vector allocation.
     */
    BasicBitBuffer();

    /**
     * Constructor from given allocator.
     *
     * \param allocator Allocator to use for internal vector allocation.
     */
    explicit BasicBitBuffer(const ALLOC& allocator);

    /**
     * Constructor from bit size.
     *
     * \param bitSize Size in bits of created bit buffer.
     * \param allocator Allocator to use for internal vector allocation.
     */
    explicit BasicBitBuffer(size_t bitSize, const ALLOC& allocator = {});

    /**
     * Constructor from span.
     *
     * \param buffer Span of bytes from which the bit buffer should be created.
     * \param allocator Allocator to use for internal vector allocation.
     */
    explicit BasicBitBuffer(Span<const uint8_t> buffer, const ALLOC& allocator = {});

    /**
     * Constructor from span and bit size.
     *
     * \param buffer Span of bytes from which the bit buffer should be created.
     * \param bitSize Number of bits stored in buffer to use.
     * \param allocator Allocator to use for internal vector allocation.
     *
     * \throw CppRuntimeException If given bit size is out of range for given Span.
     */
    explicit BasicBitBuffer(Span<const uint8_t> buffer, size_t bitSize, const ALLOC& allocator = {});

    /**
     * Constructor from moved STL vector.
     *
     * \param buffer STL vector of bytes from which the bit buffer should be created.
     */
    explicit BasicBitBuffer(std::vector<uint8_t, ALLOC>&& buffer);

    /**
     * Constructor from moved STL vector and bit size.
     *
     * \param buffer STL vector of bytes from which the bit buffer should be created.
     * \param bitSize Number of bits stored in buffer to use.
     *
     * \throw CppRuntimeException If given bit size is out of range for given vector.
     */
    explicit BasicBitBuffer(std::vector<uint8_t, ALLOC>&& buffer, size_t bitSize);

    /**
     * Method generated by default.
     * \{
     */
    ~BasicBitBuffer() = default;

    BasicBitBuffer(const BasicBitBuffer<ALLOC>&) = default;
    BasicBitBuffer(const BasicBitBuffer<ALLOC>& other, const ALLOC& allocator);
    BasicBitBuffer& operator=(const BasicBitBuffer<ALLOC>&) = default;

    BasicBitBuffer(BasicBitBuffer<ALLOC>&&) = default;
    BasicBitBuffer(const BasicBitBuffer<ALLOC>&& other, const ALLOC& allocator);
    BasicBitBuffer& operator=(BasicBitBuffer<ALLOC>&&) = default;
    /**
     * \}
     */

    /**
     * Equal operator.
     *
     * \param other The another instance of bit buffer to which compare this bit buffer.
     *
     * \return True when the bit buffers have same contents, false otherwise.
     */
    bool operator==(const BasicBitBuffer<ALLOC>& other) const;

    /**
     * Non-equal operator.
     *
     * \param other The another instance of bit buffer to which compare this bit buffer.
     *
     * \return True when the bit buffers have different contents, false otherwise.
     */
    bool operator!=(const BasicBitBuffer<ALLOC>& other) const
    {
        return !(*this == other);
    }

    /**
     * Operator less than.
     *
     * \param other The another instance of bit buffer to which compare this bit buffer.
     *
     * \return True when this bit buffer is less than the other (using lexicographical compare).
     */
    bool operator<(const BasicBitBuffer<ALLOC>& other) const;

    /**
     * Operator greater than.
     *
     * \param other The another instance of bit buffer to which compare this bit buffer.
     *
     * \return True when this bit buffer is greater than the other (using lexicographical compare).
     */
    bool operator>(const BasicBitBuffer<ALLOC>& other) const
    {
        return other < *this;
    }

    /**
     * Operator less than or equal.
     *
     * \param other The another instance of bit buffer to which compare this bit buffer.
     *
     * \return True when this bit buffer is less than or equal to the other (using lexicographical compare).
     */
    bool operator<=(const BasicBitBuffer<ALLOC>& other) const
    {
        return !(other < *this);
    }

    /**
     * Operator greater than or equal.
     *
     * \param other The another instance of bit buffer to which compare this bit buffer.
     *
     * \return True when this bit buffer is greater than or equal the other (using lexicographical compare).
     */
    bool operator>=(const BasicBitBuffer<ALLOC>& other) const
    {
        return !(*this < other);
    }

    /**
     * Calculates hash code of the bit buffer.
     *
     * \return Calculated hash code.
     */
    uint32_t hashCode() const;

    /**
     * Gets the underlying buffer.
     *
     * \return Pointer to the constant underlying buffer.
     */
    const uint8_t* getBuffer() const;

    /**
     * Gets the underlying buffer.
     *
     * \return Pointer to the underlying buffer.
     */
    uint8_t* getBuffer();

    /**
     * Gets the number of bits stored in the bit buffer.
     *
     * \return Bit buffer size in bits.
     */
    size_t getBitSize() const;

    /**
     * Gets the number of bytes stored in the bit buffer.
     *
     * Not all bits of the last byte must be used. Unused bits of the last byte are set to zero.
     *
     * \return Bit buffer size in bytes.
     */
    size_t getByteSize() const;

    /**
     * Convenience getter for the underlying buffer.
     *
     * \return Reference to the underlying vector of bytes.
     */
    const std::vector<uint8_t, ALLOC>& getBytes() const;

    /**
     * Convenience getter for the underlying buffer.
     *
     * \return The span to the underlying vector of bytes.
     */
    Span<const uint8_t> getData() const;

    /**
     * Convenience getter for the underlying buffer.
     *
     * \return The span to the underlying vector of bytes.
     */
    Span<uint8_t> getData();

private:
    uint8_t getMaskedLastByte() const;

    std::vector<uint8_t, ALLOC> m_buffer;
    size_t m_bitSize;
};

template <typename ALLOC>
BasicBitBuffer<ALLOC>::BasicBitBuffer() :
        m_buffer(ALLOC()),
        m_bitSize(0)
{}

template <typename ALLOC>
BasicBitBuffer<ALLOC>::BasicBitBuffer(const ALLOC& allocator) :
        m_buffer(allocator),
        m_bitSize(0)
{}

template <typename ALLOC>
BasicBitBuffer<ALLOC>::BasicBitBuffer(size_t bitSize, const ALLOC& allocator) :
        m_buffer((bitSize + 7) / 8, 0, allocator),
        m_bitSize(bitSize)
{}

template <typename ALLOC>
BasicBitBuffer<ALLOC>::BasicBitBuffer(Span<const uint8_t> buffer, const ALLOC& allocator) :
        m_buffer(buffer.begin(), buffer.end(), allocator),
        m_bitSize(8 * buffer.size())
{}

template <typename ALLOC>
BasicBitBuffer<ALLOC>::BasicBitBuffer(Span<const uint8_t> buffer, size_t bitSize, const ALLOC& allocator) :
        m_buffer(buffer.begin(), buffer.end(), allocator),
        m_bitSize(bitSize)
{
    const size_t byteSize = (bitSize + 7) / 8;
    if (buffer.size() < byteSize)
    {
        throw CppRuntimeException("BitBuffer: Bit size ")
                << bitSize << " out of range for given span byte size " << buffer.size() << "!";
    }
}

template <typename ALLOC>
BasicBitBuffer<ALLOC>::BasicBitBuffer(std::vector<uint8_t, ALLOC>&& buffer) :
        m_buffer(std::move(buffer)),
        m_bitSize(8 * m_buffer.size())
{}

template <typename ALLOC>
BasicBitBuffer<ALLOC>::BasicBitBuffer(std::vector<uint8_t, ALLOC>&& buffer, size_t bitSize) :
        m_buffer(std::move(buffer)),
        m_bitSize(bitSize)
{
    const size_t byteSize = (bitSize + 7) / 8;
    if (m_buffer.size() < byteSize)
    {
        throw CppRuntimeException("BitBuffer: Bit size ")
                << bitSize << " out of range for given vector byte size " << m_buffer.size() << "!";
    }
}

template <typename ALLOC>
inline BasicBitBuffer<ALLOC>::BasicBitBuffer(const BasicBitBuffer<ALLOC>& other, const ALLOC& allocator) :
        m_buffer(other.m_buffer, allocator),
        m_bitSize(other.m_bitSize)
{}

template <typename ALLOC>
inline BasicBitBuffer<ALLOC>::BasicBitBuffer(const BasicBitBuffer<ALLOC>&& other, const ALLOC& allocator) :
        m_buffer(std::move(other.m_buffer), allocator),
        m_bitSize(other.m_bitSize)
{}

template <typename ALLOC>
bool BasicBitBuffer<ALLOC>::operator==(const BasicBitBuffer<ALLOC>& other) const
{
    if (this != &other)
    {
        if (m_bitSize != other.m_bitSize)
        {
            return false;
        }

        const size_t byteSize = getByteSize();
        if (byteSize > 0)
        {
            if (byteSize > 1)
            {
                if (std::memcmp(getBuffer(), other.getBuffer(), byteSize - 1) != 0)
                {
                    return false;
                }
            }

            if (getMaskedLastByte() != other.getMaskedLastByte())
            {
                return false;
            }
        }
    }

    return true;
}

template <typename ALLOC>
bool BasicBitBuffer<ALLOC>::operator<(const BasicBitBuffer<ALLOC>& other) const
{
    const size_t byteSize1 = getByteSize();
    const size_t byteSize2 = other.getByteSize();

    if (byteSize1 == 0)
    {
        return byteSize2 != 0;
    }
    if (byteSize2 == 0)
    {
        return false;
    }

    using DifferenceType = typename std::vector<uint8_t, ALLOC>::iterator::difference_type;

    auto first1 = m_buffer.begin();
    const auto last1 = first1 + static_cast<DifferenceType>(byteSize1 - 1);
    auto first2 = other.m_buffer.begin();
    const auto last2 = first2 + static_cast<DifferenceType>(byteSize2 - 1);
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
    {
        if (*first1 < *first2)
        {
            return true;
        }
        if (*first2 < *first1)
        {
            return false;
        }
    }

    const auto lastValue1 = first1 != last1 ? *first1 : getMaskedLastByte();
    const auto lastValue2 = first2 != last2 ? *first2 : other.getMaskedLastByte();
    if (lastValue1 < lastValue2)
    {
        return true;
    }
    if (lastValue2 < lastValue1)
    {
        return false;
    }

    return (first1 == last1) && (first2 != last2);
}

template <typename ALLOC>
uint32_t BasicBitBuffer<ALLOC>::hashCode() const
{
    uint32_t result = HASH_SEED;
    const size_t byteSize = getByteSize();
    if (byteSize > 0)
    {
        if (byteSize > 1)
        {
            auto lastIt = m_buffer.begin() + static_cast<int>(byteSize) - 1;
            for (auto it = m_buffer.begin(); it != lastIt; ++it)
            {
                result = calcHashCode(result, *it);
            }
        }
        result = calcHashCode(result, getMaskedLastByte());
    }

    return result;
}

template <typename ALLOC>
const uint8_t* BasicBitBuffer<ALLOC>::getBuffer() const
{
    return m_buffer.data();
}

template <typename ALLOC>
uint8_t* BasicBitBuffer<ALLOC>::getBuffer()
{
    return m_buffer.data();
}

template <typename ALLOC>
size_t BasicBitBuffer<ALLOC>::getBitSize() const
{
    return m_bitSize;
}

template <typename ALLOC>
size_t BasicBitBuffer<ALLOC>::getByteSize() const
{
    return (m_bitSize + 7) / 8;
}

template <typename ALLOC>
const std::vector<uint8_t, ALLOC>& BasicBitBuffer<ALLOC>::getBytes() const
{
    return m_buffer;
}

template <typename ALLOC>
Span<const uint8_t> BasicBitBuffer<ALLOC>::getData() const
{
    return Span<const uint8_t>(m_buffer);
}

template <typename ALLOC>
Span<uint8_t> BasicBitBuffer<ALLOC>::getData()
{
    return Span<uint8_t>(m_buffer);
}

template <typename ALLOC>
uint8_t BasicBitBuffer<ALLOC>::getMaskedLastByte() const
{
    const size_t roundedByteSize = m_bitSize / 8;
    const uint8_t lastByteBits = static_cast<uint8_t>(m_bitSize - 8 * roundedByteSize);

    return (lastByteBits == 0)
            ? m_buffer[roundedByteSize - 1]
            : static_cast<uint8_t>(m_buffer[roundedByteSize] & (0xFFU << (8U - lastByteBits)));
}

/** Typedef to BitBuffer provided for convenience - using std::allocator<uint8_t>. */
using BitBuffer = BasicBitBuffer<>;

/** BitBufferView which is just a reference_wrapper for simplicity. */
template <typename ALLOC>
using BasicBitBufferView = std::reference_wrapper<const BasicBitBuffer<ALLOC>>;

/** Typedef to BitBufferView provided for convenience - using std::allocator<uint8_t>. */
using BitBufferView = std::reference_wrapper<const BitBuffer>;

/** Comparison operator for BitBufferView. */
/** \{ */

template <typename ALLOC>
bool operator==(const BasicBitBufferView<ALLOC>& lhs, const BasicBitBufferView<ALLOC>& rhs)
{
    return lhs.get() == rhs.get();
}

template <typename ALLOC>
bool operator!=(const BasicBitBufferView<ALLOC>& lhs, const BasicBitBufferView<ALLOC>& rhs)
{
    return lhs.get() != rhs.get();
}

template <typename ALLOC>
bool operator<(const BasicBitBufferView<ALLOC>& lhs, const BasicBitBufferView<ALLOC>& rhs)
{
    return lhs.get() < rhs.get();
}

template <typename ALLOC>
bool operator>(const BasicBitBufferView<ALLOC>& lhs, const BasicBitBufferView<ALLOC>& rhs)
{
    return lhs.get() > rhs.get();
}

template <typename ALLOC>
bool operator<=(const BasicBitBufferView<ALLOC>& lhs, const BasicBitBufferView<ALLOC>& rhs)
{
    return lhs.get() <= rhs.get();
}

template <typename ALLOC>
bool operator>=(const BasicBitBufferView<ALLOC>& lhs, const BasicBitBufferView<ALLOC>& rhs)
{
    return lhs.get() >= rhs.get();
}

/** \} */

/**
 * Allows to append BitBuffer to CppRuntimeException.
 *
 * \param exception Exception to modify.
 * \param bitBuffer Bit buffer value.
 *
 * \return Reference to the exception to allow operator chaining.
 */
template <typename ALLOC>
CppRuntimeException& operator<<(CppRuntimeException& exception, const BasicBitBuffer<ALLOC>& bitBuffer)
{
    return exception << "BitBuffer([...], " << bitBuffer.getBitSize() << ")";
}

/**
 * Calculates hash code of the given BitBuffer value using the given seed value.
 *
 * \param seedValue Seed value (current hash code).
 * \param value Value for which to calculate the hash code.
 *
 * \return Calculated hash code.
 */
template <typename ALLOC>
uint32_t calcHashCode(uint32_t seedValue, const BasicBitBuffer<ALLOC>& value)
{
    return calcHashCode(seedValue, std::hash<BasicBitBuffer<ALLOC>>{}(value));
}

namespace detail
{

template <typename ALLOC>
void validate(const BasicBitBufferView<ALLOC>&, std::string_view)
{
    // always valid
}

template <typename ALLOC>
BitSize bitSizeOf(const BasicBitBufferView<ALLOC>& bitBufferView, BitSize = 0)
{
    const BasicBitBuffer<ALLOC>& bitBuffer = bitBufferView.get();
    const VarSize bitBufferSize = fromCheckedValue<VarSize>(convertSizeToUInt32(bitBuffer.getBitSize()));

    // bit buffer consists of varsize for bit size followed by the bits
    return bitSizeOf(bitBufferSize) + bitBufferSize;
}

} // namespace detail

} // namespace zserio

namespace std
{

/**
 * Specialization of std::hash.
 */
template <typename ALLOC>
struct hash<zserio::BasicBitBuffer<ALLOC>>
{
    size_t operator()(const zserio::BasicBitBuffer<ALLOC>& bitBuffer) const
    {
        return static_cast<size_t>(bitBuffer.hashCode());
    }
};

/**
 * Specialization of std::hash.
 */
template <typename ALLOC>
struct hash<zserio::BasicBitBufferView<ALLOC>>
{
    size_t operator()(const zserio::BasicBitBufferView<ALLOC>& bitBuffer) const
    {
        return static_cast<size_t>(bitBuffer.get().hashCode());
    }
};

} // namespace std

#endif // ifndef ZSERIO_BIT_BUFFER_H_INC
