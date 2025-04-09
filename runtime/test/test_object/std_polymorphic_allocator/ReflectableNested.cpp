/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, std-polymorphicAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_polymorphic_allocator/ReflectableNested.h>

namespace test_object
{
namespace std_polymorphic_allocator
{

ReflectableNested::ReflectableNested() noexcept :
        ReflectableNested(allocator_type{})
{}

ReflectableNested::ReflectableNested(const allocator_type&) noexcept :
        value()
{}

ReflectableNested::ReflectableNested(ReflectableNested&& other_, const allocator_type&) :
        value(other_.value)
{}

ReflectableNested::ReflectableNested(const ReflectableNested& other_, const allocator_type&) :
        value(other_.value)
{}

ReflectableNested::ReflectableNested(
        ::zserio::UInt31 value_,
        const allocator_type&) :
        value(value_)
{}

bool operator==(const ::test_object::std_polymorphic_allocator::ReflectableNested& lhs, const ::test_object::std_polymorphic_allocator::ReflectableNested& rhs)
{
    return ::std::tie(
            lhs.value)
            == ::std::tie(
            rhs.value);
}

bool operator<(const ::test_object::std_polymorphic_allocator::ReflectableNested& lhs, const ::test_object::std_polymorphic_allocator::ReflectableNested& rhs)
{
    return ::std::tie(
            lhs.value)
            < ::std::tie(
            rhs.value);
}

bool operator!=(const ::test_object::std_polymorphic_allocator::ReflectableNested& lhs, const ::test_object::std_polymorphic_allocator::ReflectableNested& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_polymorphic_allocator::ReflectableNested& lhs, const ::test_object::std_polymorphic_allocator::ReflectableNested& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_polymorphic_allocator::ReflectableNested& lhs, const ::test_object::std_polymorphic_allocator::ReflectableNested& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_polymorphic_allocator::ReflectableNested& lhs, const ::test_object::std_polymorphic_allocator::ReflectableNested& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_polymorphic_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_polymorphic_allocator::ReflectableNested>::View(const ::test_object::std_polymorphic_allocator::ReflectableNested& data,
        ::zserio::Int31 dummyParam_,
        ::std::string_view stringParam_) noexcept :
        m_data(&data),
        m_dummyParam_(dummyParam_),
        m_stringParam_(stringParam_)
{}

View<::test_object::std_polymorphic_allocator::ReflectableNested>::View(const ::test_object::std_polymorphic_allocator::ReflectableNested& data,
        const View& other) noexcept :
        m_data(&data),
        m_dummyParam_(other.dummyParam()),
        m_stringParam_(other.stringParam())
{}

::zserio::Int31 View<::test_object::std_polymorphic_allocator::ReflectableNested>::dummyParam() const
{
    return m_dummyParam_;
}

::std::string_view View<::test_object::std_polymorphic_allocator::ReflectableNested>::stringParam() const
{
    return m_stringParam_;
}

::zserio::UInt31 View<::test_object::std_polymorphic_allocator::ReflectableNested>::value() const
{
    return m_data->value;
}

::zserio::UInt31 View<::test_object::std_polymorphic_allocator::ReflectableNested>::getValue() const
{
    return static_cast<::zserio::UInt31::ValueType>(value());
}

const ::test_object::std_polymorphic_allocator::ReflectableNested& View<::test_object::std_polymorphic_allocator::ReflectableNested>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableNested>& rhs)
{
    return lhs.dummyParam() == rhs.dummyParam() &&
            lhs.stringParam() == rhs.stringParam() &&
            lhs.value() == rhs.value();
}

bool operator<(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableNested>& rhs)
{
    if (lhs.dummyParam() != rhs.dummyParam())
    {
        return lhs.dummyParam() < rhs.dummyParam();
    }
    if (lhs.stringParam() != rhs.stringParam())
    {
        return lhs.stringParam() < rhs.stringParam();
    }
    if (lhs.value() != rhs.value())
    {
        return lhs.value() < rhs.value();
    }

    return false;
}

bool operator!=(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableNested>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableNested>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableNested>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableNested>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& view, ::std::string_view)
{
    validate(view.dummyParam(), "'ReflectableNested.dummyParam'");
    validate(view.stringParam(), "'ReflectableNested.stringParam'");
    validate(view.value(), "'ReflectableNested.value'");
}

template <>
BitSize bitSizeOf(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto value_ = view.value();
    endBitPosition += bitSizeOf(value_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_polymorphic_allocator::ReflectableNested>& view)
{
    auto value_ = view.value();
    write(writer, value_);
}

template <>
View<::test_object::std_polymorphic_allocator::ReflectableNested> read(BitStreamReader& reader, ::test_object::std_polymorphic_allocator::ReflectableNested& data,
        ::zserio::Int31 dummyParam_,
        ::std::string_view stringParam_)
{
    View<::test_object::std_polymorphic_allocator::ReflectableNested> view(data,
            dummyParam_,
            stringParam_);
    read(reader, data.value);
    return view;
}

const ::zserio::std_pmr::ITypeInfo& TypeInfo<::test_object::std_polymorphic_allocator::ReflectableNested, ::std::pmr::polymorphic_allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::pmr::polymorphic_allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 1> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "value", // schemaName
            ::zserio::typeInfo<::zserio::UInt31, ::std::pmr::polymorphic_allocator<uint8_t>>(), // typeInfo
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

    static const ::std::array<::zserio::BasicParameterInfo<AllocatorType>, 2> parameters = {
        ::zserio::BasicParameterInfo<AllocatorType>{
            "dummyParam",
            ::zserio::typeInfo<::zserio::Int31, ::std::pmr::polymorphic_allocator<uint8_t>>()
        },
        ::zserio::BasicParameterInfo<AllocatorType>{
            "stringParam",
            ::zserio::typeInfo<::zserio::std_pmr::String, ::std::pmr::polymorphic_allocator<uint8_t>>()
        }
    };

    static const ::std::array<::zserio::BasicFunctionInfo<AllocatorType>, 1> functions = {
        ::zserio::BasicFunctionInfo<AllocatorType>{
            "getValue",
            ::zserio::typeInfo<::zserio::UInt31, ::std::pmr::polymorphic_allocator<uint8_t>>(),
            "value()"
        }
    };

    static const ::zserio::detail::StructTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_polymorphic_allocator.ReflectableNested",
        [](const AllocatorType& allocator) -> ::zserio::std_pmr::IReflectableDataPtr
        {
            return ::std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::std_polymorphic_allocator::ReflectableNested>>(allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::std_pmr::IReflectableDataConstPtr reflectable(
        const ::test_object::std_polymorphic_allocator::ReflectableNested& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_polymorphic_allocator::ReflectableNested& object, const ::std::pmr::polymorphic_allocator<uint8_t>& alloc = {}) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>(typeInfo<::test_object::std_polymorphic_allocator::ReflectableNested>(), alloc),
                m_object(object)
        {}

        ::zserio::std_pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableNested'!";
        }

        ::zserio::std_pmr::Any getAnyValue(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) const override
        {
            return ::zserio::std_pmr::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_polymorphic_allocator::ReflectableNested& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::std_pmr::IReflectableDataPtr reflectable(
        ::test_object::std_polymorphic_allocator::ReflectableNested& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_polymorphic_allocator::ReflectableNested& object, const ::std::pmr::polymorphic_allocator<uint8_t>& alloc = {}) :
                ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>(typeInfo<::test_object::std_polymorphic_allocator::ReflectableNested>(), alloc),
                m_object(object)
        {}

        ::zserio::std_pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableNested'!";
        }

        ::zserio::std_pmr::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableNested'!";
        }

        void setField(::std::string_view name, const ::zserio::std_pmr::Any& value) override
        {
            if (name == "value")
            {
                m_object.value = ::zserio::ReflectableUtil::fromAny<::zserio::UInt31>(value);
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableNested'!";
        }

        ::zserio::std_pmr::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "value")
            {
                m_object.value = ::zserio::UInt31();
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableNested'!";
        }

        ::zserio::std_pmr::Any getAnyValue(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) const override
        {
            return ::zserio::std_pmr::Any(::std::cref(m_object), alloc);
        }

        ::zserio::std_pmr::Any getAnyValue(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) override
        {
            return ::zserio::std_pmr::Any(::std::ref(m_object), alloc);
        }

    private:
        ::test_object::std_polymorphic_allocator::ReflectableNested& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::std_pmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_polymorphic_allocator::ReflectableNested>& view, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Introspectable : public ::zserio::CompoundIntrospectableViewBase<::test_object::std_polymorphic_allocator::ReflectableNested, ::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        explicit Introspectable(const ::zserio::View<::test_object::std_polymorphic_allocator::ReflectableNested>& view_, const ::std::pmr::polymorphic_allocator<uint8_t>& alloc = {}) :
                ::zserio::CompoundIntrospectableViewBase<::test_object::std_polymorphic_allocator::ReflectableNested, ::std::pmr::polymorphic_allocator<uint8_t>>(
                        view_, alloc)
        {}

        ::zserio::std_pmr::IIntrospectableViewConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::introspectable(getValue().value(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableNested'!";
        }

        ::zserio::std_pmr::IIntrospectableViewConstPtr getParameter(::std::string_view name) const override
        {
            if (name == "dummyParam")
            {
                return ::zserio::introspectable(getValue().dummyParam(), get_allocator());
            }
            if (name == "stringParam")
            {
                return ::zserio::introspectable(getValue().stringParam(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Parameter '") << name << "' doesn't exist in 'ReflectableNested'!";
        }

        ::zserio::std_pmr::IIntrospectableViewConstPtr callFunction(::std::string_view name) const override
        {
            if (name == "getValue")
            {
                return ::zserio::introspectable(getValue().getValue(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Function '") << name << "' doesn't exist in 'ReflectableNested'!";
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, view);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_polymorphic_allocator::ReflectableNested>::operator()(const ::test_object::std_polymorphic_allocator::ReflectableNested& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.value);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_polymorphic_allocator::ReflectableNested>>::operator()(const ::zserio::View<::test_object::std_polymorphic_allocator::ReflectableNested>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.dummyParam());
    result = ::zserio::calcHashCode(result, view.stringParam());
    result = ::zserio::calcHashCode(result, view.value());
    return static_cast<size_t>(result);
}

} // namespace std
