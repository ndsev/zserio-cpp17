/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_CHOICE_H
#define TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_CHOICE_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <memory>

#include <zserio/Variant.h>
#include <zserio/ChoiceTag.h>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectableData.h>
#include <zserio/IIntrospectableView.h>
#include <zserio/View.h>
#include <zserio/String.h>
#include <zserio/Types.h>

#include <test_object/std_allocator/ReflectableEnum.h>

namespace test_object
{
namespace std_allocator
{

// forward declaration
struct ReflectableChoice;

} // namespace std_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct ChoiceTag<::test_object::std_allocator::ReflectableChoice>
{
    enum Tag : size_t
    {
        UNDEFINED_CHOICE,
        CHOICE_valueStr,
        CHOICE_value32
    };
};

} // namespace detail
} // namespace zserio

namespace test_object
{
namespace std_allocator
{

struct ReflectableChoice : ::zserio::Variant<::zserio::detail::ChoiceTag<ReflectableChoice>::Tag,
        ::std::monostate,
        ::zserio::String,
        ::zserio::UInt32>
{
    using ChoiceTag = ::zserio::detail::ChoiceTag<ReflectableChoice>::Tag;
    using Base = ::zserio::Variant<ChoiceTag,
            ::std::monostate,
            ::zserio::String,
            ::zserio::UInt32>;
    using Base::BasicVariant;
};

bool operator==(const ::test_object::std_allocator::ReflectableChoice& lhs, const ::test_object::std_allocator::ReflectableChoice& rhs);
bool operator<(const ::test_object::std_allocator::ReflectableChoice& lhs, const ::test_object::std_allocator::ReflectableChoice& rhs);
bool operator!=(const ::test_object::std_allocator::ReflectableChoice& lhs, const ::test_object::std_allocator::ReflectableChoice& rhs);
bool operator>(const ::test_object::std_allocator::ReflectableChoice& lhs, const ::test_object::std_allocator::ReflectableChoice& rhs);
bool operator<=(const ::test_object::std_allocator::ReflectableChoice& lhs, const ::test_object::std_allocator::ReflectableChoice& rhs);
bool operator>=(const ::test_object::std_allocator::ReflectableChoice& lhs, const ::test_object::std_allocator::ReflectableChoice& rhs);

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_allocator::ReflectableChoice>
{
public:
    explicit View(const ::test_object::std_allocator::ReflectableChoice& data,
            ::test_object::std_allocator::ReflectableEnum param_) noexcept;

    ::test_object::std_allocator::ReflectableEnum param() const;

    ::std::string_view valueStr() const;
    ::zserio::UInt32 value32() const;

    ::test_object::std_allocator::ReflectableChoice::ChoiceTag zserioChoiceTag() const;
    const ::test_object::std_allocator::ReflectableChoice& zserioData() const;

protected:
    View(const ::test_object::std_allocator::ReflectableChoice& data, const View& other) noexcept;

private:
    const ::test_object::std_allocator::ReflectableChoice* m_data;
    ::test_object::std_allocator::ReflectableEnum m_param_;
};

bool operator==(const View<::test_object::std_allocator::ReflectableChoice>& lhs, const View<::test_object::std_allocator::ReflectableChoice>& rhs);
bool operator<(const View<::test_object::std_allocator::ReflectableChoice>& lhs, const View<::test_object::std_allocator::ReflectableChoice>& rhs);
bool operator!=(const View<::test_object::std_allocator::ReflectableChoice>& lhs, const View<::test_object::std_allocator::ReflectableChoice>& rhs);
bool operator>(const View<::test_object::std_allocator::ReflectableChoice>& lhs, const View<::test_object::std_allocator::ReflectableChoice>& rhs);
bool operator<=(const View<::test_object::std_allocator::ReflectableChoice>& lhs, const View<::test_object::std_allocator::ReflectableChoice>& rhs);
bool operator>=(const View<::test_object::std_allocator::ReflectableChoice>& lhs, const View<::test_object::std_allocator::ReflectableChoice>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::ReflectableChoice>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::ReflectableChoice>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::ReflectableChoice>& view);

template <>
View<::test_object::std_allocator::ReflectableChoice> read(BitStreamReader& reader, ::test_object::std_allocator::ReflectableChoice& data,
        ::test_object::std_allocator::ReflectableEnum param_);

template <>
struct TypeInfo<::test_object::std_allocator::ReflectableChoice, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(const ::test_object::std_allocator::ReflectableChoice& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::ReflectableChoice& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_allocator::ReflectableChoice>& view, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::ReflectableChoice>
{
    size_t operator()(const ::test_object::std_allocator::ReflectableChoice& value) const;
};

template <>
struct hash<::zserio::View<::test_object::std_allocator::ReflectableChoice>>
{
    size_t operator()(const ::zserio::View<::test_object::std_allocator::ReflectableChoice>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_CHOICE_H
