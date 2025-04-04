/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/SizeConvertUtil.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
#include <zserio/UnionCaseException.h>

#include <test_object/polymorphic_allocator/WalkerUnion.h>

namespace test_object
{
namespace polymorphic_allocator
{

bool operator==(const ::test_object::polymorphic_allocator::WalkerUnion& lhs, const ::test_object::polymorphic_allocator::WalkerUnion& rhs)
{
    return static_cast<const ::test_object::polymorphic_allocator::WalkerUnion::Base&>(lhs) == static_cast<const ::test_object::polymorphic_allocator::WalkerUnion::Base&>(rhs);
}

bool operator<(const ::test_object::polymorphic_allocator::WalkerUnion& lhs, const ::test_object::polymorphic_allocator::WalkerUnion&  rhs)
{
    return static_cast<const ::test_object::polymorphic_allocator::WalkerUnion::Base&>(lhs) < static_cast<const ::test_object::polymorphic_allocator::WalkerUnion::Base&>(rhs);
}

bool operator!=(const ::test_object::polymorphic_allocator::WalkerUnion& lhs, const ::test_object::polymorphic_allocator::WalkerUnion& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::polymorphic_allocator::WalkerUnion& lhs, const ::test_object::polymorphic_allocator::WalkerUnion& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::polymorphic_allocator::WalkerUnion& lhs, const ::test_object::polymorphic_allocator::WalkerUnion& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::polymorphic_allocator::WalkerUnion& lhs, const ::test_object::polymorphic_allocator::WalkerUnion& rhs)
{
    return !(lhs < rhs);
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::polymorphic_allocator::WalkerUnion>::View(const ::test_object::polymorphic_allocator::WalkerUnion& data) noexcept :
        m_data(&data)
{}

View<::test_object::polymorphic_allocator::WalkerUnion>::View(const ::test_object::polymorphic_allocator::WalkerUnion& data,
        const View&) noexcept :
        m_data(&data)
{}

::zserio::UInt32 View<::test_object::polymorphic_allocator::WalkerUnion>::value() const
{
    return get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>(*m_data);
}

::std::string_view View<::test_object::polymorphic_allocator::WalkerUnion>::text() const
{
    return ::std::string_view{get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(*m_data)};
}

Array<::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerNested>, ArrayType::AUTO> View<::test_object::polymorphic_allocator::WalkerUnion>::nestedArray() const
{
    return Array<::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerNested>, ArrayType::AUTO>{get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(*m_data)};
}

::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag View<::test_object::polymorphic_allocator::WalkerUnion>::zserioChoiceTag() const
{
    return m_data->index();
}

const ::test_object::polymorphic_allocator::WalkerUnion& View<::test_object::polymorphic_allocator::WalkerUnion>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::polymorphic_allocator::WalkerUnion>& lhs, const View<::test_object::polymorphic_allocator::WalkerUnion>& rhs)
{
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return false;
    }

    switch (lhs.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
        return (lhs.value() == rhs.value());
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
        return (lhs.text() == rhs.text());
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
        return (lhs.nestedArray() == rhs.nestedArray());
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::WalkerUnion!");
    }
}

bool operator<(const View<::test_object::polymorphic_allocator::WalkerUnion>& lhs, const View<::test_object::polymorphic_allocator::WalkerUnion>& rhs)
{
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return lhs.zserioChoiceTag() < rhs.zserioChoiceTag();
    }

    switch (lhs.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
        return (lhs.value() < rhs.value());
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
        return (lhs.text() < rhs.text());
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
        return (lhs.nestedArray() < rhs.nestedArray());
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::WalkerUnion!");
    }
}

bool operator!=(const View<::test_object::polymorphic_allocator::WalkerUnion>& lhs, const View<::test_object::polymorphic_allocator::WalkerUnion>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::polymorphic_allocator::WalkerUnion>& lhs, const View<::test_object::polymorphic_allocator::WalkerUnion>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::polymorphic_allocator::WalkerUnion>& lhs, const View<::test_object::polymorphic_allocator::WalkerUnion>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::polymorphic_allocator::WalkerUnion>& lhs, const View<::test_object::polymorphic_allocator::WalkerUnion>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::WalkerUnion>& view, ::std::string_view)
{
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
        validate(view.value(), "'WalkerUnion.value'");
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
        validate(view.text(), "'WalkerUnion.text'");
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
        validate(view.nestedArray(), "'WalkerUnion.nestedArray'");
        break;
    default:
        throw UnionCaseException("No case set in union 'WalkerUnion'!");
    }
}

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::WalkerUnion>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
        endBitPosition += bitSizeOf(view.value(), endBitPosition);
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
        endBitPosition += bitSizeOf(view.text(), endBitPosition);
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
        endBitPosition += bitSizeOf(view.nestedArray(), endBitPosition);
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::WalkerUnion!");
    }

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::WalkerUnion>& view)
{
    write(writer, fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
        write(writer, view.value());
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
        write(writer, view.text());
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
        write(writer, view.nestedArray());
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::WalkerUnion!");
    }
}

template <>
View<::test_object::polymorphic_allocator::WalkerUnion> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::WalkerUnion& data)
{
    View<::test_object::polymorphic_allocator::WalkerUnion> view(data);

    VarSize choiceTag;
    read(reader, choiceTag);
    switch (static_cast<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag>(choiceTag + 1))
    {
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
        data.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>();
        read(reader, data.get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>());
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
        data.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(data.get_allocator());
        read(reader, data.get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>());
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
        data.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(data.get_allocator());
        (void)read<Array<::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerNested>, ArrayType::AUTO>>(reader, data.get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>());
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::WalkerUnion!");
    }

    return view;
}

const ::zserio::pmr::ITypeInfo& TypeInfo<::test_object::polymorphic_allocator::WalkerUnion, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 3> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "value", // schemaName
            ::zserio::typeInfo<::zserio::UInt32, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(), // typeInfo
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
        },
        ::zserio::BasicFieldInfo<AllocatorType>{
            "nestedArray", // schemaName
            ::zserio::typeInfo<::test_object::polymorphic_allocator::WalkerNested, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(), // typeInfo
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
        }
    };

    static const ::zserio::Span<::zserio::BasicParameterInfo<AllocatorType>> parameters;

    static const ::zserio::Span<::zserio::BasicFunctionInfo<AllocatorType>> functions;

    static const ::zserio::detail::UnionTypeInfo<AllocatorType> typeInfo = {
        "test_object.polymorphic_allocator.WalkerUnion",
        [](const AllocatorType& allocator) -> ::zserio::pmr::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::polymorphic_allocator::WalkerUnion>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::WalkerUnion& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::polymorphic_allocator::WalkerUnion& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::WalkerUnion>(), alloc),
                m_object(object)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>(m_object), get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(m_object), get_allocator());
            }
            if (name == "nestedArray")
            {
                return ::zserio::reflectableArray(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerUnion'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
                return "value";
            case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
                return "text";
            case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
                return "nestedArray";
            default:
                return "";
            }
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::polymorphic_allocator::WalkerUnion& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::WalkerUnion& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::polymorphic_allocator::WalkerUnion& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::WalkerUnion>(), alloc),
                m_object(object)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>(m_object), get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(m_object), get_allocator());
            }
            if (name == "nestedArray")
            {
                return ::zserio::reflectableArray(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerUnion'!";
        }

        ::zserio::pmr::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>(m_object), get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(m_object), get_allocator());
            }
            if (name == "nestedArray")
            {
                return ::zserio::reflectableArray(
                        get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerUnion'!";
        }

        void setField(::std::string_view name, const ::zserio::pmr::Any& value) override
        {
            if (name == "value")
            {
                m_object.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::UInt32>(value));
                return;
            }
            if (name == "text")
            {
                m_object.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::pmr::String>(value));
                return;
            }
            if (name == "nestedArray")
            {
                m_object.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::pmr::Vector<::test_object::polymorphic_allocator::WalkerNested>>(value));
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerUnion'!";
        }

        ::zserio::pmr::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "value")
            {
                m_object.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>();
                return ::zserio::reflectable(get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value>(m_object), get_allocator());
            }
            if (name == "text")
            {
                m_object.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(get_allocator());
                return ::zserio::reflectable(get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text>(m_object), get_allocator());
            }
            if (name == "nestedArray")
            {
                m_object.emplace<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(get_allocator());
                return ::zserio::reflectableArray(get<::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerUnion'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
                return "value";
            case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
                return "text";
            case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
                return "nestedArray";
            default:
                return "";
            }
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
        ::test_object::polymorphic_allocator::WalkerUnion& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::polymorphic_allocator::WalkerUnion>::operator()(const ::test_object::polymorphic_allocator::WalkerUnion& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, static_cast<const ::test_object::polymorphic_allocator::WalkerUnion::Base&>(value));
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::polymorphic_allocator::WalkerUnion>>::operator()(const ::zserio::View<::test_object::polymorphic_allocator::WalkerUnion>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_value:
        result = ::zserio::calcHashCode(result, view.value());
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_text:
        result = ::zserio::calcHashCode(result, view.text());
        break;
    case ::test_object::polymorphic_allocator::WalkerUnion::ChoiceTag::CHOICE_nestedArray:
        result = ::zserio::calcHashCode(result, view.nestedArray());
        break;
    default:
        throw ::zserio::CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::WalkerUnion!");
    }

    return static_cast<size_t>(result);
}

} // namespace std
