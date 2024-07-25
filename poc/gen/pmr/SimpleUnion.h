/**
 * Automatically generated by Zserio C++ generator version 1.0.1 using Zserio core 2.14.0.
 * Generator setup: writerCode, pubsubCode, serviceCode, sqlCode, stdAllocator.
 */

#ifndef BOOL_PMR_SIMPLE_UNION_H
#define BOOL_PMR_SIMPLE_UNION_H

#include <zserio/CppRuntimeVersion.h>
#if CPP_EXTENSION_RUNTIME_VERSION_NUMBER != 1000000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 1.0.1.
#endif

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/ArrayTraits.h>
#include <zserio/View.h>
#include <zserio/Variant.h>
#include <zserio/TypeWrappers.h>

namespace pmr
{

// It may be a good idea to inherit from zserio::Variant directly
// so that all useful function members are inherited too. Choice
// struct doesn't contain other fields anyway. Alternatively we need
// to generate at least the important members from zserio::Variant here
// in order for the user to skip typing .objectChoice which is just
// an implementation detail.
struct SimpleUnion
{
    using allocator_type = ::zserio::pmr::PropagatingPolymorphicAllocator<>;
    using needs_initialize_offsets = ::std::false_type;

    SimpleUnion() noexcept;

    explicit SimpleUnion(const allocator_type& allocator) noexcept;

    ::zserio::Variant<::zserio::String, ::zserio::Int8> objectChoice;
};

bool operator==(const SimpleUnion& lhs, const SimpleUnion& rhs);
bool operator<(const SimpleUnion& lhs, const SimpleUnion& rhs);
bool operator!=(const SimpleUnion& lhs, const SimpleUnion& rhs);
bool operator>(const SimpleUnion& lhs, const SimpleUnion& rhs);
bool operator<=(const SimpleUnion& lhs, const SimpleUnion& rhs);
bool operator>=(const SimpleUnion& lhs, const SimpleUnion& rhs);

} // namespace pmr

namespace zserio
{

template <>
class View<::pmr::SimpleUnion>
{
public:
    View(const ::pmr::SimpleUnion& data) noexcept;

    size_t index() const;

    ::zserio::String valueA() const;

    ::zserio::Int8 valueB() const;

private:
    const ::pmr::SimpleUnion& m_data;
};

bool operator==(const View<::pmr::SimpleUnion>& lhs, const View<::pmr::SimpleUnion>& rhs);
bool operator<(const View<::pmr::SimpleUnion>& lhs, const View<::pmr::SimpleUnion>& rhs);
bool operator!=(const View<::pmr::SimpleUnion>& lhs, const View<::pmr::SimpleUnion>& rhs);
bool operator>(const View<::pmr::SimpleUnion>& lhs, const View<::pmr::SimpleUnion>& rhs);
bool operator<=(const View<::pmr::SimpleUnion>& lhs, const View<::pmr::SimpleUnion>& rhs);
bool operator>=(const View<::pmr::SimpleUnion>& lhs, const View<::pmr::SimpleUnion>& rhs);

namespace detail
{

void validate(const View<::pmr::SimpleUnion>& view);

void write(::zserio::BitStreamWriter& writer, const View<::pmr::SimpleUnion>& view);

View<::pmr::SimpleUnion> read(::zserio::BitStreamReader& reader, ::pmr::SimpleUnion& data, const ::pmr::SimpleUnion::allocator_type& allocator = {});

size_t bitSizeOf(const View<::pmr::SimpleUnion>& view, size_t bitPosition);

} // namespace detail

} // namespace zserio

namespace std
{

template<>
struct hash<::pmr::SimpleUnion>
{
    size_t operator()(const ::pmr::SimpleUnion& data) const;
};

template<>
struct hash<::zserio::View<::pmr::SimpleUnion>>
{
    size_t operator()(const ::zserio::View<::pmr::SimpleUnion>& view) const;
};

} // namespace std

#endif // SIMPLE_UNION_H
