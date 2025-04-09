/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, std-polymorphicAllocator.
 */

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/SizeConvertUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
#include <zserio/UnionCaseException.h>

#include <test_object/std_polymorphic_allocator/ReflectableUtilUnion.h>

namespace test_object
{
namespace std_polymorphic_allocator
{

bool operator==(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& lhs, const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& rhs)
{
    return static_cast<const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::Base&>(lhs) == static_cast<const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::Base&>(rhs);
}

bool operator<(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& lhs, const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion&  rhs)
{
    return static_cast<const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::Base&>(lhs) < static_cast<const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::Base&>(rhs);
}

bool operator!=(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& lhs, const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& lhs, const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& lhs, const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& lhs, const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_polymorphic_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::View(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& data) noexcept :
        m_data(&data)
{}

View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::View(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& data,
        const View&) noexcept :
        m_data(&data)
{}

::test_object::std_polymorphic_allocator::ReflectableUtilEnum View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::reflectableUtilEnum() const
{
    return get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(*m_data);
}

::test_object::std_polymorphic_allocator::ReflectableUtilBitmask View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::reflectableUtilBitmask() const
{
    return get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(*m_data);
}

View<::test_object::std_polymorphic_allocator::ReflectableUtilObject> View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::reflectableUtilObject() const
{
    return View<::test_object::std_polymorphic_allocator::ReflectableUtilObject>{get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(*m_data)};
}

::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::zserioChoiceTag() const
{
    return m_data->index();
}

const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& rhs)
{
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return false;
    }

    switch (lhs.zserioChoiceTag())
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
        return (lhs.reflectableUtilEnum() == rhs.reflectableUtilEnum());
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
        return (lhs.reflectableUtilBitmask() == rhs.reflectableUtilBitmask());
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
        return (lhs.reflectableUtilObject() == rhs.reflectableUtilObject());
    default:
        throw CppRuntimeException("No case set in union ::test_object::std_polymorphic_allocator::ReflectableUtilUnion!");
    }
}

bool operator<(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& rhs)
{
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return lhs.zserioChoiceTag() < rhs.zserioChoiceTag();
    }

    switch (lhs.zserioChoiceTag())
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
        return (lhs.reflectableUtilEnum() < rhs.reflectableUtilEnum());
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
        return (lhs.reflectableUtilBitmask() < rhs.reflectableUtilBitmask());
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
        return (lhs.reflectableUtilObject() < rhs.reflectableUtilObject());
    default:
        throw CppRuntimeException("No case set in union ::test_object::std_polymorphic_allocator::ReflectableUtilUnion!");
    }
}

bool operator!=(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& lhs, const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& view, ::std::string_view)
{
    switch (view.zserioChoiceTag())
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
        validate(view.reflectableUtilEnum(), "'ReflectableUtilUnion.reflectableUtilEnum'");
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
        validate(view.reflectableUtilBitmask(), "'ReflectableUtilUnion.reflectableUtilBitmask'");
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
        validate(view.reflectableUtilObject(), "'ReflectableUtilUnion.reflectableUtilObject'");
        break;
    default:
        throw UnionCaseException("No case set in union 'ReflectableUtilUnion'!");
    }
}

template <>
BitSize bitSizeOf(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    switch (view.zserioChoiceTag())
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
        endBitPosition += bitSizeOf(view.reflectableUtilEnum(), endBitPosition);
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
        endBitPosition += bitSizeOf(view.reflectableUtilBitmask(), endBitPosition);
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
        endBitPosition += bitSizeOf(view.reflectableUtilObject(), endBitPosition);
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::std_polymorphic_allocator::ReflectableUtilUnion!");
    }

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& view)
{
    write(writer, fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    switch (view.zserioChoiceTag())
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
        write(writer, view.reflectableUtilEnum());
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
        write(writer, view.reflectableUtilBitmask());
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
        write(writer, view.reflectableUtilObject());
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::std_polymorphic_allocator::ReflectableUtilUnion!");
    }
}

template <>
View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion> read(BitStreamReader& reader, ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& data)
{
    View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion> view(data);

    VarSize choiceTag;
    read(reader, choiceTag);
    switch (static_cast<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag>(choiceTag + 1))
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
        data.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>();
        read(reader, data.get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>());
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
        data.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>();
        read(reader, data.get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>());
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
        data.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>();
        (void)read(reader, data.get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>());
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::std_polymorphic_allocator::ReflectableUtilUnion!");
    }

    return view;
}

const ::zserio::std_pmr::ITypeInfo& TypeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilUnion, ::std::pmr::polymorphic_allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::pmr::polymorphic_allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 3> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "reflectableUtilEnum", // schemaName
            ::zserio::typeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilEnum, ::std::pmr::polymorphic_allocator<uint8_t>>(), // typeInfo
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
            "reflectableUtilBitmask", // schemaName
            ::zserio::typeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilBitmask, ::std::pmr::polymorphic_allocator<uint8_t>>(), // typeInfo
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
            "reflectableUtilObject", // schemaName
            ::zserio::typeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilObject, ::std::pmr::polymorphic_allocator<uint8_t>>(), // typeInfo
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

    static const ::zserio::detail::UnionTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_polymorphic_allocator.ReflectableUtilUnion",
        [](const AllocatorType& allocator) -> ::zserio::std_pmr::IReflectableDataPtr
        {
            return ::std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>>(allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::std_pmr::IReflectableDataConstPtr reflectable(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& object, const ::std::pmr::polymorphic_allocator<uint8_t>& alloc = {}) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>(typeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>(), alloc),
                m_object(object)
        {}

        ::zserio::std_pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "reflectableUtilEnum")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(m_object), get_allocator());
            }
            if (name == "reflectableUtilBitmask")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(m_object), get_allocator());
            }
            if (name == "reflectableUtilObject")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilUnion'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
                return "reflectableUtilEnum";
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
                return "reflectableUtilBitmask";
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
                return "reflectableUtilObject";
            default:
                return "";
            }
        }

        ::zserio::std_pmr::Any getAnyValue(const ::std::pmr::polymorphic_allocator<uint8_t>& alloc) const override
        {
            return ::zserio::std_pmr::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::std_pmr::IReflectableDataPtr reflectable(::test_object::std_polymorphic_allocator::ReflectableUtilUnion& value, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_polymorphic_allocator::ReflectableUtilUnion& object, const ::std::pmr::polymorphic_allocator<uint8_t>& alloc = {}) :
                ::zserio::ReflectableDataAllocatorHolderBase<::std::pmr::polymorphic_allocator<uint8_t>>(typeInfo<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>(), alloc),
                m_object(object)
        {}

        ::zserio::std_pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "reflectableUtilEnum")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(m_object), get_allocator());
            }
            if (name == "reflectableUtilBitmask")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(m_object), get_allocator());
            }
            if (name == "reflectableUtilObject")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilUnion'!";
        }

        ::zserio::std_pmr::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "reflectableUtilEnum")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(m_object), get_allocator());
            }
            if (name == "reflectableUtilBitmask")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(m_object), get_allocator());
            }
            if (name == "reflectableUtilObject")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilUnion'!";
        }

        void setField(::std::string_view name, const ::zserio::std_pmr::Any& value) override
        {
            if (name == "reflectableUtilEnum")
            {
                m_object.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(
                        ::zserio::ReflectableUtil::fromAny<::test_object::std_polymorphic_allocator::ReflectableUtilEnum>(value));
                return;
            }
            if (name == "reflectableUtilBitmask")
            {
                m_object.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(
                        ::zserio::ReflectableUtil::fromAny<::test_object::std_polymorphic_allocator::ReflectableUtilBitmask>(value));
                return;
            }
            if (name == "reflectableUtilObject")
            {
                m_object.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(
                        ::zserio::ReflectableUtil::fromAny<::test_object::std_polymorphic_allocator::ReflectableUtilObject>(value));
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilUnion'!";
        }

        ::zserio::std_pmr::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "reflectableUtilEnum")
            {
                m_object.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>();
                return ::zserio::reflectable(get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum>(m_object), get_allocator());
            }
            if (name == "reflectableUtilBitmask")
            {
                m_object.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>();
                return ::zserio::reflectable(get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask>(m_object), get_allocator());
            }
            if (name == "reflectableUtilObject")
            {
                m_object.emplace<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>();
                return ::zserio::reflectable(get<::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilUnion'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
                return "reflectableUtilEnum";
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
                return "reflectableUtilBitmask";
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
                return "reflectableUtilObject";
            default:
                return "";
            }
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
        ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::std_pmr::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& view, const ::std::pmr::polymorphic_allocator<uint8_t>& allocator)
{
    class Introspectable : public ::zserio::CompoundIntrospectableViewBase<::test_object::std_polymorphic_allocator::ReflectableUtilUnion, ::std::pmr::polymorphic_allocator<uint8_t>>
    {
    public:
        explicit Introspectable(const ::zserio::View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& view_, const ::std::pmr::polymorphic_allocator<uint8_t>& alloc = {}) :
                ::zserio::CompoundIntrospectableViewBase<::test_object::std_polymorphic_allocator::ReflectableUtilUnion, ::std::pmr::polymorphic_allocator<uint8_t>>(
                        view_, alloc)
        {}

        ::zserio::std_pmr::IIntrospectableViewConstPtr getField(::std::string_view name) const override
        {
            if (name == "reflectableUtilEnum")
            {
                return ::zserio::introspectable(getValue().reflectableUtilEnum(), get_allocator());
            }
            if (name == "reflectableUtilBitmask")
            {
                return ::zserio::introspectable(getValue().reflectableUtilBitmask(), get_allocator());
            }
            if (name == "reflectableUtilObject")
            {
                return ::zserio::introspectable(getValue().reflectableUtilObject(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilUnion'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (getValue().zserioChoiceTag())
            {
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
                return "reflectableUtilEnum";
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
                return "reflectableUtilBitmask";
            case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
                return "reflectableUtilObject";
            default:
                return "";
            }
        }
    };

    return ::std::allocate_shared<Introspectable>(allocator, view);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>::operator()(const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, static_cast<const ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::Base&>(value));
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>>::operator()(const ::zserio::View<::test_object::std_polymorphic_allocator::ReflectableUtilUnion>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    switch (view.zserioChoiceTag())
    {
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilEnum:
        result = ::zserio::calcHashCode(result, view.reflectableUtilEnum());
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilBitmask:
        result = ::zserio::calcHashCode(result, view.reflectableUtilBitmask());
        break;
    case ::test_object::std_polymorphic_allocator::ReflectableUtilUnion::ChoiceTag::CHOICE_reflectableUtilObject:
        result = ::zserio::calcHashCode(result, view.reflectableUtilObject());
        break;
    default:
        throw ::zserio::CppRuntimeException("No case set in union ::test_object::std_polymorphic_allocator::ReflectableUtilUnion!");
    }

    return static_cast<size_t>(result);
}

} // namespace std
