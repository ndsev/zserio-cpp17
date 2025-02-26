/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.14.1.
 * Generator setup: stdAllocator.
 */

#ifndef POSITION_CONTAINER_LINE_POSITION_OFFSET2_D_INT32_H
#define POSITION_CONTAINER_LINE_POSITION_OFFSET2_D_INT32_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>
#include <zserio/View.h>
#include <zserio/Array.h>
#include <zserio/Types.h>
#include <zserio/Vector.h>

#include <CoordShift.h>
#include <LinePositionOffset2D.h>

struct PositionContainer_LinePositionOffset2D_int32
{
    using AllocatorType = ::std::allocator<uint8_t>;

    PositionContainer_LinePositionOffset2D_int32() noexcept;
    explicit PositionContainer_LinePositionOffset2D_int32(const AllocatorType& allocator) noexcept;

    explicit PositionContainer_LinePositionOffset2D_int32(
        ::zserio::VarSize numElements_,
        ::zserio::Vector<LinePositionOffset2D> array_,
        ::zserio::Int32 field_) noexcept;

    ::zserio::VarSize numElements;
    ::zserio::Vector<LinePositionOffset2D> array;
    ::zserio::Int32 field;
};

bool operator==(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs);
bool operator<(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs);
bool operator!=(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs);
bool operator>(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs);
bool operator<=(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs);
bool operator>=(const PositionContainer_LinePositionOffset2D_int32& lhs, const PositionContainer_LinePositionOffset2D_int32& rhs);

namespace zserio
{

template <>
class View<PositionContainer_LinePositionOffset2D_int32>
{
public:
    struct ZserioArrayArrayTraits
    {
        using OwnerType = View<PositionContainer_LinePositionOffset2D_int32>;

        static View<LinePositionOffset2D> at(const OwnerType& owner,
                const LinePositionOffset2D& element, size_t)
        {
            return View<LinePositionOffset2D>(element, CoordShift(static_cast<CoordShift::ValueType>(owner.shift())));
        }

        static void read(BitStreamReader& reader,
                const OwnerType& owner, LinePositionOffset2D& element, size_t)
        {
            (void)detail::read(reader, element, CoordShift(static_cast<CoordShift::ValueType>(owner.shift())));
        }
    };

    explicit View(const PositionContainer_LinePositionOffset2D_int32& data,
            CoordShift shift_) noexcept;

    CoordShift shift() const;

    ::zserio::VarSize numElements() const;
    Array<::zserio::Vector<LinePositionOffset2D>, ArrayType::NORMAL, ZserioArrayArrayTraits> array() const;
    ::zserio::Int32 field() const;

private:
    const PositionContainer_LinePositionOffset2D_int32& m_data;
    CoordShift m_shift_;
};

bool operator==(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs);
bool operator<(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs);
bool operator!=(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs);
bool operator>(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs);
bool operator<=(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs);
bool operator>=(const View<PositionContainer_LinePositionOffset2D_int32>& lhs, const View<PositionContainer_LinePositionOffset2D_int32>& rhs);

namespace detail
{

template <>
void validate(const View<PositionContainer_LinePositionOffset2D_int32>& view);

template <>
BitSize bitSizeOf(const View<PositionContainer_LinePositionOffset2D_int32>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<PositionContainer_LinePositionOffset2D_int32>& view);

template <>
View<PositionContainer_LinePositionOffset2D_int32> read(BitStreamReader& reader, PositionContainer_LinePositionOffset2D_int32& data,
        CoordShift shift_);

} // namespace detail

} // namespace zserio

namespace std
{

template <>
struct hash<PositionContainer_LinePositionOffset2D_int32>
{
    size_t operator()(const PositionContainer_LinePositionOffset2D_int32& data) const;
};

template <>
struct hash<::zserio::View<PositionContainer_LinePositionOffset2D_int32>>
{
    size_t operator()(const ::zserio::View<PositionContainer_LinePositionOffset2D_int32>& view) const;
};

} // namespace std

#endif // POSITION_CONTAINER_LINE_POSITION_OFFSET2_D_INT32_H
