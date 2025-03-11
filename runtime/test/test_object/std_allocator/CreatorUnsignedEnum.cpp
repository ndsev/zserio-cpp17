/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_allocator/CreatorUnsignedEnum.h>

namespace zserio
{

template <>
size_t enumToOrdinal(::test_object::std_allocator::CreatorUnsignedEnum value)
{
    switch (value)
    {
    case ::test_object::std_allocator::CreatorUnsignedEnum::ONE:
        return 0;
    case ::test_object::std_allocator::CreatorUnsignedEnum::TWO:
        return 1;
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration CreatorUnsignedEnum: ") <<
                static_cast<typename ::std::underlying_type_t<::test_object::std_allocator::CreatorUnsignedEnum>>(value) << "!";
    }
}

template <>
::test_object::std_allocator::CreatorUnsignedEnum valueToEnum(::zserio::UInt8 rawValue)
{
    switch (rawValue)
    {
    case UINT8_C(0):
    case UINT8_C(1):
        return static_cast<::test_object::std_allocator::CreatorUnsignedEnum>(static_cast<::std::underlying_type_t<::test_object::std_allocator::CreatorUnsignedEnum>>(rawValue));
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration CreatorUnsignedEnum: ") << rawValue << "!";
    }
}

namespace detail
{

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::CreatorUnsignedEnum, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::array<::zserio::ItemInfo, 2> items = {
        ::zserio::ItemInfo{ "ONE", static_cast<uint64_t>(UINT8_C(0)), false, false},
        ::zserio::ItemInfo{ "TWO", static_cast<uint64_t>(UINT8_C(1)), false, false}
    };

    static const ::zserio::detail::EnumTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_allocator.CreatorUnsignedEnum",
        ::zserio::typeInfo<::zserio::UInt8, ::std::allocator<uint8_t>>(),
        items
    };

    return typeInfo;
}

} // namespace detail

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::CreatorUnsignedEnum>::operator()(const ::test_object::std_allocator::CreatorUnsignedEnum& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::std::underlying_type_t<::test_object::std_allocator::CreatorUnsignedEnum>>(::zserio::enumToValue(value)));
    return static_cast<size_t>(result);
}

} // namespace std
