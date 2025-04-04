/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
#include <string_view>

#include <test_object/std_allocator/DebugStringParamObject.h>

namespace test_object
{
namespace std_allocator
{

DebugStringParamObject::DebugStringParamObject() noexcept :
        DebugStringParamObject(AllocatorType{})
{}

DebugStringParamObject::DebugStringParamObject(const AllocatorType& allocator) noexcept :
        text(::std::string_view{"test"}, allocator)
{}

DebugStringParamObject::DebugStringParamObject(
        ::zserio::String text_) :
        text(::std::move(text_))
{}

bool operator==(const ::test_object::std_allocator::DebugStringParamObject& lhs, const ::test_object::std_allocator::DebugStringParamObject& rhs)
{
    return ::std::tie(
            lhs.text)
            == ::std::tie(
            rhs.text);
}

bool operator<(const ::test_object::std_allocator::DebugStringParamObject& lhs, const ::test_object::std_allocator::DebugStringParamObject& rhs)
{
    return ::std::tie(
            lhs.text)
            < ::std::tie(
            rhs.text);
}

bool operator!=(const ::test_object::std_allocator::DebugStringParamObject& lhs, const ::test_object::std_allocator::DebugStringParamObject& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::DebugStringParamObject& lhs, const ::test_object::std_allocator::DebugStringParamObject& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::DebugStringParamObject& lhs, const ::test_object::std_allocator::DebugStringParamObject& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::DebugStringParamObject& lhs, const ::test_object::std_allocator::DebugStringParamObject& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::DebugStringParamObject>::View(const ::test_object::std_allocator::DebugStringParamObject& data,
        ::zserio::Int32 param_) noexcept :
        m_data(&data),
        m_param_(param_)
{}

View<::test_object::std_allocator::DebugStringParamObject>::View(const ::test_object::std_allocator::DebugStringParamObject& data,
        const View& other) noexcept :
        m_data(&data),
        m_param_(other.param())
{}

::zserio::Int32 View<::test_object::std_allocator::DebugStringParamObject>::param() const
{
    return m_param_;
}

::std::string_view View<::test_object::std_allocator::DebugStringParamObject>::text() const
{
    return ::std::string_view{
            m_data->text};
}

const ::test_object::std_allocator::DebugStringParamObject& View<::test_object::std_allocator::DebugStringParamObject>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::DebugStringParamObject>& lhs, const View<::test_object::std_allocator::DebugStringParamObject>& rhs)
{
    return lhs.param() == rhs.param() &&
            lhs.text() == rhs.text();
}

bool operator<(const View<::test_object::std_allocator::DebugStringParamObject>& lhs, const View<::test_object::std_allocator::DebugStringParamObject>& rhs)
{
    if (lhs.param() != rhs.param())
    {
        return lhs.param() < rhs.param();
    }
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }

    return false;
}

bool operator!=(const View<::test_object::std_allocator::DebugStringParamObject>& lhs, const View<::test_object::std_allocator::DebugStringParamObject>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::DebugStringParamObject>& lhs, const View<::test_object::std_allocator::DebugStringParamObject>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::DebugStringParamObject>& lhs, const View<::test_object::std_allocator::DebugStringParamObject>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::DebugStringParamObject>& lhs, const View<::test_object::std_allocator::DebugStringParamObject>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::DebugStringParamObject>& view, ::std::string_view)
{
    validate(view.param(), "'DebugStringParamObject.param'");
    validate(view.text(), "'DebugStringParamObject.text'");
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::DebugStringParamObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::DebugStringParamObject>& view)
{
    auto text_ = view.text();
    write(writer, text_);
}

template <>
View<::test_object::std_allocator::DebugStringParamObject> read(BitStreamReader& reader, ::test_object::std_allocator::DebugStringParamObject& data,
        ::zserio::Int32 param_)
{
    View<::test_object::std_allocator::DebugStringParamObject> view(data,
            param_);
    read(reader, data.text);
    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::DebugStringParamObject, ::std::allocator<uint8_t>>::get()
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

    static const ::std::array<::zserio::BasicParameterInfo<AllocatorType>, 1> parameters = {
        ::zserio::BasicParameterInfo<AllocatorType>{
            "param",
            ::zserio::typeInfo<::zserio::Int32, ::std::allocator<uint8_t>>()
        }
    };

    static const ::zserio::Span<::zserio::BasicFunctionInfo<AllocatorType>> functions;

    static const ::zserio::detail::StructTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_allocator.DebugStringParamObject",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::std_allocator::DebugStringParamObject>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(
        const ::test_object::std_allocator::DebugStringParamObject& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::DebugStringParamObject& object, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::DebugStringParamObject>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringParamObject'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::DebugStringParamObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

template <>
::zserio::IReflectableDataPtr reflectable(
        ::test_object::std_allocator::DebugStringParamObject& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_allocator::DebugStringParamObject& object, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::DebugStringParamObject>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringParamObject'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringParamObject'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::ReflectableUtil::fromAny<::zserio::String>(value);
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringParamObject'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "text")
            {
                m_object.text = ::zserio::String(get_allocator());
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'DebugStringParamObject'!";
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
        ::test_object::std_allocator::DebugStringParamObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::DebugStringParamObject>::operator()(const ::test_object::std_allocator::DebugStringParamObject& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.text);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::DebugStringParamObject>>::operator()(const ::zserio::View<::test_object::std_allocator::DebugStringParamObject>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.param());
    result = ::zserio::calcHashCode(result, view.text());
    return static_cast<size_t>(result);
}

} // namespace std
