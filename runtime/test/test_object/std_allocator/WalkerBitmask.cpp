/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/HashCodeUtil.h>
#include <zserio/StringConvertUtil.h>
#include <zserio/TypeInfo.h>
#include <zserio/ReflectableData.h>

#include <test_object/std_allocator/WalkerBitmask.h>

namespace test_object
{
namespace std_allocator
{

::zserio::String WalkerBitmask::toString(const ::zserio::String::allocator_type& allocator) const
{
    ::zserio::String result(allocator);
    if ((*this & WalkerBitmask::Values::ZERO) == WalkerBitmask::Values::ZERO)
    {
        result += result.empty() ? "ZERO" : " | ZERO";
    }

    return ::zserio::toString<::zserio::String::allocator_type>(
            static_cast<ZserioType::ValueType>(m_value), allocator) + "[" + result + "]";
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{
namespace detail
{

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::WalkerBitmask, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::array<::zserio::ItemInfo, 1> values = {
        ::zserio::ItemInfo{ "ZERO", static_cast<uint64_t>(UINT32_C(1)), false, false}
    };

    static const ::zserio::detail::BitmaskTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_allocator.WalkerBitmask",
        ::zserio::typeInfo<::zserio::UInt32, ::std::allocator<uint8_t>>(),
        values
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::WalkerBitmask value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_allocator::WalkerBitmask bitmask) :
                ::zserio::ReflectableDataBase<::std::allocator<uint8_t>>(
                        typeInfo<::test_object::std_allocator::WalkerBitmask, ::std::allocator<uint8_t>>()),
                m_bitmask(bitmask)
        {}

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(m_bitmask, alloc);
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) override
        {
            return ::zserio::Any(m_bitmask, alloc);
        }

        ::test_object::std_allocator::WalkerBitmask::ZserioType::ValueType getUInt32() const override
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

        ::zserio::String toString(const ::std::allocator<uint8_t>& alloc) const override
        {
            return m_bitmask.toString(alloc);
        }

    private:
        ::test_object::std_allocator::WalkerBitmask m_bitmask;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::WalkerBitmask>::operator()(const ::test_object::std_allocator::WalkerBitmask& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result,
            static_cast<::test_object::std_allocator::WalkerBitmask::ZserioType::ValueType>(value.getValue()));
    return static_cast<size_t>(result);
}

} // namespace std
