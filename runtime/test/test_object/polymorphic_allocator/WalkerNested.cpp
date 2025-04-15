/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>

#include <test_object/polymorphic_allocator/WalkerNested.h>

namespace test_object
{
namespace polymorphic_allocator
{

WalkerNested::WalkerNested() noexcept :
        WalkerNested(AllocatorType{})
{}

WalkerNested::WalkerNested(const AllocatorType& allocator) noexcept :
        text(allocator)
{}

WalkerNested::WalkerNested(
        ::zserio::pmr::String text_) :
        text(::std::move(text_))
{}

bool operator==(const ::test_object::polymorphic_allocator::WalkerNested& lhs, const ::test_object::polymorphic_allocator::WalkerNested& rhs)
{
    return ::std::tie(
            lhs.text)
            == ::std::tie(
            rhs.text);
}

bool operator<(const ::test_object::polymorphic_allocator::WalkerNested& lhs, const ::test_object::polymorphic_allocator::WalkerNested& rhs)
{
    return ::std::tie(
            lhs.text)
            < ::std::tie(
            rhs.text);
}

bool operator!=(const ::test_object::polymorphic_allocator::WalkerNested& lhs, const ::test_object::polymorphic_allocator::WalkerNested& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::polymorphic_allocator::WalkerNested& lhs, const ::test_object::polymorphic_allocator::WalkerNested& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::polymorphic_allocator::WalkerNested& lhs, const ::test_object::polymorphic_allocator::WalkerNested& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::polymorphic_allocator::WalkerNested& lhs, const ::test_object::polymorphic_allocator::WalkerNested& rhs)
{
    return !(lhs < rhs);
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::polymorphic_allocator::WalkerNested>::View(const ::test_object::polymorphic_allocator::WalkerNested& data) noexcept :
        m_data(&data)
{}

View<::test_object::polymorphic_allocator::WalkerNested>::View(const ::test_object::polymorphic_allocator::WalkerNested& data,
        const View&) noexcept :
        m_data(&data)
{}

::std::string_view View<::test_object::polymorphic_allocator::WalkerNested>::text() const
{
    return ::std::string_view{
            m_data->text};
}

const ::test_object::polymorphic_allocator::WalkerNested& View<::test_object::polymorphic_allocator::WalkerNested>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::polymorphic_allocator::WalkerNested>& rhs)
{
    return lhs.text() == rhs.text();
}

bool operator<(const View<::test_object::polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::polymorphic_allocator::WalkerNested>& rhs)
{
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }

    return false;
}

bool operator!=(const View<::test_object::polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::polymorphic_allocator::WalkerNested>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::polymorphic_allocator::WalkerNested>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::polymorphic_allocator::WalkerNested>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::polymorphic_allocator::WalkerNested>& lhs, const View<::test_object::polymorphic_allocator::WalkerNested>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::WalkerNested>& view, ::std::string_view)
{
    validate(view.text(), "'WalkerNested.text'");
}

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::WalkerNested>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::WalkerNested>& view)
{
    auto text_ = view.text();
    write(writer, text_);
}

template <>
View<::test_object::polymorphic_allocator::WalkerNested> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::WalkerNested& data)
{
    View<::test_object::polymorphic_allocator::WalkerNested> view(data);
    read(reader, data.text);
    return view;
}

const ::zserio::pmr::ITypeInfo& TypeInfo<::test_object::polymorphic_allocator::WalkerNested, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
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
            {}, // initializer
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
        "test_object.polymorphic_allocator.WalkerNested",
        [](const AllocatorType& allocator) -> ::zserio::pmr::IReflectableDataPtr
        {
            return ::std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::polymorphic_allocator::WalkerNested>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(
        const ::test_object::polymorphic_allocator::WalkerNested& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::polymorphic_allocator::WalkerNested& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::WalkerNested>(), alloc),
                m_object(object)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::polymorphic_allocator::WalkerNested& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value, allocator);
}

template <>
::zserio::pmr::IReflectableDataPtr reflectable(
        ::test_object::polymorphic_allocator::WalkerNested& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::polymorphic_allocator::WalkerNested& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::WalkerNested>(), alloc),
                m_object(object)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::pmr::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        void setField(::std::string_view name, const ::zserio::pmr::Any& value) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::ReflectableUtil::fromAny<::zserio::pmr::String>(value);
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::pmr::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::pmr::String(get_allocator());
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::Any(::std::cref(m_object), alloc);
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) override
        {
            return ::zserio::pmr::Any(::std::ref(m_object), alloc);
        }

    private:
        ::test_object::polymorphic_allocator::WalkerNested& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value, allocator);
}

template <>
::zserio::pmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::polymorphic_allocator::WalkerNested>& view, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Introspectable : public ::zserio::CompoundIntrospectableViewBase<::test_object::polymorphic_allocator::WalkerNested, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        Introspectable(const ::zserio::View<::test_object::polymorphic_allocator::WalkerNested>& view_, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator) :
                ::zserio::CompoundIntrospectableViewBase<::test_object::polymorphic_allocator::WalkerNested, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(
                        view_, allocator)
        {}

        ::zserio::pmr::IIntrospectableViewConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::introspectable(getValue().text(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, view, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::polymorphic_allocator::WalkerNested>::operator()(const ::test_object::polymorphic_allocator::WalkerNested& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.text);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::polymorphic_allocator::WalkerNested>>::operator()(const ::zserio::View<::test_object::polymorphic_allocator::WalkerNested>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.text());
    return static_cast<size_t>(result);
}

} // namespace std
