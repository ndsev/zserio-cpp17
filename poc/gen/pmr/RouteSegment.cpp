/**
 * Automatically generated by Zserio C++ generator version 1.0.0 using Zserio core 2.13.0.
 * Generator setup: writerCode, pubsubCode, serviceCode, sqlCode, stdAllocator.
 */

#include "RouteSegment.h"

namespace pmr
{

RouteSegment::RouteSegment() noexcept :
        RouteSegment(allocator_type())
{}

RouteSegment::RouteSegment(const allocator_type&)
{}

RouteSegment::RouteSegment(::zserio::Boolean hasEndOffset_, ::pmr::CoordShift shift_,
        // myOffset is not here because it does not make any sense to set it
        PositionContainer<LinePositionOffset2D, ::zserio::Int32> container_,
        ::zserio::pmr::vector<PositionContainer<LinePositionOffset2D, ::zserio::pmr::String>> packedContainer_,
        ::zserio::InplaceOptionalHolder<LinePositionOffset2D> endPositionWithOffset_,
        ::zserio::pmr::vector<::zserio::Int32> myArray_, BoolParamChoice myChoice_, SimpleUnion myUnion_) :
        hasEndOffset(::std::move(hasEndOffset_)),
        shift(::std::move(shift_)),
        container(::std::move(container_)),
        packedContainer(::std::move(packedContainer_)),
        endPositionWithOffset(::std::move(endPositionWithOffset_)),
        myArray(::std::move(myArray_)),
        myChoice(::std::move(myChoice_)),
        myUnion(::std::move(myUnion_))
{}

bool operator==(const RouteSegment& lhs, const RouteSegment& rhs)
{
    // myOffset is not here intentionally because offsets are not relevant for comparison
    return ::std::tie(lhs.hasEndOffset, lhs.shift, lhs.container, lhs.packedContainer,
                   lhs.endPositionWithOffset, lhs.myArray, lhs.myChoice, lhs.myUnion) ==
            ::std::tie(rhs.hasEndOffset, rhs.shift, rhs.container, rhs.packedContainer,
                    rhs.endPositionWithOffset, rhs.myArray, rhs.myChoice, rhs.myUnion);
}

bool operator<(const RouteSegment& lhs, const RouteSegment& rhs)
{
    // myOffset is not here intentionally because offsets are not relevant for comparison
    return ::std::tie(lhs.hasEndOffset, lhs.shift, lhs.container, lhs.packedContainer,
                   lhs.endPositionWithOffset, lhs.myArray, lhs.myChoice, lhs.myUnion) <
            ::std::tie(rhs.hasEndOffset, rhs.shift, rhs.container, rhs.packedContainer,
                    rhs.endPositionWithOffset, rhs.myArray, rhs.myChoice, rhs.myUnion);
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

} // namespace pmr

namespace zserio
{

View<::pmr::RouteSegment>::View(const ::pmr::RouteSegment& data) noexcept :
        m_data(data)
{}

::zserio::Boolean View<::pmr::RouteSegment>::hasEndOffset() const
{
    return m_data.hasEndOffset;
}

::pmr::CoordShift View<::pmr::RouteSegment>::shift() const
{
    return m_data.shift;
}

uint32_t View<::pmr::RouteSegment>::myOffset() const
{
    return m_data.myOffset;
}

::zserio::View<::pmr::PositionContainer<::pmr::LinePositionOffset2D, ::zserio::Int32>> View<
        ::pmr::RouteSegment>::container() const
{
    return ::zserio::View<::pmr::PositionContainer<::pmr::LinePositionOffset2D, ::zserio::Int32>>(
            m_data.container, m_data.shift);
}

// Array should probably copy the View (owner) into it so dangling references
// will be prevented
View<::pmr::RouteSegment>::ZserioArrayType_packedContainer View<::pmr::RouteSegment>::packedContainer() const
{
    return ZserioArrayType_packedContainer(*this, m_data.packedContainer);
}

::zserio::InplaceOptionalHolder<View<::pmr::LinePositionOffset2D>> View<
        ::pmr::RouteSegment>::endPositionWithOffset() const
{
    if (!m_data.endPositionWithOffset)
        return {};
    return View<::pmr::LinePositionOffset2D>(*m_data.endPositionWithOffset, m_data.shift);
}

View<::pmr::RouteSegment>::ZserioArrayType_myArray View<::pmr::RouteSegment>::myArray() const
{
    return ZserioArrayType_myArray(m_data.myArray);
}

View<::pmr::BoolParamChoice> View<::pmr::RouteSegment>::myChoice() const
{
    return View<::pmr::BoolParamChoice>(m_data.myChoice, m_data.hasEndOffset);
}

View<::pmr::SimpleUnion> View<::pmr::RouteSegment>::myUnion() const
{
    return View<::pmr::SimpleUnion>(m_data.myUnion);
}

bool operator==(const View<::pmr::RouteSegment>& lhs, const View<::pmr::RouteSegment>& rhs)
{
    // myOffset is not here intentionally because offsets are not relevant for comparison
    if (&lhs != &rhs)
    {
        return (lhs.hasEndOffset() == rhs.hasEndOffset()) && (lhs.shift() == rhs.shift()) &&
                (lhs.container() == rhs.container()) && (lhs.packedContainer() == rhs.packedContainer()) &&
                (!lhs.hasEndOffset() ? !rhs.hasEndOffset()
                                     : (lhs.endPositionWithOffset() == rhs.endPositionWithOffset())) &&
                (lhs.myArray() == rhs.myArray()) && (lhs.myChoice() == rhs.myChoice()) &&
                (lhs.myUnion() == rhs.myUnion());
    }

    return true;
}

bool operator<(const View<::pmr::RouteSegment>& lhs, const View<::pmr::RouteSegment>& rhs)
{
    // myOffset is not here intentionally because offsets are not relevant for comparison
    if (lhs.hasEndOffset() != rhs.hasEndOffset())
        return lhs.hasEndOffset() < rhs.hasEndOffset();
    if (lhs.shift() != rhs.shift())
        return lhs.shift() < rhs.shift();
    if (lhs.container() != rhs.container())
        return lhs.container() < rhs.container();
    if (lhs.packedContainer() != rhs.packedContainer())
        return lhs.packedContainer() < rhs.packedContainer();
    if (lhs.hasEndOffset() && rhs.hasEndOffset())
    {
        if (lhs.endPositionWithOffset() != rhs.endPositionWithOffset())
            return lhs.endPositionWithOffset() < rhs.endPositionWithOffset();
    }
    else if (lhs.hasEndOffset() != rhs.hasEndOffset())
    {
        return !lhs.hasEndOffset();
    }
    if (lhs.myArray() != rhs.myArray())
        return lhs.myArray() < rhs.myArray();
    if (lhs.myChoice() != rhs.myChoice())
        return lhs.myChoice() < rhs.myChoice();
    if (lhs.myUnion() != rhs.myUnion())
        return lhs.myUnion() < rhs.myUnion();

    return false;
}

bool operator!=(const View<::pmr::RouteSegment>& lhs, const View<::pmr::RouteSegment>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::pmr::RouteSegment>& lhs, const View<::pmr::RouteSegment>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::pmr::RouteSegment>& lhs, const View<::pmr::RouteSegment>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::pmr::RouteSegment>& lhs, const View<::pmr::RouteSegment>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::pmr::RouteSegment>& view)
{
    // check constraint
    if (!(view.shift() > 0))
    {
        throw ::zserio::ConstraintException("Write: Constraint violated at ::pmr::RouteSegment.shift!");
    }

    if (view.hasEndOffset())
    {
        detail::validate(*view.endPositionWithOffset());
    }

    // check array length
    if (view.packedContainer().size() != static_cast<size_t>(3))
    {
        throw ::zserio::CppRuntimeException(
                "Write: Wrong array length for field ::pmr::RouteSegment.packedContainer: ")
                << view.packedContainer().size() << " != " << static_cast<size_t>(3) << "!";
    }

    // check array length
    if (view.myArray().size() != static_cast<size_t>(3))
    {
        throw ::zserio::CppRuntimeException("Write: Wrong array length for field ::pmr::RouteSegment.myArray: ")
                << view.myArray().size() << " != " << static_cast<size_t>(3) << "!";
    }

    if (view.hasEndOffset())
    {
        detail::validate(*view.endPositionWithOffset());
    }

    detail::validate(view.myChoice());

    detail::validate(view.myUnion());
}

template <>
void write(::zserio::BitStreamWriter& writer, const View<::pmr::RouteSegment>& view)
{
    detail::write(writer, view.hasEndOffset());
    detail::write(writer, view.shift());

    detail::write(writer, UInt32(view.myOffset()));

    detail::write(writer, view.container());

    view.packedContainer().writePacked(writer);

    if (view.hasEndOffset())
    {
        writer.alignTo(UINT32_C(8));
        // check offset
        if (writer.getBitPosition() / 8 != (view.myOffset()))
        {
            throw ::zserio::CppRuntimeException(
                    "Write: Wrong offset for field ::pmr::RouteSegment.endPositionWithOffset: ")
                    << (writer.getBitPosition() / 8) << " != " << (view.myOffset()) << "!";
        }
        detail::write(writer, *view.endPositionWithOffset());
    }

    view.myArray().writePacked(writer);

    detail::write(writer, view.myChoice());

    detail::write(writer, view.myUnion());
}

template <>
View<::pmr::RouteSegment> detail::read(::zserio::BitStreamReader& reader, ::pmr::RouteSegment& data,
        const ::pmr::RouteSegment::allocator_type& alloc)
{
    View<::pmr::RouteSegment> view(data);

    detail::read(reader, data.hasEndOffset);

    detail::read(reader, data.shift);
    // check constraint
    if (!(view.shift() > 0))
    {
        throw ::zserio::ConstraintException("Write: Constraint violated at ::pmr::RouteSegment.shift!");
    }

    UInt32 myOffset;
    detail::read(reader, myOffset);
    data.myOffset = myOffset;

    detail::read(reader, data.container, alloc, data.shift);

    View<::pmr::RouteSegment>::ZserioArrayType_packedContainer(
            view, data.packedContainer, reader, ::zserio::PackedTag(), static_cast<size_t>(3));

    if (view.hasEndOffset())
    {
        reader.alignTo(UINT32_C(8));
        data.endPositionWithOffset = ::pmr::LinePositionOffset2D();
        detail::read(reader, *data.endPositionWithOffset, alloc, data.shift);
    }

    View<::pmr::RouteSegment>::ZserioArrayType_myArray(
            data.myArray, reader, ::zserio::PackedTag(), static_cast<size_t>(3));

    detail::read(reader, data.myChoice, alloc, data.hasEndOffset);

    detail::read(reader, data.myUnion, alloc);

    return view;
}

template <>
size_t bitSizeOf(const View<::pmr::RouteSegment>& view, size_t bitPosition)
{
    size_t endBitPosition = bitPosition;

    endBitPosition += detail::bitSizeOf(view.hasEndOffset(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.shift(), endBitPosition);
    endBitPosition += UINT8_C(32);
    endBitPosition += detail::bitSizeOf(view.container(), endBitPosition);
    endBitPosition += view.packedContainer().bitSizeOfPacked(endBitPosition);
    if (view.hasEndOffset())
    {
        endBitPosition = ::zserio::alignTo(8, endBitPosition);
        //::zserio::detail here is neccessary in msvc
        endBitPosition += detail::bitSizeOf(*view.endPositionWithOffset(), endBitPosition);
    }
    endBitPosition += view.myArray().bitSizeOfPacked(endBitPosition);
    endBitPosition += detail::bitSizeOf(view.myChoice(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.myUnion(), endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
size_t initializeOffsets(::pmr::RouteSegment& data, size_t endBitPosition)
{
    View<::pmr::RouteSegment> view(data);

    endBitPosition += detail::bitSizeOf(view.hasEndOffset(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.shift(), endBitPosition);
    endBitPosition += UINT8_C(32);
    endBitPosition += detail::bitSizeOf(view.container(), endBitPosition);
    endBitPosition += View<::pmr::RouteSegment>::ZserioArrayType_packedContainer(view, data.packedContainer)
                              .bitSizeOfPacked(endBitPosition);
    if (view.hasEndOffset())
    {
        endBitPosition = ::zserio::alignTo(8, endBitPosition);
        data.myOffset = static_cast<uint32_t>(endBitPosition / 8);
        endBitPosition += detail::bitSizeOf(*view.endPositionWithOffset(), endBitPosition);
    }
    endBitPosition +=
            View<::pmr::RouteSegment>::ZserioArrayType_myArray(data.myArray).bitSizeOfPacked(endBitPosition);
    endBitPosition += detail::bitSizeOf(view.myChoice(), endBitPosition);
    endBitPosition += detail::bitSizeOf(view.myUnion(), endBitPosition);

    return endBitPosition;
}

} // namespace detail

} // namespace zserio

namespace std
{

size_t hash<::pmr::RouteSegment>::operator()(const ::pmr::RouteSegment& data) const
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

    return result;
}

size_t hash<::zserio::View<::pmr::RouteSegment>>::operator()(
        const ::zserio::View<::pmr::RouteSegment>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;

    result = ::zserio::calcHashCode(result, view.hasEndOffset());
    result = ::zserio::calcHashCode(result, view.shift());
    result = ::zserio::calcHashCode(result, view.myOffset());
    result = ::zserio::calcHashCode(result, view.container());
    if (view.hasEndOffset())
    {
        result = ::zserio::calcHashCode(result, view.endPositionWithOffset());
    }
    result = ::zserio::calcHashCode(result, view.myArray());
    result = ::zserio::calcHashCode(result, view.myChoice());
    result = ::zserio::calcHashCode(result, view.myUnion());

    return result;
}

} // namespace std
