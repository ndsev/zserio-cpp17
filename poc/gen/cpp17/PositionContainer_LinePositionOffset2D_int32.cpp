/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.14.1.
 * Generator setup: stdAllocator.
 */

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>

#include <PositionContainer_LinePositionOffset2D_int32.h>

PositionContainer_LinePositionOffset2D_int32::PositionContainer_LinePositionOffset2D_int32() noexcept :
        PositionContainer_LinePositionOffset2D_int32(AllocatorType{})
{}

PositionContainer_LinePositionOffset2D_int32::PositionContainer_LinePositionOffset2D_int32(const AllocatorType& allocator) noexcept :
        numElements(),
        array(allocator),
        field()
{}

PositionContainer_LinePositionOffset2D_int32::PositionContainer_LinePositionOffset2D_int32(
        ::zserio::VarSize numElements_,
        ::zserio::Vector<LinePositionOffset2D> array_,
        ::zserio::Int32 field_) noexcept :
        numElements(numElements_),
        array(::std::move(array_)),
        field(field_)
{}

bool operator==(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs)
{
    return ::std::tie(
            lhs.numElements,
            lhs.array,
            lhs.field)
            == std::tie(
            rhs.numElements,
            rhs.array,
            rhs.field);
}

bool operator<(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs)
{
    return ::std::tie(
            lhs.numElements,
            lhs.array,
            lhs.field)
            < std::tie(
            rhs.numElements,
            rhs.array,
            rhs.field);
}

bool operator!=(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs)
{
    return rhs < lhs;
}

bool operator<=(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs)
{
    return !(lhs < rhs);
}

namespace zserio
{

View<PositionContainer_LinePositionOffset2D_int32>::View(const PositionContainer_LinePositionOffset2D_int32& data,
        CoordShift shift_) noexcept :
        m_data(data),
        m_shift_(shift_)
{}

CoordShift View<PositionContainer_LinePositionOffset2D_int32>::shift() const
{
    return m_shift_;
}

::zserio::VarSize View<PositionContainer_LinePositionOffset2D_int32>::numElements() const
{
    return m_data.numElements;
}

Array<::zserio::Vector<LinePositionOffset2D>, ArrayType::NORMAL, View<PositionContainer_LinePositionOffset2D_int32>::ZserioArrayArrayTraits> View<PositionContainer_LinePositionOffset2D_int32>::array() const
{
    return Array<::zserio::Vector<LinePositionOffset2D>, ArrayType::NORMAL, ZserioArrayArrayTraits>{m_data.array, *this};
}

::zserio::Int32 View<PositionContainer_LinePositionOffset2D_int32>::field() const
{
    return m_data.field;
}

bool operator==(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs)
{
    return lhs.shift() == rhs.shift() &&
            lhs.numElements() == rhs.numElements() &&
            lhs.array() == rhs.array() &&
            lhs.field() == rhs.field();
}

bool operator<(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs)
{
    if (lhs.shift() != rhs.shift())
    {
        return lhs.shift() < rhs.shift();
    }
    if (lhs.numElements() != rhs.numElements())
    {
        return lhs.numElements() < rhs.numElements();
    }
    if (lhs.array() != rhs.array())
    {
        return lhs.array() < rhs.array();
    }
    if (lhs.field() != rhs.field())
    {
        return lhs.field() < rhs.field();
    }

    return false;
}

bool operator!=(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<PositionContainer_LinePositionOffset2D_int32>& view)
{
    // check range
    validate(view.numElements(), "'PositionContainer_LinePositionOffset2D_int32.numElements'");
    // check array length
    validate(view.array(), static_cast<size_t>(view.numElements()),
            "'PositionContainer_LinePositionOffset2D_int32.array'");
    // check range
    validate(view.field(), "'PositionContainer_LinePositionOffset2D_int32.field'");
}

template <>
BitSize bitSizeOf(const View<PositionContainer_LinePositionOffset2D_int32>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(view.numElements(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.array(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.field(), endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<PositionContainer_LinePositionOffset2D_int32>& view)
{
    detail::write(writer, view.numElements());
    detail::write(writer, view.array());
    detail::write(writer, view.field());
}

template <>
View<PositionContainer_LinePositionOffset2D_int32> read(BitStreamReader& reader, PositionContainer_LinePositionOffset2D_int32& data,
        CoordShift shift_)
{
    View<PositionContainer_LinePositionOffset2D_int32> view(data,
            shift_);
    detail::read(reader, data.numElements);
    (void)detail::read<Array<::zserio::Vector<LinePositionOffset2D>, ArrayType::NORMAL, View<PositionContainer_LinePositionOffset2D_int32>::ZserioArrayArrayTraits>>(reader, data.array, view, view.numElements());
    detail::read(reader, data.field);
    return view;
}

} // namespace detail

} // namespace zserio

namespace std
{

size_t hash<PositionContainer_LinePositionOffset2D_int32>::operator()(const PositionContainer_LinePositionOffset2D_int32& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.numElements);
    result = ::zserio::calcHashCode(result, data.array);
    result = ::zserio::calcHashCode(result, data.field);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<PositionContainer_LinePositionOffset2D_int32>>::operator()(const ::zserio::View<PositionContainer_LinePositionOffset2D_int32>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.numElements());
    result = ::zserio::calcHashCode(result, view.array());
    result = ::zserio::calcHashCode(result, view.field());
    return static_cast<size_t>(result);
}

} // namespace std