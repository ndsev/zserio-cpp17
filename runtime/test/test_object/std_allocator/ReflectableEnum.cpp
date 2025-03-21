/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
#include <zserio/TypeInfo.h>
#include <zserio/ReflectableData.h>

#include <test_object/std_allocator/ReflectableEnum.h>

namespace zserio
{

template <>
size_t enumToOrdinal(::test_object::std_allocator::ReflectableEnum value)
{
    switch (value)
    {
    case ::test_object::std_allocator::ReflectableEnum::VALUE1:
        return 0;
    case ::test_object::std_allocator::ReflectableEnum::VALUE2:
        return 1;
    case ::test_object::std_allocator::ReflectableEnum::VALUE3:
        return 2;
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ReflectableEnum: ") <<
                static_cast<typename ::std::underlying_type_t<::test_object::std_allocator::ReflectableEnum>>(value) << "!";
    }
}

template <>
::test_object::std_allocator::ReflectableEnum valueToEnum(::zserio::Int8 rawValue)
{
    switch (rawValue)
    {
    case INT8_C(-1):
    case INT8_C(0):
    case INT8_C(1):
        return static_cast<::test_object::std_allocator::ReflectableEnum>(static_cast<::std::underlying_type_t<::test_object::std_allocator::ReflectableEnum>>(rawValue));
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ReflectableEnum: ") << rawValue << "!";
    }
}

namespace detail
{

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::ReflectableEnum, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::array<::zserio::ItemInfo, 3> items = {
        ::zserio::ItemInfo{ "VALUE1", static_cast<uint64_t>(INT8_C(-1)), false, false},
        ::zserio::ItemInfo{ "VALUE2", static_cast<uint64_t>(INT8_C(0)), false, false},
        ::zserio::ItemInfo{ "VALUE3", static_cast<uint64_t>(INT8_C(1)), false, false}
    };

    static const ::zserio::detail::EnumTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_allocator.ReflectableEnum",
        ::zserio::typeInfo<::zserio::Int8, ::std::allocator<uint8_t>>(),
        items
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::ReflectableEnum value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableBase<::std::allocator<uint8_t>>
    {
    public:
        explicit Reflectable(::test_object::std_allocator::ReflectableEnum value) :
                ::zserio::ReflectableBase<::std::allocator<uint8_t>>(
                        typeInfo<::test_object::std_allocator::ReflectableEnum, ::std::allocator<uint8_t>>()),
                m_value(value)
        {}

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(m_value, alloc);
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) override
        {
            return ::zserio::Any(m_value, alloc);
        }

        ::zserio::Int8::ValueType getInt8() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::std_allocator::ReflectableEnum>::type>(m_value);
        }

        int64_t toInt() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::std_allocator::ReflectableEnum>::type>(m_value);
        }

        double toDouble() const override
        {
            return static_cast<double>(toInt());
        }

        ::zserio::String toString(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::String(::zserio::enumToString(m_value), alloc);
        }

    private:
        ::test_object::std_allocator::ReflectableEnum m_value;
    };

    return std::allocate_shared<Reflectable>(allocator, value);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::ReflectableEnum>::operator()(const ::test_object::std_allocator::ReflectableEnum& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::std::underlying_type_t<::test_object::std_allocator::ReflectableEnum>>(::zserio::enumToValue(value)));
    return static_cast<size_t>(result);
}

} // namespace std
