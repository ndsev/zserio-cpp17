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

#include <test_object/std_allocator/CreatorObject.h>

namespace test_object
{
namespace std_allocator
{

CreatorObject::CreatorObject() noexcept :
        CreatorObject(AllocatorType{})
{}

CreatorObject::CreatorObject(const AllocatorType& allocator) noexcept :
        value(),
        nested(allocator),
        text(allocator),
        nestedArray(allocator),
        textArray(allocator),
        externArray(allocator),
        bytesArray(allocator),
        optionalBool(allocator),
        optionalNested(allocator)
{}

CreatorObject::CreatorObject(
        ::zserio::UInt32 value_,
        ::test_object::std_allocator::CreatorNested nested_,
        ::zserio::String text_,
        ::zserio::Vector<::test_object::std_allocator::CreatorNested> nestedArray_,
        ::zserio::Vector<::zserio::String> textArray_,
        ::zserio::Optional<::zserio::Vector<::zserio::BitBuffer>> externArray_,
        ::zserio::Optional<::zserio::Vector<::zserio::Bytes>> bytesArray_,
        ::zserio::Optional<::zserio::Bool> optionalBool_,
        ::zserio::Optional<::test_object::std_allocator::CreatorNested> optionalNested_) :
        value(value_),
        nested(::std::move(nested_)),
        text(::std::move(text_)),
        nestedArray(::std::move(nestedArray_)),
        textArray(::std::move(textArray_)),
        externArray(::std::move(externArray_)),
        bytesArray(::std::move(bytesArray_)),
        optionalBool(::std::move(optionalBool_)),
        optionalNested(::std::move(optionalNested_))
{}

bool operator==(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs)
{
    return ::std::tie(
            lhs.value,
            lhs.nested,
            lhs.text,
            lhs.nestedArray,
            lhs.textArray,
            lhs.externArray,
            lhs.bytesArray,
            lhs.optionalBool,
            lhs.optionalNested)
            == ::std::tie(
            rhs.value,
            rhs.nested,
            rhs.text,
            rhs.nestedArray,
            rhs.textArray,
            rhs.externArray,
            rhs.bytesArray,
            rhs.optionalBool,
            rhs.optionalNested);
}

bool operator<(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs)
{
    return ::std::tie(
            lhs.value,
            lhs.nested,
            lhs.text,
            lhs.nestedArray,
            lhs.textArray,
            lhs.externArray,
            lhs.bytesArray,
            lhs.optionalBool,
            lhs.optionalNested)
            < ::std::tie(
            rhs.value,
            rhs.nested,
            rhs.text,
            rhs.nestedArray,
            rhs.textArray,
            rhs.externArray,
            rhs.bytesArray,
            rhs.optionalBool,
            rhs.optionalNested);
}

bool operator!=(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::CreatorObject& lhs, const ::test_object::std_allocator::CreatorObject& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{


View<::test_object::std_allocator::CreatorNested> View<::test_object::std_allocator::CreatorObject>::ZserioNestedArrayArrayTraits::at(const OwnerType& owner,
        const ::test_object::std_allocator::CreatorNested& element, size_t)
{
    return View<::test_object::std_allocator::CreatorNested>(element, ::zserio::UInt32(static_cast<::zserio::UInt32::ValueType>(owner.value())));
}

void View<::test_object::std_allocator::CreatorObject>::ZserioNestedArrayArrayTraits::read(BitStreamReader& reader, const OwnerType& owner,
                ::test_object::std_allocator::CreatorNested& element, size_t)
{
    (void)detail::read(reader, element, ::zserio::UInt32(static_cast<::zserio::UInt32::ValueType>(owner.value())));
}
View<::test_object::std_allocator::CreatorObject>::View(const ::test_object::std_allocator::CreatorObject& data) noexcept :
        m_data(&data)
{}

View<::test_object::std_allocator::CreatorObject>::View(const ::test_object::std_allocator::CreatorObject& data,
        const View&) noexcept :
        m_data(&data)
{}

::zserio::UInt32 View<::test_object::std_allocator::CreatorObject>::value() const
{
    return m_data->value;
}

View<::test_object::std_allocator::CreatorNested> View<::test_object::std_allocator::CreatorObject>::nested() const
{
    return View<::test_object::std_allocator::CreatorNested>{
            m_data->nested, ::zserio::UInt32(static_cast<::zserio::UInt32::ValueType>(value()))};
}

::std::string_view View<::test_object::std_allocator::CreatorObject>::text() const
{
    return ::std::string_view{
            m_data->text};
}

Array<::zserio::Vector<::test_object::std_allocator::CreatorNested>, ArrayType::AUTO, ::zserio::ArrayStorage::IMMUTABLE, View<::test_object::std_allocator::CreatorObject>::ZserioNestedArrayArrayTraits> View<::test_object::std_allocator::CreatorObject>::nestedArray() const
{
    return Array<::zserio::Vector<::test_object::std_allocator::CreatorNested>, ArrayType::AUTO, ::zserio::ArrayStorage::IMMUTABLE, ZserioNestedArrayArrayTraits>{
            m_data->nestedArray, *this};
}

Array<::zserio::Vector<::zserio::String>, ArrayType::AUTO> View<::test_object::std_allocator::CreatorObject>::textArray() const
{
    return Array<::zserio::Vector<::zserio::String>, ArrayType::AUTO>{
            m_data->textArray};
}

::zserio::Optional<Array<::zserio::Vector<::zserio::BitBuffer>, ArrayType::AUTO>> View<::test_object::std_allocator::CreatorObject>::externArray() const
{
    if (m_data->externArray.has_value())
    {
        return ::zserio::Optional<Array<::zserio::Vector<::zserio::BitBuffer>, ArrayType::AUTO>>{
                ::std::in_place, m_data->externArray.get_allocator(), *m_data->externArray};
    }
    else
    {
        return ::zserio::Optional<Array<::zserio::Vector<::zserio::BitBuffer>, ArrayType::AUTO>>(::std::nullopt, m_data->externArray.get_allocator());
    }
}

::zserio::Optional<Array<::zserio::Vector<::zserio::Bytes>, ArrayType::AUTO>> View<::test_object::std_allocator::CreatorObject>::bytesArray() const
{
    if (m_data->bytesArray.has_value())
    {
        return ::zserio::Optional<Array<::zserio::Vector<::zserio::Bytes>, ArrayType::AUTO>>{
                ::std::in_place, m_data->bytesArray.get_allocator(), *m_data->bytesArray};
    }
    else
    {
        return ::zserio::Optional<Array<::zserio::Vector<::zserio::Bytes>, ArrayType::AUTO>>(::std::nullopt, m_data->bytesArray.get_allocator());
    }
}

::zserio::Optional<::zserio::Bool> View<::test_object::std_allocator::CreatorObject>::optionalBool() const
{
    return m_data->optionalBool;
}

::zserio::Optional<View<::test_object::std_allocator::CreatorNested>> View<::test_object::std_allocator::CreatorObject>::optionalNested() const
{
    if (m_data->optionalNested.has_value())
    {
        return ::zserio::Optional<View<::test_object::std_allocator::CreatorNested>>{
                ::std::in_place, m_data->optionalNested.get_allocator(), *m_data->optionalNested, ::zserio::UInt32(static_cast<::zserio::UInt32::ValueType>(value()))};
    }
    else
    {
        return ::zserio::Optional<View<::test_object::std_allocator::CreatorNested>>(::std::nullopt, m_data->optionalNested.get_allocator());
    }
}

const ::test_object::std_allocator::CreatorObject& View<::test_object::std_allocator::CreatorObject>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs)
{
    return lhs.value() == rhs.value() &&
            lhs.nested() == rhs.nested() &&
            lhs.text() == rhs.text() &&
            lhs.nestedArray() == rhs.nestedArray() &&
            lhs.textArray() == rhs.textArray() &&
            lhs.externArray() == rhs.externArray() &&
            lhs.bytesArray() == rhs.bytesArray() &&
            lhs.optionalBool() == rhs.optionalBool() &&
            lhs.optionalNested() == rhs.optionalNested();
}

bool operator<(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs)
{
    if (lhs.value() != rhs.value())
    {
        return lhs.value() < rhs.value();
    }
    if (lhs.nested() != rhs.nested())
    {
        return lhs.nested() < rhs.nested();
    }
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }
    if (lhs.nestedArray() != rhs.nestedArray())
    {
        return lhs.nestedArray() < rhs.nestedArray();
    }
    if (lhs.textArray() != rhs.textArray())
    {
        return lhs.textArray() < rhs.textArray();
    }
    if (lhs.externArray() != rhs.externArray())
    {
        return lhs.externArray() < rhs.externArray();
    }
    if (lhs.bytesArray() != rhs.bytesArray())
    {
        return lhs.bytesArray() < rhs.bytesArray();
    }
    if (lhs.optionalBool() != rhs.optionalBool())
    {
        return lhs.optionalBool() < rhs.optionalBool();
    }
    if (lhs.optionalNested() != rhs.optionalNested())
    {
        return lhs.optionalNested() < rhs.optionalNested();
    }

    return false;
}

bool operator!=(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::CreatorObject>& lhs, const View<::test_object::std_allocator::CreatorObject>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::CreatorObject>& view, ::std::string_view)
{
    validate(view.value(), "'CreatorObject.value'");
    validate(view.nested(), "'CreatorObject.nested'");
    validate(view.text(), "'CreatorObject.text'");
    validate(view.nestedArray(), "'CreatorObject.nestedArray'");
    validate(view.textArray(), "'CreatorObject.textArray'");
    if (view.externArray().has_value())
    {
        validate(*view.externArray(), "'CreatorObject.externArray'");
    }
    if (view.bytesArray().has_value())
    {
        validate(*view.bytesArray(), "'CreatorObject.bytesArray'");
    }
    if (view.optionalBool().has_value())
    {
        validate(*view.optionalBool(), "'CreatorObject.optionalBool'");
    }
    if (view.optionalNested().has_value())
    {
        validate(*view.optionalNested(), "'CreatorObject.optionalNested'");
    }
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::CreatorObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto value_ = view.value();
    endBitPosition += bitSizeOf(value_, endBitPosition);
    auto nested_ = view.nested();
    endBitPosition += bitSizeOf(nested_, endBitPosition);
    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);
    auto nestedArray_ = view.nestedArray();
    endBitPosition += bitSizeOf(nestedArray_, endBitPosition);
    auto textArray_ = view.textArray();
    endBitPosition += bitSizeOf(textArray_, endBitPosition);
    auto externArray_ = view.externArray();
    endBitPosition += bitSizeOf(Bool());
    if (externArray_.has_value())
    {
        endBitPosition += bitSizeOf(*externArray_, endBitPosition);
    }
    auto bytesArray_ = view.bytesArray();
    endBitPosition += bitSizeOf(Bool());
    if (bytesArray_.has_value())
    {
        endBitPosition += bitSizeOf(*bytesArray_, endBitPosition);
    }
    auto optionalBool_ = view.optionalBool();
    endBitPosition += bitSizeOf(Bool());
    if (optionalBool_.has_value())
    {
        endBitPosition += bitSizeOf(*optionalBool_, endBitPosition);
    }
    auto optionalNested_ = view.optionalNested();
    endBitPosition += bitSizeOf(Bool());
    if (optionalNested_.has_value())
    {
        endBitPosition += bitSizeOf(*optionalNested_, endBitPosition);
    }

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::CreatorObject>& view)
{
    auto value_ = view.value();
    write(writer, value_);
    auto nested_ = view.nested();
    write(writer, nested_);
    auto text_ = view.text();
    write(writer, text_);
    auto nestedArray_ = view.nestedArray();
    write(writer, nestedArray_);
    auto textArray_ = view.textArray();
    write(writer, textArray_);
    auto externArray_ = view.externArray();
    if (externArray_.has_value())
    {
        writer.writeBool(true);
        write(writer, *externArray_);
    }
    else
    {
        writer.writeBool(false);
    }
    auto bytesArray_ = view.bytesArray();
    if (bytesArray_.has_value())
    {
        writer.writeBool(true);
        write(writer, *bytesArray_);
    }
    else
    {
        writer.writeBool(false);
    }
    auto optionalBool_ = view.optionalBool();
    if (optionalBool_.has_value())
    {
        writer.writeBool(true);
        write(writer, *optionalBool_);
    }
    else
    {
        writer.writeBool(false);
    }
    auto optionalNested_ = view.optionalNested();
    if (optionalNested_.has_value())
    {
        writer.writeBool(true);
        write(writer, *optionalNested_);
    }
    else
    {
        writer.writeBool(false);
    }
}

template <>
View<::test_object::std_allocator::CreatorObject> read(BitStreamReader& reader, ::test_object::std_allocator::CreatorObject& data)
{
    View<::test_object::std_allocator::CreatorObject> view(data);
    read(reader, data.value);
    (void)read(reader, data.nested, ::zserio::UInt32(static_cast<::zserio::UInt32::ValueType>(view.value())));
    read(reader, data.text);
    (void)read<Array<::zserio::Vector<::test_object::std_allocator::CreatorNested>, ArrayType::AUTO, ::zserio::ArrayStorage::IMMUTABLE, View<::test_object::std_allocator::CreatorObject>::ZserioNestedArrayArrayTraits>>(reader, data.nestedArray, view);
    read<Array<::zserio::Vector<::zserio::String>, ArrayType::AUTO>>(reader, data.textArray);
    if (reader.readBool())
    {
        data.externArray.emplace(data.externArray.get_allocator());
        read<Array<::zserio::Vector<::zserio::BitBuffer>, ArrayType::AUTO>>(reader, *data.externArray);
    }
    if (reader.readBool())
    {
        data.bytesArray.emplace(data.bytesArray.get_allocator());
        read<Array<::zserio::Vector<::zserio::Bytes>, ArrayType::AUTO>>(reader, *data.bytesArray);
    }
    if (reader.readBool())
    {
        data.optionalBool.emplace();
        read(reader, *data.optionalBool);
    }
    if (reader.readBool())
    {
        data.optionalNested.emplace(data.optionalNested.get_allocator());
        (void)read(reader, *data.optionalNested, ::zserio::UInt32(static_cast<::zserio::UInt32::ValueType>(view.value())));
    }
    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::CreatorObject, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::std::string_view, 1> nestedTypeArguments = {
        "value()"
    };
    static const ::std::array<::std::string_view, 1> nestedArrayTypeArguments = {
        "value()"
    };
    static const ::std::array<::std::string_view, 1> optionalNestedTypeArguments = {
        "value()"
    };
    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 9> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "value", // schemaName
            ::zserio::typeInfo<::zserio::UInt32, ::std::allocator<uint8_t>>(), // typeInfo
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
            "nested", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::CreatorNested, ::std::allocator<uint8_t>>(), // typeInfo
            nestedTypeArguments, // typeArguments
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
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "nestedArray", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::CreatorNested, ::std::allocator<uint8_t>>(), // typeInfo
            nestedArrayTypeArguments, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            {}, // initializer
            false, // isOptional
            {}, // optionalClause
            {}, // constraint
            true, // isArray
            {}, // arrayLength
            false, // isPacked
            false // isImplicit
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "textArray", // schemaName
            ::zserio::typeInfo<::zserio::String, ::std::allocator<uint8_t>>(), // typeInfo
            {}, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            {}, // initializer
            false, // isOptional
            {}, // optionalClause
            {}, // constraint
            true, // isArray
            {}, // arrayLength
            false, // isPacked
            false // isImplicit
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "externArray", // schemaName
            ::zserio::typeInfo<::zserio::BitBuffer, ::std::allocator<uint8_t>>(), // typeInfo
            {}, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            {}, // initializer
            true, // isOptional
            {}, // optionalClause
            {}, // constraint
            true, // isArray
            {}, // arrayLength
            false, // isPacked
            false // isImplicit
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "bytesArray", // schemaName
            ::zserio::typeInfo<::zserio::Bytes, ::std::allocator<uint8_t>>(), // typeInfo
            {}, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            {}, // initializer
            true, // isOptional
            {}, // optionalClause
            {}, // constraint
            true, // isArray
            {}, // arrayLength
            false, // isPacked
            false // isImplicit
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "optionalBool", // schemaName
            ::zserio::typeInfo<::zserio::Bool, ::std::allocator<uint8_t>>(), // typeInfo
            {}, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            {}, // initializer
            true, // isOptional
            {}, // optionalClause
            {}, // constraint
            false, // isArray
            {}, // arrayLength
            false, // isPacked
            false // isImplicit
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "optionalNested", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::CreatorNested, ::std::allocator<uint8_t>>(), // typeInfo
            optionalNestedTypeArguments, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            {}, // initializer
            true, // isOptional
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
        "test_object.std_allocator.CreatorObject",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::std_allocator::CreatorObject>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(
        const ::test_object::std_allocator::CreatorObject& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::CreatorObject& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::CreatorObject>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "nested")
            {
                return ::zserio::reflectable(m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "nestedArray")
            {
                return ::zserio::reflectableArray(m_object.nestedArray, get_allocator());
            }
            if (name == "textArray")
            {
                return ::zserio::reflectableArray(m_object.textArray, get_allocator());
            }
            if (name == "externArray")
            {
                if (!m_object.externArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.externArray, get_allocator());
            }
            if (name == "bytesArray")
            {
                if (!m_object.bytesArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.bytesArray, get_allocator());
            }
            if (name == "optionalBool")
            {
                if (!m_object.optionalBool.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.optionalBool, get_allocator());
            }
            if (name == "optionalNested")
            {
                if (!m_object.optionalNested.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.optionalNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorObject'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::CreatorObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

template <>
::zserio::IReflectableDataPtr reflectable(
        ::test_object::std_allocator::CreatorObject& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_allocator::CreatorObject& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::CreatorObject>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "nested")
            {
                return ::zserio::reflectable(m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "nestedArray")
            {
                return ::zserio::reflectableArray(m_object.nestedArray, get_allocator());
            }
            if (name == "textArray")
            {
                return ::zserio::reflectableArray(m_object.textArray, get_allocator());
            }
            if (name == "externArray")
            {
                if (!m_object.externArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.externArray, get_allocator());
            }
            if (name == "bytesArray")
            {
                if (!m_object.bytesArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.bytesArray, get_allocator());
            }
            if (name == "optionalBool")
            {
                if (!m_object.optionalBool.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.optionalBool, get_allocator());
            }
            if (name == "optionalNested")
            {
                if (!m_object.optionalNested.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.optionalNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorObject'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "nested")
            {
                return ::zserio::reflectable(m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "nestedArray")
            {
                return ::zserio::reflectableArray(m_object.nestedArray, get_allocator());
            }
            if (name == "textArray")
            {
                return ::zserio::reflectableArray(m_object.textArray, get_allocator());
            }
            if (name == "externArray")
            {
                if (!m_object.externArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.externArray, get_allocator());
            }
            if (name == "bytesArray")
            {
                if (!m_object.bytesArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.bytesArray, get_allocator());
            }
            if (name == "optionalBool")
            {
                if (!m_object.optionalBool.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.optionalBool, get_allocator());
            }
            if (name == "optionalNested")
            {
                if (!m_object.optionalNested.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.optionalNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorObject'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "value")
            {
                if (value.isType<::zserio::UInt32>())
                {
                    m_object.value = value.get<::zserio::UInt32>();
                }
                else
                {
                    m_object.value = value.get<::zserio::UInt32::ValueType>();
                }
                return;
            }
            if (name == "nested")
            {
                m_object.nested = value.get<::test_object::std_allocator::CreatorNested>();
                return;
            }
            if (name == "text")
            {
                m_object.text = value.get<::zserio::String>();
                return;
            }
            if (name == "nestedArray")
            {
                m_object.nestedArray = value.get<::zserio::Vector<::test_object::std_allocator::CreatorNested>>();
                return;
            }
            if (name == "textArray")
            {
                m_object.textArray = value.get<::zserio::Vector<::zserio::String>>();
                return;
            }
            if (name == "externArray")
            {
                if (value.isType<::std::nullptr_t>())
                {
                    m_object.externArray.reset();
                    return;
                }

                m_object.externArray = value.get<::zserio::Vector<::zserio::BitBuffer>>();
                return;
            }
            if (name == "bytesArray")
            {
                if (value.isType<::std::nullptr_t>())
                {
                    m_object.bytesArray.reset();
                    return;
                }

                m_object.bytesArray = value.get<::zserio::Vector<::zserio::Bytes>>();
                return;
            }
            if (name == "optionalBool")
            {
                if (value.isType<::std::nullptr_t>())
                {
                    m_object.optionalBool.reset();
                    return;
                }

                if (value.isType<::zserio::Bool>())
                {
                    m_object.optionalBool = value.get<::zserio::Bool>();
                }
                else
                {
                    m_object.optionalBool = value.get<::zserio::Bool::ValueType>();
                }
                return;
            }
            if (name == "optionalNested")
            {
                if (value.isType<::std::nullptr_t>())
                {
                    m_object.optionalNested.reset();
                    return;
                }

                m_object.optionalNested = value.get<::test_object::std_allocator::CreatorNested>();
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorObject'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "value")
            {
                m_object.value = ::zserio::UInt32();
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "nested")
            {
                m_object.nested = ::test_object::std_allocator::CreatorNested(get_allocator());
                return ::zserio::reflectable(m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                m_object.text = ::zserio::String(get_allocator());
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "nestedArray")
            {
                m_object.nestedArray = ::zserio::Vector<::test_object::std_allocator::CreatorNested>(get_allocator());
                return ::zserio::reflectableArray(m_object.nestedArray, get_allocator());
            }
            if (name == "textArray")
            {
                m_object.textArray = ::zserio::Vector<::zserio::String>(get_allocator());
                return ::zserio::reflectableArray(m_object.textArray, get_allocator());
            }
            if (name == "externArray")
            {
                m_object.externArray = ::zserio::Vector<::zserio::BitBuffer>(get_allocator());
                return ::zserio::reflectableArray(*m_object.externArray, get_allocator());
            }
            if (name == "bytesArray")
            {
                m_object.bytesArray = ::zserio::Vector<::zserio::Bytes>(get_allocator());
                return ::zserio::reflectableArray(*m_object.bytesArray, get_allocator());
            }
            if (name == "optionalBool")
            {
                m_object.optionalBool = ::zserio::Bool();
                return ::zserio::reflectable(*m_object.optionalBool, get_allocator());
            }
            if (name == "optionalNested")
            {
                m_object.optionalNested = ::test_object::std_allocator::CreatorNested(get_allocator());
                return ::zserio::reflectable(*m_object.optionalNested, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorObject'!";
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
        ::test_object::std_allocator::CreatorObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::CreatorObject>::operator()(const ::test_object::std_allocator::CreatorObject& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.value);
    result = ::zserio::calcHashCode(result, data.nested);
    result = ::zserio::calcHashCode(result, data.text);
    result = ::zserio::calcHashCode(result, data.nestedArray);
    result = ::zserio::calcHashCode(result, data.textArray);
    result = ::zserio::calcHashCode(result, data.externArray);
    result = ::zserio::calcHashCode(result, data.bytesArray);
    result = ::zserio::calcHashCode(result, data.optionalBool);
    result = ::zserio::calcHashCode(result, data.optionalNested);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::CreatorObject>>::operator()(const ::zserio::View<::test_object::std_allocator::CreatorObject>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.value());
    result = ::zserio::calcHashCode(result, view.nested());
    result = ::zserio::calcHashCode(result, view.text());
    result = ::zserio::calcHashCode(result, view.nestedArray());
    result = ::zserio::calcHashCode(result, view.textArray());
    if (view.externArray())
    {
        result = ::zserio::calcHashCode(result, view.externArray());
    }
    if (view.bytesArray())
    {
        result = ::zserio::calcHashCode(result, view.bytesArray());
    }
    if (view.optionalBool())
    {
        result = ::zserio::calcHashCode(result, view.optionalBool());
    }
    if (view.optionalNested())
    {
        result = ::zserio::calcHashCode(result, view.optionalNested());
    }
    return static_cast<size_t>(result);
}

} // namespace std
