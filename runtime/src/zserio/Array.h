#ifndef ZSERIO_ARRAY_H_INC
#define ZSERIO_ARRAY_H_INC

#include <string_view>
#include <type_traits>

#include "zserio/ArrayLengthException.h"
#include "zserio/ArrayTraits.h"
#include "zserio/BitPositionUtil.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

namespace zserio
{

/**
 * Array type enum which defined type of the underlying array.
 */
enum ArrayType
{
    NORMAL, /**< Normal zserio array which has size defined by the Zserio schema. */
    IMPLICIT, /**< Implicit zserio array which size is defined by number of remaining bits in the bit stream. */
    ALIGNED, /**< Aligned zserio array which is normal zserio array with indexed offsets. */
    AUTO, /**< Auto zserio array which has size stored in a hidden field before the array. */
    ALIGNED_AUTO /**< Aligned auto zserio array which is auto zserio array with indexed offsets. */
};

/**
 * Array storage type. Normally all array view's are immutable except for arrays which are used as
 * offsets.
 */
enum ArrayStorage
{
    IMMUTABLE, /**< Immutable array. */
    MUTABLE /**< Mutable array, used only when the array is used for offsets. */
};

namespace detail
{

template <typename RAW_ARRAY, ArrayStorage ARRAY_STORAGE>
struct array_storage
{
    using type = const RAW_ARRAY&;
};

template <typename RAW_ARRAY>
struct array_storage<RAW_ARRAY, ArrayStorage::MUTABLE>
{
    using type = RAW_ARRAY&;
};

template <typename RAW_ARRAY, ArrayStorage ARRAY_STORAGE>
using array_storage_t = typename array_storage<RAW_ARRAY, ARRAY_STORAGE>::type;

} // namespace detail

template <ArrayType ARRAY_TYPE, typename = void>
class ArrayBase
{
public:
    static constexpr bool HAS_SCHEMA_SIZE = false;
};

template <ArrayType ARRAY_TYPE>
class ArrayBase<ARRAY_TYPE, std::enable_if_t<(ARRAY_TYPE == NORMAL || ARRAY_TYPE == ALIGNED)>>
{
public:
    explicit ArrayBase(size_t schemaSize) :
            m_schemaSize(schemaSize)
    {}

    size_t schemaSize() const
    {
        return m_schemaSize;
    }

    static constexpr bool HAS_SCHEMA_SIZE = true;

private:
    size_t m_schemaSize;
};

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE = ArrayStorage::IMMUTABLE,
        typename ARRAY_TRAITS = ArrayTraits<typename RAW_ARRAY::value_type>>
class Array : public ArrayBase<ARRAY_TYPE>
{
public:
    /** Array type. */
    static constexpr ArrayType TYPE = ARRAY_TYPE;

    /** Array storage type. */
    static constexpr ArrayStorage STORAGE = ARRAY_STORAGE;

    /** Typedef for the raw array. */
    using RawArray = RAW_ARRAY;

    /** Typedef for the value type. */
    using ValueType = typename RawArray::value_type;

    /** Typedef for allocator type. */
    using AllocatorType = typename RawArray::allocator_type;

    /** Typedef for the array traits. */
    using Traits = ARRAY_TRAITS;

    /** Whether the array has defined size in the schema. */
    static constexpr bool HAS_SCHEMA_SIZE = ArrayBase<ARRAY_TYPE>::HAS_SCHEMA_SIZE;

    /** Forward declaration of the Array const iterator. */
    class ConstIterator;

    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    /**
     * Typedef for the array's owner type.
     *
     * Owner type is needed for proper expressions evaluation. If no owner is needed for expressions evaluation,
     * detail::DummyArrayOwner is used.
     */
    using OwnerType = detail::array_owner_type_t<Traits>;

    /**
     * Constructor from l-value raw array.
     *
     * \param rawArray Raw array.
     */
    template <typename OWNER_TYPE_ = OwnerType,
            std::enable_if_t<detail::is_dummy_array_owner_v<OWNER_TYPE_> && !HAS_SCHEMA_SIZE, int> = 0>
    explicit Array(detail::array_storage_t<RawArray, STORAGE> rawArray) :
            m_rawArray(rawArray)
    {}

    /**
     * Constructor from l-value raw array.
     *
     * \param rawArray Raw array.
     * \param schemaSize Array size defined by the schema.
     */
    template <typename OWNER_TYPE_ = OwnerType,
            std::enable_if_t<detail::is_dummy_array_owner_v<OWNER_TYPE_> && HAS_SCHEMA_SIZE, int> = 0>
    explicit Array(detail::array_storage_t<RawArray, STORAGE> rawArray, size_t schemaSize) :
            ArrayBase<ARRAY_TYPE>(schemaSize),
            m_rawArray(rawArray)
    {}

    /**
     * Constructor from l-value raw array.
     *
     * \param rawArray Raw array.
     * \param owner View to the array's owner.
     */
    template <typename OWNER_TYPE_ = OwnerType,
            std::enable_if_t<!detail::is_dummy_array_owner_v<OWNER_TYPE_> && !HAS_SCHEMA_SIZE, int> = 0>
    explicit Array(detail::array_storage_t<RawArray, STORAGE> rawArray, const OwnerType& owner) :
            m_rawArray(rawArray),
            m_owner(owner)
    {}

    /**
     * Constructor from l-value raw array.
     *
     * \param rawArray Raw array.
     * \param owner View to the array's owner.
     * \param schemaSize Array size defined by the schema.
     */
    template <typename OWNER_TYPE_ = OwnerType,
            std::enable_if_t<!detail::is_dummy_array_owner_v<OWNER_TYPE_> && HAS_SCHEMA_SIZE, int> = 0>
    explicit Array(
            detail::array_storage_t<RawArray, STORAGE> rawArray, const OwnerType& owner, size_t schemaSize) :
            ArrayBase<ARRAY_TYPE>(schemaSize),
            m_rawArray(rawArray),
            m_owner(owner)
    {}

    /**
     * Method generated by default.
     *
     * \{
     */
    ~Array() = default;
    Array(const Array& other) = default;
    Array& operator=(const Array& other) = default;
    Array(Array&& other) = default;
    Array& operator=(Array&& other) = default;
    /**
     * \}
     */

    /**
     * Gets underlying raw data array.
     *
     * \return Reference to underlying data array.
     */
    /** \{ */
    const RawArray& zserioData() const
    {
        return m_rawArray;
    }

    template <ArrayStorage ARRAY_STORAGE_ = STORAGE>
    std::enable_if_t<ARRAY_STORAGE_ == ArrayStorage::MUTABLE, RawArray&> zserioData()
    {
        return m_rawArray;
    }
    /** \} */

    /**
     * Operator equality.
     *
     * \param other Array to compare.
     *
     * \return True when the underlying raw arrays have same contents, false otherwise.
     */
    bool operator==(const Array& other) const
    {
        using AtResult = decltype(std::declval<const Array&>().at(std::declval<size_t>()));
        if constexpr (std::is_same_v<View<ValueType>, AtResult>)
        {
            const size_t thisSize = size();
            const size_t otherSize = other.size();
            if (thisSize != otherSize)
            {
                return false;
            }

            for (size_t i = 0; i < thisSize; ++i)
            {
                if ((*this)[i] != other[i])
                {
                    return false;
                }
            }

            return true;
        }
        else
        {
            return m_rawArray == other.m_rawArray;
        }
    }

    /**
     * Operator less than.
     *
     * \param other Array to compare.
     *
     * \return True when this array is less than the other array, false otherwise.
     */
    bool operator<(const Array& other) const
    {
        using AtResult = decltype(std::declval<const Array&>().at(std::declval<size_t>()));
        if constexpr (std::is_same_v<View<ValueType>, AtResult>)
        {
            const size_t thisSize = size();
            const size_t otherSize = other.size();
            const size_t maxSize = std::max(thisSize, otherSize);

            for (size_t i = 0; i < maxSize; ++i)
            {
                if ((*this)[i] < other[i])
                {
                    return true;
                }
                if (other[i] < (*this)[i])
                {
                    return false;
                }
            }

            return thisSize < otherSize;
        }
        else
        {
            return m_rawArray < other.m_rawArray;
        }
    }

    /**
     * Operator inequality.
     *
     * \param other Array to compare.
     *
     * \return True when the arrays have different contents, false otherwise.
     */
    bool operator!=(const Array& other) const
    {
        return !operator==(other);
    }

    /**
     * Operator greater than.
     *
     * \param other Array to compare.
     *
     * \return True when this arrays is greater than the other array, false otherwise.
     */
    bool operator>(const Array& other) const
    {
        return other.operator<(*this);
    }

    /**
     * Operator less than or equal.
     *
     * \param other Array to compare.
     *
     * \return True when this arrays is less than or equal to the other array, false otherwise.
     */
    bool operator<=(const Array& other) const
    {
        return !other.operator<(*this);
    }

    /**
     * Operator greater than or equal.
     *
     * \param other Array to compare.
     *
     * \return True when this arrays is greater than or equal to the other array, false otherwise.
     */
    bool operator>=(const Array& other) const
    {
        return !operator<(other);
    }

    /**
     * Gets length of the array.
     *
     * \return Length of the array.
     */
    size_t size() const
    {
        return m_rawArray.size();
    }

    /**
     * Gets whether the array is empty.
     *
     * \return True when the array is empty, false otherwise.
     */
    bool empty() const
    {
        return m_rawArray.empty();
    }

    /**
     * Returns element view at the given index.
     *
     * \param index Element index.
     * \return View to the specified element.
     */
    decltype(auto) at(size_t index) const
    {
        if constexpr (detail::array_traits_has_at_v<Traits>)
        {
            return Traits::at(m_owner, m_rawArray.at(index), index);
        }
        else
        {
            return m_rawArray.at(index);
        }
    }

    /**
     * Returns element view at the given index.
     * Like STL vector's operator[] this is does not check the array size.
     *
     * \param index Element index.
     * \return View to the specified element.
     */
    decltype(auto) operator[](size_t index) const
    {
        if constexpr (detail::array_traits_has_at_v<Traits>)
        {
            return Traits::at(m_owner, m_rawArray[index], index);
        }
        else
        {
            return m_rawArray[index];
        }
    }

    /**
     * Returns element view for the first element in container.
     *
     * \return View to the first element.
     */
    decltype(auto) front() const
    {
        return at(0);
    }

    /**
     * Returns element view for the last element in container.
     *
     * \return View to the last element.
     */
    decltype(auto) back() const
    {
        return at(size() - 1);
    }

    /**
     * Returns an constant iterator to the beginning.
     *
     * \return Constant iterator to the beginning.
     */
    /** \{ */
    ConstIterator cbegin() const noexcept
    {
        return ConstIterator(this, 0);
    }

    ConstIterator begin() const noexcept
    {
        return cbegin();
    }
    /** \} */

    /**
     * Returns and constant iterator to the end.
     *
     * \return Constant iterator to the end.
     */
    /** \{ */
    ConstIterator cend() const noexcept
    {
        return ConstIterator(this, size());
    }

    ConstIterator end() const noexcept
    {
        return cend();
    }
    /** \} */

    /**
     * Returns an constant reverse iterator to the beginning of the reversed array.
     *
     * \return Constant reverse iterator to the beginning of the reversed array.
     */
    /** \{ */
    ConstReverseIterator crbegin() const noexcept
    {
        return ConstReverseIterator(end());
    }

    ConstReverseIterator rbegin() const noexcept
    {
        return crbegin();
    }
    /** \} */

    /**
     * Returns an constant reverse iterator to the end of the reversed array.
     *
     * \return Constant reverse iterator to the end of the reversed array.
     */
    /** \{ */
    ConstReverseIterator crend() const noexcept
    {
        return ConstReverseIterator(begin());
    }

    ConstReverseIterator rend() const noexcept
    {
        return crend();
    }
    /** \} */

    /**
     * Implementation of Array constant iterator.
     */
    class ConstIterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = decltype(std::declval<Array>().at(std::declval<size_t>()));
        using difference_type = std::ptrdiff_t;
        using pointer = void;
        using reference = value_type; // we always return by value!

        /** Helper needed to implement operator-> on the ConstIterator. */
        struct ArrowHelper
        {
            value_type* operator->()
            {
                return std::addressof(value);
            }

            value_type value;
        };

        ConstIterator(const Array* array, size_t index) :
                m_array(array),
                m_index(index)
        {}

        value_type operator*() const
        {
            return m_array->at(m_index);
        }

        ArrowHelper operator->() const
        {
            return ArrowHelper{m_array->at(m_index)};
        }

        value_type operator[](difference_type offset) const
        {
            return m_array->at(m_index + static_cast<size_t>(offset));
        }

        ConstIterator& operator++()
        {
            ++m_index;
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator tmp = *this;
            m_index++;
            return tmp;
        }

        ConstIterator& operator--()
        {
            --m_index;
            return *this;
        }

        ConstIterator operator--(int)
        {
            ConstIterator tmp = *this;
            m_index--;
            return tmp;
        }

        ConstIterator& operator+=(difference_type offset)
        {
            m_index += static_cast<size_t>(offset);
            return *this;
        }

        ConstIterator operator+(difference_type offset) const
        {
            return ConstIterator(m_array, m_index + static_cast<size_t>(offset));
        }

        friend ConstIterator operator+(difference_type offset, const ConstIterator& other)
        {
            return ConstIterator(other.m_array, other.m_index + static_cast<size_t>(offset));
        }

        ConstIterator& operator-=(difference_type offset)
        {
            m_index -= static_cast<size_t>(offset);
            return *this;
        }

        ConstIterator operator-(difference_type offset) const
        {
            return ConstIterator(m_array, m_index - static_cast<size_t>(offset));
        }

        difference_type operator-(const ConstIterator& other) const
        {
            return static_cast<difference_type>(m_index - other.m_index);
        }

        bool operator==(const ConstIterator& other) const
        {
            return m_index == other.m_index;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return !(*this == other);
        }

        bool operator<(const ConstIterator& other) const
        {
            if (m_index != other.m_index)
            {
                return m_index < other.m_index;
            }

            return false;
        }

        bool operator>(const ConstIterator& other) const
        {
            return other < *this;
        }

        bool operator<=(const ConstIterator& other) const
        {
            return !(other < *this);
        }

        bool operator>=(const ConstIterator& other) const
        {
            return !(*this < other);
        }

    private:
        const Array* m_array;
        size_t m_index;
    };

private:
    detail::array_storage_t<RawArray, STORAGE> m_rawArray;
    OwnerType m_owner; // view to owner type, parameters are copied by value and that is ok
};

namespace detail
{

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS>
void validate(
        const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array, std::string_view fieldName)
{
    if constexpr (Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>::HAS_SCHEMA_SIZE)
    {
        if (array.size() != array.schemaSize())
        {
            throw ArrayLengthException("Wrong array length for field '")
                    << fieldName << "' (" << array.size() << " != " << array.schemaSize() << ")!";
        }
    }

    validate(VarSize{convertSizeToUInt32(array.size())}, fieldName);

    for (size_t i = 0; i < array.size(); ++i)
    {
        validate(array[i], fieldName);
    }
}

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS>
BitSize bitSizeOf(
        const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array, BitSize bitPosition = 0)
{
    BitSize endBitPosition = bitPosition;

    if constexpr (ARRAY_TYPE == ArrayType::AUTO || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
    {
        endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(array.size())));
    }

    for (size_t i = 0; i < array.size(); ++i)
    {
        if constexpr (ARRAY_TYPE == ArrayType::ALIGNED || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
        {
            endBitPosition = alignTo(8, endBitPosition);
        }

        endBitPosition += bitSizeOf(array[i], endBitPosition);
    }

    return endBitPosition - bitPosition;
}

struct DummyOffsetSetter
{
    static void setOffset(size_t /*index*/, BitSize /*byteOffset*/)
    {}
};

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS,
        typename OFFSET_SETTER = DummyOffsetSetter>
BitSize initializeOffsets(const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array,
        BitSize bitPosition, const OFFSET_SETTER& offsetSetter = OFFSET_SETTER())
{
    using ValueType = typename RAW_ARRAY::value_type;
    using ArrayT = Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>;

    BitSize endBitPosition = bitPosition;

    if constexpr (ARRAY_TYPE == ArrayType::AUTO || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
    {
        endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(array.size())));
    }

    for (size_t i = 0; i < array.size(); ++i)
    {
        if constexpr (ARRAY_TYPE == ArrayType::ALIGNED || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
        {
            endBitPosition = alignTo(8, endBitPosition);
            offsetSetter.setOffset(i, endBitPosition / 8);
        }

        using AtResult = decltype(std::declval<const ArrayT&>().at(std::declval<size_t>()));
        if constexpr (std::is_same_v<View<ValueType>, AtResult>)
        {
            endBitPosition += initializeOffsets(array[i], endBitPosition);
        }
        else
        {
            endBitPosition += bitSizeOf(array[i], endBitPosition);
        }
    }

    return endBitPosition - bitPosition;
}

template <typename ARRAY>
size_t readArrayLength(BitStreamReader& reader, size_t arrayLength)
{
    if constexpr (ARRAY::TYPE != ArrayType::AUTO && ARRAY::TYPE != ArrayType::ALIGNED_AUTO &&
            ARRAY::TYPE != ArrayType::IMPLICIT)
    {
        return arrayLength;
    }
    else if constexpr (ARRAY::TYPE == ArrayType::AUTO || ARRAY::TYPE == ArrayType::ALIGNED_AUTO)
    {
        return reader.readVarSize();
    }
    else
    {
        using ArrayTraits = typename ARRAY::Traits;

        const size_t remainingBits = reader.getBufferBitSize() - reader.getBitPosition();
        return remainingBits / ArrayTraits::bitSizeOf();
    }
}

template <typename ARRAY>
void read(BitStreamReader& reader, typename ARRAY::RawArray& rawArray, typename ARRAY::OwnerType& owner,
        size_t arrayLength = 0)
{
    using ArrayTraits = typename ARRAY::Traits;

    const size_t readLength = readArrayLength<ARRAY>(reader, arrayLength);
    rawArray.clear();
    rawArray.reserve(readLength);
    for (size_t i = 0; i < readLength; ++i)
    {
        if constexpr (ARRAY::TYPE == ArrayType::ALIGNED || ARRAY::TYPE == ArrayType::ALIGNED_AUTO)
        {
            reader.alignTo(8);
        }

        if constexpr (has_allocator_v<typename ARRAY::ValueType>)
        {
            rawArray.emplace_back(rawArray.get_allocator());
        }
        else
        {
            rawArray.emplace_back();
        }
        ArrayTraits::read(reader, owner, rawArray.back(), i);
    }
}

template <typename ARRAY, std::enable_if_t<is_dummy_array_owner_v<typename ARRAY::OwnerType>, int> = 0>
void read(BitStreamReader& reader, typename ARRAY::RawArray& rawArray, size_t arrayLength = 0)
{
    DummyArrayOwner owner;
    read<ARRAY>(reader, rawArray, owner, arrayLength);
}

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS>
void write(BitStreamWriter& writer, const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array)
{
    if constexpr (ARRAY_TYPE == ArrayType::AUTO || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
    {
        write(writer, fromCheckedValue<VarSize>(convertSizeToUInt32(array.size())));
    }

    for (size_t i = 0; i < array.size(); ++i)
    {
        if constexpr (ARRAY_TYPE == ArrayType::ALIGNED || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
        {
            writer.alignTo(8);
        }

        write(writer, array[i]);
    }
}

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS>
BitSize bitSizeOfPacked(
        const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array, BitSize bitPosition = 0)
{
    static_assert(ARRAY_TYPE != ArrayType::IMPLICIT, "Implicit array cannot be packed!");

    BitSize endBitPosition = bitPosition;

    const size_t arrayLength = array.size();
    if constexpr (ARRAY_TYPE == ArrayType::AUTO || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
    {
        endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(arrayLength)));
    }

    if (arrayLength > 0)
    {
        detail::packing_context_type_t<typename RAW_ARRAY::value_type> context;

        for (size_t i = 0; i < arrayLength; ++i)
        {
            initContext(context, array[i]);
        }

        for (size_t i = 0; i < arrayLength; ++i)
        {
            if constexpr (ARRAY_TYPE == ArrayType::ALIGNED || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
            {
                endBitPosition = alignTo(8, endBitPosition);
            }

            endBitPosition += bitSizeOf(context, array[i], endBitPosition);
        }
    }

    return endBitPosition - bitPosition;
}

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS,
        typename OFFSET_SETTER = DummyOffsetSetter>
BitSize initializeOffsetsPacked(const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array,
        BitSize bitPosition, const OFFSET_SETTER& offsetInitializer = OFFSET_SETTER())
{
    static_assert(ARRAY_TYPE != ArrayType::IMPLICIT, "Implicit array cannot be packed!");

    using ValueType = typename RAW_ARRAY::value_type;
    using ArrayT = Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>;

    BitSize endBitPosition = bitPosition;

    const size_t arrayLength = array.size();
    if constexpr (ARRAY_TYPE == ArrayType::AUTO || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
    {
        endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(arrayLength)));
    }

    if (arrayLength > 0)
    {
        detail::packing_context_type_t<ValueType> context;

        for (size_t i = 0; i < arrayLength; ++i)
        {
            initContext(context, array[i]);
        }

        for (size_t i = 0; i < arrayLength; ++i)
        {
            if constexpr (ARRAY_TYPE == ArrayType::ALIGNED || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
            {
                endBitPosition = alignTo(8, endBitPosition);
                offsetInitializer.setOffset(i, endBitPosition / 8);
            }

            using AtResult = decltype(std::declval<const ArrayT&>().at(std::declval<size_t>()));
            if constexpr (std::is_same_v<View<ValueType>, AtResult>)
            {
                endBitPosition += initializeOffsets(context, array[i], endBitPosition);
            }
            else
            {
                endBitPosition += bitSizeOf(context, array[i], endBitPosition);
            }
        }
    }

    return endBitPosition - bitPosition;
}

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS>
void writePacked(
        BitStreamWriter& writer, const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array)
{
    static_assert(ARRAY_TYPE != ArrayType::IMPLICIT, "Implicit array cannot be packed!");

    const size_t arrayLength = array.size();
    if constexpr (ARRAY_TYPE == ArrayType::AUTO || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
    {
        write(writer, fromCheckedValue<VarSize>(convertSizeToUInt32(array.size())));
    }

    if (arrayLength > 0)
    {
        detail::packing_context_type_t<typename RAW_ARRAY::value_type> context;

        for (size_t i = 0; i < arrayLength; ++i)
        {
            initContext(context, array[i]);
        }

        for (size_t i = 0; i < arrayLength; ++i)
        {
            if constexpr (ARRAY_TYPE == ArrayType::ALIGNED || ARRAY_TYPE == ArrayType::ALIGNED_AUTO)
            {
                writer.alignTo(8);
            }

            write(context, writer, array[i]);
        }
    }
}

template <typename ARRAY>
void readPacked(BitStreamReader& reader, typename ARRAY::RawArray& rawArray, typename ARRAY::OwnerType& owner,
        size_t arrayLength = 0)
{
    using ArrayTraits = typename ARRAY::Traits;

    const size_t readLength = readArrayLength<ARRAY>(reader, arrayLength);
    rawArray.clear();

    if (readLength > 0)
    {
        rawArray.reserve(readLength);

        detail::packing_context_type_t<typename ARRAY::ValueType> context;

        for (size_t i = 0; i < readLength; ++i)
        {
            if constexpr (ARRAY::TYPE == ArrayType::ALIGNED || ARRAY::TYPE == ArrayType::ALIGNED_AUTO)
            {
                reader.alignTo(8);
            }

            if constexpr (has_allocator_v<typename ARRAY::ValueType>)
            {
                rawArray.emplace_back(rawArray.get_allocator());
            }
            else
            {
                rawArray.emplace_back();
            }
            ArrayTraits::read(context, reader, owner, rawArray.back(), i);
        }
    }
}

template <typename ARRAY, std::enable_if_t<is_dummy_array_owner_v<typename ARRAY::OwnerType>, int> = 0>
void readPacked(BitStreamReader& reader, typename ARRAY::RawArray& rawArray, size_t arrayLength = 0)
{
    DummyArrayOwner owner;
    readPacked<ARRAY>(reader, rawArray, owner, arrayLength);
}

} // namespace detail

template <typename RAW_ARRAY, ArrayType ARRAY_TYPE, ArrayStorage ARRAY_STORAGE, typename ARRAY_TRAITS>
uint32_t calcHashCode(
        uint32_t seedValue, const Array<RAW_ARRAY, ARRAY_TYPE, ARRAY_STORAGE, ARRAY_TRAITS>& array)
{
    uint32_t result = seedValue;
    for (size_t i = 0; i < array.size(); ++i)
    {
        result = calcHashCode(result, array[i]);
    }

    return result;
}

} // namespace zserio

#endif // ZSERIO_ARRAY_H_INC
