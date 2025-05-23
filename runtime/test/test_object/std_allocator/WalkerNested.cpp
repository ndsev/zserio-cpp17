/**
 * Automatically generated by Zserio C++17 generator version 0.4.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_allocator/WalkerNested.h>

namespace test_object
{
namespace std_allocator
{

WalkerNested::WalkerNested() noexcept :
        WalkerNested(allocator_type{})
{}

WalkerNested::WalkerNested(const allocator_type& allocator) noexcept :
        text(allocator)
{}

WalkerNested::WalkerNested(WalkerNested&& other, const allocator_type& allocator) :
        text(std::move(other.text), allocator)
{}

WalkerNested::WalkerNested(const WalkerNested& other, const allocator_type& allocator) :
        text(other.text, allocator)
{}

WalkerNested::WalkerNested(
        ::zserio::String text_,
        const allocator_type& allocator) :
        text(::std::move(text_), allocator)
{}

bool operator==(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs)
{
    return ::std::tie(
            lhs.text)
            == ::std::tie(
            rhs.text);
}

bool operator<(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs)
{
    return ::std::tie(
            lhs.text)
            < ::std::tie(
            rhs.text);
}

bool operator!=(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::WalkerNested& lhs, const ::test_object::std_allocator::WalkerNested& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::WalkerNested>::View(const ::test_object::std_allocator::WalkerNested& data) noexcept :
        m_data(&data)
{}

View<::test_object::std_allocator::WalkerNested>::View(const ::test_object::std_allocator::WalkerNested& data,
        const View&) noexcept :
        m_data(&data)
{}

::std::string_view View<::test_object::std_allocator::WalkerNested>::text() const
{
    return ::std::string_view{
            m_data->text};
}

const ::test_object::std_allocator::WalkerNested& View<::test_object::std_allocator::WalkerNested>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs)
{
    return lhs.text() == rhs.text();
}

bool operator<(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs)
{
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }

    return false;
}

bool operator!=(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::WalkerNested>& lhs, const View<::test_object::std_allocator::WalkerNested>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::WalkerNested>& view, ::std::string_view)
{
    validate(view.text(), "'WalkerNested.text'");
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::WalkerNested>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::WalkerNested>& view)
{
    auto text_ = view.text();
    write(writer, text_);
}

template <>
View<::test_object::std_allocator::WalkerNested> read(BitStreamReader& reader, ::test_object::std_allocator::WalkerNested& data)
{
    View<::test_object::std_allocator::WalkerNested> view(data);
    read(reader, data.text);
    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::WalkerNested, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 1> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "text", // schemaName
            ::zserio::typeInfo<::zserio::String, ::std::allocator<uint8_t>>(), // typeInfo
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
        "test_object.std_allocator.WalkerNested",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return ::std::allocate_shared<::zserio::detail::ReflectableDataOwner<::test_object::std_allocator::WalkerNested>>(allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(
        const ::test_object::std_allocator::WalkerNested& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::WalkerNested& object, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::WalkerNested>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::WalkerNested& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::IReflectableDataPtr reflectable(
        ::test_object::std_allocator::WalkerNested& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::detail::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::detail::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::detail::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_allocator::WalkerNested& object, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::detail::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::WalkerNested>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::ReflectableUtil::fromAny<::zserio::String>(value);
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::String(get_allocator());
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) override
        {
            return ::zserio::Any(::std::ref(m_object), alloc);
        }

    private:
        ::test_object::std_allocator::WalkerNested& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_allocator::WalkerNested>& view, const ::std::allocator<uint8_t>& allocator)
{
    class Introspectable : public ::zserio::detail::CompoundIntrospectableViewBase<::test_object::std_allocator::WalkerNested, ::std::allocator<uint8_t>>
    {
    public:
        explicit Introspectable(const ::zserio::View<::test_object::std_allocator::WalkerNested>& view_, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::detail::CompoundIntrospectableViewBase<::test_object::std_allocator::WalkerNested, ::std::allocator<uint8_t>>(
                        view_, alloc)
        {}

        ::zserio::IIntrospectableViewConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::introspectable(getValue().text(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerNested'!";
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, view);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::WalkerNested>::operator()(const ::test_object::std_allocator::WalkerNested& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.text);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::WalkerNested>>::operator()(const ::zserio::View<::test_object::std_allocator::WalkerNested>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.text());
    return static_cast<size_t>(result);
}

} // namespace std
