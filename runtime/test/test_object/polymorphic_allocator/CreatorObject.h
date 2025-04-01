/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_CREATOR_OBJECT_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_CREATOR_OBJECT_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>
#include <zserio/pmr/Optional.h>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/View.h>
#include <zserio/pmr/PropagatingPolymorphicAllocator.h>
#include <zserio/Array.h>
#include <zserio/Types.h>
#include <zserio/pmr/BitBuffer.h>
#include <zserio/pmr/Bytes.h>
#include <zserio/pmr/String.h>
#include <zserio/pmr/Vector.h>

#include <test_object/polymorphic_allocator/CreatorNested.h>

namespace test_object
{
namespace polymorphic_allocator
{

struct CreatorObject
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    CreatorObject() noexcept;
    explicit CreatorObject(const AllocatorType& allocator) noexcept;

    explicit CreatorObject(
            ::zserio::UInt32 value_,
            ::test_object::polymorphic_allocator::CreatorNested nested_,
            ::zserio::pmr::String text_,
            ::zserio::pmr::Vector<::test_object::polymorphic_allocator::CreatorNested> nestedArray_,
            ::zserio::pmr::Vector<::zserio::pmr::String> textArray_,
            ::zserio::pmr::Optional<::zserio::pmr::Vector<::zserio::pmr::BitBuffer>> externArray_,
            ::zserio::pmr::Optional<::zserio::pmr::Vector<::zserio::pmr::Bytes>> bytesArray_,
            ::zserio::pmr::Optional<::zserio::Bool> optionalBool_,
            ::zserio::pmr::Optional<::test_object::polymorphic_allocator::CreatorNested> optionalNested_);

    ::zserio::UInt32 value;
    ::test_object::polymorphic_allocator::CreatorNested nested;
    ::zserio::pmr::String text;
    ::zserio::pmr::Vector<::test_object::polymorphic_allocator::CreatorNested> nestedArray;
    ::zserio::pmr::Vector<::zserio::pmr::String> textArray;
    ::zserio::pmr::Optional<::zserio::pmr::Vector<::zserio::pmr::BitBuffer>> externArray;
    ::zserio::pmr::Optional<::zserio::pmr::Vector<::zserio::pmr::Bytes>> bytesArray;
    ::zserio::pmr::Optional<::zserio::Bool> optionalBool;
    ::zserio::pmr::Optional<::test_object::polymorphic_allocator::CreatorNested> optionalNested;
};

bool operator==(const ::test_object::polymorphic_allocator::CreatorObject& lhs, const ::test_object::polymorphic_allocator::CreatorObject& rhs);
bool operator<(const ::test_object::polymorphic_allocator::CreatorObject& lhs, const ::test_object::polymorphic_allocator::CreatorObject& rhs);
bool operator!=(const ::test_object::polymorphic_allocator::CreatorObject& lhs, const ::test_object::polymorphic_allocator::CreatorObject& rhs);
bool operator>(const ::test_object::polymorphic_allocator::CreatorObject& lhs, const ::test_object::polymorphic_allocator::CreatorObject& rhs);
bool operator<=(const ::test_object::polymorphic_allocator::CreatorObject& lhs, const ::test_object::polymorphic_allocator::CreatorObject& rhs);
bool operator>=(const ::test_object::polymorphic_allocator::CreatorObject& lhs, const ::test_object::polymorphic_allocator::CreatorObject& rhs);

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::polymorphic_allocator::CreatorObject>
{
public:
    struct ZserioNestedArrayArrayTraits
    {
        using OwnerType = View<::test_object::polymorphic_allocator::CreatorObject>;

        static View<::test_object::polymorphic_allocator::CreatorNested> at(const OwnerType& owner,
                const ::test_object::polymorphic_allocator::CreatorNested& element, size_t index);

        static void read(BitStreamReader& reader, const OwnerType& owner,
                ::test_object::polymorphic_allocator::CreatorNested& element, size_t index);
    };

    explicit View(const ::test_object::polymorphic_allocator::CreatorObject& data) noexcept;

    ::zserio::UInt32 value() const;
    View<::test_object::polymorphic_allocator::CreatorNested> nested() const;
    ::std::string_view text() const;
    Array<const ::test_object::polymorphic_allocator::CreatorNested, ZserioNestedArrayArrayTraits> nestedArray() const;
    Array<const ::zserio::pmr::String> textArray() const;
    ::zserio::pmr::Optional<Array<const ::zserio::pmr::BitBuffer>> externArray() const;
    ::zserio::pmr::Optional<Array<const ::zserio::pmr::Bytes>> bytesArray() const;
    ::zserio::pmr::Optional<::zserio::Bool> optionalBool() const;
    ::zserio::pmr::Optional<View<::test_object::polymorphic_allocator::CreatorNested>> optionalNested() const;

    const ::test_object::polymorphic_allocator::CreatorObject& zserioData() const;

protected:
    View(const ::test_object::polymorphic_allocator::CreatorObject& data, const View& other) noexcept;

private:
    const ::test_object::polymorphic_allocator::CreatorObject* m_data;
};

bool operator==(const View<::test_object::polymorphic_allocator::CreatorObject>& lhs, const View<::test_object::polymorphic_allocator::CreatorObject>& rhs);
bool operator<(const View<::test_object::polymorphic_allocator::CreatorObject>& lhs, const View<::test_object::polymorphic_allocator::CreatorObject>& rhs);
bool operator!=(const View<::test_object::polymorphic_allocator::CreatorObject>& lhs, const View<::test_object::polymorphic_allocator::CreatorObject>& rhs);
bool operator>(const View<::test_object::polymorphic_allocator::CreatorObject>& lhs, const View<::test_object::polymorphic_allocator::CreatorObject>& rhs);
bool operator<=(const View<::test_object::polymorphic_allocator::CreatorObject>& lhs, const View<::test_object::polymorphic_allocator::CreatorObject>& rhs);
bool operator>=(const View<::test_object::polymorphic_allocator::CreatorObject>& lhs, const View<::test_object::polymorphic_allocator::CreatorObject>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::CreatorObject>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::CreatorObject>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::CreatorObject>& view);

template <>
View<::test_object::polymorphic_allocator::CreatorObject> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::CreatorObject& data);

template <>
struct TypeInfo<::test_object::polymorphic_allocator::CreatorObject, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::CreatorObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::CreatorObject& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::CreatorObject>
{
    size_t operator()(const ::test_object::polymorphic_allocator::CreatorObject& data) const;
};

template <>
struct hash<::zserio::View<::test_object::polymorphic_allocator::CreatorObject>>
{
    size_t operator()(const ::zserio::View<::test_object::polymorphic_allocator::CreatorObject>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_CREATOR_OBJECT_H
