/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_WALKER_NESTED_H
#define TEST_OBJECT_STD_ALLOCATOR_WALKER_NESTED_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <memory>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectableData.h>
#include <zserio/IIntrospectableView.h>
#include <zserio/View.h>
#include <memory>
#include <zserio/String.h>

namespace test_object
{
namespace std_allocator
{

struct WalkerNested
{
    using AllocatorType = ::std::allocator<uint8_t>;

    WalkerNested() noexcept;
    explicit WalkerNested(const AllocatorType& allocator) noexcept;

    explicit WalkerNested(
            ::zserio::String text_);

    ::zserio::String text;
};

bool operator==(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs);
bool operator<(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs);
bool operator!=(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs);
bool operator>(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs);
bool operator<=(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs);
bool operator>=(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs);

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_allocator::WalkerNested>
{
public:
    explicit View(const ::test_object::std_allocator::WalkerNested& data) noexcept;

    ::std::string_view text() const;

    const ::test_object::std_allocator::WalkerNested& zserioData() const;

protected:
    View(const ::test_object::std_allocator::WalkerNested& data, const View& other) noexcept;

private:
    const ::test_object::std_allocator::WalkerNested* m_data;
};

bool operator==(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs);
bool operator<(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs);
bool operator!=(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs);
bool operator>(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs);
bool operator<=(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs);
bool operator>=(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::WalkerNested>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::WalkerNested>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::WalkerNested>& view);

template <>
View<::test_object::std_allocator::WalkerNested> read(BitStreamReader& reader, ::test_object::std_allocator::WalkerNested& data);

template <>
struct TypeInfo<::test_object::std_allocator::WalkerNested, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(const ::test_object::std_allocator::WalkerNested& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::WalkerNested& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_allocator::WalkerNested>& view, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::WalkerNested>
{
    size_t operator()(const ::test_object::std_allocator::WalkerNested& data) const;
};

template <>
struct hash<::zserio::View<::test_object::std_allocator::WalkerNested>>
{
    size_t operator()(const ::zserio::View<::test_object::std_allocator::WalkerNested>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_WALKER_NESTED_H
