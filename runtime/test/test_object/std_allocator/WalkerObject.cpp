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
#include <zserio/MissedOptionalException.h>

#include <test_object/std_allocator/WalkerObject.h>

namespace test_object
{
namespace std_allocator
{

WalkerObject::WalkerObject() noexcept :
        WalkerObject(AllocatorType{})
{}

WalkerObject::WalkerObject(const AllocatorType& allocator) noexcept :
        identifier(),
        nested(allocator),
        text(allocator),
        unionArray(allocator),
        optionalUnionArray(allocator),
        choiceSelector(),
        choiceField(allocator)
{}

WalkerObject::WalkerObject(
        ::zserio::UInt32 identifier_,
        ::zserio::Optional<::test_object::std_allocator::WalkerNested> nested_,
        ::zserio::String text_,
        ::zserio::Vector<::test_object::std_allocator::WalkerUnion> unionArray_,
        ::zserio::Optional<::zserio::Vector<::test_object::std_allocator::WalkerUnion>> optionalUnionArray_,
        ::zserio::UInt8 choiceSelector_,
        ::test_object::std_allocator::WalkerChoice choiceField_) :
        identifier(identifier_),
        nested(::std::move(nested_)),
        text(::std::move(text_)),
        unionArray(::std::move(unionArray_)),
        optionalUnionArray(::std::move(optionalUnionArray_)),
        choiceSelector(choiceSelector_),
        choiceField(::std::move(choiceField_))
{}

bool operator==(const ::test_object::std_allocator::WalkerObject& lhs, const ::test_object::std_allocator::WalkerObject& rhs)
{
    return ::std::tie(
            lhs.identifier,
            lhs.nested,
            lhs.text,
            lhs.unionArray,
            lhs.optionalUnionArray,
            lhs.choiceSelector,
            lhs.choiceField)
            == ::std::tie(
            rhs.identifier,
            rhs.nested,
            rhs.text,
            rhs.unionArray,
            rhs.optionalUnionArray,
            rhs.choiceSelector,
            rhs.choiceField);
}

bool operator<(const ::test_object::std_allocator::WalkerObject& lhs, const ::test_object::std_allocator::WalkerObject& rhs)
{
    return ::std::tie(
            lhs.identifier,
            lhs.nested,
            lhs.text,
            lhs.unionArray,
            lhs.optionalUnionArray,
            lhs.choiceSelector,
            lhs.choiceField)
            < ::std::tie(
            rhs.identifier,
            rhs.nested,
            rhs.text,
            rhs.unionArray,
            rhs.optionalUnionArray,
            rhs.choiceSelector,
            rhs.choiceField);
}

bool operator!=(const ::test_object::std_allocator::WalkerObject& lhs, const ::test_object::std_allocator::WalkerObject& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::WalkerObject& lhs, const ::test_object::std_allocator::WalkerObject& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::WalkerObject& lhs, const ::test_object::std_allocator::WalkerObject& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::WalkerObject& lhs, const ::test_object::std_allocator::WalkerObject& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::WalkerObject>::View(const ::test_object::std_allocator::WalkerObject& data) noexcept :
        m_data(&data)
{}

View<::test_object::std_allocator::WalkerObject>::View(const ::test_object::std_allocator::WalkerObject& data,
        const View&) noexcept :
        m_data(&data)
{}

::zserio::UInt32 View<::test_object::std_allocator::WalkerObject>::identifier() const
{
    return m_data->identifier;
}

::zserio::Optional<View<::test_object::std_allocator::WalkerNested>> View<::test_object::std_allocator::WalkerObject>::nested() const
{
    if (m_data->nested.has_value())
    {
        return ::zserio::Optional<View<::test_object::std_allocator::WalkerNested>>{
                ::std::in_place, m_data->nested.get_allocator(), *m_data->nested};
    }
    else
    {
        return ::zserio::Optional<View<::test_object::std_allocator::WalkerNested>>(::std::nullopt, m_data->nested.get_allocator());
    }
}

::std::string_view View<::test_object::std_allocator::WalkerObject>::text() const
{
    return ::std::string_view{
            m_data->text};
}

Array<::zserio::Vector<::test_object::std_allocator::WalkerUnion>, ArrayType::AUTO> View<::test_object::std_allocator::WalkerObject>::unionArray() const
{
    return Array<::zserio::Vector<::test_object::std_allocator::WalkerUnion>, ArrayType::AUTO>{
            m_data->unionArray};
}

::zserio::Optional<Array<::zserio::Vector<::test_object::std_allocator::WalkerUnion>, ArrayType::AUTO>> View<::test_object::std_allocator::WalkerObject>::optionalUnionArray() const
{
    if (m_data->optionalUnionArray.has_value())
    {
        return ::zserio::Optional<Array<::zserio::Vector<::test_object::std_allocator::WalkerUnion>, ArrayType::AUTO>>{
                ::std::in_place, m_data->optionalUnionArray.get_allocator(), *m_data->optionalUnionArray};
    }
    else
    {
        return ::zserio::Optional<Array<::zserio::Vector<::test_object::std_allocator::WalkerUnion>, ArrayType::AUTO>>(::std::nullopt, m_data->optionalUnionArray.get_allocator());
    }
}

::zserio::UInt8 View<::test_object::std_allocator::WalkerObject>::choiceSelector() const
{
    return m_data->choiceSelector;
}

View<::test_object::std_allocator::WalkerChoice> View<::test_object::std_allocator::WalkerObject>::choiceField() const
{
    return View<::test_object::std_allocator::WalkerChoice>{
            m_data->choiceField, ::zserio::UInt8(static_cast<::zserio::UInt8::ValueType>(choiceSelector()))};
}

const ::test_object::std_allocator::WalkerObject& View<::test_object::std_allocator::WalkerObject>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::WalkerObject>& lhs, const View<::test_object::std_allocator::WalkerObject>& rhs)
{
    return lhs.identifier() == rhs.identifier() &&
            lhs.nested() == rhs.nested() &&
            lhs.text() == rhs.text() &&
            lhs.unionArray() == rhs.unionArray() &&
            lhs.optionalUnionArray() == rhs.optionalUnionArray() &&
            lhs.choiceSelector() == rhs.choiceSelector() &&
            lhs.choiceField() == rhs.choiceField();
}

bool operator<(const View<::test_object::std_allocator::WalkerObject>& lhs, const View<::test_object::std_allocator::WalkerObject>& rhs)
{
    if (lhs.identifier() != rhs.identifier())
    {
        return lhs.identifier() < rhs.identifier();
    }
    if (lhs.nested() != rhs.nested())
    {
        return lhs.nested() < rhs.nested();
    }
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }
    if (lhs.unionArray() != rhs.unionArray())
    {
        return lhs.unionArray() < rhs.unionArray();
    }
    if (lhs.optionalUnionArray() != rhs.optionalUnionArray())
    {
        return lhs.optionalUnionArray() < rhs.optionalUnionArray();
    }
    if (lhs.choiceSelector() != rhs.choiceSelector())
    {
        return lhs.choiceSelector() < rhs.choiceSelector();
    }
    if (lhs.choiceField() != rhs.choiceField())
    {
        return lhs.choiceField() < rhs.choiceField();
    }

    return false;
}

bool operator!=(const View<::test_object::std_allocator::WalkerObject>& lhs, const View<::test_object::std_allocator::WalkerObject>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::WalkerObject>& lhs, const View<::test_object::std_allocator::WalkerObject>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::WalkerObject>& lhs, const View<::test_object::std_allocator::WalkerObject>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::WalkerObject>& lhs, const View<::test_object::std_allocator::WalkerObject>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::WalkerObject>& view, ::std::string_view)
{
    validate(view.identifier(), "'WalkerObject.identifier'");
    // check non-auto optional
    if ((view.identifier() != 0) && !view.nested().has_value())
    {
        throw MissedOptionalException("Optional field 'WalkerObject.nested' is used but not set!");
    }
    if (view.nested().has_value())
    {
        validate(*view.nested(), "'WalkerObject.nested'");
    }
    validate(view.text(), "'WalkerObject.text'");
    validate(view.unionArray(), "'WalkerObject.unionArray'");
    if (view.optionalUnionArray().has_value())
    {
        validate(*view.optionalUnionArray(), "'WalkerObject.optionalUnionArray'");
    }
    validate(view.choiceSelector(), "'WalkerObject.choiceSelector'");
    validate(view.choiceField(), "'WalkerObject.choiceField'");
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::WalkerObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto identifier_ = view.identifier();
    endBitPosition += bitSizeOf(identifier_, endBitPosition);
    auto nested_ = view.nested();
    if (nested_.has_value())
    {
        endBitPosition += bitSizeOf(*nested_, endBitPosition);
    }
    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);
    auto unionArray_ = view.unionArray();
    endBitPosition += bitSizeOf(unionArray_, endBitPosition);
    auto optionalUnionArray_ = view.optionalUnionArray();
    endBitPosition += bitSizeOf(Bool());
    if (optionalUnionArray_.has_value())
    {
        endBitPosition += bitSizeOf(*optionalUnionArray_, endBitPosition);
    }
    auto choiceSelector_ = view.choiceSelector();
    endBitPosition += bitSizeOf(choiceSelector_, endBitPosition);
    auto choiceField_ = view.choiceField();
    endBitPosition += bitSizeOf(choiceField_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::WalkerObject>& view)
{
    auto identifier_ = view.identifier();
    write(writer, identifier_);
    auto nested_ = view.nested();
    if (nested_.has_value())
    {
        write(writer, *nested_);
    }
    auto text_ = view.text();
    write(writer, text_);
    auto unionArray_ = view.unionArray();
    write(writer, unionArray_);
    auto optionalUnionArray_ = view.optionalUnionArray();
    if (optionalUnionArray_.has_value())
    {
        writer.writeBool(true);
        write(writer, *optionalUnionArray_);
    }
    else
    {
        writer.writeBool(false);
    }
    auto choiceSelector_ = view.choiceSelector();
    write(writer, choiceSelector_);
    auto choiceField_ = view.choiceField();
    write(writer, choiceField_);
}

template <>
View<::test_object::std_allocator::WalkerObject> read(BitStreamReader& reader, ::test_object::std_allocator::WalkerObject& data)
{
    View<::test_object::std_allocator::WalkerObject> view(data);
    read(reader, data.identifier);
    if (view.identifier() != 0)
    {
        data.nested.emplace(data.nested.get_allocator());
        (void)read(reader, *data.nested);
    }
    read(reader, data.text);
    (void)read<Array<::zserio::Vector<::test_object::std_allocator::WalkerUnion>, ArrayType::AUTO>>(reader, data.unionArray);
    if (reader.readBool())
    {
        data.optionalUnionArray.emplace(data.optionalUnionArray.get_allocator());
        (void)read<Array<::zserio::Vector<::test_object::std_allocator::WalkerUnion>, ArrayType::AUTO>>(reader, *data.optionalUnionArray);
    }
    read(reader, data.choiceSelector);
    (void)read(reader, data.choiceField, ::zserio::UInt8(static_cast<::zserio::UInt8::ValueType>(view.choiceSelector())));
    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::WalkerObject, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::std::string_view, 1> choiceFieldTypeArguments = {
        "choiceSelector()"
    };
    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 7> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "identifier", // schemaName
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
            ::zserio::typeInfo<::test_object::std_allocator::WalkerNested, ::std::allocator<uint8_t>>(), // typeInfo
            {}, // typeArguments
            false, // isExtended
            {}, // alignment
            {}, // offset
            {}, // initializer
            true, // isOptional
            "identifier() != 0", // optionalClause
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
            "unionArray", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::WalkerUnion, ::std::allocator<uint8_t>>(), // typeInfo
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
            "optionalUnionArray", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::WalkerUnion, ::std::allocator<uint8_t>>(), // typeInfo
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
            "choiceSelector", // schemaName
            ::zserio::typeInfo<::zserio::UInt8, ::std::allocator<uint8_t>>(), // typeInfo
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
            "choiceField", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::WalkerChoice, ::std::allocator<uint8_t>>(), // typeInfo
            choiceFieldTypeArguments, // typeArguments
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
        "test_object.std_allocator.WalkerObject",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableOwner<::test_object::std_allocator::WalkerObject>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(
        const ::test_object::std_allocator::WalkerObject& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::WalkerObject& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::WalkerObject>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "identifier")
            {
                return ::zserio::reflectable(m_object.identifier, get_allocator());
            }
            if (name == "nested")
            {
                if (!m_object.nested.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "unionArray")
            {
                return ::zserio::reflectableArray(m_object.unionArray, get_allocator());
            }
            if (name == "optionalUnionArray")
            {
                if (!m_object.optionalUnionArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.optionalUnionArray, get_allocator());
            }
            if (name == "choiceSelector")
            {
                return ::zserio::reflectable(m_object.choiceSelector, get_allocator());
            }
            if (name == "choiceField")
            {
                return ::zserio::reflectable(m_object.choiceField, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerObject'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::WalkerObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

template <>
::zserio::IReflectableDataPtr reflectable(
        ::test_object::std_allocator::WalkerObject& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        explicit Reflectable(::test_object::std_allocator::WalkerObject& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::WalkerObject>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "identifier")
            {
                return ::zserio::reflectable(m_object.identifier, get_allocator());
            }
            if (name == "nested")
            {
                if (!m_object.nested.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "unionArray")
            {
                return ::zserio::reflectableArray(m_object.unionArray, get_allocator());
            }
            if (name == "optionalUnionArray")
            {
                if (!m_object.optionalUnionArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.optionalUnionArray, get_allocator());
            }
            if (name == "choiceSelector")
            {
                return ::zserio::reflectable(m_object.choiceSelector, get_allocator());
            }
            if (name == "choiceField")
            {
                return ::zserio::reflectable(m_object.choiceField, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerObject'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "identifier")
            {
                return ::zserio::reflectable(m_object.identifier, get_allocator());
            }
            if (name == "nested")
            {
                if (!m_object.nested.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectable(*m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "unionArray")
            {
                return ::zserio::reflectableArray(m_object.unionArray, get_allocator());
            }
            if (name == "optionalUnionArray")
            {
                if (!m_object.optionalUnionArray.has_value())
                {
                    return nullptr;
                }

                return ::zserio::reflectableArray(*m_object.optionalUnionArray, get_allocator());
            }
            if (name == "choiceSelector")
            {
                return ::zserio::reflectable(m_object.choiceSelector, get_allocator());
            }
            if (name == "choiceField")
            {
                return ::zserio::reflectable(m_object.choiceField, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerObject'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "identifier")
            {
                m_object.identifier =
                        value.get<::zserio::UInt32>();
                return;
            }
            if (name == "nested")
            {
                if (value.isType<::std::nullptr_t>())
                {
                    m_object.nested.reset();
                    return;
                }

                m_object.nested =
                        value.get<::test_object::std_allocator::WalkerNested>();
                return;
            }
            if (name == "text")
            {
                m_object.text =
                        value.get<::zserio::String>();
                return;
            }
            if (name == "unionArray")
            {
                m_object.unionArray =
                        value.get<::zserio::Vector<::test_object::std_allocator::WalkerUnion>>();
                return;
            }
            if (name == "optionalUnionArray")
            {
                if (value.isType<::std::nullptr_t>())
                {
                    m_object.optionalUnionArray.reset();
                    return;
                }

                m_object.optionalUnionArray =
                        value.get<::zserio::Vector<::test_object::std_allocator::WalkerUnion>>();
                return;
            }
            if (name == "choiceSelector")
            {
                m_object.choiceSelector =
                        value.get<::zserio::UInt8>();
                return;
            }
            if (name == "choiceField")
            {
                m_object.choiceField =
                        value.get<::test_object::std_allocator::WalkerChoice>();
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerObject'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "identifier")
            {
                m_object.identifier =
                        ::zserio::UInt32();
                return ::zserio::reflectable(m_object.identifier, get_allocator());
            }
            if (name == "nested")
            {
                m_object.nested =
                        ::test_object::std_allocator::WalkerNested(get_allocator());
                return ::zserio::reflectable(*m_object.nested, get_allocator());
            }
            if (name == "text")
            {
                m_object.text =
                        ::zserio::String(get_allocator());
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "unionArray")
            {
                m_object.unionArray =
                        ::zserio::Vector<::test_object::std_allocator::WalkerUnion>(get_allocator());
                return ::zserio::reflectableArray(m_object.unionArray, get_allocator());
            }
            if (name == "optionalUnionArray")
            {
                m_object.optionalUnionArray =
                        ::zserio::Vector<::test_object::std_allocator::WalkerUnion>(get_allocator());
                return ::zserio::reflectableArray(*m_object.optionalUnionArray, get_allocator());
            }
            if (name == "choiceSelector")
            {
                m_object.choiceSelector =
                        ::zserio::UInt8();
                return ::zserio::reflectable(m_object.choiceSelector, get_allocator());
            }
            if (name == "choiceField")
            {
                m_object.choiceField =
                        ::test_object::std_allocator::WalkerChoice(get_allocator());
                return ::zserio::reflectable(m_object.choiceField, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerObject'!";
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
        ::test_object::std_allocator::WalkerObject& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::WalkerObject>::operator()(const ::test_object::std_allocator::WalkerObject& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.identifier);
    result = ::zserio::calcHashCode(result, data.nested);
    result = ::zserio::calcHashCode(result, data.text);
    result = ::zserio::calcHashCode(result, data.unionArray);
    result = ::zserio::calcHashCode(result, data.optionalUnionArray);
    result = ::zserio::calcHashCode(result, data.choiceSelector);
    result = ::zserio::calcHashCode(result, data.choiceField);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::WalkerObject>>::operator()(const ::zserio::View<::test_object::std_allocator::WalkerObject>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.identifier());
    if (view.identifier() != 0)
    {
        result = ::zserio::calcHashCode(result, view.nested());
    }
    result = ::zserio::calcHashCode(result, view.text());
    result = ::zserio::calcHashCode(result, view.unionArray());
    if (view.optionalUnionArray())
    {
        result = ::zserio::calcHashCode(result, view.optionalUnionArray());
    }
    result = ::zserio::calcHashCode(result, view.choiceSelector());
    result = ::zserio::calcHashCode(result, view.choiceField());
    return static_cast<size_t>(result);
}

} // namespace std
