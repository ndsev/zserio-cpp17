/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#ifndef TEST_OBJECT_STD_ALLOCATOR_CREATOR_ENUM_H
#define TEST_OBJECT_STD_ALLOCATOR_CREATOR_ENUM_H

#include <zserio/CppRuntimeVersion.h>
#if CPP17_EXTENSION_RUNTIME_VERSION_NUMBER != 2000
    #error Version mismatch between Zserio runtime library and Zserio C++ generator!
    #error Please update your Zserio runtime library to the version 0.2.0.
#endif

#include <array>

#include <memory>
#include <zserio/Enums.h>
#include <zserio/ITypeInfo.h>
#include <zserio/IReflectableData.h>
#include <zserio/IIntrospectableView.h>
#include <zserio/Types.h>

namespace test_object
{
namespace std_allocator
{

enum class CreatorEnum : ::zserio::Int8::ValueType
{
    ONE = INT8_C(0),
    TWO = INT8_C(1),
    MinusOne = INT8_C(-1)
};

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

// This is full specialization of enumeration traits and methods for CreatorEnum enumeration.
template <>
struct EnumTraits<::test_object::std_allocator::CreatorEnum>
{
    static constexpr ::std::array<::std::string_view, 3> names =
    {{
        "ONE",
        "TWO",
        "MinusOne"
    }};

    static constexpr ::std::array<::test_object::std_allocator::CreatorEnum, 3> values =
    {{
        ::test_object::std_allocator::CreatorEnum::ONE,
        ::test_object::std_allocator::CreatorEnum::TWO,
        ::test_object::std_allocator::CreatorEnum::MinusOne
    }};

    static constexpr ::std::string_view enumName = "CreatorEnum";

    using ZserioType = ::zserio::Int8;
};

template <>
size_t enumToOrdinal<::test_object::std_allocator::CreatorEnum>(::test_object::std_allocator::CreatorEnum value);

template <>
::test_object::std_allocator::CreatorEnum valueToEnum<::test_object::std_allocator::CreatorEnum>(::zserio::Int8 rawValue);

namespace detail
{

template <>
struct TypeInfo<::test_object::std_allocator::CreatorEnum, ::std::allocator<uint8_t>>
{
    static const ::zserio::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::CreatorEnum value, const ::std::allocator<uint8_t>& allocator);

template <>
::zserio::IIntrospectableViewConstPtr introspectable(::test_object::std_allocator::CreatorEnum value, const ::std::allocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::std_allocator::CreatorEnum>
{
    size_t operator()(const ::test_object::std_allocator::CreatorEnum& value) const;
};

} // namespace std

#endif // TEST_OBJECT_STD_ALLOCATOR_CREATOR_ENUM_H
