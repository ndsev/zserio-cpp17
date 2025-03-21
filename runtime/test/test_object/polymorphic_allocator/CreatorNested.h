/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#ifndef TEST_OBJECT_POLYMORPHIC_ALLOCATOR_CREATOR_NESTED_H
#define TEST_OBJECT_POLYMORPHIC_ALLOCATOR_CREATOR_NESTED_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>
#include <zserio/pmr/ITypeInfo.h>
#include <zserio/pmr/IReflectableData.h>
#include <zserio/View.h>
#include <zserio/pmr/PropagatingPolymorphicAllocator.h>
#include <zserio/Types.h>
#include <zserio/pmr/BitBuffer.h>
#include <zserio/pmr/Bytes.h>
#include <zserio/pmr/String.h>

#include <test_object/polymorphic_allocator/CreatorBitmask.h>
#include <test_object/polymorphic_allocator/CreatorEnum.h>

namespace test_object
{
namespace polymorphic_allocator
{

struct CreatorNested
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    CreatorNested() noexcept;
    explicit CreatorNested(const AllocatorType& allocator) noexcept;

    explicit CreatorNested(
            ::zserio::UInt32 value_,
            ::zserio::pmr::String text_,
            ::zserio::pmr::BitBuffer externData_,
            ::zserio::pmr::Bytes bytesData_,
            ::test_object::polymorphic_allocator::CreatorEnum creatorEnum_,
            ::test_object::polymorphic_allocator::CreatorBitmask creatorBitmask_);

        ::zserio::UInt32 value;
        ::zserio::pmr::String text;
        ::zserio::pmr::BitBuffer externData;
        ::zserio::pmr::Bytes bytesData;
        ::test_object::polymorphic_allocator::CreatorEnum creatorEnum;
        ::test_object::polymorphic_allocator::CreatorBitmask creatorBitmask;
};

bool operator==(const ::test_object::polymorphic_allocator::CreatorNested& lhs, const ::test_object::polymorphic_allocator::CreatorNested& rhs);
bool operator<(const ::test_object::polymorphic_allocator::CreatorNested& lhs, const ::test_object::polymorphic_allocator::CreatorNested& rhs);
bool operator!=(const ::test_object::polymorphic_allocator::CreatorNested& lhs, const ::test_object::polymorphic_allocator::CreatorNested& rhs);
bool operator>(const ::test_object::polymorphic_allocator::CreatorNested& lhs, const ::test_object::polymorphic_allocator::CreatorNested& rhs);
bool operator<=(const ::test_object::polymorphic_allocator::CreatorNested& lhs, const ::test_object::polymorphic_allocator::CreatorNested& rhs);
bool operator>=(const ::test_object::polymorphic_allocator::CreatorNested& lhs, const ::test_object::polymorphic_allocator::CreatorNested& rhs);

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::polymorphic_allocator::CreatorNested>
{
public:
    explicit View(const ::test_object::polymorphic_allocator::CreatorNested& data,
            ::zserio::UInt32 param_) noexcept;

    ::zserio::UInt32 param() const;

    ::zserio::UInt32 value() const;
    ::std::string_view text() const;
    ::zserio::pmr::BitBufferView externData() const;
    BytesView bytesData() const;
    ::test_object::polymorphic_allocator::CreatorEnum creatorEnum() const;
    ::test_object::polymorphic_allocator::CreatorBitmask creatorBitmask() const;

    const ::test_object::polymorphic_allocator::CreatorNested& zserioData() const;

protected:
    View(const ::test_object::polymorphic_allocator::CreatorNested& data, const View& other) noexcept;

private:
    const ::test_object::polymorphic_allocator::CreatorNested* m_data;
    ::zserio::UInt32 m_param_;
};

bool operator==(const View<::test_object::polymorphic_allocator::CreatorNested>& lhs, const View<::test_object::polymorphic_allocator::CreatorNested>& rhs);
bool operator<(const View<::test_object::polymorphic_allocator::CreatorNested>& lhs, const View<::test_object::polymorphic_allocator::CreatorNested>& rhs);
bool operator!=(const View<::test_object::polymorphic_allocator::CreatorNested>& lhs, const View<::test_object::polymorphic_allocator::CreatorNested>& rhs);
bool operator>(const View<::test_object::polymorphic_allocator::CreatorNested>& lhs, const View<::test_object::polymorphic_allocator::CreatorNested>& rhs);
bool operator<=(const View<::test_object::polymorphic_allocator::CreatorNested>& lhs, const View<::test_object::polymorphic_allocator::CreatorNested>& rhs);
bool operator>=(const View<::test_object::polymorphic_allocator::CreatorNested>& lhs, const View<::test_object::polymorphic_allocator::CreatorNested>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::CreatorNested>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::CreatorNested>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::CreatorNested>& view);

template <>
View<::test_object::polymorphic_allocator::CreatorNested> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::CreatorNested& data,
        ::zserio::UInt32 param_);

template <>
struct TypeInfo<::test_object::polymorphic_allocator::CreatorNested, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::CreatorNested& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::CreatorNested& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::polymorphic_allocator::CreatorNested>
{
    size_t operator()(const ::test_object::polymorphic_allocator::CreatorNested& data) const;
};

template <>
struct hash<::zserio::View<::test_object::polymorphic_allocator::CreatorNested>>
{
    size_t operator()(const ::zserio::View<::test_object::polymorphic_allocator::CreatorNested>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_POLYMORPHIC_ALLOCATOR_CREATOR_NESTED_H
