/**
 * Automatically generated by Zserio C++ generator version 1.0.1 using Zserio core 2.14.0.
 * Generator setup: writerCode, pubsubCode, serviceCode, sqlCode, stdAllocator.
 */

#include "SimpleUnion.h"

SimpleUnion::SimpleUnion() noexcept :
        SimpleUnion(allocator_type())
{}

SimpleUnion::SimpleUnion(const allocator_type& allocator) noexcept
//: objectChoice(allocator)
{}

bool operator==(const SimpleUnion& lhs, const SimpleUnion& rhs)
{
    return lhs.objectChoice == rhs.objectChoice;
}

bool operator<(const SimpleUnion& lhs, const SimpleUnion& rhs)
{
    return lhs.objectChoice < rhs.objectChoice;
}

bool operator!=(const SimpleUnion& lhs, const SimpleUnion& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const SimpleUnion& lhs, const SimpleUnion& rhs)
{
    return rhs < lhs;
}

bool operator<=(const SimpleUnion& lhs, const SimpleUnion& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const SimpleUnion& lhs, const SimpleUnion& rhs)
{
    return !(lhs < rhs);
}

namespace zserio
{

View<SimpleUnion>::View(const SimpleUnion& data) noexcept :
        m_data(data)
{}

SimpleUnion::ChoiceTag View<SimpleUnion>::index() const
{
    return static_cast<SimpleUnion::ChoiceTag>(m_data.objectChoice.index());
}

::zserio::String View<SimpleUnion>::valueA() const
{
    return m_data.objectChoice.get<SimpleUnion::VALUE_A_IDX>();
}

::zserio::Int8 View<SimpleUnion>::valueB() const
{
    return m_data.objectChoice.get<SimpleUnion::VALUE_B_IDX>();
}

bool operator==(const View<SimpleUnion>& lhs, const View<SimpleUnion>& rhs)
{
    if (lhs.index() != rhs.index())
        return false;

    switch (lhs.index())
    {
    case 0:
        return lhs.valueA() == rhs.valueA();
    default:
        return lhs.valueB() == rhs.valueB();
    }
}

bool operator<(const View<SimpleUnion>& lhs, const View<SimpleUnion>& rhs)
{
    if (lhs.index() < rhs.index())
        return true;
    if (rhs.index() < lhs.index())
        return false;

    switch (lhs.index())
    {
    case 0:
        return lhs.valueA() < rhs.valueA();
    default:
        return lhs.valueB() < rhs.valueB();
    }
}

bool operator!=(const View<SimpleUnion>& lhs, const View<SimpleUnion>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<SimpleUnion>& lhs, const View<SimpleUnion>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<SimpleUnion>& lhs, const View<SimpleUnion>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<SimpleUnion>& lhs, const View<SimpleUnion>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<SimpleUnion>& view)
{}

template <>
void write(::zserio::BitStreamWriter& writer, const View<SimpleUnion>& view)
{
    detail::write(writer, ::zserio::VarSize(::zserio::convertSizeToUInt32(view.index())));

    switch (view.index())
    {
    case 0:
        detail::write(writer, view.valueA());
        break;
    default:
        detail::write(writer, view.valueB());
        break;
    }
}

template <>
View<SimpleUnion> read(
        ::zserio::BitStreamReader& reader, SimpleUnion& data, const SimpleUnion::allocator_type& allocator)
{
    View<SimpleUnion> view(data);

    ::zserio::VarSize index;
    detail::read(reader, index);

    switch (index)
    {
    case 0:
        data.objectChoice.emplace<SimpleUnion::VALUE_A_IDX>();
        detail::read(reader, data.objectChoice.get<SimpleUnion::VALUE_A_IDX>(), allocator);
        break;
    default:
        data.objectChoice.emplace<SimpleUnion::VALUE_B_IDX>();
        detail::read(reader, data.objectChoice.get<SimpleUnion::VALUE_B_IDX>());
        break;
    }

    return view;
}

template <>
size_t bitSizeOf(const View<SimpleUnion>& view, size_t bitPosition)
{
    size_t endBitPosition = bitPosition;

    endBitPosition +=
            detail::bitSizeOf(::zserio::VarSize(::zserio::convertSizeToUInt32(view.index())), endBitPosition);

    switch (view.index())
    {
    case 0:
        endBitPosition += detail::bitSizeOf(view.valueA(), endBitPosition);
        break;
    default:
        endBitPosition += detail::bitSizeOf(view.valueB(), endBitPosition);
        break;
    }

    return endBitPosition - bitPosition;
}

} // namespace detail

} // namespace zserio

namespace std
{

size_t hash<SimpleUnion>::operator()(const SimpleUnion& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.objectChoice);
    return result;
}

size_t hash<::zserio::View<SimpleUnion>>::operator()(const ::zserio::View<SimpleUnion>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;

    result = ::zserio::calcHashCode(result, static_cast<size_t>(view.index()));

    switch (view.index())
    {
    case 0:
        result = ::zserio::calcHashCode(result, view.valueA());
        break;
    default:
        result = ::zserio::calcHashCode(result, view.valueB());
        break;
    }
    return result;
}

} // namespace std