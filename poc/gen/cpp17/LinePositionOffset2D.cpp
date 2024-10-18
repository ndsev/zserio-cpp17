/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.14.1.
 * Generator setup: stdAllocator.
 */

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>

#include <LinePositionOffset2D.h>

LinePositionOffset2D::LinePositionOffset2D() noexcept :
        LinePositionOffset2D(AllocatorType{})
{}

LinePositionOffset2D::LinePositionOffset2D(const AllocatorType& allocator) noexcept :
        linePosition(),
        numBits(),
        offset(allocator)
{}

LinePositionOffset2D::LinePositionOffset2D(
        LinePosition linePosition_,
        CoordWidth numBits_,
        PositionOffset2D offset_) noexcept :
        linePosition(linePosition_),
        numBits(numBits_),
        offset(::std::move(offset_))
{}

bool operator==(const LinePositionOffset2D& lhs, const LinePositionOffset2D& rhs)
{
    return ::std::tie(
            lhs.linePosition,
            lhs.numBits,
            lhs.offset)
            == std::tie(
            rhs.linePosition,
            rhs.numBits,
            rhs.offset);
}

bool operator<(const LinePositionOffset2D& lhs, const LinePositionOffset2D& rhs)
{
    return ::std::tie(
            lhs.linePosition,
            lhs.numBits,
            lhs.offset)
            < std::tie(
            rhs.linePosition,
            rhs.numBits,
            rhs.offset);
}

bool operator!=(const LinePositionOffset2D& lhs, const LinePositionOffset2D& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const LinePositionOffset2D& lhs, const LinePositionOffset2D& rhs)
{
    return rhs < lhs;
}

bool operator<=(const LinePositionOffset2D& lhs, const LinePositionOffset2D& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const LinePositionOffset2D& lhs, const LinePositionOffset2D& rhs)
{
    return !(lhs < rhs);
}

namespace zserio
{

View<LinePositionOffset2D>::View(const LinePositionOffset2D& data,
        CoordShift shift_) noexcept :
        m_data(data),
        m_shift_(shift_)
{}

CoordShift View<LinePositionOffset2D>::shift() const
{
    return m_shift_;
}

LinePosition View<LinePositionOffset2D>::linePosition() const
{
    return m_data.linePosition;
}

CoordWidth View<LinePositionOffset2D>::numBits() const
{
    return m_data.numBits;
}

View<PositionOffset2D> View<LinePositionOffset2D>::offset() const
{
    return View<PositionOffset2D>{m_data.offset, CoordWidth(static_cast<CoordWidth::ValueType>(numBits())), CoordShift(static_cast<CoordShift::ValueType>(shift()))};
}

bool operator==(const View<LinePositionOffset2D>& lhs, const View<LinePositionOffset2D>& rhs)
{
    return lhs.shift() == rhs.shift() &&
            lhs.linePosition() == rhs.linePosition() &&
            lhs.numBits() == rhs.numBits() &&
            lhs.offset() == rhs.offset();
}

bool operator<(const View<LinePositionOffset2D>& lhs, const View<LinePositionOffset2D>& rhs)
{
    if (lhs.shift() != rhs.shift())
    {
        return lhs.shift() < rhs.shift();
    }
    if (lhs.linePosition() != rhs.linePosition())
    {
        return lhs.linePosition() < rhs.linePosition();
    }
    if (lhs.numBits() != rhs.numBits())
    {
        return lhs.numBits() < rhs.numBits();
    }
    if (lhs.offset() != rhs.offset())
    {
        return lhs.offset() < rhs.offset();
    }

    return false;
}

bool operator!=(const View<LinePositionOffset2D>& lhs, const View<LinePositionOffset2D>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<LinePositionOffset2D>& lhs, const View<LinePositionOffset2D>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<LinePositionOffset2D>& lhs, const View<LinePositionOffset2D>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<LinePositionOffset2D>& lhs, const View<LinePositionOffset2D>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<LinePositionOffset2D>& view)
{
    // check range
    validate(view.linePosition(), "'LinePositionOffset2D.linePosition'");
    // check range
    validate(view.numBits(), "'LinePositionOffset2D.numBits'");
}

template <>
BitSize bitSizeOf(const View<LinePositionOffset2D>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(view.linePosition(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.numBits(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.offset(), endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<LinePositionOffset2D>& view)
{
    detail::write(writer, view.linePosition());
    detail::write(writer, view.numBits());
    detail::write(writer, view.offset());
}

template <>
View<LinePositionOffset2D> read(BitStreamReader& reader, LinePositionOffset2D& data,
        CoordShift shift_)
{
    View<LinePositionOffset2D> view(data,
            shift_);
    detail::read(reader, data.linePosition);
    detail::read(reader, data.numBits);
    (void)detail::read(reader, data.offset, CoordWidth(static_cast<CoordWidth::ValueType>(view.numBits())), CoordShift(static_cast<CoordShift::ValueType>(view.shift())));
    return view;
}

template <>
void initContext(PackingContext<LinePositionOffset2D>& packingContext, const View<LinePositionOffset2D>& view)
{
    initContext(packingContext.linePosition, view.linePosition());
    initContext(packingContext.numBits, view.numBits());
    initContext(packingContext.offset, view.offset());
}

template <>
BitSize bitSizeOf(PackingContext<LinePositionOffset2D>& packingContext, const View<LinePositionOffset2D>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(packingContext.linePosition, view.linePosition(), endBitPosition);
    endBitPosition += detail::bitSizeOf(packingContext.numBits, view.numBits(), endBitPosition);
    endBitPosition += detail::bitSizeOf(packingContext.offset, view.offset(), endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(PackingContext<LinePositionOffset2D>& packingContext, BitStreamWriter& writer, const View<LinePositionOffset2D>& view)
{
    detail::write(packingContext.linePosition, writer, view.linePosition());
    detail::write(packingContext.numBits, writer, view.numBits());
    detail::write(packingContext.offset, writer, view.offset());
}

template <>
void read(PackingContext<LinePositionOffset2D>& packingContext, BitStreamReader& reader, LinePositionOffset2D& data,
        CoordShift shift_)
{
    View<LinePositionOffset2D> view(data,
            shift_);
    detail::read(packingContext.linePosition, reader, data.linePosition);
    detail::read(packingContext.numBits, reader, data.numBits);
    (void)detail::read(packingContext.offset, reader, data.offset, CoordWidth(static_cast<CoordWidth::ValueType>(view.numBits())), CoordShift(static_cast<CoordShift::ValueType>(view.shift())));
    (void)view;
}

} // namespace detail

} // namespace zserio

namespace std
{

size_t hash<LinePositionOffset2D>::operator()(const LinePositionOffset2D& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.linePosition);
    result = ::zserio::calcHashCode(result, data.numBits);
    result = ::zserio::calcHashCode(result, data.offset);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<LinePositionOffset2D>>::operator()(const ::zserio::View<LinePositionOffset2D>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.linePosition());
    result = ::zserio::calcHashCode(result, view.numBits());
    result = ::zserio::calcHashCode(result, view.offset());
    return static_cast<size_t>(result);
}

} // namespace std