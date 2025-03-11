/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_WALKER_UNION_H
#define TEST_OBJECT_STD_ALLOCATOR_WALKER_UNION_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>

#include <zserio/Variant.h>
#include <zserio/ChoiceTag.h>
#include <zserio/ITypeInfo.h>
#include <zserio/View.h>
#include <zserio/Array.h>
#include <zserio/String.h>
#include <zserio/Types.h>
#include <zserio/Vector.h>

#include <test_object/std_allocator/WalkerNested.h>

namespace test_object
{
namespace std_allocator
{

// forward declaration
struct WalkerUnion;

} // namespace std_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

template <>
struct ChoiceTag<::test_object::std_allocator::WalkerUnion>
{
    enum Tag : size_t
    {
        UNDEFINED_CHOICE,
        CHOICE_value,
        CHOICE_text,
        CHOICE_nestedArray
    };
};

} // namespace detail
} // namespace zserio

namespace test_object
{
namespace std_allocator
{

struct WalkerUnion : ::zserio::Variant<::zserio::detail::ChoiceTag<WalkerUnion>::Tag,
        ::std::monostate,
        ::zserio::UInt32,
        ::zserio::String,
        ::zserio::Vector<::test_object::std_allocator::WalkerNested>>
{
    using ChoiceTag = ::zserio::detail::ChoiceTag<WalkerUnion>::Tag;
    using Base = ::zserio::Variant<ChoiceTag,
            ::std::monostate,
            ::zserio::UInt32,
            ::zserio::String,
            ::zserio::Vector<::test_object::std_allocator::WalkerNested>>;
    using Base::BasicVariant;
};

bool operator==(const ::test_object::std_allocator::WalkerUnion& lhs, const ::test_object::std_allocator::WalkerUnion& rhs);
bool operator<(const ::test_object::std_allocator::WalkerUnion& lhs, const ::test_object::std_allocator::WalkerUnion& rhs);
bool operator!=(const ::test_object::std_allocator::WalkerUnion& lhs, const ::test_object::std_allocator::WalkerUnion& rhs);
bool operator>(const ::test_object::std_allocator::WalkerUnion& lhs, const ::test_object::std_allocator::WalkerUnion& rhs);
bool operator<=(const ::test_object::std_allocator::WalkerUnion& lhs, const ::test_object::std_allocator::WalkerUnion& rhs);
bool operator>=(const ::test_object::std_allocator::WalkerUnion& lhs, const ::test_object::std_allocator::WalkerUnion& rhs);

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_allocator::WalkerUnion>
{
public:
    explicit View(const ::test_object::std_allocator::WalkerUnion& data) noexcept;

    ::zserio::UInt32 value() const;
    ::std::string_view text() const;
    Array<::zserio::Vector<::test_object::std_allocator::WalkerNested>, ArrayType::AUTO> nestedArray() const;

    ::test_object::std_allocator::WalkerUnion::ChoiceTag zserioChoiceTag() const;
    const ::test_object::std_allocator::WalkerUnion& zserioData() const;

protected:
    View(const ::test_object::std_allocator::WalkerUnion& data, const View& other) noexcept;

private:
    const ::test_object::std_allocator::WalkerUnion* m_data;
};

bool operator==(const View<::test_object::std_allocator::WalkerUnion>& lhs, const View<::test_object::std_allocator::WalkerUnion>& rhs);
bool operator<(const View<::test_object::std_allocator::WalkerUnion>& lhs, const View<::test_object::std_allocator::WalkerUnion>& rhs);
bool operator!=(const View<::test_object::std_allocator::WalkerUnion>& lhs, const View<::test_object::std_allocator::WalkerUnion>& rhs);
bool operator>(const View<::test_object::std_allocator::WalkerUnion>& lhs, const View<::test_object::std_allocator::WalkerUnion>& rhs);
bool operator<=(const View<::test_object::std_allocator::WalkerUnion>& lhs, const View<::test_object::std_allocator::WalkerUnion>& rhs);
bool operator>=(const View<::test_object::std_allocator::WalkerUnion>& lhs, const View<::test_object::std_allocator::WalkerUnion>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::WalkerUnion>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::WalkerUnion>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::WalkerUnion>& view);

template <>
View<::test_object::std_allocator::WalkerUnion> read(BitStreamReader& reader, ::test_object::std_allocator::WalkerUnion& data);

template <>
struct TypeInfo<::test_object::std_allocator::WalkerUnion, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail
} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::WalkerUnion>
{
    size_t operator()(const ::test_object::std_allocator::WalkerUnion& value) const;
};

template <>
struct hash<::zserio::View<::test_object::std_allocator::WalkerUnion>>
{
    size_t operator()(const ::zserio::View<::test_object::std_allocator::WalkerUnion>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_WALKER_UNION_H
