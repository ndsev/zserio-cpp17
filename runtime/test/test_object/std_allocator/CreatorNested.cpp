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

#include <test_object/std_allocator/CreatorNested.h>

namespace test_object
{
namespace std_allocator
{

CreatorNested::CreatorNested() noexcept :
        CreatorNested(AllocatorType{})
{}

CreatorNested::CreatorNested(const AllocatorType& allocator) noexcept :
        value(),
        text(allocator),
        externData(allocator),
        bytesData(allocator),
        creatorEnum(),
        creatorBitmask()
{}

CreatorNested::CreatorNested(
        ::zserio::UInt32 value_,
        ::zserio::String text_,
        ::zserio::BitBuffer externData_,
        ::zserio::Bytes bytesData_,
        ::test_object::std_allocator::CreatorEnum creatorEnum_,
        ::test_object::std_allocator::CreatorBitmask creatorBitmask_) :
        value(value_),
        text(::std::move(text_)),
        externData(::std::move(externData_)),
        bytesData(::std::move(bytesData_)),
        creatorEnum(creatorEnum_),
        creatorBitmask(creatorBitmask_)
{}

bool operator==(const ::test_object::std_allocator::CreatorNested& lhs, const ::test_object::std_allocator::CreatorNested& rhs)
{
    return ::std::tie(
            lhs.value,
            lhs.text,
            lhs.externData,
            lhs.bytesData,
            lhs.creatorEnum,
            lhs.creatorBitmask)
            == ::std::tie(
            rhs.value,
            rhs.text,
            rhs.externData,
            rhs.bytesData,
            rhs.creatorEnum,
            rhs.creatorBitmask);
}

bool operator<(const ::test_object::std_allocator::CreatorNested& lhs, const ::test_object::std_allocator::CreatorNested& rhs)
{
    return ::std::tie(
            lhs.value,
            lhs.text,
            lhs.externData,
            lhs.bytesData,
            lhs.creatorEnum,
            lhs.creatorBitmask)
            < ::std::tie(
            rhs.value,
            rhs.text,
            rhs.externData,
            rhs.bytesData,
            rhs.creatorEnum,
            rhs.creatorBitmask);
}

bool operator!=(const ::test_object::std_allocator::CreatorNested& lhs, const ::test_object::std_allocator::CreatorNested& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const ::test_object::std_allocator::CreatorNested& lhs, const ::test_object::std_allocator::CreatorNested& rhs)
{
    return rhs < lhs;
}

bool operator<=(const ::test_object::std_allocator::CreatorNested& lhs, const ::test_object::std_allocator::CreatorNested& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const ::test_object::std_allocator::CreatorNested& lhs, const ::test_object::std_allocator::CreatorNested& rhs)
{
    return !(lhs < rhs);
}

} // namespace std_allocator
} // namespace test_object

namespace zserio
{

View<::test_object::std_allocator::CreatorNested>::View(const ::test_object::std_allocator::CreatorNested& data,
        ::zserio::UInt32 param_) noexcept :
        m_data(&data),
        m_param_(param_)
{}

View<::test_object::std_allocator::CreatorNested>::View(const ::test_object::std_allocator::CreatorNested& data,
        const View& other) noexcept :
        m_data(&data),
        m_param_(other.param())
{}

::zserio::UInt32 View<::test_object::std_allocator::CreatorNested>::param() const
{
    return m_param_;
}

::zserio::UInt32 View<::test_object::std_allocator::CreatorNested>::value() const
{
    return m_data->value;
}

::std::string_view View<::test_object::std_allocator::CreatorNested>::text() const
{
    return ::std::string_view{
            m_data->text};
}

::zserio::BitBufferView View<::test_object::std_allocator::CreatorNested>::externData() const
{
    return ::zserio::BitBufferView{
            m_data->externData};
}

BytesView View<::test_object::std_allocator::CreatorNested>::bytesData() const
{
    return BytesView{
            m_data->bytesData};
}

::test_object::std_allocator::CreatorEnum View<::test_object::std_allocator::CreatorNested>::creatorEnum() const
{
    return m_data->creatorEnum;
}

::test_object::std_allocator::CreatorBitmask View<::test_object::std_allocator::CreatorNested>::creatorBitmask() const
{
    return m_data->creatorBitmask;
}

const ::test_object::std_allocator::CreatorNested& View<::test_object::std_allocator::CreatorNested>::zserioData() const
{
    return *m_data;
}

bool operator==(const View<::test_object::std_allocator::CreatorNested>& lhs, const View<::test_object::std_allocator::CreatorNested>& rhs)
{
    return lhs.param() == rhs.param() &&
            lhs.value() == rhs.value() &&
            lhs.text() == rhs.text() &&
            lhs.externData() == rhs.externData() &&
            lhs.bytesData() == rhs.bytesData() &&
            lhs.creatorEnum() == rhs.creatorEnum() &&
            lhs.creatorBitmask() == rhs.creatorBitmask();
}

bool operator<(const View<::test_object::std_allocator::CreatorNested>& lhs, const View<::test_object::std_allocator::CreatorNested>& rhs)
{
    if (lhs.param() != rhs.param())
    {
        return lhs.param() < rhs.param();
    }
    if (lhs.value() != rhs.value())
    {
        return lhs.value() < rhs.value();
    }
    if (lhs.text() != rhs.text())
    {
        return lhs.text() < rhs.text();
    }
    if (lhs.externData() != rhs.externData())
    {
        return lhs.externData() < rhs.externData();
    }
    if (lhs.bytesData() != rhs.bytesData())
    {
        return lhs.bytesData() < rhs.bytesData();
    }
    if (lhs.creatorEnum() != rhs.creatorEnum())
    {
        return lhs.creatorEnum() < rhs.creatorEnum();
    }
    if (lhs.creatorBitmask() != rhs.creatorBitmask())
    {
        return lhs.creatorBitmask() < rhs.creatorBitmask();
    }

    return false;
}

bool operator!=(const View<::test_object::std_allocator::CreatorNested>& lhs, const View<::test_object::std_allocator::CreatorNested>& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const View<::test_object::std_allocator::CreatorNested>& lhs, const View<::test_object::std_allocator::CreatorNested>& rhs)
{
    return rhs < lhs;
}

bool operator<=(const View<::test_object::std_allocator::CreatorNested>& lhs, const View<::test_object::std_allocator::CreatorNested>& rhs)
{
    return !(rhs < lhs);
}

bool operator>=(const View<::test_object::std_allocator::CreatorNested>& lhs, const View<::test_object::std_allocator::CreatorNested>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <>
void validate(const View<::test_object::std_allocator::CreatorNested>& view, ::std::string_view)
{
    validate(view.param(), "'CreatorNested.param'");
    validate(view.value(), "'CreatorNested.value'");
    validate(view.text(), "'CreatorNested.text'");
    validate(view.externData(), "'CreatorNested.externData'");
    validate(view.bytesData(), "'CreatorNested.bytesData'");
    validate(view.creatorEnum(), "'CreatorNested.creatorEnum'");
    validate(view.creatorBitmask(), "'CreatorNested.creatorBitmask'");
}

template <>
BitSize bitSizeOf(const View<::test_object::std_allocator::CreatorNested>& view, BitSize bitPosition)
{
    BitSize endBitPosition = bitPosition;

    auto value_ = view.value();
    endBitPosition += bitSizeOf(value_, endBitPosition);
    auto text_ = view.text();
    endBitPosition += bitSizeOf(text_, endBitPosition);
    auto externData_ = view.externData();
    endBitPosition += bitSizeOf(externData_, endBitPosition);
    auto bytesData_ = view.bytesData();
    endBitPosition += bitSizeOf(bytesData_, endBitPosition);
    auto creatorEnum_ = view.creatorEnum();
    endBitPosition += bitSizeOf(creatorEnum_, endBitPosition);
    auto creatorBitmask_ = view.creatorBitmask();
    endBitPosition += bitSizeOf(creatorBitmask_, endBitPosition);

    return endBitPosition - bitPosition;
}

template <>
void write(BitStreamWriter& writer, const View<::test_object::std_allocator::CreatorNested>& view)
{
    auto value_ = view.value();
    write(writer, value_);
    auto text_ = view.text();
    write(writer, text_);
    auto externData_ = view.externData();
    write(writer, externData_);
    auto bytesData_ = view.bytesData();
    write(writer, bytesData_);
    auto creatorEnum_ = view.creatorEnum();
    write(writer, creatorEnum_);
    auto creatorBitmask_ = view.creatorBitmask();
    write(writer, creatorBitmask_);
}

template <>
View<::test_object::std_allocator::CreatorNested> read(BitStreamReader& reader, ::test_object::std_allocator::CreatorNested& data,
        ::zserio::UInt32 param_)
{
    View<::test_object::std_allocator::CreatorNested> view(data,
            param_);
    read(reader, data.value);
    read(reader, data.text);
    read(reader, data.externData);
    read(reader, data.bytesData);
    read(reader, data.creatorEnum);
    read(reader, data.creatorBitmask);
    return view;
}

const ::zserio::ITypeInfo& TypeInfo<::test_object::std_allocator::CreatorNested, ::std::allocator<uint8_t>>::get()
{
    using AllocatorType = ::std::allocator<uint8_t>;

    static const ::std::string_view templateName;
    static const ::zserio::Span<::zserio::BasicTemplateArgumentInfo<AllocatorType>> templateArguments;

    static const ::std::array<::zserio::BasicFieldInfo<AllocatorType>, 6> fields = {
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
            "externData", // schemaName
            ::zserio::typeInfo<::zserio::BitBuffer, ::std::allocator<uint8_t>>(), // typeInfo
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
            "bytesData", // schemaName
            ::zserio::typeInfo<::zserio::Bytes, ::std::allocator<uint8_t>>(), // typeInfo
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
            "creatorEnum", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::CreatorEnum, ::std::allocator<uint8_t>>(), // typeInfo
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
            "creatorBitmask", // schemaName
            ::zserio::typeInfo<::test_object::std_allocator::CreatorBitmask, ::std::allocator<uint8_t>>(), // typeInfo
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
            ::zserio::typeInfo<::zserio::UInt32, ::std::allocator<uint8_t>>()
        }
    };

    static const ::zserio::Span<::zserio::BasicFunctionInfo<AllocatorType>> functions;

    static const ::zserio::detail::StructTypeInfo<AllocatorType> typeInfo = {
        "test_object.std_allocator.CreatorNested",
        [](const AllocatorType& allocator) -> ::zserio::IReflectableDataPtr
        {
            return std::allocate_shared<::zserio::ReflectableOwner<::test_object::std_allocator::CreatorNested>>(allocator, allocator);
        },
        templateName, templateArguments, fields, parameters, functions
    };

    return typeInfo;
}

} // namespace detail

template <>
::zserio::IReflectableDataConstPtr reflectable(
        const ::test_object::std_allocator::CreatorNested& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        using ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>::getField;
        using ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>::getAnyValue;

        explicit Reflectable(const ::test_object::std_allocator::CreatorNested& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableConstAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::CreatorNested>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "externData")
            {
                return ::zserio::reflectable(m_object.externData, get_allocator());
            }
            if (name == "bytesData")
            {
                return ::zserio::reflectable(m_object.bytesData, get_allocator());
            }
            if (name == "creatorEnum")
            {
                return ::zserio::reflectable(m_object.creatorEnum, get_allocator());
            }
            if (name == "creatorBitmask")
            {
                return ::zserio::reflectable(m_object.creatorBitmask, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorNested'!";
        }

        ::zserio::Any getAnyValue(const ::std::allocator<uint8_t>& alloc) const override
        {
            return ::zserio::Any(::std::cref(m_object), alloc);
        }

    private:
        const ::test_object::std_allocator::CreatorNested& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

template <>
::zserio::IReflectableDataPtr reflectable(
        ::test_object::std_allocator::CreatorNested& object, const ::std::allocator<uint8_t>& allocator)
{
    class Reflectable : public ::zserio::ReflectableAllocatorHolderBase<::std::allocator<uint8_t>>
    {
    public:
        explicit Reflectable(::test_object::std_allocator::CreatorNested& object_, const ::std::allocator<uint8_t>& alloc) :
                ::zserio::ReflectableAllocatorHolderBase<::std::allocator<uint8_t>>(typeInfo<::test_object::std_allocator::CreatorNested>(), alloc),
                m_object(object_)
        {}

        ::zserio::IReflectableDataConstPtr getField(::std::string_view name) const override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "externData")
            {
                return ::zserio::reflectable(m_object.externData, get_allocator());
            }
            if (name == "bytesData")
            {
                return ::zserio::reflectable(m_object.bytesData, get_allocator());
            }
            if (name == "creatorEnum")
            {
                return ::zserio::reflectable(m_object.creatorEnum, get_allocator());
            }
            if (name == "creatorBitmask")
            {
                return ::zserio::reflectable(m_object.creatorBitmask, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorNested'!";
        }

        ::zserio::IReflectableDataPtr getField(::std::string_view name) override
        {
            if (name == "value")
            {
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "text")
            {
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "externData")
            {
                return ::zserio::reflectable(m_object.externData, get_allocator());
            }
            if (name == "bytesData")
            {
                return ::zserio::reflectable(m_object.bytesData, get_allocator());
            }
            if (name == "creatorEnum")
            {
                return ::zserio::reflectable(m_object.creatorEnum, get_allocator());
            }
            if (name == "creatorBitmask")
            {
                return ::zserio::reflectable(m_object.creatorBitmask, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorNested'!";
        }

        void setField(::std::string_view name, const ::zserio::Any& value) override
        {
            if (name == "value")
            {
                m_object.value =
                        value.get<::zserio::UInt32>();
                return;
            }
            if (name == "text")
            {
                m_object.text =
                        value.get<::zserio::String>();
                return;
            }
            if (name == "externData")
            {
                m_object.externData =
                        value.get<::zserio::BitBuffer>();
                return;
            }
            if (name == "bytesData")
            {
                m_object.bytesData =
                        value.get<::zserio::Bytes>();
                return;
            }
            if (name == "creatorEnum")
            {
                if (value.isType<::test_object::std_allocator::CreatorEnum>())
                {
                    m_object.creatorEnum =
                            value.get<::test_object::std_allocator::CreatorEnum>();
                }
                else if (value.isType<typename EnumTraits<::test_object::std_allocator::CreatorEnum>::ZserioType>())
                {
                    m_object.creatorEnum =
                            valueToEnum<::test_object::std_allocator::CreatorEnum>(value.get<typename EnumTraits<::test_object::std_allocator::CreatorEnum>::ZserioType>());
                }
                else
                {
                    m_object.creatorEnum =
                            valueToEnum<::test_object::std_allocator::CreatorEnum>(value.get<std::underlying_type_t<::test_object::std_allocator::CreatorEnum>>());
                }
                return;
            }
            if (name == "creatorBitmask")
            {
                if (value.isType<::test_object::std_allocator::CreatorBitmask>())
                {
                    m_object.creatorBitmask =
                            value.get<::test_object::std_allocator::CreatorBitmask>();
                }
                else if (value.isType<::test_object::std_allocator::CreatorBitmask::ZserioType>())
                {
                    m_object.creatorBitmask =
                            ::test_object::std_allocator::CreatorBitmask(value.get<::test_object::std_allocator::CreatorBitmask::ZserioType>());
                }
                else
                {
                    m_object.creatorBitmask =
                            ::test_object::std_allocator::CreatorBitmask(value.get<::test_object::std_allocator::CreatorBitmask::ZserioType::ValueType>());
                }
                return;
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorNested'!";
        }

        ::zserio::IReflectableDataPtr createField(::std::string_view name) override
        {
            if (name == "value")
            {
                m_object.value =
                        ::zserio::UInt32();
                return ::zserio::reflectable(m_object.value, get_allocator());
            }
            if (name == "text")
            {
                m_object.text =
                        ::zserio::String(get_allocator());
                return ::zserio::reflectable(m_object.text, get_allocator());
            }
            if (name == "externData")
            {
                m_object.externData =
                        ::zserio::BitBuffer(get_allocator());
                return ::zserio::reflectable(m_object.externData, get_allocator());
            }
            if (name == "bytesData")
            {
                m_object.bytesData =
                        ::zserio::Bytes(get_allocator());
                return ::zserio::reflectable(m_object.bytesData, get_allocator());
            }
            if (name == "creatorEnum")
            {
                m_object.creatorEnum =
                        ::test_object::std_allocator::CreatorEnum();
                return ::zserio::reflectable(m_object.creatorEnum, get_allocator());
            }
            if (name == "creatorBitmask")
            {
                m_object.creatorBitmask =
                        ::test_object::std_allocator::CreatorBitmask();
                return ::zserio::reflectable(m_object.creatorBitmask, get_allocator());
            }
            throw ::zserio::CppRuntimeException("Field '") << name << "' doesn't exist in 'CreatorNested'!";
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
        ::test_object::std_allocator::CreatorNested& m_object;
    };

    return std::allocate_shared<Reflectable>(allocator, object, allocator);
}

} // namespace zserio

namespace std
{

size_t hash<::test_object::std_allocator::CreatorNested>::operator()(const ::test_object::std_allocator::CreatorNested& data) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, data.value);
    result = ::zserio::calcHashCode(result, data.text);
    result = ::zserio::calcHashCode(result, data.externData);
    result = ::zserio::calcHashCode(result, data.bytesData);
    result = ::zserio::calcHashCode(result, data.creatorEnum);
    result = ::zserio::calcHashCode(result, data.creatorBitmask);
    return static_cast<size_t>(result);
}

size_t hash<::zserio::View<::test_object::std_allocator::CreatorNested>>::operator()(const ::zserio::View<::test_object::std_allocator::CreatorNested>& view) const
{
    uint32_t result = ::zserio::HASH_SEED;
    result = ::zserio::calcHashCode(result, view.param());
    result = ::zserio::calcHashCode(result, view.value());
    result = ::zserio::calcHashCode(result, view.text());
    result = ::zserio::calcHashCode(result, view.externData());
    result = ::zserio::calcHashCode(result, view.bytesData());
    result = ::zserio::calcHashCode(result, view.creatorEnum());
    result = ::zserio::calcHashCode(result, view.creatorBitmask());
    return static_cast<size_t>(result);
}

} // namespace std
