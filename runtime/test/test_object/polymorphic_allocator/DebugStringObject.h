/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_DEBUG_STRING_OBJECT_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_DEBUG_STRING_OBJECT_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <memory>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/pmr/IIntrospectableView.h>
#include <zserio/View.h>
#include <zserio/pmr/PropagatingPolymorphicAllocator.h>
#include <zserio/pmr/String.h>

namespace test_object
{
namespace polymorphic_allocator
{

struct DebugStringObject
{
    using allocator_type = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    DebugStringObject() noexcept;
    explicit DebugStringObject(const allocator_type& allocator) noexcept;

    DebugStringObject(DebugStringObject&&) = default;
    DebugStringObject(DebugStringObject&& other_, const allocator_type& allocator);

    DebugStringObject(const DebugStringObject&) = default;
    DebugStringObject(const DebugStringObject& other_, const allocator_type& allocator);

    DebugStringObject& operator=(DebugStringObject&&) = default;
    DebugStringObject& operator=(const DebugStringObject&) = default;

    ~DebugStringObject() = default;

    explicit DebugStringObject(
            ::zserio::pmr::String text_,
            const allocator_type& allocator = {});

    ::zserio::pmr::String text;
};

bool operator==(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs);
bool operator<(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs);
bool operator!=(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs);
bool operator>(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs);
bool operator<=(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs);
bool operator>=(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs);

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::polymorphic_allocator::DebugStringObject>
{
public:
    explicit View(const ::test_object::polymorphic_allocator::DebugStringObject& data) noexcept;

    ::std::string_view text() const;

    const ::test_object::polymorphic_allocator::DebugStringObject& zserioData() const;

protected:
    View(const ::test_object::polymorphic_allocator::DebugStringObject& data, const View& other) noexcept;

private:
    const ::test_object::polymorphic_allocator::DebugStringObject* m_data;
};

bool operator==(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs);
bool operator<(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs);
bool operator!=(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs);
bool operator>(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs);
bool operator<=(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs);
bool operator>=(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::DebugStringObject>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::DebugStringObject>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::DebugStringObject>& view);

template <>
View<::test_object::polymorphic_allocator::DebugStringObject> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::DebugStringObject& data);

template <>
struct TypeInfo<::test_object::polymorphic_allocator::DebugStringObject, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::DebugStringObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::DebugStringObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::polymorphic_allocator::DebugStringObject>& view, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::DebugStringObject>
{
    size_t operator()(const ::test_object::polymorphic_allocator::DebugStringObject& data) const;
};

template <>
struct hash<::zserio::View<::test_object::polymorphic_allocator::DebugStringObject>>
{
    size_t operator()(const ::zserio::View<::test_object::polymorphic_allocator::DebugStringObject>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_DEBUG_STRING_OBJECT_H
