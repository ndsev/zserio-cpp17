/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, std-polymorphicAllocator.
 */

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_polymorphic_allocator/ReflectableUtilEnum.h>

namespace zserio
{

template <>
size_t enumToOrdinal(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value)
{
    switch (value)
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilEnum::ONE:
        return 0;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilEnum::TWO:
        return 1;
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ReflectableUtilEnum: ") <<
                static_cast<typename ::std::underlying_type_t<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>>(value) << "!";
    }
}

template <>
::test_object::std_polymorphic_allocator::ReflectableUtilEnum valueToEnum(::zserio::Int8 rawValue)
{
    switch (rawValue)
    {
    case INT8_C(0):
    case INT8_C(1):
        return static_cast<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>(static_cast<::std::underlying_type_t<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>>(rawValue));
    default:
        throw ::zserio::CppRuntimeException("Unknown value for enumeration ReflectableUtilEnum: ") << rawValue << "!";
    }
}

namespace detail
{

const ::zserio::std_pmr::ITypeInfo& TypeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, ::std::pmr::polymorphic_allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::pmr::polymorphic_allocator<uint8_t>;

    static const ::std::array<::zserio::ItemInfo, 2> items = {
        ::zserio::ItemInfo{ "ONE", static_cast<uint64_t>(INT8_C(0)), false, false},
        ::zserio::ItemInfo{ "TWO", static_cast<uint64_t>(INT8_C(1)), false, false}
    };

    static const ::zserio::detail::EnumTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_polymorphic_allocator.ReflectableUtilEnum",
        ::zserio::typeInfo<::zserio::Int8, ::std::pmr::polymorphic_allocator<uint8_t>>(),
        items
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::std_pmr::IReflectableDataPtr reflectable(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataBase<::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataBase<::std::pmr::polymorphic_allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value) :
                ::zserio::ReflectableDataBase<::std::pmr::polymorphic_allocator<uint8_t>>(
                        typeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, ::std::pmr::polymorphic_allocator<uint8_t>>()),
                m_value(value)
        {}

        ::zserio::std_pmr::Any getAnyValue(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) const override
        {
            return ::zserio::std_pmr::Any(m_value, alloc);
        }

        ::zserio::std_pmr::Any getAnyValue(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) override
        {
            return ::zserio::std_pmr::Any(m_value, alloc);
        }

        ::zserio::Int8::ValueType getInt8() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>::type>(m_value);
        }

        int64_t toInt() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>::type>(m_value);
        }

        double toDouble() const override
        {
            return static_cast<double>(toInt());
        }

        ::zserio::std_pmr::String toString(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) const override
        {
            return ::zserio::std_pmr::String(::zserio::enumToString(m_value), alloc);
        }

    private:
        ::test_object::std_polymorphic_allocator::ReflectableUtilEnum m_value;
    };

    return std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::std_pmr::IIntrospectableViewConstPtr introspectable(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Introspectable : public ::zserio::SimpleIntrospectableViewBase<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, ::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        explicit Introspectable(::test_object::std_polymorphic_allocator::ReflectableUtilEnum value) :
                ::zserio::SimpleIntrospectableViewBase<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, ::std::pmr::polymorphic_allocator<uint8_t>>(
                        typeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, ::std::pmr::polymorphic_allocator<uint8_t>>(), value)
        {}

        ::zserio::Int8::ValueType getInt8() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>::type>(getValue());
        }

        int64_t toInt() const override
        {
            return static_cast<typename ::std::underlying_type<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>::type>(getValue());
        }

        double toDouble() const override
        {
            return static_cast<double>(toInt());
        }

        ::zserio::std_pmr::String toString(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) const override
        {
            return ::zserio::std_pmr::String(::zserio::enumToString(getValue()), alloc);
        }
    };

    return std::allocate_shared<Introspectable>(allocator, value);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>::operator()(const ::test_object::std_polymorphic_allocator::ReflectableUtilEnum& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::std::underlying_type_t<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>>(::zserio::enumToValue(value)));
    return static_cast<size_t>(result);
}

} // namespace std
