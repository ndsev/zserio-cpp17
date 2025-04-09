/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, std-polymorphicAllocator.
 */

#ifndef TEST_OBJECT_STD_POLYMORPHIC_ALLOCATOR_WALKER_NESTED_H
#define TEST_OBJECT_STD_POLYMORPHIC_ALLOCATOR_WALKER_NESTED_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <memory>
#include <zserio/std_pmr/ITypeInfo.h>
#include <zserio/std_pmr/IReflectableData.h>
#include <zserio/std_pmr/IIntrospectableView.h>
#include <zserio/View.h>
#include <memory_resource>
#include <zserio/std_pmr/String.h>

namespace test_object
{
namespace std_polymorphic_allocator
{

struct WalkerNested
{
    using allocator_type = ::std::pmr::polymorphic_allocator<uint8_t>;

    WalkerNested() noexcept;
    explicit WalkerNested(const allocator_type& allocator) noexcept;

    WalkerNested(WalkerNested&&) = default;
    WalkerNested(WalkerNested&& other_, const allocator_type& allocator);

    WalkerNested(const WalkerNested&) = default;
    WalkerNested(const WalkerNested& other_, const allocator_type& allocator);

    WalkerNested& operator=(WalkerNested&&) = default;
    WalkerNested& operator=(const WalkerNested&) = default;

    ~WalkerNested() = default;

    explicit WalkerNested(
            ::zserio::std_pmr::String text_,
            const allocator_type& allocator = {});

    ::zserio::std_pmr::String text;
};

bool operator==(const ::test_object::std_polymorphic_allocator::WalkerNested& lhs, const ::test_object::std_polymorphic_allocator::WalkerNested& rhs);
bool operator<(const ::test_object::std_polymorphic_allocator::WalkerNested& lhs, const ::test_object::std_polymorphic_allocator::WalkerNested& rhs);
bool operator!=(const ::test_object::std_polymorphic_allocator::WalkerNested& lhs, const ::test_object::std_polymorphic_allocator::WalkerNested& rhs);
bool operator>(const ::test_object::std_polymorphic_allocator::WalkerNested& lhs, const ::test_object::std_polymorphic_allocator::WalkerNested& rhs);
bool operator<=(const ::test_object::std_polymorphic_allocator::WalkerNested& lhs, const ::test_object::std_polymorphic_allocator::WalkerNested& rhs);
bool operator>=(const ::test_object::std_polymorphic_allocator::WalkerNested& lhs, const ::test_object::std_polymorphic_allocator::WalkerNested& rhs);

} // namespace std_polymorphic_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_polymorphic_allocator::WalkerNested>
{
public:
    explicit View(const ::test_object::std_polymorphic_allocator::WalkerNested& data) noexcept;

    ::std::string_view text() const;

    const ::test_object::std_polymorphic_allocator::WalkerNested& zserioData() const;

protected:
    View(const ::test_object::std_polymorphic_allocator::WalkerNested& data, const View& other) noexcept;

private:
    const ::test_object::std_polymorphic_allocator::WalkerNested* m_data;
};

bool operator==(const View<::test_object::std_polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::std_polymorphic_allocator::WalkerNested>& rhs);
bool operator<(const View<::test_object::std_polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::std_polymorphic_allocator::WalkerNested>& rhs);
bool operator!=(const View<::test_object::std_polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::std_polymorphic_allocator::WalkerNested>& rhs);
bool operator>(const View<::test_object::std_polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::std_polymorphic_allocator::WalkerNested>& rhs);
bool operator<=(const View<::test_object::std_polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::std_polymorphic_allocator::WalkerNested>& rhs);
bool operator>=(const View<::test_object::std_polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::std_polymorphic_allocator::WalkerNested>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::std_polymorphic_allocator::WalkerNested>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::std_polymorphic_allocator::WalkerNested>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_polymorphic_allocator::WalkerNested>& view);

template <>
View<::test_object::std_polymorphic_allocator::WalkerNested> read(BitStreamReader& reader, ::test_object::std_polymorphic_allocator::WalkerNested& data);

template <>
struct TypeInfo<::test_object::std_polymorphic_allocator::WalkerNested, ::std::pmr::polymorphic_allocator<uint8_t>>
{
    static const ::zserio::std_pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::std_pmr::IReflectableDataConstPtr reflectable(const ::test_object::std_polymorphic_allocator::WalkerNested& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);

template <>
::zserio::std_pmr::IReflectableDataPtr reflectable(::test_object::std_polymorphic_allocator::WalkerNested& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);

template <>
::zserio::std_pmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_polymorphic_allocator::WalkerNested>& view, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_polymorphic_allocator::WalkerNested>
{
    size_t operator()(const ::test_object::std_polymorphic_allocator::WalkerNested& data) const;
};

template <>
struct hash<::zserio::View<::test_object::std_polymorphic_allocator::WalkerNested>>
{
    size_t operator()(const ::zserio::View<::test_object::std_polymorphic_allocator::WalkerNested>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_POLYMORPHIC_ALLOCATOR_WALKER_NESTED_H
