/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.14.1.
 * Generator setup: stdAllocator.
 */

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>

#include <RouteSegment.h>
#include <zserio/ConstraintException.h>
#include <zserio/MissedOptionalException.h>

RouteSegment::RouteSegment() noexcept :
        RouteSegment(AllocatorType{})
{}

RouteSegment::RouteSegment(const AllocatorType& allocator) noexcept :
        hasEndOffset(),
        shift(),
        myOffset(),
        container(allocator),
        packedContainer(allocator),
        endPositionWithOffset(allocator),
        myArray(allocator),
        myChoice(allocator),
        myUnion(allocator)
{}

RouteSegment::RouteSegment(
        ::zserio::Bool hasEndOffset_,
        CoordShift shift_,
        ::zserio::UInt32 myOffset_,
        PositionContainer_LinePositionOffset2D_int32 container_,
        ::zserio::Vector<PositionContainer_LinePositionOffset2D_string> packedContainer_,
        ::zserio::Optional<LinePositionOffset2D> endPositionWithOffset_,
        ::zserio::Vector<::zserio::Int32> myArray_,
        BoolParamChoice myChoice_,
        SimpleUnion myUnion_) noexcept :
        hasEndOffset(hasEndOffset_),
        shift(shift_),
        myOffset(myOffset_),
        container(::std::move(container_)),
        packedContainer(::std::move(packedContainer_)),
        endPositionWithOffset(::std::move(endPositionWithOffset_)),
        myArray(::std::move(myArray_)),
        myChoice(::std::move(myChoice_)),
        myUnion(::std::move(myUnion_))
{}

bool operator==(const RouteSegment& lhs, const RouteSegment& rhs)
{
    return ::std::tie(
            lhs.hasEndOffset,
            lhs.shift,
            lhs.myOffset,
            lhs.container,
            lhs.packedContainer,
            lhs.endPositionWithOffset,
            lhs.myArray,
            lhs.myChoice,
            lhs.myUnion)
            == std::tie(
            rhs.hasEndOffset,
            rhs.shift,
            rhs.myOffset,
            rhs.container,
            rhs.packedContainer,
            rhs.endPositionWithOffset,
            rhs.myArray,
            rhs.myChoice,
            rhs.myUnion);
}

bool operator<(const RouteSegment& lhs, const RouteSegment& rhs)
{
    return ::std::tie(
            lhs.hasEndOffset,
            lhs.shift,
            lhs.myOffset,
            lhs.container,
            lhs.packedContainer,
            lhs.endPositionWithOffset,
            lhs.myArray,
            lhs.myChoice,
            lhs.myUnion)
            < std::tie(
            rhs.hasEndOffset,
            rhs.shift,
            rhs.myOffset,
            rhs.container,
            rhs.packedContainer,
            rhs.endPositionWithOffset,
            rhs.myArray,
            rhs.myChoice,
            rhs.myUnion);
}

bool operator!=(const RouteSegment& lhs, const RouteSegment& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const RouteSegment& lhs, const RouteSegment& rhs)
{
    return rhs < lhs;
}

bool operator<=(const RouteSegment& lhs, const RouteSegment& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const RouteSegment& lhs, const RouteSegment& rhs)
{
    return !(lhs < rhs);
}

namespace zserio
{

View<RouteSegment>::View(const RouteSegment& data) noexcept :
        m_data(data)
{}

::zserio::Bool View<RouteSegment>::hasEndOffset() const
{
    return m_data.hasEndOffset;
}

CoordShift View<RouteSegment>::shift() const
{
    return m_data.shift;
}

::zserio::UInt32 View<RouteSegment>::myOffset() const
{
    return m_data.myOffset;
}

View<PositionContainer_LinePositionOffset2D_int32> View<RouteSegment>::container() const
{
    return View<PositionContainer_LinePositionOffset2D_int32>{m_data.container, CoordShift(static_cast<CoordShift::ValueType>(shift()))};
}

Array<::zserio::Vector<PositionContainer_LinePositionOffset2D_string>, ArrayType::NORMAL, View<RouteSegment>::ZserioPackedContainerArrayTraits> View<RouteSegment>::packedContainer() const
{
    return Array<::zserio::Vector<PositionContainer_LinePositionOffset2D_string>, ArrayType::NORMAL, ZserioPackedContainerArrayTraits>{m_data.packedContainer, *this};
}

::zserio::Optional<View<LinePositionOffset2D>> View<RouteSegment>::endPositionWithOffset() const
{
    if (!m_data.endPositionWithOffset)
    {
        return {};
    }

    return View<LinePositionOffset2D>{*m_data.endPositionWithOffset, CoordShift(static_cast<CoordShift::ValueType>(shift()))};
}

Array<::zserio::Vector<::zserio::Int32>, ArrayType::NORMAL> View<RouteSegment>::myArray() const
{
    return Array<::zserio::Vector<::zserio::Int32>, ArrayType::NORMAL>{m_data.myArray};
}

View<BoolParamChoice> View<RouteSegment>::myChoice() const
{
    return View<BoolParamChoice>{m_data.myChoice, ::zserio::Bool(static_cast<::zserio::Bool::ValueType>(hasEndOffset()))};
}

View<SimpleUnion> View<RouteSegment>::myUnion() const
{
    return View<SimpleUnion>{m_data.myUnion};
}

bool operator==(const View<RouteSegment>& lhs, const View<RouteSegment>& rhs)
{
    return lhs.hasEndOffset() == rhs.hasEndOffset() &&
            lhs.shift() == rhs.shift() &&
            lhs.myOffset() == rhs.myOffset() &&
            lhs.container() == rhs.container() &&
            lhs.packedContainer() == rhs.packedContainer() &&
            (!(lhs.hasEndOffset()) ? !(rhs.hasEndOffset()) : *lhs.endPositionWithOffset() == *rhs.endPositionWithOffset()) &&
            lhs.myArray() == rhs.myArray() &&
            lhs.myChoice() == rhs.myChoice() &&
            lhs.myUnion() == rhs.myUnion();
}

bool operator<(const View<RouteSegment>& lhs, const View<RouteSegment>& rhs)
{
    if (lhs.hasEndOffset() != rhs.hasEndOffset())
    {
        return static_cast<int>(lhs.hasEndOffset()) < static_cast<int>(rhs.hasEndOffset());
    }
    if (lhs.shift() != rhs.shift())
    {
        return lhs.shift() < rhs.shift();
    }
    if (lhs.myOffset() != rhs.myOffset())
    {
        return lhs.myOffset() < rhs.myOffset();
    }
    if (lhs.container() != rhs.container())
    {
        return lhs.container() < rhs.container();
    }
    if (lhs.packedContainer() != rhs.packedContainer())
    {
        return lhs.packedContainer() < rhs.packedContainer();
    }
    if ((lhs.hasEndOffset()) && (rhs.hasEndOffset()))
    {
        if (*lhs.endPositionWithOffset() != *rhs.endPositionWithOffset())
        {
            return *lhs.endPositionWithOffset() < *rhs.endPositionWithOffset();
        }
    }
    else if ((lhs.hasEndOffset()) != (rhs.hasEndOffset()))
    {
        return !(lhs.hasEndOffset());
    }
    if (lhs.myArray() != rhs.myArray())
    {
        return lhs.myArray() < rhs.myArray();
    }
    if (lhs.myChoice() != rhs.myChoice())
    {
        return lhs.myChoice() < rhs.myChoice();
    }
    if (lhs.myUnion() != rhs.myUnion())
    {
        return lhs.myUnion() < rhs.myUnion();
    }

    return false;
}

bool operator!=(const View<RouteSegment>& lhs, const View<RouteSegment>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<RouteSegment>& lhs, const View<RouteSegment>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<RouteSegment>& lhs, const View<RouteSegment>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<RouteSegment>& lhs, const View<RouteSegment>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<RouteSegment>& view)
{
    // check range
    validate(view.hasEndOffset(), "'RouteSegment.hasEndOffset'");
    // check constraint
    if (!(view.shift() > 0))
    {
        throw ConstraintException("Constraint violated at 'RouteSegment.shift'!");
    }
    // check range
    validate(view.shift(), "'RouteSegment.shift'");
    // check range
    validate(view.myOffset(), "'RouteSegment.myOffset'");
    // check array length
    validate(view.packedContainer(), static_cast<size_t>(3),
            "'RouteSegment.packedContainer'");
    // check non-auto optional
    if (view.hasEndOffset() && !view.endPositionWithOffset())
    {
        throw MissedOptionalException("Optional field 'RouteSegment.endPositionWithOffset' is used but not set!");
    }
    // check array length
    validate(view.myArray(), static_cast<size_t>(3),
            "'RouteSegment.myArray'");
}

template <>
BitSize bitSizeOf(const View<RouteSegment>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(view.hasEndOffset(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.shift(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.myOffset(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.container(), endBitPosition);
    endBitPosition += detail::bitSizeOfPacked(view.packedContainer(), endBitPosition);
    if (view.hasEndOffset())
    {
        endBitPosition += detail::bitSizeOf(*view.endPositionWithOffset(), endBitPosition);
    }
    endBitPosition += detail::bitSizeOfPacked(view.myArray(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.myChoice(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.myUnion(), endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<RouteSegment>& view)
{
    detail::write(writer, view.hasEndOffset());
    detail::write(writer, view.shift());
    detail::write(writer, view.myOffset());
    detail::write(writer, view.container());
    detail::writePacked(writer, view.packedContainer());
    if (view.hasEndOffset())
    {
        detail::write(writer, *view.endPositionWithOffset());
    }
    detail::writePacked(writer, view.myArray());
    detail::write(writer, view.myChoice());
    detail::write(writer, view.myUnion());
}

template <>
View<RouteSegment> read(BitStreamReader& reader, RouteSegment& data)
{
    View<RouteSegment> view(data);
    detail::read(reader, data.hasEndOffset);
    detail::read(reader, data.shift);
    // check constraint
    if (!(view.shift() > 0))
    {
        throw ConstraintException("Constraint violated at 'RouteSegment.shift'!");
    }
    detail::read(reader, data.myOffset);
    (void)detail::read(reader, data.container, CoordShift(static_cast<CoordShift::ValueType>(view.shift())));
    (void)detail::readPacked<Array<::zserio::Vector<PositionContainer_LinePositionOffset2D_string>, ArrayType::NORMAL, View<RouteSegment>::ZserioPackedContainerArrayTraits>>(reader, data.packedContainer, view, 3);
    if (view.hasEndOffset())
    {
        data.endPositionWithOffset = LinePositionOffset2D(data.endPositionWithOffset.get_allocator());
        (void)detail::read(reader, *data.endPositionWithOffset, CoordShift(static_cast<CoordShift::ValueType>(view.shift())));
    }
    detail::readPacked<Array<::zserio::Vector<::zserio::Int32>, ArrayType::NORMAL>>(reader, data.myArray, 3);
    (void)detail::read(reader, data.myChoice, ::zserio::Bool(static_cast<::zserio::Bool::ValueType>(view.hasEndOffset())));
    (void)detail::read(reader, data.myUnion);
    return view;
}

} // namespace detail

} // namespace zserio

namespace std
{

size_t hash<RouteSegment>::operator()(const RouteSegment& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.hasEndOffset);
    result = ::zserio::calcHashCode(result, data.shift);
    result = ::zserio::calcHashCode(result, data.myOffset);
    result = ::zserio::calcHashCode(result, data.container);
    result = ::zserio::calcHashCode(result, data.packedContainer);
    result = ::zserio::calcHashCode(result, data.endPositionWithOffset);
    result = ::zserio::calcHashCode(result, data.myArray);
    result = ::zserio::calcHashCode(result, data.myChoice);
    result = ::zserio::calcHashCode(result, data.myUnion);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<RouteSegment>>::operator()(const ::zserio::View<RouteSegment>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.hasEndOffset());
    result = ::zserio::calcHashCode(result, view.shift());
    result = ::zserio::calcHashCode(result, view.myOffset());
    result = ::zserio::calcHashCode(result, view.container());
    result = ::zserio::calcHashCode(result, view.packedContainer());
    if (view.hasEndOffset())
    {
        result = ::zserio::calcHashCode(result, view.endPositionWithOffset());
    }
    result = ::zserio::calcHashCode(result, view.myArray());
    result = ::zserio::calcHashCode(result, view.myChoice());
    result = ::zserio::calcHashCode(result, view.myUnion());
    return static_cast<size_t>(result);
}

} // namespace std