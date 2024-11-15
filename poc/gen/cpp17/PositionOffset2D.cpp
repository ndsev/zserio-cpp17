/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.14.1.
 * Generator setup: stdAllocator.
 */

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>

#include <PositionOffset2D.h>

PositionOffset2D::PositionOffset2D() noexcept :
        PositionOffset2D(AllocatorType{})
{}

PositionOffset2D::PositionOffset2D(const AllocatorType&) noexcept :
        deltaLongitude(),
        deltaLatitude()
{}

PositionOffset2D::PositionOffset2D(
        ::zserio::DynInt32<> deltaLongitude_,
        ::zserio::DynInt32<> deltaLatitude_) noexcept :
        deltaLongitude(deltaLongitude_),
        deltaLatitude(deltaLatitude_)
{}

bool operator==(const PositionOffset2D& lhs, const PositionOffset2D& rhs)
{
    return ::std::tie(
            lhs.deltaLongitude,
            lhs.deltaLatitude)
            == std::tie(
            rhs.deltaLongitude,
            rhs.deltaLatitude);
}

bool operator<(const PositionOffset2D& lhs, const PositionOffset2D& rhs)
{
    return ::std::tie(
            lhs.deltaLongitude,
            lhs.deltaLatitude)
            < std::tie(
            rhs.deltaLongitude,
            rhs.deltaLatitude);
}

bool operator!=(const PositionOffset2D& lhs, const PositionOffset2D& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const PositionOffset2D& lhs, const PositionOffset2D& rhs)
{
    return rhs < lhs;
}

bool operator<=(const PositionOffset2D& lhs, const PositionOffset2D& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const PositionOffset2D& lhs, const PositionOffset2D& rhs)
{
    return !(lhs < rhs);
}

namespace zserio
{

View<PositionOffset2D>::View(const PositionOffset2D& data,
        CoordWidth numBits_,
        CoordShift shift_) noexcept :
        m_data(data),
        m_numBits_(numBits_),
        m_shift_(shift_)
{}

CoordWidth View<PositionOffset2D>::numBits() const
{
    return m_numBits_;
}

CoordShift View<PositionOffset2D>::shift() const
{
    return m_shift_;
}

View<::zserio::DynInt32<>> View<PositionOffset2D>::deltaLongitude() const
{
    return View<::zserio::DynInt32<>>{m_data.deltaLongitude, static_cast<uint8_t>(numBits() + 1)};
}

View<::zserio::DynInt32<>> View<PositionOffset2D>::deltaLatitude() const
{
    return View<::zserio::DynInt32<>>{m_data.deltaLatitude, static_cast<uint8_t>(numBits() + 1)};
}

DeltaLongitude View<PositionOffset2D>::lon() const
{
    return deltaLongitude().value() << shift();
}

DeltaLatitude View<PositionOffset2D>::lat() const
{
    return deltaLatitude().value() << shift();
}

bool operator==(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs)
{
    return lhs.numBits() == rhs.numBits() &&
            lhs.shift() == rhs.shift() &&
            lhs.deltaLongitude() == rhs.deltaLongitude() &&
            lhs.deltaLatitude() == rhs.deltaLatitude();
}

bool operator<(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs)
{
    if (lhs.numBits() != rhs.numBits())
    {
        return lhs.numBits() < rhs.numBits();
    }
    if (lhs.shift() != rhs.shift())
    {
        return lhs.shift() < rhs.shift();
    }
    if (lhs.deltaLongitude() != rhs.deltaLongitude())
    {
        return lhs.deltaLongitude() < rhs.deltaLongitude();
    }
    if (lhs.deltaLatitude() != rhs.deltaLatitude())
    {
        return lhs.deltaLatitude() < rhs.deltaLatitude();
    }

    return false;
}

bool operator!=(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<PositionOffset2D>& view)
{
    // check range
    validate(view.deltaLongitude(), "'PositionOffset2D.deltaLongitude'");
    // check range
    validate(view.deltaLatitude(), "'PositionOffset2D.deltaLatitude'");
}

template <>
BitSize bitSizeOf(const View<PositionOffset2D>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(view.deltaLongitude(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.deltaLatitude(), endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<PositionOffset2D>& view)
{
    detail::write(writer, view.deltaLongitude());
    detail::write(writer, view.deltaLatitude());
}

template <>
View<PositionOffset2D> read(BitStreamReader& reader, PositionOffset2D& data,
        CoordWidth numBits_,
        CoordShift shift_)
{
    View<PositionOffset2D> view(data,
            numBits_,
            shift_);
    detail::read(reader, data.deltaLongitude, static_cast<uint8_t>(view.numBits() + 1));
    detail::read(reader, data.deltaLatitude, static_cast<uint8_t>(view.numBits() + 1));
    return view;
}

template <>
void initContext(PackingContext<PositionOffset2D>& packingContext, const View<PositionOffset2D>& view)
{
    initContext(packingContext.deltaLongitude, view.deltaLongitude());
    initContext(packingContext.deltaLatitude, view.deltaLatitude());
}

template <>
BitSize bitSizeOf(PackingContext<PositionOffset2D>& packingContext, const View<PositionOffset2D>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(packingContext.deltaLongitude, view.deltaLongitude(), endBitPosition);
    endBitPosition += detail::bitSizeOf(packingContext.deltaLatitude, view.deltaLatitude(), endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(PackingContext<PositionOffset2D>& packingContext, BitStreamWriter& writer, const View<PositionOffset2D>& view)
{
    detail::write(packingContext.deltaLongitude, writer, view.deltaLongitude());
    detail::write(packingContext.deltaLatitude, writer, view.deltaLatitude());
}

template <>
void read(PackingContext<PositionOffset2D>& packingContext, BitStreamReader& reader, PositionOffset2D& data,
        CoordWidth numBits_,
        CoordShift shift_)
{
    View<PositionOffset2D> view(data,
            numBits_,
            shift_);
    detail::read(packingContext.deltaLongitude, reader, data.deltaLongitude, static_cast<uint8_t>(view.numBits() + 1));
    detail::read(packingContext.deltaLatitude, reader, data.deltaLatitude, static_cast<uint8_t>(view.numBits() + 1));
    (void)view;
}

} // namespace detail

} // namespace zserio

namespace std
{

size_t hash<PositionOffset2D>::operator()(const PositionOffset2D& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.deltaLongitude);
    result = ::zserio::calcHashCode(result, data.deltaLatitude);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<PositionOffset2D>>::operator()(const ::zserio::View<PositionOffset2D>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.deltaLongitude());
    result = ::zserio::calcHashCode(result, view.deltaLatitude());
    return static_cast<size_t>(result);
}

} // namespace std
