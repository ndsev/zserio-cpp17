/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UNION_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UNION_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <memory>

#include <zserio/pmr/Variant.h>
#include <zserio/ChoiceTag.h>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/View.h>
#include <zserio/Types.h>
#include <zserio/pmr/String.h>

namespace test_object
{
namespace polymorphic_allocator
{

// forward declaration
struct ReflectableUnion;

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct ChoiceTag<::test_object::polymorphic_allocator::ReflectableUnion>
{
    enum Tag : size_t
    {
        UNDEFINED_CHOICE,
        CHOICE_value32,
        CHOICE_valueStr
    };
};

} // namespace detail
} // namespace zserio

namespace test_object
{
namespace polymorphic_allocator
{

struct ReflectableUnion : ::zserio::pmr::Variant<::zserio::detail::ChoiceTag<ReflectableUnion>::Tag,
        ::std::monostate,
        ::zserio::UInt32,
        ::zserio::pmr::String>
{
    using ChoiceTag = ::zserio::detail::ChoiceTag<ReflectableUnion>::Tag;
    using Base = ::zserio::pmr::Variant<ChoiceTag,
            ::std::monostate,
            ::zserio::UInt32,
            ::zserio::pmr::String>;
    using Base::BasicVariant;
};

bool operator==(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs);
bool operator<(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs);
bool operator!=(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs);
bool operator>(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs);
bool operator<=(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs);
bool operator>=(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs);

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::polymorphic_allocator::ReflectableUnion>
{
public:
    explicit View(const ::test_object::polymorphic_allocator::ReflectableUnion& data) noexcept;

    ::zserio::UInt32 value32() const;
    ::std::string_view valueStr() const;

    ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag zserioChoiceTag() const;
    const ::test_object::polymorphic_allocator::ReflectableUnion& zserioData() const;

protected:
    View(const ::test_object::polymorphic_allocator::ReflectableUnion& data, const View& other) noexcept;

private:
    const ::test_object::polymorphic_allocator::ReflectableUnion* m_data;
};

bool operator==(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs);
bool operator<(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs);
bool operator!=(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs);
bool operator>(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs);
bool operator<=(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs);
bool operator>=(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::ReflectableUnion>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::ReflectableUnion>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::ReflectableUnion>& view);

template <>
View<::test_object::polymorphic_allocator::ReflectableUnion> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::ReflectableUnion& data);

template <>
struct TypeInfo<::test_object::polymorphic_allocator::ReflectableUnion, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::ReflectableUnion& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableUnion& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::ReflectableUnion>
{
    size_t operator()(const ::test_object::polymorphic_allocator::ReflectableUnion& value) const;
};

template <>
struct hash<::zserio::View<::test_object::polymorphic_allocator::ReflectableUnion>>
{
    size_t operator()(const ::zserio::View<::test_object::polymorphic_allocator::ReflectableUnion>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UNION_H
