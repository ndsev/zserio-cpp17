/**
 * Automatically generated by Zserio C++17 generator version 0.4.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, ppmrAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
#include <string_view>

#include <test_object/ppmr_allocator/DebugStringObject.h>

namespace test_object
{
namespace ppmr_allocator
{

DebugStringObject::DebugStringObject() noexcept :
        DebugStringObject(allocator_type{})
{}

DebugStringObject::DebugStringObject(const allocator_type& allocator) noexcept :
        text(::std::string_view{"test"}, allocator)
{}

DebugStringObject::DebugStringObject(DebugStringObject&& other, const allocator_type& allocator) :
        text(std::move(other.text), allocator)
{}

DebugStringObject::DebugStringObject(const DebugStringObject& other, const allocator_type& allocator) :
        text(other.text, allocator)
{}

DebugStringObject::DebugStringObject(
        ::zserio::ppmr::String text_,
        const allocator_type& allocator) :
        text(::std::move(text_), allocator)
{}

bool operator==(const ::test_object::ppmr_allocator::DebugStringObject& lhs, const ::test_object::ppmr_allocator::DebugStringObject& rhs)
{
    return ::std::tie(
            lhs.text)
            == ::std::tie(
            rhs.text);
}

bool operator<(const ::test_object::ppmr_allocator::DebugStringObject& lhs, const ::test_object::ppmr_allocator::DebugStringObject& rhs)
{
    return ::std::tie(
            lhs.text)
            < ::std::tie(
            rhs.text);
}

bool operator!=(const ::test_object::ppmr_allocator::DebugStringObject& lhs, const ::test_object::ppmr_allocator::DebugStringObject& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::ppmr_allocator::DebugStringObject& lhs, const ::test_object::ppmr_allocator::DebugStringObject& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::ppmr_allocator::DebugStringObject& lhs, const ::test_object::ppmr_allocator::DebugStringObject& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::ppmr_allocator::DebugStringObject& lhs, const ::test_object::ppmr_allocator::DebugStringObject& rhs)
{
    return !(lhs < rhs);
}

} // namespace ppmr_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::ppmr_allocator::DebugStringObject>::View(const ::test_object::ppmr_allocator::DebugStringObject& data) noexcept :
        m_data(&data)
{}

View<::test_object::ppmr_allocator::DebugStringObject>::View(const ::test_object::ppmr_allocator::DebugStringObject& data,
        const View&) noexcept :
        m_data(&data)
{}

::std::string_view View<::test_object::ppmr_allocator::DebugStringObject>::text() const
{
    return ::std::string_view{
            m_data->text};
}

const ::test_object::ppmr_allocator::DebugStringObject& View<::test_object::ppmr_allocator::DebugStringObject>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::ppmr_allocator::DebugStringObject>& lhs, const View<::test_object::ppmr_allocator::DebugStringObject>& rhs)
{
    return lhs.text() == rhs.text();
}

bool operator<(const View<::test_object::ppmr_allocator::DebugStringObject>& lhs, const View<::test_object::ppmr_allocator::DebugStringObject>& rhs)
{
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }

    return false;
}

bool operator!=(const View<::test_object::ppmr_allocator::DebugStringObject>& lhs, const View<::test_object::ppmr_allocator::DebugStringObject>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::ppmr_allocator::DebugStringObject>& lhs, const View<::test_object::ppmr_allocator::DebugStringObject>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::ppmr_allocator::DebugStringObject>& lhs, const View<::test_object::ppmr_allocator::DebugStringObject>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::ppmr_allocator::DebugStringObject>& lhs, const View<::test_object::ppmr_allocator::DebugStringObject>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::ppmr_allocator::DebugStringObject>& view, ::std::string_view)
{
    validate(view.text(), "'DebugStringObject.text'");
}

template <>
BitSize bitSizeOf(const View<::test_object::ppmr_allocator::DebugStringObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::ppmr_allocator::DebugStringObject>& view)
{
    auto text_ = view.text();
    write(writer, text_);
}

template <>
View<::test_object::ppmr_allocator::DebugStringObject> read(BitStreamReader& reader, ::test_object::ppmr_allocator::DebugStringObject& data)
{
    View<::test_object::ppmr_allocator::DebugStringObject> view(data);
    read(reader, data.text);
    return view;
}

const ::zserio::ppmr::ITypeInfo& TypeInfo<::test_object::ppmr_allocator::DebugStringObject, ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
{
    using AllocatorType = ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 1> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "text", // schemaName
            ::zserio::typeInfo<::zserio::ppmr::String, ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>(), // typeInfo
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
        "test_object.ppmr_allocator.DebugStringObject",
        [](const AllocatorType& allocator) -> ::zserio::ppmr::IReflectableDataPtr
        {
            return ::std::allocate_shared<::zserio::detail::ReflectableDataOwner<::test_object::ppmr_allocator::DebugStringObject>>(allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::ppmr::IReflectableDataConstPtr reflectable(
        const ::test_object::ppmr_allocator::DebugStringObject& value, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::ppmr_allocator::DebugStringObject& object, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& alloc = {}) :
                ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::ppmr_allocator::DebugStringObject>(), alloc),
                m_object(object)
        {}

        ::zserio::ppmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringObject'!";
        }

        ::zserio::ppmr::Any getAnyValue(const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::ppmr::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::ppmr_allocator::DebugStringObject& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::ppmr::IReflectableDataPtr reflectable(
        ::test_object::ppmr_allocator::DebugStringObject& value, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::detail::ReflectableDataAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::detail::ReflectableDataAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::detail::ReflectableDataAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::ppmr_allocator::DebugStringObject& object, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& alloc = {}) :
                ::zserio::detail::ReflectableDataAllocatorHolderBase<::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::ppmr_allocator::DebugStringObject>(), alloc),
                m_object(object)
        {}

        ::zserio::ppmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringObject'!";
        }

        ::zserio::ppmr::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringObject'!";
        }

        void setField(::std::string_view name, const ::zserio::ppmr::Any& value) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::ReflectableUtil::fromAny<::zserio::ppmr::String>(value);
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringObject'!";
        }

        ::zserio::ppmr::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::ppmr::String(get_allocator());
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringObject'!";
        }

        ::zserio::ppmr::Any getAnyValue(const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::ppmr::Any(::std::cref(m_object), alloc);
        }

        ::zserio::ppmr::Any getAnyValue(const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) override
        {
            return ::zserio::ppmr::Any(::std::ref(m_object), alloc);
        }

    private:
        ::test_object::ppmr_allocator::DebugStringObject& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::ppmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::ppmr_allocator::DebugStringObject>& view, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Introspectable : public ::zserio::detail::CompoundIntrospectableViewBase<::test_object::ppmr_allocator::DebugStringObject, ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        explicit Introspectable(const ::zserio::View<::test_object::ppmr_allocator::DebugStringObject>& view_, const ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>& alloc = {}) :
                ::zserio::detail::CompoundIntrospectableViewBase<::test_object::ppmr_allocator::DebugStringObject, ::zserio::ppmr::PropagatingPolymorphicAllocator<uint8_t>>(
                        view_, alloc)
        {}

        ::zserio::ppmr::IIntrospectableViewConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::introspectable(getValue().text(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringObject'!";
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, view);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::ppmr_allocator::DebugStringObject>::operator()(const ::test_object::ppmr_allocator::DebugStringObject& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.text);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::ppmr_allocator::DebugStringObject>>::operator()(const ::zserio::View<::test_object::ppmr_allocator::DebugStringObject>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.text());
    return static_cast<size_t>(result);
}

} // namespace std
