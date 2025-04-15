/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_NESTED_H
#define TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_NESTED_H

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
#include <zserio/Types.h>

namespace test_object
{
namespace std_allocator
{

struct ReflectableNested
{
    using AllocatorType = ::std::allocator<uint8_t>;

    ReflectableNested() noexcept;
    explicit ReflectableNested(const AllocatorType& allocator) noexcept;

    explicit ReflectableNested(
            ::zserio::UInt31 value_);

    ::zserio::UInt31 value;
};

bool operator==(const ::test_object::std_allocator::ReflectableNested& lhs, const ::test_object::std_allocator::ReflectableNested& rhs);
bool operator<(const ::test_object::std_allocator::ReflectableNested& lhs, const ::test_object::std_allocator::ReflectableNested& rhs);
bool operator!=(const ::test_object::std_allocator::ReflectableNested& lhs, const ::test_object::std_allocator::ReflectableNested& rhs);
bool operator>(const ::test_object::std_allocator::ReflectableNested& lhs, const ::test_object::std_allocator::ReflectableNested& rhs);
bool operator<=(const ::test_object::std_allocator::ReflectableNested& lhs, const ::test_object::std_allocator::ReflectableNested& rhs);
bool operator>=(const ::test_object::std_allocator::ReflectableNested& lhs, const ::test_object::std_allocator::ReflectableNested& rhs);

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_allocator::ReflectableNested>
{
public:
    explicit View(const ::test_object::std_allocator::ReflectableNested& data,
            ::zserio::Int31 dummyParam_,
            ::std::string_view stringParam_) noexcept;

    ::zserio::Int31 dummyParam() const;
    ::std::string_view stringParam() const;

    ::zserio::UInt31 value() const;

    ::zserio::UInt31 getValue() const;

    const ::test_object::std_allocator::ReflectableNested& zserioData() const;

protected:
    View(const ::test_object::std_allocator::ReflectableNested& data, const View& other) noexcept;

private:
    const ::test_object::std_allocator::ReflectableNested* m_data;
    ::zserio::Int31 m_dummyParam_;
    ::std::string_view m_stringParam_;
};

bool operator==(const View<::test_object::std_allocator::ReflectableNested>& lhs, const View<::test_object::std_allocator::ReflectableNested>& rhs);
bool operator<(const View<::test_object::std_allocator::ReflectableNested>& lhs, const View<::test_object::std_allocator::ReflectableNested>& rhs);
bool operator!=(const View<::test_object::std_allocator::ReflectableNested>& lhs, const View<::test_object::std_allocator::ReflectableNested>& rhs);
bool operator>(const View<::test_object::std_allocator::ReflectableNested>& lhs, const View<::test_object::std_allocator::ReflectableNested>& rhs);
bool operator<=(const View<::test_object::std_allocator::ReflectableNested>& lhs, const View<::test_object::std_allocator::ReflectableNested>& rhs);
bool operator>=(const View<::test_object::std_allocator::ReflectableNested>& lhs, const View<::test_object::std_allocator::ReflectableNested>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::ReflectableNested>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::ReflectableNested>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::ReflectableNested>& view);

template <>
View<::test_object::std_allocator::ReflectableNested> read(BitStreamReader& reader, ::test_object::std_allocator::ReflectableNested& data,
        ::zserio::Int31 dummyParam_,
        ::std::string_view stringParam_);

template <>
struct TypeInfo<::test_object::std_allocator::ReflectableNested, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(const ::test_object::std_allocator::ReflectableNested& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::ReflectableNested& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_allocator::ReflectableNested>& view, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::ReflectableNested>
{
    size_t operator()(const ::test_object::std_allocator::ReflectableNested& data) const;
};

template <>
struct hash<::zserio::View<::test_object::std_allocator::ReflectableNested>>
{
    size_t operator()(const ::zserio::View<::test_object::std_allocator::ReflectableNested>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_NESTED_H
