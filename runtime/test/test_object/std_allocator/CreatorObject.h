/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_CREATOR_OBJECT_H
#define TEST_OBJECT_STD_ALLOCATOR_CREATOR_OBJECT_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>
#include <zserio/Optional.h>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectableData.h>
#include <zserio/View.h>
#include <memory>
#include <zserio/Array.h>
#include <zserio/BitBuffer.h>
#include <zserio/Bytes.h>
#include <zserio/String.h>
#include <zserio/Types.h>
#include <zserio/Vector.h>

#include <test_object/std_allocator/CreatorNested.h>

namespace test_object
{
namespace std_allocator
{

struct CreatorObject
{
    using AllocatorType = ::std::allocator<uint8_t>;

    CreatorObject() noexcept;
    explicit CreatorObject(const AllocatorType& allocator) noexcept;

    explicit CreatorObject(
            ::zserio::UInt32 value_,
            ::test_object::std_allocator::CreatorNested nested_,
            ::zserio::String text_,
            ::zserio::Vector<::test_object::std_allocator::CreatorNested> nestedArray_,
            ::zserio::Vector<::zserio::String> textArray_,
            ::zserio::Optional<::zserio::Vector<::zserio::BitBuffer>> externArray_,
            ::zserio::Optional<::zserio::Vector<::zserio::Bytes>> bytesArray_,
            ::zserio::Optional<::zserio::Bool> optionalBool_,
            ::zserio::Optional<::test_object::std_allocator::CreatorNested> optionalNested_);

    ::zserio::UInt32 value;
    ::test_object::std_allocator::CreatorNested nested;
    ::zserio::String text;
    ::zserio::Vector<::test_object::std_allocator::CreatorNested> nestedArray;
    ::zserio::Vector<::zserio::String> textArray;
    ::zserio::Optional<::zserio::Vector<::zserio::BitBuffer>> externArray;
    ::zserio::Optional<::zserio::Vector<::zserio::Bytes>> bytesArray;
    ::zserio::Optional<::zserio::Bool> optionalBool;
    ::zserio::Optional<::test_object::std_allocator::CreatorNested> optionalNested;
};

bool operator==(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs);
bool operator<(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs);
bool operator!=(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs);
bool operator>(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs);
bool operator<=(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs);
bool operator>=(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs);

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_allocator::CreatorObject>
{
public:
    struct ZserioNestedArrayArrayTraits
    {
        using OwnerType = View<::test_object::std_allocator::CreatorObject>;

        static View<::test_object::std_allocator::CreatorNested> at(const OwnerType& owner,
                const ::test_object::std_allocator::CreatorNested& element, size_t index);

        static void read(BitStreamReader& reader, const OwnerType& owner,
                ::test_object::std_allocator::CreatorNested& element, size_t index);
    };

    explicit View(const ::test_object::std_allocator::CreatorObject& data) noexcept;

    ::zserio::UInt32 value() const;
    View<::test_object::std_allocator::CreatorNested> nested() const;
    ::std::string_view text() const;
    Array<const ::test_object::std_allocator::CreatorNested, ZserioNestedArrayArrayTraits> nestedArray() const;
    Array<const ::zserio::String> textArray() const;
    ::zserio::Optional<Array<const ::zserio::BitBuffer>> externArray() const;
    ::zserio::Optional<Array<const ::zserio::Bytes>> bytesArray() const;
    ::zserio::Optional<::zserio::Bool> optionalBool() const;
    ::zserio::Optional<View<::test_object::std_allocator::CreatorNested>> optionalNested() const;

    const ::test_object::std_allocator::CreatorObject& zserioData() const;

protected:
    View(const ::test_object::std_allocator::CreatorObject& data, const View& other) noexcept;

private:
    const ::test_object::std_allocator::CreatorObject* m_data;
};

bool operator==(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs);
bool operator<(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs);
bool operator!=(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs);
bool operator>(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs);
bool operator<=(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs);
bool operator>=(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::CreatorObject>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::CreatorObject>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::CreatorObject>& view);

template <>
View<::test_object::std_allocator::CreatorObject> read(BitStreamReader& reader, ::test_object::std_allocator::CreatorObject& data);

template <>
struct TypeInfo<::test_object::std_allocator::CreatorObject, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(const ::test_object::std_allocator::CreatorObject& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::CreatorObject& value, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::CreatorObject>
{
    size_t operator()(const ::test_object::std_allocator::CreatorObject& data) const;
};

template <>
struct hash<::zserio::View<::test_object::std_allocator::CreatorObject>>
{
    size_t operator()(const ::zserio::View<::test_object::std_allocator::CreatorObject>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_CREATOR_OBJECT_H
