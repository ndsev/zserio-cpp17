/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
#include <zserio/TypeInfo.h>
#include <zserio/ReflectableData.h>

#include <test_object/polymorphic_allocator/ReflectableUtilBitmask.h>

namespace test_object
{
namespace polymorphic_allocator
{

::zserio::pmr::String ReflectableUtilBitmask::toString(const ::zserio::pmr::String::allocator_type& allocator) const
{
    ::zserio::pmr::String result(allocator);
    if ((*this & ReflectableUtilBitmask::Values::READ) == ReflectableUtilBitmask::Values::READ)
    {
        result += result.empty() ? "READ" : " | READ";
    }
    if ((*this & ReflectableUtilBitmask::Values::WRITE) == ReflectableUtilBitmask::Values::WRITE)
    {
        result += result.empty() ? "WRITE" : " | WRITE";
    }

    return ::zserio::toString<::zserio::pmr::String::allocator_type>(
            static_cast<ZserioType::ValueType>(m_value), allocator) + "[" + result + "]";
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

const ::zserio::pmr::ITypeInfo& TypeInfo<::test_object::polymorphic_allocator::ReflectableUtilBitmask, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    static const ::std::array<::zserio::ItemInfo, 2> values = {
        ::zserio::ItemInfo{ "READ", static_cast<uint64_t>(UINT8_C(1)), false, false},
        ::zserio::ItemInfo{ "WRITE", static_cast<uint64_t>(UINT8_C(2)), false, false}
    };

    static const ::zserio::detail::BitmaskTypeInfo<AllocatorType> typeInfo = {
        "test_object.polymorphic_allocator.ReflectableUtilBitmask",
        ::zserio::typeInfo<::zserio::UInt8, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(),
        values
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableUtilBitmask value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        explicit Reflectable(::test_object::polymorphic_allocator::ReflectableUtilBitmask bitmask) :
                ::zserio::ReflectableBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::ReflectableUtilBitmask>()),
                m_bitmask(bitmask)
        {}

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::Any(m_bitmask, alloc);
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) override
        {
            return ::zserio::pmr::Any(m_bitmask, alloc);
        }

        ::test_object::polymorphic_allocator::ReflectableUtilBitmask::ZserioType::ValueType getUInt8() const override
        {
            return m_bitmask.getValue();
        }

        uint64_t toUInt() const override
        {
            return m_bitmask.getValue();
        }

        double toDouble() const override
        {
            return static_cast<double>(toUInt());
        }

        ::zserio::pmr::String toString(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return m_bitmask.toString(alloc);
        }

    private:
        ::test_object::polymorphic_allocator::ReflectableUtilBitmask m_bitmask;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::polymorphic_allocator::ReflectableUtilBitmask>::operator()(const ::test_object::polymorphic_allocator::ReflectableUtilBitmask& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::test_object::polymorphic_allocator::ReflectableUtilBitmask::ZserioType::ValueType>(value.getValue()));
    return static_cast<size_t>(result);
}

} // namespace std
