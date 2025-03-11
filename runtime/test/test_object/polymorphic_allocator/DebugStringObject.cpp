/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/TypeInfo.h>
#include <string_view>

#include <test_object/polymorphic_allocator/DebugStringObject.h>

namespace test_object
{
namespace polymorphic_allocator
{

DebugStringObject::DebugStringObject() noexcept :
        DebugStringObject(AllocatorType{})
{}

DebugStringObject::DebugStringObject(const AllocatorType& allocator) noexcept :
        text(::std::string_view{"test"}, allocator)
{}

DebugStringObject::DebugStringObject(
        ::zserio::pmr::String text_) :
        text(::std::move(text_))
{}

bool operator==(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs)
{
    return ::std::tie(
            lhs.text)
            == ::std::tie(
            rhs.text);
}

bool operator<(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs)
{
    return ::std::tie(
            lhs.text)
            < ::std::tie(
            rhs.text);
}

bool operator!=(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::polymorphic_allocator::DebugStringObject& lhs, const ::test_object::polymorphic_allocator::DebugStringObject& rhs)
{
    return !(lhs < rhs);
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::polymorphic_allocator::DebugStringObject>::View(const ::test_object::polymorphic_allocator::DebugStringObject& data) noexcept :
        m_data(&data)
{}

View<::test_object::polymorphic_allocator::DebugStringObject>::View(const ::test_object::polymorphic_allocator::DebugStringObject& data,
        const View&) noexcept :
        m_data(&data)
{}

::std::string_view View<::test_object::polymorphic_allocator::DebugStringObject>::text() const
{
    return ::std::string_view{
            m_data->text};
}

const ::test_object::polymorphic_allocator::DebugStringObject& View<::test_object::polymorphic_allocator::DebugStringObject>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs)
{
    return lhs.text() == rhs.text();
}

bool operator<(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs)
{
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }

    return false;
}

bool operator!=(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::polymorphic_allocator::DebugStringObject>& lhs, const View<::test_object::polymorphic_allocator::DebugStringObject>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::DebugStringObject>& view, ::std::string_view)
{
    validate(view.text(), "'DebugStringObject.text'");
}

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::DebugStringObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::DebugStringObject>& view)
{
    auto text_ = view.text();
    write(writer, text_);
}

template <>
View<::test_object::polymorphic_allocator::DebugStringObject> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::DebugStringObject& data)
{
    View<::test_object::polymorphic_allocator::DebugStringObject> view(data);
    read(reader, data.text);
    return view;
}

const ::zserio::pmr::ITypeInfo& TypeInfo<::test_object::polymorphic_allocator::DebugStringObject, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 1> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "text", // schemaName
            ::zserio::typeInfo<::zserio::pmr::String, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(), // typeInfo
            {}, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            "::std::string_view{\"test\"}", // initializer
            false, // isOptional
            {}, // optionalClause
            {}, // constraint
            false, // isArray
            {}, // arrayLength
            false, // isPacked
            false // isImplicit
        }
    };

    static const ::zserio::Span<::zserio::BasicParameterInfo<AllocatorType>> parameters;

    static const ::zserio::Span<::zserio::BasicFunctionInfo<AllocatorType>> functions;

    static const ::zserio::detail::StructTypeInfo<AllocatorType> typeInfo = {
        "test_object.polymorphic_allocator.DebugStringObject", nullptr,
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail
} // namespace zserio

namespace std
{

size_t hash<::test_object::polymorphic_allocator::DebugStringObject>::operator()(const ::test_object::polymorphic_allocator::DebugStringObject& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.text);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::polymorphic_allocator::DebugStringObject>>::operator()(const ::zserio::View<::test_object::polymorphic_allocator::DebugStringObject>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.text());
    return static_cast<size_t>(result);
}

} // namespace std
