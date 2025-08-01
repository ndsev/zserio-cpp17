/**
 * Automatically generated by Zserio C++17 generator version 0.5.0 using Zserio core 2.17.0.
 * Generator setup: typeInfoCode, stdAllocator.
 */

#include <zserio/BitPositionUtil.h>
#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/IntrospectableView.h>
#include <zserio/ReflectableData.h>
#include <zserio/ReflectableUtil.h>
#include <zserio/TypeInfo.h>

#include <test_object/std_allocator/ReflectableUtilObject.h>

namespace test_object
{
namespace std_allocator
{

ReflectableUtilObject::ReflectableUtilObject() noexcept :
        ReflectableUtilObject(allocator_type{})
{}

ReflectableUtilObject::ReflectableUtilObject(const allocator_type& allocator) noexcept :
        choiceParam(),
        reflectableUtilChoice(allocator)
{}

ReflectableUtilObject::ReflectableUtilObject(ReflectableUtilObject&& other, const allocator_type& allocator) :
        choiceParam(other.choiceParam),
        reflectableUtilChoice(std::move(other.reflectableUtilChoice), allocator)
{}

ReflectableUtilObject::ReflectableUtilObject(const ReflectableUtilObject& other, const allocator_type& allocator) :
        choiceParam(other.choiceParam),
        reflectableUtilChoice(other.reflectableUtilChoice, allocator)
{}

ReflectableUtilObject::ReflectableUtilObject(
        ::zserio::UInt8 choiceParam_,
        ::test_object::std_allocator::ReflectableUtilChoice reflectableUtilChoice_,
        const allocator_type& allocator) :
        choiceParam(choiceParam_),
        reflectableUtilChoice(::std::move(reflectableUtilChoice_), allocator)
{}

bool operator==(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs)
{
    return ::std::tie(
            lhs.choiceParam,
            lhs.reflectableUtilChoice)
            == ::std::tie(
            rhs.choiceParam,
            rhs.reflectableUtilChoice);
}

bool operator<(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs)
{
    return ::std::tie(
            lhs.choiceParam,
            lhs.reflectableUtilChoice)
            < ::std::tie(
            rhs.choiceParam,
            rhs.reflectableUtilChoice);
}

bool operator!=(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::ReflectableUtilObject& lhs, const ::test_object::std_allocator::ReflectableUtilObject& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::ReflectableUtilObject>::View(const ::test_object::std_allocator::ReflectableUtilObject& data) noexcept :
        m_data(&data)
{}

View<::test_object::std_allocator::ReflectableUtilObject>::View(const ::test_object::std_allocator::ReflectableUtilObject& data,
        const View&) noexcept :
        m_data(&data)
{}

::zserio::UInt8 View<::test_object::std_allocator::ReflectableUtilObject>::choiceParam() const
{
    return m_data->choiceParam;
}

View<::test_object::std_allocator::ReflectableUtilChoice> View<::test_object::std_allocator::ReflectableUtilObject>::reflectableUtilChoice() const
{
    return View<::test_object::std_allocator::ReflectableUtilChoice>(m_data->reflectableUtilChoice, detail::makeParameter<0, ::test_object::std_allocator::ReflectableUtilChoice>(choiceParam()));
}

const ::test_object::std_allocator::ReflectableUtilObject& View<::test_object::std_allocator::ReflectableUtilObject>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs)
{
    return lhs.choiceParam() == rhs.choiceParam() &&
            lhs.reflectableUtilChoice() == rhs.reflectableUtilChoice();
}

bool operator<(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs)
{
    if (lhs.choiceParam() != rhs.choiceParam())
    {
        return lhs.choiceParam() < rhs.choiceParam();
    }
    if (lhs.reflectableUtilChoice() != rhs.reflectableUtilChoice())
    {
        return lhs.reflectableUtilChoice() < rhs.reflectableUtilChoice();
    }

    return false;
}

bool operator!=(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::ReflectableUtilObject>& lhs, const View<::test_object::std_allocator::ReflectableUtilObject>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

void ObjectTraits<::test_object::std_allocator::ReflectableUtilObject>::validate(const View<::test_object::std_allocator::ReflectableUtilObject>& view, ::std::string_view)
{
    detail::validate(view.choiceParam(), "'ReflectableUtilObject.choiceParam'");
    detail::validate(view.reflectableUtilChoice(), "'ReflectableUtilObject.reflectableUtilChoice'");
}

BitSize ObjectTraits<::test_object::std_allocator::ReflectableUtilObject>::bitSizeOf(const View<::test_object::std_allocator::ReflectableUtilObject>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto choiceParam_ = view.choiceParam();
    endBitPosition += detail::bitSizeOf(choiceParam_, endBitPosition);
    auto reflectableUtilChoice_ = view.reflectableUtilChoice();
    endBitPosition += detail::bitSizeOf(reflectableUtilChoice_, endBitPosition);

    return endBitPosition - bitPosition;
}

void ObjectTraits<::test_object::std_allocator::ReflectableUtilObject>::write(BitStreamWriter& writer, const View<::test_object::std_allocator::ReflectableUtilObject>& view)
{
    auto choiceParam_ = view.choiceParam();
    detail::write(writer, choiceParam_);
    auto reflectableUtilChoice_ = view.reflectableUtilChoice();
    detail::write(writer, reflectableUtilChoice_);
}

View<::test_object::std_allocator::ReflectableUtilObject> ObjectTraits<::test_object::std_allocator::ReflectableUtilObject>::read(BitStreamReader& reader, ::test_object::std_allocator::ReflectableUtilObject& data)
{
    View<::test_object::std_allocator::ReflectableUtilObject> view(data);
    detail::read(reader, data.choiceParam);
    (void)detail::read(reader, data.reflectableUtilChoice, detail::makeParameter<0, ::test_object::std_allocator::ReflectableUtilChoice>(view.choiceParam()));
    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::ReflectableUtilObject, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::std::string_view, 1> reflectableUtilChoiceTypeArguments = {
        "choiceParam()"
    };
    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 2> fields = {
        ::zserio::BasicFieldInfo<AllocatorType>{
            "choiceParam", // schemaName
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
            "reflectableUtilChoice", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::ReflectableUtilChoice, ::std::allocator<uint8_t>>(), // typeInfo
            reflectableUtilChoiceTypeArguments, // typeArguments
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
        "test_object.std_allocator.ReflectableUtilObject",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return ::std::allocate_shared<::zserio::detail::ReflectableDataOwner<::test_object::std_allocator::ReflectableUtilObject>>(allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

::zserio::IReflectableDataConstPtr Reflectable<::test_object::std_allocator::ReflectableUtilObject, ::std::allocator<uint8_t>>::create(
        const ::test_object::std_allocator::ReflectableUtilObject& value, const ::std::allocator<uint8_t>& allocator)
{
    class ReflectableImpl : public ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using Base = ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>;
        using Base::getField;
        using Base::getAnyValue;

        explicit ReflectableImpl(const ::test_object::std_allocator::ReflectableUtilObject& object, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::detail::ReflectableDataConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::ReflectableUtilObject>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "choiceParam")
            {
                return ::zserio::reflectable(m_object.choiceParam, get_allocator());
            }
            if (name == "reflectableUtilChoice")
            {
                return ::zserio::reflectable(m_object.reflectableUtilChoice, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilObject'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::ReflectableUtilObject& m_object;
    };

    return ::std::allocate_shared<ReflectableImpl>(allocator, value);
}

::zserio::IReflectableDataPtr Reflectable<::test_object::std_allocator::ReflectableUtilObject, ::std::allocator<uint8_t>>::create(
        ::test_object::std_allocator::ReflectableUtilObject& value, const ::std::allocator<uint8_t>& allocator)
{
    class ReflectableImpl : public ::zserio::detail::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using Base = ::zserio::detail::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>;
        using Base::getField;
        using Base::getAnyValue;

        explicit ReflectableImpl(::test_object::std_allocator::ReflectableUtilObject& object, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::detail::ReflectableDataAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::ReflectableUtilObject>(), alloc),
                m_object(object)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "choiceParam")
            {
                return ::zserio::reflectable(m_object.choiceParam, get_allocator());
            }
            if (name == "reflectableUtilChoice")
            {
                return ::zserio::reflectable(m_object.reflectableUtilChoice, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilObject'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "choiceParam")
            {
                return ::zserio::reflectable(m_object.choiceParam, get_allocator());
            }
            if (name == "reflectableUtilChoice")
            {
                return ::zserio::reflectable(m_object.reflectableUtilChoice, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilObject'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "choiceParam")
            {
                m_object.choiceParam = ::zserio::ReflectableUtil::fromAny<::zserio::UInt8>(value);
                return;
            }
            if (name == "reflectableUtilChoice")
            {
                m_object.reflectableUtilChoice = ::zserio::ReflectableUtil::fromAny<::test_object::std_allocator::ReflectableUtilChoice>(value);
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilObject'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "choiceParam")
            {
                m_object.choiceParam = ::zserio::UInt8();
                return ::zserio::reflectable(m_object.choiceParam, get_allocator());
            }
            if (name == "reflectableUtilChoice")
            {
                m_object.reflectableUtilChoice = ::test_object::std_allocator::ReflectableUtilChoice(get_allocator());
                return ::zserio::reflectable(m_object.reflectableUtilChoice, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilObject'!";
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
        ::test_object::std_allocator::ReflectableUtilObject& m_object;
    };

    return ::std::allocate_shared<ReflectableImpl>(allocator, value);
}

::zserio::IIntrospectableViewConstPtr Introspectable<::test_object::std_allocator::ReflectableUtilObject, ::std::allocator<uint8_t>>::create(
        const View<::test_object::std_allocator::ReflectableUtilObject>& view, const ::std::allocator<uint8_t>& allocator)
{
    class IntrospectableImpl : public ::zserio::detail::CompoundIntrospectableViewBase<::test_object::std_allocator::ReflectableUtilObject, ::std::allocator<uint8_t>>
    {
    public:
        explicit IntrospectableImpl(const ::zserio::View<::test_object::std_allocator::ReflectableUtilObject>& view_, const ::std::allocator<uint8_t>& alloc = {}) :
                ::zserio::detail::CompoundIntrospectableViewBase<::test_object::std_allocator::ReflectableUtilObject, ::std::allocator<uint8_t>>(
                        view_, alloc)
        {}

        ::zserio::IIntrospectableViewConstPtr getField(::std::string_view name) const override
        {
            if (name == "choiceParam")
            {
                return ::zserio::introspectable(getValue().choiceParam(), get_allocator());
            }
            if (name == "reflectableUtilChoice")
            {
                return ::zserio::introspectable(getValue().reflectableUtilChoice(), get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'ReflectableUtilObject'!";
        }
    };

    return ::std::allocate_shared<IntrospectableImpl>(allocator, view);
}

} // namespace detail
} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::ReflectableUtilObject>::operator()(const ::test_object::std_allocator::ReflectableUtilObject& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.choiceParam);
    result = ::zserio::calcHashCode(result, data.reflectableUtilChoice);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::ReflectableUtilObject>>::operator()(const ::zserio::View<::test_object::std_allocator::ReflectableUtilObject>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.choiceParam());
    result = ::zserio::calcHashCode(result, view.reflectableUtilChoice());
    return static_cast<size_t>(result);
}

} // namespace std
