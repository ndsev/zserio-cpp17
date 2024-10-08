/**
 * Automatically generated by Zserio C++ generator version 1.0.0 using Zserio core 2.13.0.
 * Generator setup: writerCode, pubsubCode, serviceCode, sqlCode, stdAllocator.
 */

#ifndef POSITION_OFFSET2_D_H
#define POSITION_OFFSET2_D_H

#include <zserio/CppRuntimeVersion.h>
#if CPP_EXTENSION_RUNTIME_VERSION_NUMBER != 1000000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 1.0.0.
#endif

#include <CoordShift.h>
#include <CoordWidth.h>
#include <DeltaLatitude.h>
#include <DeltaLongitude.h>
#include <zserio/ArrayTraits.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/TypeWrappers.h>
#include <zserio/View.h>

struct PositionOffset2D
{
    using allocator_type = ::std::allocator<uint8_t>;
    using needs_initialize_offsets = ::std::false_type;

    PositionOffset2D() noexcept;

    explicit PositionOffset2D(const allocator_type&);

    PositionOffset2D(::zserio::DynInt32 deltaLongitude_, ::zserio::DynInt32 deltaLatitude_);

    ::zserio::DynInt32 deltaLongitude;
    ::zserio::DynInt32 deltaLatitude;
};

bool operator==(const PositionOffset2D& lhs, const PositionOffset2D& rhs);
bool operator<(const PositionOffset2D& lhs, const PositionOffset2D& rhs);
bool operator!=(const PositionOffset2D& lhs, const PositionOffset2D& rhs);
bool operator>(const PositionOffset2D& lhs, const PositionOffset2D& rhs);
bool operator<=(const PositionOffset2D& lhs, const PositionOffset2D& rhs);
bool operator>=(const PositionOffset2D& lhs, const PositionOffset2D& rhs);

namespace zserio
{

template <>
class View<PositionOffset2D>
{
public:
    View(const PositionOffset2D& data, CoordWidth numBits_, CoordShift shift_) noexcept;

    CoordWidth numBits() const;
    CoordShift shift() const;

    ::zserio::DynInt32 deltaLongitude() const;
    ::zserio::DynInt32 deltaLatitude() const;

    DeltaLongitude lon() const;
    DeltaLatitude lat() const;

private:
    const PositionOffset2D& m_data;
    CoordWidth m_numBits_;
    CoordShift m_shift_;
};

bool operator==(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs);
bool operator<(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs);
bool operator!=(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs);
bool operator>(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs);
bool operator<=(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs);
bool operator>=(const View<PositionOffset2D>& lhs, const View<PositionOffset2D>& rhs);

namespace detail
{

template <>
void validate(const View<PositionOffset2D>& view);

template <>
void write(::zserio::BitStreamWriter& writer, const View<PositionOffset2D>& view);

View<PositionOffset2D> readImpl(::zserio::BitStreamReader& reader, PositionOffset2D& data,
        const PositionOffset2D::allocator_type& allocator, CoordWidth numBits_, CoordShift shift_);

template <typename... ARGS>
View<PositionOffset2D> read(::zserio::BitStreamReader& reader, PositionOffset2D& data,
        const PositionOffset2D::allocator_type& allocator, ARGS&&... args)
{
    return readImpl(reader, data, allocator, ::std::forward<ARGS>(args)...);
}

template <>
size_t bitSizeOf(const View<PositionOffset2D>& view, size_t bitPosition);

} // namespace detail

} // namespace zserio

namespace std
{

template <>
struct hash<PositionOffset2D>
{
    size_t operator()(const PositionOffset2D& data) const;
};

template <>
struct hash<::zserio::View<PositionOffset2D>>
{
    size_t operator()(const ::zserio::View<PositionOffset2D>& view) const;
};

} // namespace std

#endif // POSITION_OFFSET2_D_H
