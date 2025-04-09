/**
 * Automatically generated by Zserio C++17 generator version 0.2.0 using Zserio core 2.16.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/ChoiceCaseException.h>
#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_allocator/WalkerChoice.h>

namespace test_object
{
namespace std_allocator
{

bool operator==(const ::test_object::std_allocator::WalkerChoice& lhs, const ::test_object::std_allocator::WalkerChoice& rhs)
{
    return static_cast<const ::test_object::std_allocator::WalkerChoice::Base&>(lhs) == static_cast<const ::test_object::std_allocator::WalkerChoice::Base&>(rhs);
}

bool operator<(const ::test_object::std_allocator::WalkerChoice& lhs, const ::test_object::std_allocator::WalkerChoice&  rhs)
{
    return static_cast<const ::test_object::std_allocator::WalkerChoice::Base&>(lhs) < static_cast<const ::test_object::std_allocator::WalkerChoice::Base&>(rhs);
}

bool operator!=(const ::test_object::std_allocator::WalkerChoice& lhs, const ::test_object::std_allocator::WalkerChoice& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::WalkerChoice& lhs, const ::test_object::std_allocator::WalkerChoice& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::WalkerChoice& lhs, const ::test_object::std_allocator::WalkerChoice& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::WalkerChoice& lhs, const ::test_object::std_allocator::WalkerChoice& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::WalkerChoice>::View(const ::test_object::std_allocator::WalkerChoice& data,
        ::zserio::UInt8 selector_) noexcept :
        m_data(&data),
        m_selector_(selector_)
{}

View<::test_object::std_allocator::WalkerChoice>::View(const ::test_object::std_allocator::WalkerChoice& data,
        const View& other) noexcept :
        m_data(&data),
        m_selector_(other.selector())
{}

::zserio::UInt8 View<::test_object::std_allocator::WalkerChoice>::selector() const
{
    return m_selector_;
}

::zserio::UInt8 View<::test_object::std_allocator::WalkerChoice>::value8() const
{
    return get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>(*m_data);
}

::zserio::UInt16 View<::test_object::std_allocator::WalkerChoice>::value16() const
{
    return get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>(*m_data);
}

::zserio::UInt32 View<::test_object::std_allocator::WalkerChoice>::value32() const
{
    return get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>(*m_data);
}

::zserio::UInt64 View<::test_object::std_allocator::WalkerChoice>::value64() const
{
    return get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>(*m_data);
}

::test_object::std_allocator::WalkerChoice::ChoiceTag View<::test_object::std_allocator::WalkerChoice>::zserioChoiceTag() const
{
    return m_data->index();
}

const ::test_object::std_allocator::WalkerChoice& View<::test_object::std_allocator::WalkerChoice>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::WalkerChoice>& lhs, const View<::test_object::std_allocator::WalkerChoice>& rhs)
{
    if (lhs.selector() != rhs.selector())
    {
        return false;
    }

    switch (lhs.selector())
    {
    case 8:
        return (lhs.value8() == rhs.value8());
    case 16:
        return (lhs.value16() == rhs.value16());
    case 32:
        return (lhs.value32() == rhs.value32());
    case 64:
        return (lhs.value64() == rhs.value64());
    default:
        return true; // empty
    }
}

bool operator<(const View<::test_object::std_allocator::WalkerChoice>& lhs, const View<::test_object::std_allocator::WalkerChoice>& rhs)
{
    if (lhs.selector() != rhs.selector())
    {
        return lhs.selector() < rhs.selector();
    }

    switch (lhs.selector())
    {
    case 8:
        return (lhs.value8() < rhs.value8());
    case 16:
        return (lhs.value16() < rhs.value16());
    case 32:
        return (lhs.value32() < rhs.value32());
    case 64:
        return (lhs.value64() < rhs.value64());
    default:
        return false; // empty
    }
}

bool operator!=(const View<::test_object::std_allocator::WalkerChoice>& lhs, const View<::test_object::std_allocator::WalkerChoice>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::WalkerChoice>& lhs, const View<::test_object::std_allocator::WalkerChoice>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::WalkerChoice>& lhs, const View<::test_object::std_allocator::WalkerChoice>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::WalkerChoice>& lhs, const View<::test_object::std_allocator::WalkerChoice>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::WalkerChoice>& view, ::std::string_view)
{
    validate(view.selector(), "'WalkerChoice.selector'");
    switch (view.selector())
    {
    case 8:
        // check choice case
        if (view.zserioChoiceTag() != ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8)
        {
            throw ChoiceCaseException("Wrong case set in choice 'WalkerChoice' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
                    " != " << static_cast<size_t>(::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8) << ")!";
        }
        validate(view.value8(), "'WalkerChoice.value8'");
        break;
    case 16:
        // check choice case
        if (view.zserioChoiceTag() != ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16)
        {
            throw ChoiceCaseException("Wrong case set in choice 'WalkerChoice' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
                    " != " << static_cast<size_t>(::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16) << ")!";
        }
        validate(view.value16(), "'WalkerChoice.value16'");
        break;
    case 32:
        // check choice case
        if (view.zserioChoiceTag() != ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32)
        {
            throw ChoiceCaseException("Wrong case set in choice 'WalkerChoice' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
                    " != " << static_cast<size_t>(::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32) << ")!";
        }
        validate(view.value32(), "'WalkerChoice.value32'");
        break;
    case 64:
        // check choice case
        if (view.zserioChoiceTag() != ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64)
        {
            throw ChoiceCaseException("Wrong case set in choice 'WalkerChoice' (") << static_cast<size_t>(view.zserioChoiceTag()) <<
                    " != " << static_cast<size_t>(::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64) << ")!";
        }
        validate(view.value64(), "'WalkerChoice.value64'");
        break;
    default:
        // empty
        break;
    }
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::WalkerChoice>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;
    switch (view.selector())
    {
    case 8:
        endBitPosition += bitSizeOf(view.value8(), endBitPosition);
        break;
    case 16:
        endBitPosition += bitSizeOf(view.value16(), endBitPosition);
        break;
    case 32:
        endBitPosition += bitSizeOf(view.value32(), endBitPosition);
        break;
    case 64:
        endBitPosition += bitSizeOf(view.value64(), endBitPosition);
        break;
    default:
        // empty
        break;
    }

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::WalkerChoice>& view)
{
    switch (view.selector())
    {
    case 8:
        write(writer, view.value8());
        break;
    case 16:
        write(writer, view.value16());
        break;
    case 32:
        write(writer, view.value32());
        break;
    case 64:
        write(writer, view.value64());
        break;
    default:
        // empty
        break;
    }
}

template <>
View<::test_object::std_allocator::WalkerChoice> read(BitStreamReader& reader, ::test_object::std_allocator::WalkerChoice& data,
        ::zserio::UInt8 selector_)
{
    View<::test_object::std_allocator::WalkerChoice> view(data,
            selector_);
    switch (view.selector())
    {
    case 8:
        data.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>();
        read(reader, data.get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>());
        break;
    case 16:
        data.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>();
        read(reader, data.get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>());
        break;
    case 32:
        data.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>();
        read(reader, data.get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>());
        break;
    case 64:
        data.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>();
        read(reader, data.get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>());
        break;
    default:
        // empty
        break;
    }

    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::WalkerChoice, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 4> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "value8", // schemaName
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
            "value16", // schemaName
            ::zserio::typeInfo<::zserio::UInt16, ::std::allocator<uint8_t>>(), // typeInfo
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
            "value64", // schemaName
            ::zserio::typeInfo<::zserio::UInt64, ::std::allocator<uint8_t>>(), // typeInfo
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
            "selector",
            ::zserio::typeInfo<::zserio::UInt8, ::std::allocator<uint8_t>>()
        }
    };

    static const ::zserio::Span<::zserio::BasicFunctionInfo<AllocatorType>> functions;

    static const std::array<::std::string_view, 1> case0Expressions = {
        "8"
    };
    static const std::array<::std::string_view, 1> case1Expressions = {
        "16"
    };
    static const std::array<::std::string_view, 1> case2Expressions = {
        "32"
    };
    static const std::array<::std::string_view, 1> case3Expressions = {
        "64"
    };
    static const ::std::array<::zserio::BasicCaseInfo<AllocatorType>, 5> cases = {
        ::zserio::BasicCaseInfo<AllocatorType>{
            case0Expressions,
            &fields[0]
        },
        ::zserio::BasicCaseInfo<AllocatorType>{
            case1Expressions,
            &fields[1]
        },
        ::zserio::BasicCaseInfo<AllocatorType>{
            case2Expressions,
            &fields[2]
        },
        ::zserio::BasicCaseInfo<AllocatorType>{
            case3Expressions,
            &fields[3]
        },
        ::zserio::BasicCaseInfo<AllocatorType>{
            {}, nullptr
        }
    };

    static const ::zserio::detail::ChoiceTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_allocator.WalkerChoice",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return ::std::allocate_shared<::zserio::ReflectableDataOwner<::test_object::std_allocator::WalkerChoice>>(allocator);
        },
        templateName, templateArguments,
        fields, parameters, functions, "selector()", cases
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(const ::test_object::std_allocator::WalkerChoice& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::WalkerChoice& object, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::WalkerChoice>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value8")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>(m_object), get_allocator());
            }
            if (name == "value16")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "value64")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerChoice'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8:
                return "value8";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16:
                return "value16";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32:
                return "value32";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64:
                return "value64";
            default:
                return "";
            }
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::WalkerChoice& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::IReflectableDataPtr reflectable(::test_object::std_allocator::WalkerChoice& value, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(::test_object::std_allocator::WalkerChoice& object, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::WalkerChoice>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value8")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>(m_object), get_allocator());
            }
            if (name == "value16")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "value64")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerChoice'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "value8")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>(m_object), get_allocator());
            }
            if (name == "value16")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "value64")
            {
                return ::zserio::reflectable(
                        get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerChoice'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "value8")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::UInt8>(value));
                return;
            }
            if (name == "value16")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::UInt16>(value));
                return;
            }
            if (name == "value32")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::UInt32>(value));
                return;
            }
            if (name == "value64")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>(
                        ::zserio::ReflectableUtil::fromAny<::zserio::UInt64>(value));
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerChoice'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "value8")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>();
                return ::zserio::reflectable(get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8>(m_object), get_allocator());
            }
            if (name == "value16")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>();
                return ::zserio::reflectable(get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16>(m_object), get_allocator());
            }
            if (name == "value32")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>();
                return ::zserio::reflectable(get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32>(m_object), get_allocator());
            }
            if (name == "value64")
            {
                m_object.emplace<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>();
                return ::zserio::reflectable(get<::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64>(m_object), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerChoice'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (m_object.index())
            {
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8:
                return "value8";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16:
                return "value16";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32:
                return "value32";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64:
                return "value64";
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
        ::test_object::std_allocator::WalkerChoice& m_object;
    };

    return ::std::allocate_shared<Reflectable>(allocator, value);
}

template <>
::zserio::IIntrospectableViewConstPtr introspectable(const View<::test_object::std_allocator::WalkerChoice>& view, const ::std::allocator<uint8_t>& allocator)
{
    class Introspectable : public ::zserio::CompoundIntrospectableViewBase<::test_object::std_allocator::WalkerChoice, ::std::allocator<uint8_t>>
    {
    public:
        Introspectable(const ::zserio::View<::test_object::std_allocator::WalkerChoice>& view_, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::CompoundIntrospectableViewBase<::test_object::std_allocator::WalkerChoice, ::std::allocator<uint8_t>>(
                        view_, alloc)
        {}

        ::zserio::IIntrospectableViewConstPtr getField(::std::string_view name) const override
        {
            if (name == "value8")
            {
                return ::zserio::introspectable(getValue().value8(), get_allocator());
            }
            if (name == "value16")
            {
                return ::zserio::introspectable(getValue().value16(), get_allocator());
            }
            if (name == "value32")
            {
                return ::zserio::introspectable(getValue().value32(), get_allocator());
            }
            if (name == "value64")
            {
                return ::zserio::introspectable(getValue().value64(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'WalkerChoice'!";
        }

        ::zserio::IIntrospectableViewConstPtr getParameter(::std::string_view name) const override
        {
            if (name == "selector")
            {
                return ::zserio::introspectable(getValue().selector(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Parameter '") << name << "' doesn't exist in 'WalkerChoice'!";
        }

        ::std::string_view getChoice() const override
        {
            switch (getValue().zserioChoiceTag())
            {
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value8:
                return "value8";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value16:
                return "value16";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value32:
                return "value32";
            case ::test_object::std_allocator::WalkerChoice::ChoiceTag::CHOICE_value64:
                return "value64";
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

size_t hash<::test_object::std_allocator::WalkerChoice>::operator()(const ::test_object::std_allocator::WalkerChoice& value) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, static_cast<const ::test_object::std_allocator::WalkerChoice::Base&>(value));
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::WalkerChoice>>::operator()(const ::zserio::View<::test_object::std_allocator::WalkerChoice>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.selector());
    switch (view.selector())
    {
    case 8:
        result = ::zserio::calcHashCode(result, view.value8());
        break;
    case 16:
        result = ::zserio::calcHashCode(result, view.value16());
        break;
    case 32:
        result = ::zserio::calcHashCode(result, view.value32());
        break;
    case 64:
        result = ::zserio::calcHashCode(result, view.value64());
        break;
    default:
        // empty
        break;
    }

    return static_cast<size_t>(result);
}

} // namespace std
