/**
 * Automatically generated by Zserio C++17 generator version 0.5.0 using Zserio core 2.17.0.
 * Generator setup: typeInfoCode, pmrAllocator.
 */

#ifndef TEST_OBJECT_PMR_ALLOCATOR_REFLECTABLE_UNION_H
#define TEST_OBJECT_PMR_ALLOCATOR_REFLECTABLE_UNION_H

#include <memory>

#include <zserio/pmr/Variant.h>
#include <zserio/ChoiceTag.h>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/pmr/IIntrospectableView.h>
#include <zserio/View.h>
#include <zserio/Types.h>
#include <zserio/pmr/String.h>

namespace test_object
{
namespace pmr_allocator
{

// forward declaration
struct ReflectableUnion;

} // namespace pmr_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct ChoiceTag<::test_object::pmr_allocator::ReflectableUnion>
{
    enum Tag : size_t
    {
        ZSERIO_UNDEFINED,
        value32,
        valueStr
    };
};

} // namespace detail
} // namespace zserio

namespace test_object
{
namespace pmr_allocator
{

struct ReflectableUnion : ::zserio::pmr::Variant<::zserio::detail::ChoiceTag<ReflectableUnion>::Tag,
        ::std::monostate,
        ::zserio::UInt32,
        ::zserio::pmr::String>
{
    using Tag = ::zserio::detail::ChoiceTag<ReflectableUnion>::Tag;
    using Base = ::zserio::pmr::Variant<Tag,
            ::std::monostate,
            ::zserio::UInt32,
            ::zserio::pmr::String>;
    using Base::Base;
};

bool operator==(const ::test_object::pmr_allocator::ReflectableUnion& lhs, const ::test_object::pmr_allocator::ReflectableUnion& rhs);
bool operator<(const ::test_object::pmr_allocator::ReflectableUnion& lhs, const ::test_object::pmr_allocator::ReflectableUnion& rhs);
bool operator!=(const ::test_object::pmr_allocator::ReflectableUnion& lhs, const ::test_object::pmr_allocator::ReflectableUnion& rhs);
bool operator>(const ::test_object::pmr_allocator::ReflectableUnion& lhs, const ::test_object::pmr_allocator::ReflectableUnion& rhs);
bool operator<=(const ::test_object::pmr_allocator::ReflectableUnion& lhs, const ::test_object::pmr_allocator::ReflectableUnion& rhs);
bool operator>=(const ::test_object::pmr_allocator::ReflectableUnion& lhs, const ::test_object::pmr_allocator::ReflectableUnion& rhs);

} // namespace pmr_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::pmr_allocator::ReflectableUnion>
{
public:
    explicit View(const ::test_object::pmr_allocator::ReflectableUnion& data) noexcept;

    ::zserio::UInt32 value32() const;
    ::std::string_view valueStr() const;

    ::test_object::pmr_allocator::ReflectableUnion::Tag zserioChoiceTag() const;
    const ::test_object::pmr_allocator::ReflectableUnion& zserioData() const;

protected:
    View(const ::test_object::pmr_allocator::ReflectableUnion& data, const View& other) noexcept;

private:
    const ::test_object::pmr_allocator::ReflectableUnion* m_data;
};

bool operator==(const View<::test_object::pmr_allocator::ReflectableUnion>& lhs, const View<::test_object::pmr_allocator::ReflectableUnion>& rhs);
bool operator<(const View<::test_object::pmr_allocator::ReflectableUnion>& lhs, const View<::test_object::pmr_allocator::ReflectableUnion>& rhs);
bool operator!=(const View<::test_object::pmr_allocator::ReflectableUnion>& lhs, const View<::test_object::pmr_allocator::ReflectableUnion>& rhs);
bool operator>(const View<::test_object::pmr_allocator::ReflectableUnion>& lhs, const View<::test_object::pmr_allocator::ReflectableUnion>& rhs);
bool operator<=(const View<::test_object::pmr_allocator::ReflectableUnion>& lhs, const View<::test_object::pmr_allocator::ReflectableUnion>& rhs);
bool operator>=(const View<::test_object::pmr_allocator::ReflectableUnion>& lhs, const View<::test_object::pmr_allocator::ReflectableUnion>& rhs);

namespace detail
{

template <>
struct ObjectTraits<::test_object::pmr_allocator::ReflectableUnion>
{
    static void validate(const View<::test_object::pmr_allocator::ReflectableUnion>& view, ::std::string_view fieldName);

    static BitSize bitSizeOf(const View<::test_object::pmr_allocator::ReflectableUnion>& view, BitSize bitPosition);

    static void write(BitStreamWriter& writer, const View<::test_object::pmr_allocator::ReflectableUnion>& view);

    static View<::test_object::pmr_allocator::ReflectableUnion> read(BitStreamReader& reader, ::test_object::pmr_allocator::ReflectableUnion& data);
};

template <>
struct TypeInfo<::test_object::pmr_allocator::ReflectableUnion, ::std::pmr::polymorphic_allocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

template <>
struct Reflectable<::test_object::pmr_allocator::ReflectableUnion, ::std::pmr::polymorphic_allocator<uint8_t>>
{
    static ::zserio::pmr::IReflectableDataConstPtr create(
            const ::test_object::pmr_allocator::ReflectableUnion& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);

    static ::zserio::pmr::IReflectableDataPtr create(
            ::test_object::pmr_allocator::ReflectableUnion& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);
};

template <>
struct Introspectable<::test_object::pmr_allocator::ReflectableUnion, ::std::pmr::polymorphic_allocator<uint8_t>>
{
    static ::zserio::pmr::IIntrospectableViewConstPtr create(
            const View<::test_object::pmr_allocator::ReflectableUnion>& view, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);
};

} // namespace detail
} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::pmr_allocator::ReflectableUnion>
{
    size_t operator()(const ::test_object::pmr_allocator::ReflectableUnion& value) const;
};

template <>
struct hash<::zserio::View<::test_object::pmr_allocator::ReflectableUnion>>
{
    size_t operator()(const ::zserio::View<::test_object::pmr_allocator::ReflectableUnion>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_PMR_ALLOCATOR_REFLECTABLE_UNION_H
