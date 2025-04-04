/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
#include <zserio/TypeInfo.h>
#include <zserio/ReflectableData.h>

#include <test_object/polymorphic_allocator/ReflectableEnum.h>

namespace zserio
{

template <>
size_t enumToOrdinal(::test_object::polymorphic_allocator::ReflectableEnum value)
{
    switch (value)
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        return 0;
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        return 1;
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE3:
        return 2;
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ReflectableEnum: ") <<
                static_cast<typename ::std::underlying_type_t<::test_object::polymorphic_allocator::ReflectableEnum>>(value) << "!";
    }
}

template <>
::test_object::polymorphic_allocator::ReflectableEnum valueToEnum(::zserio::Int8 rawValue)
{
    switch (rawValue)
    {
    case INT8_C(-1):
    case INT8_C(0):
    case INT8_C(1):
        return static_cast<::test_object::polymorphic_allocator::ReflectableEnum>(static_cast<::std::underlying_type_t<::test_object::polymorphic_allocator::ReflectableEnum>>(rawValue));
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ReflectableEnum: ") << rawValue << "!";
    }
}

namespace detail
{

const ::zserio::pmr::ITypeInfo& TypeInfo<::test_object::polymorphic_allocator::ReflectableEnum, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    static const ::std::array<::zserio::ItemInfo, 3> items = {
        ::zserio::ItemInfo{ "VALUE1", static_cast<uint64_t>(INT8_C(-1)), false, false},
        ::zserio::ItemInfo{ "VALUE2", static_cast<uint64_t>(INT8_C(0)), false, false},
        ::zserio::ItemInfo{ "VALUE3", static_cast<uint64_t>(INT8_C(1)), false, false}
    };

    static const ::zserio::detail::EnumTypeInfo<AllocatorType> typeInfo = {
        "test_object.polymorphic_allocator.ReflectableEnum",
        ::zserio::typeInfo<::zserio::Int8, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(),
        items
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableEnum value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::polymorphic_allocator::ReflectableEnum value) :
                ::zserio::ReflectableDataBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(
                        typeInfo<::test_object::polymorphic_allocator::ReflectableEnum, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>()),
                m_value(value)
        {}

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::Any(m_value, alloc);
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) override
        {
            return ::zserio::pmr::Any(m_value, alloc);
        }

        ::zserio::Int8::ValueType getInt8() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::polymorphic_allocator::ReflectableEnum>::type>(m_value);
        }

        int64_t toInt() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::polymorphic_allocator::ReflectableEnum>::type>(m_value);
        }

        double toDouble() const override
        {
            return static_cast<double>(toInt());
        }

        ::zserio::pmr::String toString(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::String(::zserio::enumToString(m_value), alloc);
        }

    private:
        ::test_object::polymorphic_allocator::ReflectableEnum m_value;
    };

    return std::allocate_shared<Reflectable>(allocator, value);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::polymorphic_allocator::ReflectableEnum>::operator()(const ::test_object::polymorphic_allocator::ReflectableEnum& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::std::underlying_type_t<::test_object::polymorphic_allocator::ReflectableEnum>>(::zserio::enumToValue(value)));
    return static_cast<size_t>(result);
}

} // namespace std
