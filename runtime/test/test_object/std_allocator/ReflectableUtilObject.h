/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_UTIL_OBJECT_H
#define TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_UTIL_OBJECT_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <memory>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectableData.h>
#include <zserio/View.h>
#include <memory>
#include <zserio/Types.h>

#include <test_object/std_allocator/ReflectableUtilChoice.h>

namespace test_object
{
namespace std_allocator
{

struct ReflectableUtilObject
{
    using AllocatorType = ::std::allocator<uint8_t>;

    ReflectableUtilObject() noexcept;
    explicit ReflectableUtilObject(const AllocatorType& allocator) noexcept;

    explicit ReflectableUtilObject(
            ::zserio::UInt8 choiceParam_,
            ::test_object::std_allocator::ReflectableUtilChoice reflectableUtilChoice_);

    ::zserio::UInt8 choiceParam;
    ::test_object::std_allocator::ReflectableUtilChoice reflectableUtilChoice;
};

bool operator==(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs);
bool operator<(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs);
bool operator!=(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs);
bool operator>(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs);
bool operator<=(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs);
bool operator>=(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs);

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

template <>
class View<::test_object::std_allocator::ReflectableUtilObject>
{
public:
    explicit View(const ::test_object::std_allocator::ReflectableUtilObject& data) noexcept;

    ::zserio::UInt8 choiceParam() const;
    View<::test_object::std_allocator::ReflectableUtilChoice> reflectableUtilChoice() const;

    const ::test_object::std_allocator::ReflectableUtilObject& zserioData() const;

protected:
    View(const ::test_object::std_allocator::ReflectableUtilObject& data, const View& other) noexcept;

private:
    const ::test_object::std_allocator::ReflectableUtilObject* m_data;
};

bool operator==(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs);
bool operator<(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs);
bool operator!=(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs);
bool operator>(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs);
bool operator<=(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs);
bool operator>=(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs);

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::ReflectableUtilObject>& view, ::std::string_view fieldName);

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::ReflectableUtilObject>& view, BitSize bitPosition);

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::ReflectableUtilObject>& view);

template <>
View<::test_object::std_allocator::ReflectableUtilObject> read(BitStreamReader& reader, ::test_object::std_allocator::ReflectableUtilObject& data);

template <>
struct TypeInfo<::test_object::std_allocator::ReflectableUtilObject, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(const ::test_object::std_allocator::ReflectableUtilObject& value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::ReflectableUtilObject& value, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::ReflectableUtilObject>
{
    size_t operator()(const ::test_object::std_allocator::ReflectableUtilObject& data) const;
};

template <>
struct hash<::zserio::View<::test_object::std_allocator::ReflectableUtilObject>>
{
    size_t operator()(const ::zserio::View<::test_object::std_allocator::ReflectableUtilObject>& view) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_UTIL_OBJECT_H
