/**
 * Automatically generated by Zserio C++17 generator version 0.5.0 using Zserio core 2.17.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_NESTED_H
#define TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_NESTED_H

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
    using allocator_type = ::std::allocator<uint8_t>;

    ReflectableNested() noexcept;
    explicit ReflectableNested(const allocator_type& allocator) noexcept;

    ReflectableNested(ReflectableNested&&) = default;
    ReflectableNested(ReflectableNested&& other, const allocator_type& allocator);

    ReflectableNested(const ReflectableNested&) = default;
    ReflectableNested(const ReflectableNested& other, const allocator_type& allocator);

    ReflectableNested& operator=(ReflectableNested&&) = default;
    ReflectableNested& operator=(const ReflectableNested&) = default;

    ~ReflectableNested() = default;

    explicit ReflectableNested(
            ::zserio::UInt31 value_,
            const allocator_type& allocator = {});

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
struct ObjectTraits<::test_object::std_allocator::ReflectableNested>
{
    using Parameters = std::tuple<
            ::zserio::Int31,
            ::zserio::String>;

    static void validate(const View<::test_object::std_allocator::ReflectableNested>& view, ::std::string_view fieldName);

    static BitSize bitSizeOf(const View<::test_object::std_allocator::ReflectableNested>& view, BitSize bitPosition);

    static void write(BitStreamWriter& writer, const View<::test_object::std_allocator::ReflectableNested>& view);

    static View<::test_object::std_allocator::ReflectableNested> read(BitStreamReader& reader, ::test_object::std_allocator::ReflectableNested& data,
            ::zserio::Int31 dummyParam_,
            ::std::string_view stringParam_);
};

template <>
struct TypeInfo<::test_object::std_allocator::ReflectableNested, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

template <>
struct Reflectable<::test_object::std_allocator::ReflectableNested, ::std::allocator<uint8_t>>
{
    static ::zserio::IReflectableDataConstPtr create(const ::test_object::std_allocator::ReflectableNested& value,
            const ::std::allocator<uint8_t>& allocator);

    static ::zserio::IReflectableDataPtr create(::test_object::std_allocator::ReflectableNested& value,
            const ::std::allocator<uint8_t>& allocator);
};

template <>
struct Introspectable<::test_object::std_allocator::ReflectableNested, ::std::allocator<uint8_t>>
{
    static ::zserio::IIntrospectableViewConstPtr create(const View<::test_object::std_allocator::ReflectableNested>& view,
            const ::std::allocator<uint8_t>& allocator);
};

} // namespace detail
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
