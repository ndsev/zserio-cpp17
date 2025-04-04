/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/SizeConvertUtil.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>
#include <zserio/UnionCaseException.h>

#include <test_object/polymorphic_allocator/ReflectableUnion.h>

namespace test_object
{
namespace polymorphic_allocator
{

bool operator==(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs)
{
    return static_cast<const ::test_object::polymorphic_allocator::ReflectableUnion::Base&>(lhs) == static_cast<const ::test_object::polymorphic_allocator::ReflectableUnion::Base&>(rhs);
}

bool operator<(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion&  rhs)
{
    return static_cast<const ::test_object::polymorphic_allocator::ReflectableUnion::Base&>(lhs) < static_cast<const ::test_object::polymorphic_allocator::ReflectableUnion::Base&>(rhs);
}

bool operator!=(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::polymorphic_allocator::ReflectableUnion& lhs, const ::test_object::polymorphic_allocator::ReflectableUnion& rhs)
{
    return !(lhs < rhs);
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::polymorphic_allocator::ReflectableUnion>::View(const ::test_object::polymorphic_allocator::ReflectableUnion& data) noexcept :
        m_data(&data)
{}

View<::test_object::polymorphic_allocator::ReflectableUnion>::View(const ::test_object::polymorphic_allocator::ReflectableUnion& data,
        const View&) noexcept :
        m_data(&data)
{}

::zserio::UInt32 View<::test_object::polymorphic_allocator::ReflectableUnion>::value32() const
{
    return get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>(*m_data);
}

::std::string_view View<::test_object::polymorphic_allocator::ReflectableUnion>::valueStr() const
{
    return ::std::string_view{get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(*m_data)};
}

::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag View<::test_object::polymorphic_allocator::ReflectableUnion>::zserioChoiceTag() const
{
    return m_data->index();
}

const ::test_object::polymorphic_allocator::ReflectableUnion& View<::test_object::polymorphic_allocator::ReflectableUnion>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs)
{
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return false;
    }

    switch (lhs.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
        return (lhs.value32() == rhs.value32());
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
        return (lhs.valueStr() == rhs.valueStr());
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::ReflectableUnion!");
    }
}

bool operator<(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs)
{
    if (lhs.zserioChoiceTag() != rhs.zserioChoiceTag())
    {
        return lhs.zserioChoiceTag() < rhs.zserioChoiceTag();
    }

    switch (lhs.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
        return (lhs.value32() < rhs.value32());
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
        return (lhs.valueStr() < rhs.valueStr());
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::ReflectableUnion!");
    }
}

bool operator!=(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::polymorphic_allocator::ReflectableUnion>& lhs, const View<::test_object::polymorphic_allocator::ReflectableUnion>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::ReflectableUnion>& view, ::std::string_view)
{
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
        validate(view.value32(), "'ReflectableUnion.value32'");
        break;
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
        validate(view.valueStr(), "'ReflectableUnion.valueStr'");
        break;
    default:
        throw UnionCaseException("No case set in union 'ReflectableUnion'!");
    }
}

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::ReflectableUnion>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    endBitPosition += bitSizeOf(fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
        endBitPosition += bitSizeOf(view.value32(), endBitPosition);
        break;
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
        endBitPosition += bitSizeOf(view.valueStr(), endBitPosition);
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::ReflectableUnion!");
    }

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::ReflectableUnion>& view)
{
    write(writer, fromCheckedValue<VarSize>(convertSizeToUInt32(view.zserioChoiceTag()) - 1));
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
        write(writer, view.value32());
        break;
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
        write(writer, view.valueStr());
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::ReflectableUnion!");
    }
}

template <>
View<::test_object::polymorphic_allocator::ReflectableUnion> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::ReflectableUnion& data)
{
    View<::test_object::polymorphic_allocator::ReflectableUnion> view(data);

    VarSize choiceTag;
    read(reader, choiceTag);
    switch (static_cast<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag>(choiceTag + 1))
    {
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
        data.emplace<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>();
        read(reader, data.get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>());
        break;
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
        data.emplace<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(data.get_allocator());
        read(reader, data.get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>());
        break;
    default:
        throw CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::ReflectableUnion!");
    }

    return view;
}

const ::zserio::pmr::ITypeInfo& TypeInfo<::test_object::polymorphic_allocator::ReflectableUnion, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 2> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "value32", // schemaName
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
            "valueStr", // schemaName
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

    static const ::zserio::detail::UnionTypeInfo<AllocatorType> typeInfo = {
        "test_object.polymorphic_allocator.ReflectableUnion",
        [](const AllocatorType& allocator) -> ::zserio::pmr::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::polymorphic_allocator::ReflectableUnion>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::ReflectableUnion& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::polymorphic_allocator::ReflectableUnion& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::ReflectableUnion>(), alloc),
                m_object(object)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "valueStr")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUnion'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
                return "value32";
            case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
                return "valueStr";
            default:
                return "";
            }
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::polymorphic_allocator::ReflectableUnion& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableUnion& value, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::polymorphic_allocator::ReflectableUnion& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::ReflectableUnion>(), alloc),
                m_object(object)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "valueStr")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUnion'!";
        }

        ::zserio::pmr::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "valueStr")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUnion'!";
        }

        void setField(::std::string_view name, const ::zserio::pmr::Any& value) override
        {
            if (name == "value32")
            {
                m_object.emplace<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::UInt32>(value));
                return;
            }
            if (name == "valueStr")
            {
                m_object.emplace<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::pmr::String>(value));
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUnion'!";
        }

        ::zserio::pmr::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "value32")
            {
                m_object.emplace<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>();
                return ::zserio::reflectable(get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "valueStr")
            {
                m_object.emplace<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(get_allocator());
                return ::zserio::reflectable(get<::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUnion'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
                return "value32";
            case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
                return "valueStr";
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
        ::test_object::polymorphic_allocator::ReflectableUnion& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::polymorphic_allocator::ReflectableUnion>::operator()(const ::test_object::polymorphic_allocator::ReflectableUnion& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, static_cast<const ::test_object::polymorphic_allocator::ReflectableUnion::Base&>(value));
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::polymorphic_allocator::ReflectableUnion>>::operator()(const ::zserio::View<::test_object::polymorphic_allocator::ReflectableUnion>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    switch (view.zserioChoiceTag())
    {
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_value32:
        result = ::zserio::calcHashCode(result, view.value32());
        break;
    case ::test_object::polymorphic_allocator::ReflectableUnion::ChoiceTag::CHOICE_valueStr:
        result = ::zserio::calcHashCode(result, view.valueStr());
        break;
    default:
        throw ::zserio::CppRuntimeException("No case set in union ::test_object::polymorphic_allocator::ReflectableUnion!");
    }

    return static_cast<size_t>(result);
}

} // namespace std
