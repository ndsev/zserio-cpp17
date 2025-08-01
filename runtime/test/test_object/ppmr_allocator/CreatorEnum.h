/**
 * Automatically generated by Zserio C++17 generator version 0.5.0 using Zserio core 2.17.0.
 * Generator setup: typeInfoCode, ppmrAllocator.
 */

#ifndef TEST_OBJECT_PPMR_ALLOCATOR_CREATOR_ENUM_H
#define TEST_OBJECT_PPMR_ALLOCATOR_CREATOR_ENUM_H

#include <array>

#include <zserio/ppmr/PropagatingPolymorphicAllocator.h>
#include <zserio/Enums.h>
#include <zserio/ppmr/ITypeInfo.h>
#include <zserio/ppmr/IReflectableData.h>
#include <zserio/ppmr/IIntrospectableView.h>
#include <zserio/Types.h>

namespace test_object
{
namespace ppmr_allocator
{

enum class CreatorEnum : ::zserio::Int8::ValueType
{
    ONE = INT8_C(0),
    TWO = INT8_C(1),
    MinusOne = INT8_C(-1)
};

} // namespace ppmr_allocator
} // namespace test_object

namespace zserio
{

// This is full specialization of enumeration traits and methods for CreatorEnum enumeration.
template <>
struct EnumTraits<::test_object::ppmr_allocator::CreatorEnum>
{
    static constexpr ::std::array<::std::string_view, 3> names =
    {{
        "ONE",
        "TWO",
        "MinusOne"
    }};

    static constexpr ::std::array<::test_object::ppmr_allocator::CreatorEnum, 3> values =
    {{
        ::test_object::ppmr_allocator::CreatorEnum::ONE,
        ::test_object::ppmr_allocator::CreatorEnum::TWO,
        ::test_object::ppmr_allocator::CreatorEnum::MinusOne
    }};

    static constexpr ::std::string_view enumName = "CreatorEnum";

    using ZserioType = ::zserio::Int8;
};

template <>
size_t enumToOrdinal<::test_object::ppmr_allocator::CreatorEnum>(::test_object::ppmr_allocator::CreatorEnum value);

template <>
::test_object::ppmr_allocator::CreatorEnum valueToEnum<::test_object::ppmr_allocator::CreatorEnum>(::zserio::Int8 rawValue);

namespace detail
{

template <>
struct TypeInfo<::test_object::ppmr_allocator::CreatorEnum, ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>
{
    static const ::zserio::ppmr::ITypeInfo& get();
};

} // namespace detail

template <>
::zserio::ppmr::IReflectableDataPtr reflectable(::test_object::ppmr_allocator::CreatorEnum value, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

template <>
::zserio::ppmr::IIntrospectableViewConstPtr introspectable(::test_object::ppmr_allocator::CreatorEnum value, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator);

} // namespace zserio

namespace std
{

template <>
struct hash<::test_object::ppmr_allocator::CreatorEnum>
{
    size_t operator()(const ::test_object::ppmr_allocator::CreatorEnum& value) const;
};

} // namespace std

#endif // TEST_OBJECT_PPMR_ALLOCATOR_CREATOR_ENUM_H
