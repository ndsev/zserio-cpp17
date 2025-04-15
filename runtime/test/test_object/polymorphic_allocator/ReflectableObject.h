/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_OBJECT_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_OBJECT_H

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
#include <zserio/Types.h>
#include <zserio/pmr/String.h>

#include <test_object/polymorphic_allocator/ReflectableChoice.h>
#include <test_object/polymorphic_allocator/ReflectableEnum.h>
#include <test_object/polymorphic_allocator/ReflectableNested.h>
#include <test_object/polymorphic_allocator/ReflectableUnion.h>

namespace test_object
{
namespace polymorphic_allocator
{

struct ReflectableObject
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    ReflectableObject() noexcept;
    explicit ReflectableObject(const AllocatorType& allocator) noexcept;

    explicit ReflectableObject(
            ::zserio::pmr::String stringField_,
            ::test_object::polymorphic_allocator::ReflectableNested reflectableNested_,
            ::test_object::polymorphic_allocator::ReflectableEnum reflectableEnum_,
            ::test_object::polymorphic_allocator::ReflectableChoice reflectableChoice_,
            ::test_object::polymorphic_allocator::ReflectableUnion reflectableUnion_);

    ::zserio::pmr::String stringField;
    ::test_object::polymorphic_allocator::ReflectableNested reflectableNested;
    ::test_object::polymorphic_allocator::ReflectableEnum reflectableEnum;
    ::test_object::polymorphic_allocator::ReflectableChoice reflectableChoice;
    ::test_object::polymorphic_allocator::ReflectableUnion reflectableUnion;
};

bool operator==(const ::test_object::polymorphic_allocator::ReflectableObject& lhs, const ::test_object::polymorphic_allocator::ReflectableObject& rhs);
bool operator<(const ::test_object::polymorphic_allocator::ReflectableObject& lhs, const ::test_object::polymorphic_allocator::ReflectableObject& rhs);
bool operator!=(const ::test_object::polymorphic_allocator::ReflectableObject& lhs, const ::test_object::polymorphic_allocator::ReflectableObject& rhs);
bool operator>(const ::test_object::polymorphic_allocator::ReflectableObject& lhs, const ::test_object::polymorphic_allocator::ReflectableObject& rhs);
bool operator<=(const ::test_object::polymorphic_allocator::ReflectableObject& lhs, const ::test_object::polymorphic_allocator::ReflectableObject& rhs);
bool operator>=(const ::test_object::polymorphic_allocator::ReflectableObject& lhs, const ::test_object::polymorphic_allocator::ReflectableObject& rhs);

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::polymorphic_allocator::ReflectableObject>
{
public:
    explicit View(const ::test_object::polymorphic_allocator::ReflectableObject& data) noexcept;

    ::std::string_view stringField() const;
    View<::test_object::polymorphic_allocator::ReflectableNested> reflectableNested() const;
    ::test_object::polymorphic_allocator::ReflectableEnum reflectableEnum() const;
    View<::test_object::polymorphic_allocator::ReflectableChoice> reflectableChoice() const;
    View<::test_object::polymorphic_allocator::ReflectableUnion> reflectableUnion() const;

    const ::test_object::polymorphic_allocator::ReflectableObject& zserioData() const;

protected:
    View(const ::test_object::polymorphic_allocator::ReflectableObject& data, const View& other) noexcept;

private:
    const ::test_object::polymorphic_allocator::ReflectableObject* m_data;
};

bool operator==(const View<::test_object::polymorphic_allocator::ReflectableObject>& lhs, const View<::test_object::polymorphic_allocator::ReflectableObject>& rhs);
bool operator<(const View<::test_object::polymorphic_allocator::ReflectableObject>& lhs, const View<::test_object::polymorphic_allocator::ReflectableObject>& rhs);
bool operator!=(const View<::test_object::polymorphic_allocator::ReflectableObject>& lhs, const View<::test_object::polymorphic_allocator::ReflectableObject>& rhs);
bool operator>(const View<::test_object::polymorphic_allocator::ReflectableObject>& lhs, const View<::test_object::polymorphic_allocator::ReflectableObject>& rhs);
bool operator<=(const View<::test_object::polymorphic_allocator::ReflectableObject>& lhs, const View<::test_object::polymorphic_allocator::ReflectableObject>& rhs);
bool operator>=(const View<::test_object::polymorphic_allocator::ReflectableObject>& lhs, const View<::test_object::polymorphic_allocator::ReflectableObject>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::ReflectableObject>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::ReflectableObject>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::ReflectableObject>& view);

template <>
View<::test_object::polymorphic_allocator::ReflectableObject> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::ReflectableObject& data);

template <>
struct TypeInfo<::test_object::polymorphic_allocator::ReflectableObject, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::ReflectableObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::polymorphic_allocator::ReflectableObject>& view, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::ReflectableObject>
{
    size_t operator()(const ::test_object::polymorphic_allocator::ReflectableObject& data) const;
};

template <>
struct hash<::zserio::View<::test_object::polymorphic_allocator::ReflectableObject>>
{
    size_t operator()(const ::zserio::View<::test_object::polymorphic_allocator::ReflectableObject>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_OBJECT_H
