#ifndef ZSERIO_REFLECTABLE_DATA_H_INC
#define ZSERIO_REFLECTABLE_DATA_H_INC

#include "zserio/Enums.h"
#include "zserio/IReflectableData.h"
#include "zserio/IntrospectableDataBase.h"
#include "zserio/ReflectableUtil.h"
#include "zserio/TypeInfoUtil.h"

namespace zserio
{

namespace detail
{

/**
 * Base class for all reflectable data implementations.
 */
template <typename ALLOC>
class ReflectableDataBase : public IntrospectableDataBase<IBasicReflectableData<ALLOC>, ALLOC>
{
public:
    using Ptr = typename IBasicReflectableData<ALLOC>::Ptr;
    using ConstPtr = typename IBasicReflectableData<ALLOC>::ConstPtr;

    using IntrospectableDataBase<IBasicReflectableData<ALLOC>, ALLOC>::getField;
    using IntrospectableDataBase<IBasicReflectableData<ALLOC>, ALLOC>::at;
    using IntrospectableDataBase<IBasicReflectableData<ALLOC>, ALLOC>::operator[];
    using IntrospectableDataBase<IBasicReflectableData<ALLOC>, ALLOC>::getAnyValue;

    /**
     * Constructor.
     *
     * \param typeInfo Type info of the object.
     */
    explicit ReflectableDataBase(const IBasicTypeInfo<ALLOC>& typeInfo);

    /** Destructor. */
    ~ReflectableDataBase() override = 0;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    ReflectableDataBase(const ReflectableDataBase&) = delete;
    ReflectableDataBase& operator=(const ReflectableDataBase&) = delete;

    ReflectableDataBase(const ReflectableDataBase&&) = delete;
    ReflectableDataBase& operator=(const ReflectableDataBase&&) = delete;
    /**
     * \}
     */

    Ptr getField(std::string_view name) override;
    Ptr createField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;

    void resize(size_t size) override;
    Ptr at(size_t index) override;
    Ptr operator[](size_t index) override;
    void setAt(const BasicAny<ALLOC>& value, size_t index) override;
    void append(const BasicAny<ALLOC>& value) override;

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override;
    BasicAny<ALLOC> getAnyValue() override;

    ConstPtr find(std::string_view path) const override;
    Ptr find(std::string_view path) override;
    Ptr operator[](std::string_view path) override;

private:
    ConstPtr getFieldFromObject(const IBasicReflectableData<ALLOC>& object, std::string_view name) const;
    ConstPtr getFromObject(const IBasicReflectableData<ALLOC>& object, std::string_view path, size_t pos) const;
    Ptr getFieldFromObject(IBasicReflectableData<ALLOC>& object, std::string_view name);
    Ptr getFromObject(IBasicReflectableData<ALLOC>& object, std::string_view path, size_t pos);
};

/**
 * Base class for numeric (arithmetic) types, string view and span.
 *
 * Hold the value.
 */
template <typename T, typename ALLOC>
class BuiltinReflectableDataBase : public ReflectableDataBase<ALLOC>
{
private:
    using Base = ReflectableDataBase<ALLOC>;
    using ReflectableDataBase<ALLOC>::getAnyValue;

protected:
    using Type = T;

    BuiltinReflectableDataBase(const IBasicTypeInfo<ALLOC>& typeInfo, T value) :
            Base(typeInfo),
            m_value(value)
    {}

    T getValue() const
    {
        return m_value;
    }

public:
    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(m_value, allocator);
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override
    {
        return BasicAny<ALLOC>(m_value, allocator);
    }

private:
    T m_value;
};

/**
 * Base class for integral reflectables.
 *
 * Implements toString() and toDouble() conversions.
 */
template <typename T, typename ALLOC>
class IntegralReflectableDataBase : public BuiltinReflectableDataBase<T, ALLOC>
{
protected:
    static_assert(std::is_integral_v<typename T::ValueType>, "T must be a signed integral type!");

    using Base = BuiltinReflectableDataBase<T, ALLOC>;

public:
    IntegralReflectableDataBase(const IBasicTypeInfo<ALLOC>& typeInfo, T value) :
            Base(typeInfo, value)
    {}

    double toDouble() const override
    {
        return static_cast<double>(Base::getValue());
    }

    BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const override
    {
        return zserio::toString<ALLOC>(Base::getValue(), allocator);
    }
};

/**
 * Base class for signed integral reflectables.
 *
 * Implements toInt() conversion.
 */
template <typename T, typename ALLOC>
class SignedReflectableDataBase : public IntegralReflectableDataBase<T, ALLOC>
{
protected:
    static_assert(std::is_signed_v<typename T::ValueType>, "T must be a signed integral type!");

    using Base = IntegralReflectableDataBase<T, ALLOC>;

    using Base::Base;

public:
    int64_t toInt() const override
    {
        return Base::getValue();
    }
};

/**
 * Base class for unsigned integral reflectables.
 *
 * Implements toUInt() conversion.
 */
template <typename T, typename ALLOC>
class UnsignedReflectableDataBase : public IntegralReflectableDataBase<T, ALLOC>
{
protected:
    static_assert(std::is_unsigned<typename T::ValueType>::value, "T must be an unsigned integral type!");

    using Base = IntegralReflectableDataBase<T, ALLOC>;

    using Base::Base;

public:
    uint64_t toUInt() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for values of bool type.
 */
template <typename ALLOC>
class BoolReflectableData : public UnsignedReflectableDataBase<Bool, ALLOC>
{
private:
    using Base = UnsignedReflectableDataBase<Bool, ALLOC>;

public:
    explicit BoolReflectableData(Bool value) :
            Base(typeInfo<zserio::Bool, ALLOC>(), value)
    {}

    bool getBool() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for int8 type.
 */
template <typename T, typename ALLOC>
class Int8ReflectableData : public SignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = SignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<int8_t, typename T::ValueType>, "T must be based on int8_t!");

public:
    explicit Int8ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int8_t getInt8() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for int16 type.
 */
template <typename T, typename ALLOC>
class Int16ReflectableData : public SignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = SignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<int16_t, typename T::ValueType>, "T must be based on int16_t!");

public:
    explicit Int16ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int16_t getInt16() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for int32 type.
 */
template <typename T, typename ALLOC>
class Int32ReflectableData : public SignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = SignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<int32_t, typename T::ValueType>, "T must be based on int32_t!");

public:
    explicit Int32ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int32_t getInt32() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for int64 type.
 */
template <typename T, typename ALLOC>
class Int64ReflectableData : public SignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = SignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<int64_t, typename T::ValueType>, "T must be based on int64_t!");

public:
    explicit Int64ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int64_t getInt64() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for uint8 type.
 */
template <typename T, typename ALLOC>
class UInt8ReflectableData : public UnsignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<uint8_t, typename T::ValueType>, "T must be based on uint8_t!");

public:
    explicit UInt8ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint8_t getUInt8() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for uint16 type.
 */
template <typename T, typename ALLOC>
class UInt16ReflectableData : public UnsignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<uint16_t, typename T::ValueType>, "T must be based on uint16_t!");

public:
    explicit UInt16ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint16_t getUInt16() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for uint32 type.
 */
template <typename T, typename ALLOC>
class UInt32ReflectableData : public UnsignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<uint32_t, typename T::ValueType>, "T must be based on uint32_t!");

public:
    explicit UInt32ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint32_t getUInt32() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for uint64 type.
 */
template <typename T, typename ALLOC>
class UInt64ReflectableData : public UnsignedReflectableDataBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableDataBase<T, ALLOC>;

    static_assert(std::is_same_v<uint64_t, typename T::ValueType>, "T must be based on uint64_t!");

public:
    explicit UInt64ReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint64_t getUInt64() const override
    {
        return Base::getValue();
    }
};

/**
 * Base class for floating point reflectables.
 */
template <typename T, typename ALLOC>
class FloatingPointReflectableDataBase : public BuiltinReflectableDataBase<T, ALLOC>
{
protected:
    static_assert(std::is_floating_point_v<typename T::ValueType>, "T must be a floating point type!");

    using Base = BuiltinReflectableDataBase<T, ALLOC>;
    using Base::Base;

public:
    double toDouble() const override
    {
        return static_cast<double>(Base::getValue());
    }
};

/**
 * Reflectable for values of float type.
 */
template <typename T, typename ALLOC>
class FloatReflectableData : public FloatingPointReflectableDataBase<T, ALLOC>
{
private:
    using Base = FloatingPointReflectableDataBase<T, ALLOC>;

public:
    explicit FloatReflectableData(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    float getFloat() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for values of double type.
 */
template <typename ALLOC>
class DoubleReflectableData : public FloatingPointReflectableDataBase<Float64, ALLOC>
{
private:
    using Base = FloatingPointReflectableDataBase<Float64, ALLOC>;

public:
    explicit DoubleReflectableData(Float64 value) :
            Base(typeInfo<Float64, ALLOC>(), value)
    {}

    double getDouble() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for values of bytes type.
 */
template <typename ALLOC>
class BytesReflectableData : public BuiltinReflectableDataBase<BytesView, ALLOC>
{
private:
    using Base = BuiltinReflectableDataBase<BytesView, ALLOC>;

public:
    explicit BytesReflectableData(BytesView value) :
            Base(typeInfo<BasicBytes<ALLOC>>(), value)
    {}

    BytesView getBytes() const override
    {
        return Base::getValue();
    }
};

/**
 * Reflectable for values of string type.
 */
template <typename ALLOC>
class StringReflectableData : public BuiltinReflectableDataBase<std::string_view, ALLOC>
{
private:
    using Base = BuiltinReflectableDataBase<std::string_view, ALLOC>;

public:
    explicit StringReflectableData(std::string_view value) :
            Base(typeInfo<BasicString<RebindAlloc<ALLOC, char>>>(), value)
    {}

    std::string_view getStringView() const override
    {
        return Base::getValue();
    }

    BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const override
    {
        return zserio::toString(Base::getValue(), allocator);
    }
};

/**
 * Reflectable for values of bit buffer type.
 */
template <typename ALLOC>
class BitBufferReflectableData
        : public BuiltinReflectableDataBase<std::reference_wrapper<const BasicBitBuffer<ALLOC>>, ALLOC>
{
private:
    using Base = BuiltinReflectableDataBase<std::reference_wrapper<const BasicBitBuffer<ALLOC>>, ALLOC>;

public:
    explicit BitBufferReflectableData(std::reference_wrapper<const BasicBitBuffer<ALLOC>> value) :
            Base(typeInfo<BasicBitBuffer<ALLOC>>(), value)
    {}

    const BasicBitBuffer<ALLOC>& getBitBuffer() const override
    {
        return Base::getValue();
    }
};

/**
 * Base class for reflectable which needs to hold an allocator.
 */
template <typename ALLOC>
class ReflectableDataAllocatorHolderBase : public ReflectableDataBase<ALLOC>, public AllocatorHolder<ALLOC>
{
public:
    ReflectableDataAllocatorHolderBase(const IBasicTypeInfo<ALLOC>& typeInfo, const ALLOC& allocator) :
            ReflectableDataBase<ALLOC>(typeInfo),
            AllocatorHolder<ALLOC>(allocator)
    {}
};

/**
 * Base class for constant reflectable which needs to hold an allocator.
 *
 * Overrides non constant methods and throws exception with info about constness.
 */
template <typename ALLOC>
class ReflectableDataConstAllocatorHolderBase : public ReflectableDataAllocatorHolderBase<ALLOC>
{
private:
    using Base = ReflectableDataAllocatorHolderBase<ALLOC>;

public:
    using Base::Base;
    using Base::getTypeInfo;

    IBasicReflectableDataPtr<ALLOC> getField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override;
};

/**
 * Base class for reflectable arrays.
 *
 * Overrides all generic methods with default throw behavior.
 */
template <typename ALLOC>
class ReflectableDataArrayBase : public ReflectableDataAllocatorHolderBase<ALLOC>
{
private:
    using Base = ReflectableDataAllocatorHolderBase<ALLOC>;

public:
    using Base::Base;
    using Base::getTypeInfo;

    bool isArray() const override
    {
        return true;
    }

    IBasicReflectableDataConstPtr<ALLOC> getField(std::string_view name) const override;
    IBasicReflectableDataPtr<ALLOC> getField(std::string_view name) override;
    IBasicReflectableDataPtr<ALLOC> createField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;

    IBasicReflectableDataConstPtr<ALLOC> operator[](size_t index) const override;
    IBasicReflectableDataPtr<ALLOC> operator[](size_t index) override;

    bool getBool() const override;
    int8_t getInt8() const override;
    int16_t getInt16() const override;
    int32_t getInt32() const override;
    int64_t getInt64() const override;
    uint8_t getUInt8() const override;
    uint16_t getUInt16() const override;
    uint32_t getUInt32() const override;
    uint64_t getUInt64() const override;
    float getFloat() const override;
    double getDouble() const override;
    BytesView getBytes() const override;
    std::string_view getStringView() const override;
    const BasicBitBuffer<ALLOC>& getBitBuffer() const override;

    int64_t toInt() const override;
    uint64_t toUInt() const override;
    double toDouble() const override;
    BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const override;
};

/**
 * Base class for constant reflectable arrays.
 *
 * Overrides non constant methods and throws exception with info about constness.
 */
template <typename ALLOC>
class ReflectableDataConstArrayBase : public ReflectableDataArrayBase<ALLOC>
{
private:
    using Base = ReflectableDataArrayBase<ALLOC>;

public:
    using Base::Base;
    using Base::getTypeInfo;

    void resize(size_t index) override;
    IBasicReflectableDataPtr<ALLOC> at(size_t index) override;
    IBasicReflectableDataPtr<ALLOC> operator[](size_t index) override;
    void setAt(const BasicAny<ALLOC>& value, size_t index) override;
    void append(const BasicAny<ALLOC>& value) override;

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override;
};

} // namespace detail

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(Bool value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::BoolReflectableData<ALLOC>>(allocator, value);
}

template <BitSize BIT_SIZE, bool IS_SIGNED, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(
        detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED> value, const ALLOC& allocator = ALLOC())
{
    using Type = detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>;
    using ValueType = typename Type::ValueType;

    if constexpr (std::is_signed_v<ValueType>)
    {
        if constexpr (sizeof(ValueType) > 4)
        {
            return std::allocate_shared<detail::Int64ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(ValueType) > 2)
        {
            return std::allocate_shared<detail::Int32ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(ValueType) > 1)
        {
            return std::allocate_shared<detail::Int16ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<detail::Int8ReflectableData<Type, ALLOC>>(allocator, value);
        }
    }
    else
    {
        if constexpr (sizeof(ValueType) > 4)
        {
            return std::allocate_shared<detail::UInt64ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(ValueType) > 2)
        {
            return std::allocate_shared<detail::UInt32ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(ValueType) > 1)
        {
            return std::allocate_shared<detail::UInt16ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<detail::UInt8ReflectableData<Type, ALLOC>>(allocator, value);
        }
    }
}

template <typename T, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(detail::DynIntWrapper<T> value, const ALLOC& allocator = ALLOC())
{
    using Type = detail::DynIntWrapper<T>;

    if constexpr (std::is_signed_v<T>)
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<detail::Int64ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<detail::Int32ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<detail::Int16ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<detail::Int8ReflectableData<Type, ALLOC>>(allocator, value);
        }
    }
    else
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<detail::UInt64ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<detail::UInt32ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<detail::UInt16ReflectableData<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<detail::UInt8ReflectableData<Type, ALLOC>>(allocator, value);
        }
    }
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarInt16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::Int16ReflectableData<VarInt16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarInt32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::Int32ReflectableData<VarInt32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarInt64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::Int64ReflectableData<VarInt64, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarInt value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::Int64ReflectableData<VarInt, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarUInt16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::UInt16ReflectableData<VarUInt16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarUInt32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::UInt32ReflectableData<VarUInt32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarUInt64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::UInt64ReflectableData<VarUInt64, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarUInt value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::UInt64ReflectableData<VarUInt, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(VarSize value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::UInt32ReflectableData<VarSize, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(Float16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::FloatReflectableData<Float16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(Float32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::FloatReflectableData<Float32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(Float64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::DoubleReflectableData<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(const BasicBytes<ALLOC>& value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::BytesReflectableData<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(BytesView value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::BytesReflectableData<ALLOC>>(allocator, value);
}

template <typename STRING_ALLOC, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(
        const BasicString<STRING_ALLOC>& value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::StringReflectableData<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(std::string_view value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::StringReflectableData<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectable(BasicBitBuffer<ALLOC>& value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::BitBufferReflectableData<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataConstPtr<ALLOC> reflectable(
        const BasicBitBuffer<ALLOC>& value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::BitBufferReflectableData<ALLOC>>(allocator, value);
}

namespace detail
{

/**
 * Reflectable for arrays of builtin types (except dynamic bit field arrays).
 */
/** \{ */
template <typename RAW_ARRAY, typename ALLOC>
class ReflectableDataConstArray : public ReflectableDataConstArrayBase<ALLOC>
{
private:
    using Base = ReflectableDataConstArrayBase<ALLOC>;

public:
    using Base::at;
    using Base::getTypeInfo;
    using Base::operator[];
    using Base::getAnyValue;

    explicit ReflectableDataConstArray(const RAW_ARRAY& rawArray, const ALLOC& allocator = {}) :
            Base(typeInfo<typename RAW_ARRAY::value_type, ALLOC>(), allocator),
            m_rawArray(rawArray)
    {}

    size_t size() const override
    {
        return m_rawArray.size();
    }

    IBasicReflectableDataConstPtr<ALLOC> at(size_t index) const override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return reflectable(m_rawArray[index], Base::get_allocator());
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

private:
    const RAW_ARRAY& m_rawArray;
};

template <typename RAW_ARRAY, typename ALLOC>
class ReflectableDataArray : public ReflectableDataArrayBase<ALLOC>
{
private:
    using Base = ReflectableDataArrayBase<ALLOC>;

public:
    using Base::getAnyValue;
    using Base::getTypeInfo;

    explicit ReflectableDataArray(RAW_ARRAY& rawArray, const ALLOC& allocator = {}) :
            Base(typeInfo<typename RAW_ARRAY::value_type, ALLOC>(), allocator),
            m_rawArray(rawArray)
    {}

    size_t size() const override
    {
        return m_rawArray.size();
    }

    void resize(size_t size) override
    {
        m_rawArray.resize(size);
    }

    IBasicReflectableDataConstPtr<ALLOC> at(size_t index) const override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return reflectable(m_rawArray[index], Base::get_allocator());
    }

    IBasicReflectableDataPtr<ALLOC> at(size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return reflectable(m_rawArray[index], Base::get_allocator());
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        m_rawArray[index] = ReflectableUtil::fromAny<typename RAW_ARRAY::value_type>(value);
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        m_rawArray.push_back(ReflectableUtil::fromAny<typename RAW_ARRAY::value_type>(value));
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override
    {
        return BasicAny<ALLOC>(std::ref(m_rawArray), allocator);
    }

private:
    RAW_ARRAY& m_rawArray;
};

/**
 * Wrapper around reflectable which actually owns the reflected object.
 *
 * This is needed in ZserioTreeCreator to be able to generically create the new instance of a zserio object.
 */
template <typename T, typename ALLOC = typename T::allocator_type>
class ReflectableDataOwner : public IBasicReflectableData<ALLOC>
{
public:
    ReflectableDataOwner() :
            ReflectableDataOwner(ALLOC())
    {}

    explicit ReflectableDataOwner(const ALLOC& allocator) :
            m_object(allocator),
            m_reflectable(reflectable(m_object, allocator))
    {}

    const IBasicTypeInfo<ALLOC>& getTypeInfo() const override
    {
        return m_reflectable->getTypeInfo();
    }

    bool isArray() const override
    {
        return m_reflectable->isArray();
    }

    IBasicReflectableDataConstPtr<ALLOC> getField(std::string_view name) const override
    {
        return m_reflectable->getField(name);
    }

    IBasicReflectableDataPtr<ALLOC> getField(std::string_view name) override
    {
        return m_reflectable->getField(name);
    }

    IBasicReflectableDataPtr<ALLOC> createField(std::string_view name) override
    {
        return m_reflectable->createField(name);
    }

    void setField(std::string_view name, const BasicAny<ALLOC>& value) override
    {
        m_reflectable->setField(name, value);
    }

    IBasicReflectableDataConstPtr<ALLOC> find(std::string_view path) const override
    {
        return m_reflectable->find(path);
    }

    IBasicReflectableDataPtr<ALLOC> find(std::string_view path) override
    {
        return m_reflectable->find(path);
    }

    IBasicReflectableDataConstPtr<ALLOC> operator[](std::string_view path) const override
    {
        return m_reflectable->operator[](path);
    }

    IBasicReflectableDataPtr<ALLOC> operator[](std::string_view path) override
    {
        return m_reflectable->operator[](path);
    }

    std::string_view getChoice() const override
    {
        return m_reflectable->getChoice();
    }

    size_t size() const override
    {
        return m_reflectable->size();
    }

    void resize(size_t size) override
    {
        m_reflectable->resize(size);
    }

    IBasicReflectableDataConstPtr<ALLOC> at(size_t index) const override
    {
        return m_reflectable->at(index);
    }

    IBasicReflectableDataPtr<ALLOC> at(size_t index) override
    {
        return m_reflectable->at(index);
    }

    IBasicReflectableDataConstPtr<ALLOC> operator[](size_t index) const override
    {
        return m_reflectable->operator[](index);
    }

    IBasicReflectableDataPtr<ALLOC> operator[](size_t index) override
    {
        return m_reflectable->operator[](index);
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        m_reflectable->setAt(value, index);
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        m_reflectable->append(value);
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return m_reflectable->getAnyValue(allocator);
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override
    {
        return m_reflectable->getAnyValue(allocator);
    }

    BasicAny<ALLOC> getAnyValue() const override
    {
        return getAnyValue(ALLOC());
    }

    BasicAny<ALLOC> getAnyValue() override
    {
        return getAnyValue(ALLOC());
    }

    // exact checked getters
    bool getBool() const override
    {
        return m_reflectable->getBool();
    }
    int8_t getInt8() const override
    {
        return m_reflectable->getInt8();
    }
    int16_t getInt16() const override
    {
        return m_reflectable->getInt16();
    }
    int32_t getInt32() const override
    {
        return m_reflectable->getInt32();
    }
    int64_t getInt64() const override
    {
        return m_reflectable->getInt64();
    }
    uint8_t getUInt8() const override
    {
        return m_reflectable->getUInt8();
    }
    uint16_t getUInt16() const override
    {
        return m_reflectable->getUInt16();
    }
    uint32_t getUInt32() const override
    {
        return m_reflectable->getUInt32();
    }
    uint64_t getUInt64() const override
    {
        return m_reflectable->getUInt64();
    }
    float getFloat() const override
    {
        return m_reflectable->getFloat();
    }
    double getDouble() const override
    {
        return m_reflectable->getDouble();
    }
    Span<const uint8_t> getBytes() const override
    {
        return m_reflectable->getBytes();
    }
    std::string_view getStringView() const override
    {
        return m_reflectable->getStringView();
    }
    const BasicBitBuffer<ALLOC>& getBitBuffer() const override
    {
        return m_reflectable->getBitBuffer();
    }

    // convenience conversions
    int64_t toInt() const override
    {
        return m_reflectable->toInt();
    }
    uint64_t toUInt() const override
    {
        return m_reflectable->toUInt();
    }
    double toDouble() const override
    {
        return m_reflectable->toDouble();
    }
    BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const override
    {
        return m_reflectable->toString(allocator);
    }
    BasicString<RebindAlloc<ALLOC, char>> toString() const override
    {
        return toString(ALLOC());
    }

private:
    T m_object;
    IBasicReflectableDataPtr<ALLOC> m_reflectable;
};

// implementation of base classes methods

template <typename ALLOC>
ReflectableDataBase<ALLOC>::ReflectableDataBase(const IBasicTypeInfo<ALLOC>& typeInfo) :
        IntrospectableDataBase<IBasicReflectableData<ALLOC>, ALLOC>(typeInfo)
{}

template <typename ALLOC>
ReflectableDataBase<ALLOC>::~ReflectableDataBase() = default;

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::getField(std::string_view)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' has no fields to get!";
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::createField(std::string_view)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' has no fields to create!";
}

template <typename ALLOC>
void ReflectableDataBase<ALLOC>::setField(std::string_view, const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' has no fields to set!";
}

template <typename ALLOC>
void ReflectableDataBase<ALLOC>::resize(size_t)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::at(size_t)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::operator[](size_t)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
void ReflectableDataBase<ALLOC>::setAt(const BasicAny<ALLOC>&, size_t)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
void ReflectableDataBase<ALLOC>::append(const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableDataBase<ALLOC>::getAnyValue(const ALLOC&)
{
    throw CppRuntimeException("Type '")
            << ReflectableDataBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableDataBase<ALLOC>::getAnyValue()
{
    return getAnyValue(ALLOC());
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::ConstPtr ReflectableDataBase<ALLOC>::find(std::string_view path) const
{
    return getFromObject(*this, path, 0);
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::find(std::string_view path)
{
    return getFromObject(*this, path, 0);
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::operator[](std::string_view path)
{
    return find(path);
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::ConstPtr ReflectableDataBase<ALLOC>::getFieldFromObject(
        const IBasicReflectableData<ALLOC>& object, std::string_view name) const
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& fields = typeInfo.getFields();
        auto fieldsIt =
                std::find_if(fields.begin(), fields.end(), [name](const BasicFieldInfo<ALLOC>& fieldInfo) {
                    return fieldInfo.schemaName == name;
                });
        if (fieldsIt != fields.end())
        {
            return object.getField(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::ConstPtr ReflectableDataBase<ALLOC>::getFromObject(
        const IBasicReflectableData<ALLOC>& object, std::string_view path, size_t pos) const
{
    try
    {
        const size_t dotPos = path.find('.', pos);
        const bool isLast = dotPos == std::string_view::npos;
        const std::string_view name =
                path.substr(pos, dotPos == std::string_view::npos ? std::string_view::npos : dotPos - pos);

        auto field = getFieldFromObject(object, name);
        if (field)
        {
            return isLast ? std::move(field) : getFromObject(*field, path, dotPos + 1);
        }
    }
    catch (const CppRuntimeException&)
    {}

    return nullptr;
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::getFieldFromObject(
        IBasicReflectableData<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& fields = typeInfo.getFields();
        auto fieldsIt =
                std::find_if(fields.begin(), fields.end(), [name](const BasicFieldInfo<ALLOC>& fieldInfo) {
                    return fieldInfo.schemaName == name;
                });
        if (fieldsIt != fields.end())
        {
            return object.getField(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
typename ReflectableDataBase<ALLOC>::Ptr ReflectableDataBase<ALLOC>::getFromObject(
        IBasicReflectableData<ALLOC>& object, std::string_view path, size_t pos)
{
    try
    {
        const size_t dotPos = path.find('.', pos);
        const bool isLast = dotPos == std::string_view::npos;
        const std::string_view name =
                path.substr(pos, dotPos == std::string_view::npos ? std::string_view::npos : dotPos - pos);

        auto field = getFieldFromObject(object, name);
        if (field)
        {
            return isLast ? std::move(field) : getFromObject(*field, path, dotPos + 1);
        }
    }
    catch (const CppRuntimeException&)
    {}

    return nullptr;
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> ReflectableDataConstAllocatorHolderBase<ALLOC>::getField(std::string_view)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
void ReflectableDataConstAllocatorHolderBase<ALLOC>::setField(std::string_view, const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableDataConstAllocatorHolderBase<ALLOC>::getAnyValue(const ALLOC&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
IBasicReflectableDataConstPtr<ALLOC> ReflectableDataArrayBase<ALLOC>::getField(std::string_view) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> ReflectableDataArrayBase<ALLOC>::getField(std::string_view)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> ReflectableDataArrayBase<ALLOC>::createField(std::string_view)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
void ReflectableDataArrayBase<ALLOC>::setField(std::string_view, const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectableDataConstPtr<ALLOC> ReflectableDataArrayBase<ALLOC>::operator[](size_t index) const
{
    return this->at(index);
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> ReflectableDataArrayBase<ALLOC>::operator[](size_t index)
{
    return this->at(index);
}

template <typename ALLOC>
bool ReflectableDataArrayBase<ALLOC>::getBool() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int8_t ReflectableDataArrayBase<ALLOC>::getInt8() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int16_t ReflectableDataArrayBase<ALLOC>::getInt16() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int32_t ReflectableDataArrayBase<ALLOC>::getInt32() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int64_t ReflectableDataArrayBase<ALLOC>::getInt64() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint8_t ReflectableDataArrayBase<ALLOC>::getUInt8() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint16_t ReflectableDataArrayBase<ALLOC>::getUInt16() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint32_t ReflectableDataArrayBase<ALLOC>::getUInt32() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint64_t ReflectableDataArrayBase<ALLOC>::getUInt64() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
float ReflectableDataArrayBase<ALLOC>::getFloat() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
double ReflectableDataArrayBase<ALLOC>::getDouble() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
BytesView ReflectableDataArrayBase<ALLOC>::getBytes() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
std::string_view ReflectableDataArrayBase<ALLOC>::getStringView() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
const BasicBitBuffer<ALLOC>& ReflectableDataArrayBase<ALLOC>::getBitBuffer() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int64_t ReflectableDataArrayBase<ALLOC>::toInt() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint64_t ReflectableDataArrayBase<ALLOC>::toUInt() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
double ReflectableDataArrayBase<ALLOC>::toDouble() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
BasicString<RebindAlloc<ALLOC, char>> ReflectableDataArrayBase<ALLOC>::toString(const ALLOC&) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
void ReflectableDataConstArrayBase<ALLOC>::resize(size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> ReflectableDataConstArrayBase<ALLOC>::at(size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
IBasicReflectableDataPtr<ALLOC> ReflectableDataConstArrayBase<ALLOC>::operator[](size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
void ReflectableDataConstArrayBase<ALLOC>::setAt(const BasicAny<ALLOC>&, size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
void ReflectableDataConstArrayBase<ALLOC>::append(const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableDataConstArrayBase<ALLOC>::getAnyValue(const ALLOC&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

} // namespace detail

template <typename T, typename VECTOR_ALLOC, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataConstPtr<ALLOC> reflectableArray(
        const std::vector<T, VECTOR_ALLOC>& array, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::ReflectableDataConstArray<std::vector<T, VECTOR_ALLOC>, ALLOC>>(
            allocator, array);
}

template <typename T, typename VECTOR_ALLOC, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableDataPtr<ALLOC> reflectableArray(
        std::vector<T, VECTOR_ALLOC>& array, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<detail::ReflectableDataArray<std::vector<T, VECTOR_ALLOC>, ALLOC>>(
            allocator, array);
}

} // namespace zserio

#endif // ZSERIO_REFLECTABLE_DATA_H_INC
