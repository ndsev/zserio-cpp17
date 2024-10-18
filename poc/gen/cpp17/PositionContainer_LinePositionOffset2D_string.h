/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.14.1.
 * Generator setup: stdAllocator.
 */

#ifndef POSITION_CONTAINER_LINE_POSITION_OFFSET2_D_STRING_H
#define POSITION_CONTAINER_LINE_POSITION_OFFSET2_D_STRING_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>
#include <zserio/DeltaContext.h>
#include <zserio/View.h>
#include <zserio/Array.h>
#include <zserio/String.h>
#include <zserio/Types.h>
#include <zserio/Vector.h>

#include <CoordShift.h>
#include <LinePositionOffset2D.h>

struct PositionContainer_LinePositionOffset2D_string
{
    using AllocatorType = ::std::allocator<uint8_t>;

    PositionContainer_LinePositionOffset2D_string() noexcept;
    explicit PositionContainer_LinePositionOffset2D_string(const AllocatorType& allocator) noexcept;

    explicit PositionContainer_LinePositionOffset2D_string(
        ::zserio::VarSize numElements_,
        ::zserio::Vector<LinePositionOffset2D> array_,
        ::zserio::String field_) noexcept;

    ::zserio::VarSize numElements;
    ::zserio::Vector<LinePositionOffset2D> array;
    ::zserio::String field;
};

bool operator==(const PositionContainer_LinePositionOffset2D_string& lhs, const PositionContainer_LinePositionOffset2D_string& rhs);
bool operator<(const PositionContainer_LinePositionOffset2D_string& lhs, const PositionContainer_LinePositionOffset2D_string& rhs);
bool operator!=(const PositionContainer_LinePositionOffset2D_string& lhs, const PositionContainer_LinePositionOffset2D_string& rhs);
bool operator>(const PositionContainer_LinePositionOffset2D_string& lhs, const PositionContainer_LinePositionOffset2D_string& rhs);
bool operator<=(const PositionContainer_LinePositionOffset2D_string& lhs, const PositionContainer_LinePositionOffset2D_string& rhs);
bool operator>=(const PositionContainer_LinePositionOffset2D_string& lhs, const PositionContainer_LinePositionOffset2D_string& rhs);

namespace zserio
{

template <>
class View<PositionContainer_LinePositionOffset2D_string>
{
public:
    struct ZserioArrayArrayTraits
    {
        using OwnerType = View<PositionContainer_LinePositionOffset2D_string>;

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
        static void read(detail::PackingContext<LinePositionOffset2D>& packingContext, BitStreamReader& reader,
                const OwnerType& owner, LinePositionOffset2D& element, size_t)
        {
            detail::read(packingContext, reader, element, CoordShift(static_cast<CoordShift::ValueType>(owner.shift())));
        }
    };

    explicit View(const PositionContainer_LinePositionOffset2D_string& data,
            CoordShift shift_) noexcept;

    CoordShift shift() const;

    ::zserio::VarSize numElements() const;
    Array<::zserio::Vector<LinePositionOffset2D>, ArrayType::NORMAL, ZserioArrayArrayTraits> array() const;
    ::std::string_view field() const;

private:
    const PositionContainer_LinePositionOffset2D_string& m_data;
    CoordShift m_shift_;
};

bool operator==(const View<PositionContainer_LinePositionOffset2D_string>& lhs, const View<PositionContainer_LinePositionOffset2D_string>& rhs);
bool operator<(const View<PositionContainer_LinePositionOffset2D_string>& lhs, const View<PositionContainer_LinePositionOffset2D_string>& rhs);
bool operator!=(const View<PositionContainer_LinePositionOffset2D_string>& lhs, const View<PositionContainer_LinePositionOffset2D_string>& rhs);
bool operator>(const View<PositionContainer_LinePositionOffset2D_string>& lhs, const View<PositionContainer_LinePositionOffset2D_string>& rhs);
bool operator<=(const View<PositionContainer_LinePositionOffset2D_string>& lhs, const View<PositionContainer_LinePositionOffset2D_string>& rhs);
bool operator>=(const View<PositionContainer_LinePositionOffset2D_string>& lhs, const View<PositionContainer_LinePositionOffset2D_string>& rhs);

namespace detail
{

template <>
void validate(const View<PositionContainer_LinePositionOffset2D_string>& view);

template <>
BitSize bitSizeOf(const View<PositionContainer_LinePositionOffset2D_string>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<PositionContainer_LinePositionOffset2D_string>& view);

template <>
View<PositionContainer_LinePositionOffset2D_string> read(BitStreamReader& reader, PositionContainer_LinePositionOffset2D_string& data,
        CoordShift shift_);

template <>
struct PackingContext<PositionContainer_LinePositionOffset2D_string>
{
    DeltaContext numElements;
};

template <>
void initContext(PackingContext<PositionContainer_LinePositionOffset2D_string>& packingContext, const View<PositionContainer_LinePositionOffset2D_string>& view);

template <>
BitSize bitSizeOf(PackingContext<PositionContainer_LinePositionOffset2D_string>& packingContext, const View<PositionContainer_LinePositionOffset2D_string>& view,
        BitSize bitPosition);

template <>
void write(PackingContext<PositionContainer_LinePositionOffset2D_string>& packingContext, BitStreamWriter& writer, const View<PositionContainer_LinePositionOffset2D_string>& view);

template <>
void read(PackingContext<PositionContainer_LinePositionOffset2D_string>& packingContext, BitStreamReader& reader, PositionContainer_LinePositionOffset2D_string& data,
        CoordShift shift_);

} // namespace detail

} // namespace zserio

namespace std
{

template <>
struct hash<PositionContainer_LinePositionOffset2D_string>
{
    size_t operator()(const PositionContainer_LinePositionOffset2D_string& data) const;
};

template <>
struct hash<::zserio::View<PositionContainer_LinePositionOffset2D_string>>
{
    size_t operator()(const ::zserio::View<PositionContainer_LinePositionOffset2D_string>& view) const;
};

} // namespace std

#endif // POSITION_CONTAINER_LINE_POSITION_OFFSET2_D_STRING_H