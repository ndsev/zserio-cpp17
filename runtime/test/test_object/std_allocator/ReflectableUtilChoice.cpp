/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/ChoiceCaseException.h>
#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_allocator/ReflectableUtilChoice.h>

namespace test_object
{
namespace std_allocator
{

bool operator==(const ::test_object::std_allocator::ReflectableUtilChoice& lhs, const ::test_object::std_allocator::ReflectableUtilChoice& rhs)
{
    return static_cast<const ::test_object::std_allocator::ReflectableUtilChoice::Base&>(lhs) == static_cast<const ::test_object::std_allocator::ReflectableUtilChoice::Base&>(rhs);
}

bool operator<(const ::test_object::std_allocator::ReflectableUtilChoice& lhs, const ::test_object::std_allocator::ReflectableUtilChoice&  rhs)
{
    return static_cast<const ::test_object::std_allocator::ReflectableUtilChoice::Base&>(lhs) < static_cast<const ::test_object::std_allocator::ReflectableUtilChoice::Base&>(rhs);
}

bool operator!=(const ::test_object::std_allocator::ReflectableUtilChoice& lhs, const ::test_object::std_allocator::ReflectableUtilChoice& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::ReflectableUtilChoice& lhs, const ::test_object::std_allocator::ReflectableUtilChoice& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::ReflectableUtilChoice& lhs, const ::test_object::std_allocator::ReflectableUtilChoice& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::ReflectableUtilChoice& lhs, const ::test_object::std_allocator::ReflectableUtilChoice& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::ReflectableUtilChoice>::View(const ::test_object::std_allocator::ReflectableUtilChoice& data,
        ::zserio::UInt8 param_) noexcept :
        m_data(&data),
        m_param_(param_)
{}

View<::test_object::std_allocator::ReflectableUtilChoice>::View(const ::test_object::std_allocator::ReflectableUtilChoice& data,
        const View& other) noexcept :
        m_data(&data),
        m_param_(other.param())
{}

::zserio::UInt8 View<::test_object::std_allocator::ReflectableUtilChoice>::param() const
{
    return m_param_;
}

Array<const ::zserio::UInt32> View<::test_object::std_allocator::ReflectableUtilChoice>::array() const
{
    return Array<const ::zserio::UInt32>{get<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>(*m_data)};
}

::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag View<::test_object::std_allocator::ReflectableUtilChoice>::zserioChoiceTag() const
{
    return m_data->index();
}

const ::test_object::std_allocator::ReflectableUtilChoice& View<::test_object::std_allocator::ReflectableUtilChoice>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::std_allocator::ReflectableUtilChoice>& rhs)
{
    if (lhs.param() != rhs.param())
    {
        return false;
    }

    switch (lhs.param())
    {
    case 1:
    case 2:
        return (lhs.array() == rhs.array());
    default:
        return true; // empty
    }
}

bool operator<(const View<::test_object::std_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::std_allocator::ReflectableUtilChoice>& rhs)
{
    if (lhs.param() != rhs.param())
    {
        return lhs.param() < rhs.param();
    }

    switch (lhs.param())
    {
    case 1:
    case 2:
        return (lhs.array() < rhs.array());
    default:
        return false; // empty
    }
}

bool operator!=(const View<::test_object::std_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::std_allocator::ReflectableUtilChoice>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::std_allocator::ReflectableUtilChoice>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::std_allocator::ReflectableUtilChoice>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::ReflectableUtilChoice>& lhs, const View<::test_object::std_allocator::ReflectableUtilChoice>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::ReflectableUtilChoice>& view, ::std::string_view)
{
    validate(view.param(), "'ReflectableUtilChoice.param'");
    switch (view.param())
    {
    case 1:
    case 2:
        // check choice case
        if (view.zserioChoiceTag() != ::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array)
        {
            throw ChoiceCaseException("Wrong case set in choice 'ReflectableUtilChoice' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
                    " != " << static_cast<size_t>(::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array) << ")!";
        }
        validate<ArrayType::AUTO>(view.array(), "'ReflectableUtilChoice.array'");
        break;
    default:
        // empty
        break;
    }
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::ReflectableUtilChoice>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    switch (view.param())
    {
    case 1:
    case 2:
        endBitPosition += bitSizeOf<ArrayType::AUTO>(view.array(), endBitPosition);
        break;
    default:
        // empty
        break;
    }

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::ReflectableUtilChoice>& view)
{
    switch (view.param())
    {
    case 1:
    case 2:
        write<ArrayType::AUTO>(writer, view.array());
        break;
    default:
        // empty
        break;
    }
}

template <>
View<::test_object::std_allocator::ReflectableUtilChoice> read(BitStreamReader& reader, ::test_object::std_allocator::ReflectableUtilChoice& data,
        ::zserio::UInt8 param_)
{
    View<::test_object::std_allocator::ReflectableUtilChoice> view(data,
            param_);
    switch (view.param())
    {
    case 1:
    case 2:
        data.emplace<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>();
        read<ArrayType::AUTO>(reader, data.get<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>());
        break;
    default:
        // empty
        break;
    }

    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::ReflectableUtilChoice, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 1> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "array", // schemaName
            ::zserio::typeInfo<::zserio::UInt32, ::std::allocator<uint8_t>>(), // typeInfo
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

    static const ::std::array<::zserio::BasicParameterInfo<AllocatorType>, 1> parameters = {
        ::zserio::BasicParameterInfo<AllocatorType>{
            "param",
            ::zserio::typeInfo<::zserio::UInt8, ::std::allocator<uint8_t>>()
        }
    };

    static const ::zserio::Span<::zserio::BasicFunctionInfo<AllocatorType>> functions;

    static const std::array<::std::string_view, 2> case0Expressions = {
        "1",
        "2"
    };
    static const ::std::array<::zserio::BasicCaseInfo<AllocatorType>, 2> cases = {
        ::zserio::BasicCaseInfo<AllocatorType>{
            case0Expressions,
            &fields[0]
        },
        ::zserio::BasicCaseInfo<AllocatorType>{
            {}, nullptr
        }
    };

    static const ::zserio::detail::ChoiceTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_allocator.ReflectableUtilChoice",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::std_allocator::ReflectableUtilChoice>>(allocator, allocator);
        },
        templateName, templateArguments,
        fields, parameters, functions, "param()", cases
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(const ::test_object::std_allocator::ReflectableUtilChoice& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::ReflectableUtilChoice& object, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::ReflectableUtilChoice>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "array")
            {
                return ::zserio::reflectableArray(
                        get<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilChoice'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array:
                return "array";
            default:
                return "";
            }
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::ReflectableUtilChoice& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::ReflectableUtilChoice& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_allocator::ReflectableUtilChoice& object, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::ReflectableUtilChoice>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "array")
            {
                return ::zserio::reflectableArray(
                        get<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilChoice'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "array")
            {
                return ::zserio::reflectableArray(
                        get<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilChoice'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "array")
            {
                m_object.emplace<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::Vector<::zserio::UInt32>>(value));
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilChoice'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "array")
            {
                m_object.emplace<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>(get_allocator());
                return ::zserio::reflectableArray(get<::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilChoice'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::std_allocator::ReflectableUtilChoice::ChoiceTag::CHOICE_array:
                return "array";
            default:
                return "";
            }
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
        ::test_object::std_allocator::ReflectableUtilChoice& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, value, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::ReflectableUtilChoice>::operator()(const ::test_object::std_allocator::ReflectableUtilChoice& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, static_cast<const ::test_object::std_allocator::ReflectableUtilChoice::Base&>(value));
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::ReflectableUtilChoice>>::operator()(const ::zserio::View<::test_object::std_allocator::ReflectableUtilChoice>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.param());
    switch (view.param())
    {
    case 1:
    case 2:
        result = ::zserio::calcHashCode(result, view.array());
        break;
    default:
        // empty
        break;
    }

    return static_cast<size_t>(result);
}

} // namespace std
