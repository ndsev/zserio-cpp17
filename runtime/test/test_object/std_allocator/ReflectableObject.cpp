/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/ReflectableData.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_allocator/ReflectableObject.h>

namespace test_object
{
namespace std_allocator
{

ReflectableObject::ReflectableObject() noexcept :
        ReflectableObject(AllocatorType{})
{}

ReflectableObject::ReflectableObject(const AllocatorType& allocator) noexcept :
        stringField(allocator),
        reflectableNested(allocator)
{}

ReflectableObject::ReflectableObject(
        ::zserio::String stringField_,
        ::test_object::std_allocator::ReflectableNested reflectableNested_) :
        stringField(::std::move(stringField_)),
        reflectableNested(::std::move(reflectableNested_))
{}

bool operator==(const ::test_object::std_allocator::ReflectableObject& lhs, const ::test_object::std_allocator::ReflectableObject& rhs)
{
    return ::std::tie(
            lhs.stringField,
            lhs.reflectableNested)
            == ::std::tie(
            rhs.stringField,
            rhs.reflectableNested);
}

bool operator<(const ::test_object::std_allocator::ReflectableObject& lhs, const ::test_object::std_allocator::ReflectableObject& rhs)
{
    return ::std::tie(
            lhs.stringField,
            lhs.reflectableNested)
            < ::std::tie(
            rhs.stringField,
            rhs.reflectableNested);
}

bool operator!=(const ::test_object::std_allocator::ReflectableObject& lhs, const ::test_object::std_allocator::ReflectableObject& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::ReflectableObject& lhs, const ::test_object::std_allocator::ReflectableObject& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::ReflectableObject& lhs, const ::test_object::std_allocator::ReflectableObject& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::ReflectableObject& lhs, const ::test_object::std_allocator::ReflectableObject& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::ReflectableObject>::View(const ::test_object::std_allocator::ReflectableObject& data) noexcept :
        m_data(&data)
{}

View<::test_object::std_allocator::ReflectableObject>::View(const ::test_object::std_allocator::ReflectableObject& data,
        const View&) noexcept :
        m_data(&data)
{}

::std::string_view View<::test_object::std_allocator::ReflectableObject>::stringField() const
{
    return ::std::string_view{
            m_data->stringField};
}

View<::test_object::std_allocator::ReflectableNested> View<::test_object::std_allocator::ReflectableObject>::reflectableNested() const
{
    return View<::test_object::std_allocator::ReflectableNested>{
            m_data->reflectableNested, ::zserio::Int31(static_cast<::zserio::Int31::ValueType>(13)), stringField()};
}

const ::test_object::std_allocator::ReflectableObject& View<::test_object::std_allocator::ReflectableObject>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::ReflectableObject>& lhs, const View<::test_object::std_allocator::ReflectableObject>& rhs)
{
    return lhs.stringField() == rhs.stringField() &&
            lhs.reflectableNested() == rhs.reflectableNested();
}

bool operator<(const View<::test_object::std_allocator::ReflectableObject>& lhs, const View<::test_object::std_allocator::ReflectableObject>& rhs)
{
    if (lhs.stringField() != rhs.stringField())
    {
        return lhs.stringField() < rhs.stringField();
    }
    if (lhs.reflectableNested() != rhs.reflectableNested())
    {
        return lhs.reflectableNested() < rhs.reflectableNested();
    }

    return false;
}

bool operator!=(const View<::test_object::std_allocator::ReflectableObject>& lhs, const View<::test_object::std_allocator::ReflectableObject>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::ReflectableObject>& lhs, const View<::test_object::std_allocator::ReflectableObject>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::ReflectableObject>& lhs, const View<::test_object::std_allocator::ReflectableObject>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::ReflectableObject>& lhs, const View<::test_object::std_allocator::ReflectableObject>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::ReflectableObject>& view, ::std::string_view)
{
    validate(view.stringField(), "'ReflectableObject.stringField'");
    validate(view.reflectableNested(), "'ReflectableObject.reflectableNested'");
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::ReflectableObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto stringField_ = view.stringField();
    endBitPosition += bitSizeOf(stringField_, endBitPosition);
    auto reflectableNested_ = view.reflectableNested();
    endBitPosition += bitSizeOf(reflectableNested_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::ReflectableObject>& view)
{
    auto stringField_ = view.stringField();
    write(writer, stringField_);
    auto reflectableNested_ = view.reflectableNested();
    write(writer, reflectableNested_);
}

template <>
View<::test_object::std_allocator::ReflectableObject> read(BitStreamReader& reader, ::test_object::std_allocator::ReflectableObject& data)
{
    View<::test_object::std_allocator::ReflectableObject> view(data);
    read(reader, data.stringField);
    (void)read(reader, data.reflectableNested, ::zserio::Int31(static_cast<::zserio::Int31::ValueType>(13)), view.stringField());
    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::ReflectableObject, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::std::string_view, 2> reflectableNestedTypeArguments = {
        "13",
        "stringField()"
    };
    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 2> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "stringField", // schemaName
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
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "reflectableNested", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::ReflectableNested, ::std::allocator<uint8_t>>(), // typeInfo
            reflectableNestedTypeArguments, // typeArguments
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
        "test_object.std_allocator.ReflectableObject",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableOwner<::test_object::std_allocator::ReflectableObject>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(
        const ::test_object::std_allocator::ReflectableObject& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::ReflectableObject& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::ReflectableObject>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "stringField")
            {
                return ::zserio::reflectable(m_object.stringField, get_allocator());
            }
            if (name == "reflectableNested")
            {
                return ::zserio::reflectable(m_object.reflectableNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableObject'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::ReflectableObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

template <>
::zserio::IReflectableDataPtr reflectable(
        ::test_object::std_allocator::ReflectableObject& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        explicit Reflectable(::test_object::std_allocator::ReflectableObject& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::ReflectableObject>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "stringField")
            {
                return ::zserio::reflectable(m_object.stringField, get_allocator());
            }
            if (name == "reflectableNested")
            {
                return ::zserio::reflectable(m_object.reflectableNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableObject'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "stringField")
            {
                return ::zserio::reflectable(m_object.stringField, get_allocator());
            }
            if (name == "reflectableNested")
            {
                return ::zserio::reflectable(m_object.reflectableNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableObject'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "stringField")
            {
                m_object.stringField =
                        value.get<::zserio::String>();
                return;
            }
            if (name == "reflectableNested")
            {
                m_object.reflectableNested =
                        value.get<::test_object::std_allocator::ReflectableNested>();
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableObject'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "stringField")
            {
                m_object.stringField =
                        ::zserio::String(get_allocator());
                return ::zserio::reflectable(m_object.stringField, get_allocator());
            }
            if (name == "reflectableNested")
            {
                m_object.reflectableNested =
                        ::test_object::std_allocator::ReflectableNested(get_allocator());
                return ::zserio::reflectable(m_object.reflectableNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableObject'!";
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
        ::test_object::std_allocator::ReflectableObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::ReflectableObject>::operator()(const ::test_object::std_allocator::ReflectableObject& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.stringField);
    result = ::zserio::calcHashCode(result, data.reflectableNested);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::ReflectableObject>>::operator()(const ::zserio::View<::test_object::std_allocator::ReflectableObject>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.stringField());
    result = ::zserio::calcHashCode(result, view.reflectableNested());
    return static_cast<size_t>(result);
}

} // namespace std
