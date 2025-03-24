/**
 * Automatically generated by Zserio C++17 generator version 0.1.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, polymorphicAllocator.
 */

#include <zserio/ChoiceCaseException.h>
#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/ReflectableData.h>
#include <zserio/TypeInfo.h>

#include <test_object/polymorphic_allocator/ReflectableChoice.h>

namespace test_object
{
namespace polymorphic_allocator
{

bool operator==(const ::test_object::polymorphic_allocator::ReflectableChoice& lhs, const ::test_object::polymorphic_allocator::ReflectableChoice& rhs)
{
    return static_cast<const ::test_object::polymorphic_allocator::ReflectableChoice::Base&>(lhs) == static_cast<const ::test_object::polymorphic_allocator::ReflectableChoice::Base&>(rhs);
}

bool operator<(const ::test_object::polymorphic_allocator::ReflectableChoice& lhs, const ::test_object::polymorphic_allocator::ReflectableChoice&  rhs)
{
    return static_cast<const ::test_object::polymorphic_allocator::ReflectableChoice::Base&>(lhs) < static_cast<const ::test_object::polymorphic_allocator::ReflectableChoice::Base&>(rhs);
}

bool operator!=(const ::test_object::polymorphic_allocator::ReflectableChoice& lhs, const ::test_object::polymorphic_allocator::ReflectableChoice& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::polymorphic_allocator::ReflectableChoice& lhs, const ::test_object::polymorphic_allocator::ReflectableChoice& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::polymorphic_allocator::ReflectableChoice& lhs, const ::test_object::polymorphic_allocator::ReflectableChoice& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::polymorphic_allocator::ReflectableChoice& lhs, const ::test_object::polymorphic_allocator::ReflectableChoice& rhs)
{
    return !(lhs < rhs);
}

} // namespace polymorphic_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::polymorphic_allocator::ReflectableChoice>::View(const ::test_object::polymorphic_allocator::ReflectableChoice& data,
        ::test_object::polymorphic_allocator::ReflectableEnum param_) noexcept :
        m_data(&data),
        m_param_(param_)
{}

View<::test_object::polymorphic_allocator::ReflectableChoice>::View(const ::test_object::polymorphic_allocator::ReflectableChoice& data,
        const View& other) noexcept :
        m_data(&data),
        m_param_(other.param())
{}

::test_object::polymorphic_allocator::ReflectableEnum View<::test_object::polymorphic_allocator::ReflectableChoice>::param() const
{
    return m_param_;
}

::std::string_view View<::test_object::polymorphic_allocator::ReflectableChoice>::valueStr() const
{
    return ::std::string_view{get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(*m_data)};
}

::zserio::UInt32 View<::test_object::polymorphic_allocator::ReflectableChoice>::value32() const
{
    return get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>(*m_data);
}

::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag View<::test_object::polymorphic_allocator::ReflectableChoice>::zserioChoiceTag() const
{
    return m_data->index();
}

const ::test_object::polymorphic_allocator::ReflectableChoice& View<::test_object::polymorphic_allocator::ReflectableChoice>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::polymorphic_allocator::ReflectableChoice>& lhs, const View<::test_object::polymorphic_allocator::ReflectableChoice>& rhs)
{
    if (lhs.param() != rhs.param())
    {
        return false;
    }

    switch (lhs.param())
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        return (lhs.valueStr() == rhs.valueStr());
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        return (lhs.value32() == rhs.value32());
    default:
        return true; // empty
    }
}

bool operator<(const View<::test_object::polymorphic_allocator::ReflectableChoice>& lhs, const View<::test_object::polymorphic_allocator::ReflectableChoice>& rhs)
{
    if (lhs.param() != rhs.param())
    {
        return lhs.param() < rhs.param();
    }

    switch (lhs.param())
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        return (lhs.valueStr() < rhs.valueStr());
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        return (lhs.value32() < rhs.value32());
    default:
        return false; // empty
    }
}

bool operator!=(const View<::test_object::polymorphic_allocator::ReflectableChoice>& lhs, const View<::test_object::polymorphic_allocator::ReflectableChoice>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::polymorphic_allocator::ReflectableChoice>& lhs, const View<::test_object::polymorphic_allocator::ReflectableChoice>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::polymorphic_allocator::ReflectableChoice>& lhs, const View<::test_object::polymorphic_allocator::ReflectableChoice>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::polymorphic_allocator::ReflectableChoice>& lhs, const View<::test_object::polymorphic_allocator::ReflectableChoice>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::polymorphic_allocator::ReflectableChoice>& view, ::std::string_view)
{
    validate(view.param(), "'ReflectableChoice.param'");
    switch (view.param())
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        // check choice case
        if (view.zserioChoiceTag() != ::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr)
        {
            throw ChoiceCaseException("Wrong case set in choice 'ReflectableChoice' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
                    " != " << static_cast<size_t>(::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr) << ")!";
        }
        validate(view.valueStr(), "'ReflectableChoice.valueStr'");
        break;
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        // check choice case
        if (view.zserioChoiceTag() != ::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32)
        {
            throw ChoiceCaseException("Wrong case set in choice 'ReflectableChoice' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
                    " != " << static_cast<size_t>(::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32) << ")!";
        }
        validate(view.value32(), "'ReflectableChoice.value32'");
        break;
    default:
        // empty
        break;
    }
}

template <>
BitSize bitSizeOf(const View<::test_object::polymorphic_allocator::ReflectableChoice>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    switch (view.param())
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        endBitPosition += bitSizeOf(view.valueStr(), endBitPosition);
        break;
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        endBitPosition += bitSizeOf(view.value32(), endBitPosition);
        break;
    default:
        // empty
        break;
    }

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::polymorphic_allocator::ReflectableChoice>& view)
{
    switch (view.param())
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        write(writer, view.valueStr());
        break;
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        write(writer, view.value32());
        break;
    default:
        // empty
        break;
    }
}

template <>
View<::test_object::polymorphic_allocator::ReflectableChoice> read(BitStreamReader& reader, ::test_object::polymorphic_allocator::ReflectableChoice& data,
        ::test_object::polymorphic_allocator::ReflectableEnum param_)
{
    View<::test_object::polymorphic_allocator::ReflectableChoice> view(data,
            param_);
    switch (view.param())
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        data.emplace<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(data.get_allocator());
        read(reader, data.get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>());
        break;
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        data.emplace<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>();
        read(reader, data.get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>());
        break;
    default:
        // empty
        break;
    }

    return view;
}

const ::zserio::pmr::ITypeInfo& TypeInfo<::test_object::polymorphic_allocator::ReflectableChoice, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::get()
{
    using AllocatorType = ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 2> fields = {
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
        },
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
        }
    };

    static const ::std::array<::zserio::BasicParameterInfo<AllocatorType>, 1> parameters = {
        ::zserio::BasicParameterInfo<AllocatorType>{
            "param",
            ::zserio::typeInfo<::test_object::polymorphic_allocator::ReflectableEnum, ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>()
        }
    };

    static const ::zserio::Span<::zserio::BasicFunctionInfo<AllocatorType>> functions;

    static const std::array<::std::string_view, 1> case0Expressions = {
        "::test_object::polymorphic_allocator::ReflectableEnum::VALUE1"
    };
    static const std::array<::std::string_view, 1> case1Expressions = {
        "::test_object::polymorphic_allocator::ReflectableEnum::VALUE2"
    };
    static const ::std::array<::zserio::BasicCaseInfo<AllocatorType>, 3> cases = {
        ::zserio::BasicCaseInfo<AllocatorType>{
            case0Expressions,
            &fields[0]
        },
        ::zserio::BasicCaseInfo<AllocatorType>{
            case1Expressions,
            &fields[1]
        },
        ::zserio::BasicCaseInfo<AllocatorType>{
            {}, nullptr
        }
    };

    static const ::zserio::detail::ChoiceTypeInfo<AllocatorType> typeInfo = {
        "test_object.polymorphic_allocator.ReflectableChoice",
        [](const AllocatorType& allocator) -> ::zserio::pmr::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::polymorphic_allocator::ReflectableChoice>>(allocator, allocator);
        },
        templateName, templateArguments,
        fields, parameters, functions, "param()", cases
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::pmr::IReflectableDataConstPtr reflectable(const ::test_object::polymorphic_allocator::ReflectableChoice& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::polymorphic_allocator::ReflectableChoice& object_, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::ReflectableChoice>(), alloc),
                m_object(object_)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "valueStr")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableChoice'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr:
                return "valueStr";
            case ::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32:
                return "value32";
            default:
                return "";
            }
        }

        ::zserio::pmr::Any getAnyValue(const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) const override
        {
            return ::zserio::pmr::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::polymorphic_allocator::ReflectableChoice& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

template <>
::zserio::pmr::IReflectableDataPtr reflectable(::test_object::polymorphic_allocator::ReflectableChoice& object, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::polymorphic_allocator::ReflectableChoice& object_, const ::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataAllocatorHolderBase<::zserio::pmr::PropagatingPolymorphicAllocator<uint8_t>>(typeInfo<::test_object::polymorphic_allocator::ReflectableChoice>(), alloc),
                m_object(object_)
        {}

        ::zserio::pmr::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "valueStr")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableChoice'!";
        }

        ::zserio::pmr::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "valueStr")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableChoice'!";
        }

        void setField(::std::string_view name, const ::zserio::pmr::Any& value) override
        {
            if (name == "valueStr")
            {
                m_object.emplace<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(
                        value.get<::zserio::pmr::String>());
                return;
            }
            if (name == "value32")
            {
                if (value.isType<::zserio::UInt32>())
                {
                    m_object.emplace<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>(
                            value.get<::zserio::UInt32>());
                }
                else
                {
                    m_object.emplace<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>(
                            value.get<::zserio::UInt32::ValueType>());
                }
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableChoice'!";
        }

        ::zserio::pmr::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "valueStr")
            {
                m_object.emplace<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(get_allocator());
                return ::zserio::reflectable(get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                m_object.emplace<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>();
                return ::zserio::reflectable(get<::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableChoice'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_valueStr:
                return "valueStr";
            case ::test_object::polymorphic_allocator::ReflectableChoice::ChoiceTag::CHOICE_value32:
                return "value32";
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
        ::test_object::polymorphic_allocator::ReflectableChoice& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::polymorphic_allocator::ReflectableChoice>::operator()(const ::test_object::polymorphic_allocator::ReflectableChoice& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, static_cast<const ::test_object::polymorphic_allocator::ReflectableChoice::Base&>(value));
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::polymorphic_allocator::ReflectableChoice>>::operator()(const ::zserio::View<::test_object::polymorphic_allocator::ReflectableChoice>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.param());
    switch (view.param())
    {
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE1:
        result = ::zserio::calcHashCode(result, view.valueStr());
        break;
    case ::test_object::polymorphic_allocator::ReflectableEnum::VALUE2:
        result = ::zserio::calcHashCode(result, view.value32());
        break;
    default:
        // empty
        break;
    }

    return static_cast<size_t>(result);
}

} // namespace std
