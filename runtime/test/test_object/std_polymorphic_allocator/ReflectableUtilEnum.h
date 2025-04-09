/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, std-polymorphicAllocator.
 */

#ifndef TEST_OBJECT_STD_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UTIL_ENUM_H
#define TEST_OBJECT_STD_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UTIL_ENUM_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <array>

#include <memory_resource>
#include <zserio/Enums.h>
#include <zserio/std_pmr/ITypeInfo.h>
#include <zserio/std_pmr/IReflectableData.h>
#include <zserio/std_pmr/IIntrospectableView.h>
#include <zserio/Types.h>

namespace test_object
{
namespace std_polymorphic_allocator
{

enum class ReflectableUtilEnum : ::zserio::Int8::ValueType
{
    ONE = INT8_C(0),
    TWO = INT8_C(1)
};

} // namespace std_polymorphic_allocator
} // namespace test_object

namespace zserio
{

// This is full specialization of enumeration traits and methods for ReflectableUtilEnum enumeration.
template <>
struct EnumTraits<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>
{
    static constexpr ::std::array<::std::string_view, 2> names =
    {{
        "ONE",
        "TWO"
    }};

    static constexpr ::std::array<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, 2> values =
    {{
        ::test_object::std_polymorphic_allocator::ReflectableUtilEnum::ONE,
        ::test_object::std_polymorphic_allocator::ReflectableUtilEnum::TWO
    }};

    static constexpr ::std::string_view enumName = "ReflectableUtilEnum";

    using ZserioType = ::zserio::Int8;
};

template <>
size_t enumToOrdinal<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value);

template <>
::test_object::std_polymorphic_allocator::ReflectableUtilEnum valueToEnum<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>(::zserio::Int8 rawValue);

namespace detail
{

template <>
struct TypeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, ::std::pmr::polymorphic_allocator<uint8_t>>
{
    static const ::zserio::std_pmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::std_pmr::IReflectableDataPtr reflectable(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);

template <>
::zserio::std_pmr::IIntrospectableViewConstPtr introspectable(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>
{
    size_t operator()(const ::test_object::std_polymorphic_allocator::ReflectableUtilEnum& value) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_POLYMORPHIC_ALLOCATOR_REFLECTABLE_UTIL_ENUM_H
