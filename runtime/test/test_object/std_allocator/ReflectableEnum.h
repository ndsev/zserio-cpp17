/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_ENUM_H
#define TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_ENUM_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 1000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.1.0.
#endif

#include <array>

#include <memory>
#include <zserio/Enums.h>
#include <zserio/ITypeInfo.h>
#include <zserio/Types.h>

namespace test_object
{
namespace std_allocator
{

enum class ReflectableEnum : ::zserio::Int8::ValueType
{
    VALUE1 = INT8_C(-1),
    VALUE2 = INT8_C(0),
    VALUE3 = INT8_C(1)
};

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

// This is full specialization of enumeration traits and methods for ReflectableEnum enumeration.
template <>
struct EnumTraits<::test_object::std_allocator::ReflectableEnum>
{
    static constexpr ::std::array<::std::string_view, 3> names =
    {{
        "VALUE1",
        "VALUE2",
        "VALUE3"
    }};

    static constexpr ::std::array<::test_object::std_allocator::ReflectableEnum, 3> values =
    {{
        ::test_object::std_allocator::ReflectableEnum::VALUE1,
        ::test_object::std_allocator::ReflectableEnum::VALUE2,
        ::test_object::std_allocator::ReflectableEnum::VALUE3
    }};

    static constexpr ::std::string_view enumName = "ReflectableEnum";

    using ZserioType = ::zserio::Int8;
};

template <>
size_t enumToOrdinal<::test_object::std_allocator::ReflectableEnum>(::test_object::std_allocator::ReflectableEnum value);

template <>
::test_object::std_allocator::ReflectableEnum valueToEnum<::test_object::std_allocator::ReflectableEnum>(::zserio::Int8 rawValue);

namespace detail
{

template <>
struct TypeInfo<::test_object::std_allocator::ReflectableEnum, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::ReflectableEnum>
{
    size_t operator()(const ::test_object::std_allocator::ReflectableEnum& value) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_REFLECTABLE_ENUM_H
