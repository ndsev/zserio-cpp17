#ifndef ZSERIO_REFLECTABLE_H_INC
#define ZSERIO_REFLECTABLE_H_INC

#include <functional>
#include <type_traits>

#include "zserio/AllocatorHolder.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/Enums.h"
#include "zserio/IReflectable.h"
#include "zserio/TypeInfo.h"
#include "zserio/TypeInfoUtil.h"

namespace zserio
{

/**
 * Base class for all reflectable implementations.
 *
 * Implements the find() feature and overrides all generic methods except of write() and bitSizeOf() with
 * default throw behavior.
 */
template <typename ALLOC>
class ReflectableBase : public IBasicReflectable<ALLOC>
{
public:
    /**
     * Constructor.
     *
     * \param typeInfo Type info of the reflected object.
     */
    explicit ReflectableBase(const IBasicTypeInfo<ALLOC>& typeInfo);

    /** Destructor. */
    ~ReflectableBase() override = 0;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    ReflectableBase(const ReflectableBase&) = delete;
    ReflectableBase& operator=(const ReflectableBase&) = delete;

    ReflectableBase(const ReflectableBase&&) = delete;
    ReflectableBase& operator=(const ReflectableBase&&) = delete;
    /**
     * \}
     */

    const IBasicTypeInfo<ALLOC>& getTypeInfo() const override;
    bool isArray() const override;

    IBasicReflectableConstPtr<ALLOC> getField(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> getField(std::string_view name) override;
    IBasicReflectablePtr<ALLOC> createField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;

    size_t size() const override;
    void resize(size_t size) override;
    IBasicReflectableConstPtr<ALLOC> at(size_t index) const override;
    IBasicReflectablePtr<ALLOC> at(size_t index) override;
    IBasicReflectableConstPtr<ALLOC> operator[](size_t index) const override;
    IBasicReflectablePtr<ALLOC> operator[](size_t index) override;
    void setAt(const BasicAny<ALLOC>& value, size_t index) override;
    void append(const BasicAny<ALLOC>& value) override;

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override;
    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override;
    BasicAny<ALLOC> getAnyValue() const override;
    BasicAny<ALLOC> getAnyValue() override;

    // exact checked getters
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

    // convenience conversions
    int64_t toInt() const override;
    uint64_t toUInt() const override;
    double toDouble() const override;
    BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const override;
    BasicString<RebindAlloc<ALLOC, char>> toString() const override;

private:
    const IBasicTypeInfo<ALLOC>& m_typeInfo;
};

/**
 * Base class for numeric (arithmetic) types, string view and span.
 *
 * Hold the value.
 */
template <typename T, typename ALLOC>
class BuiltinReflectableBase : public ReflectableBase<ALLOC>
{
private:
    using Base = ReflectableBase<ALLOC>;

protected:
    using Type = T;

    BuiltinReflectableBase(const IBasicTypeInfo<ALLOC>& typeInfo, T value) :
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
 * Implements toString() and toDouble() conversions, implements write() for all integral builtin types.
 *
 * Hold dynamic bit size even though it has sense only for dynamic bit fields (otherwise it's always set to 0).
 * This solution was chosen for simplicity.
 */
template <typename T, typename ALLOC>
class IntegralReflectableBase : public BuiltinReflectableBase<T, ALLOC>
{
protected:
    static_assert(std::is_integral_v<typename T::ValueType>, "T must be a signed integral type!");

    using Base = BuiltinReflectableBase<T, ALLOC>;

public:
    IntegralReflectableBase(const IBasicTypeInfo<ALLOC>& typeInfo, T value) :
            Base(typeInfo, value)
    {}

    double toDouble() const override
    {
        return static_cast<double>(Base::getValue());
    }

    BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const override
    {
        return ::zserio::toString<ALLOC>(Base::getValue(), allocator);
    }
};

/**
 * Base class for signed integral reflectables.
 *
 * Implements toInt() conversion.
 */
template <typename T, typename ALLOC>
class SignedReflectableBase : public IntegralReflectableBase<T, ALLOC>
{
protected:
    static_assert(std::is_signed_v<typename T::ValueType>, "T must be a signed integral type!");

    using Base = IntegralReflectableBase<T, ALLOC>;

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
class UnsignedReflectableBase : public IntegralReflectableBase<T, ALLOC>
{
protected:
    static_assert(std::is_unsigned<typename T::ValueType>::value, "T must be an unsigned integral type!");

    using Base = IntegralReflectableBase<T, ALLOC>;

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
class BoolReflectable : public UnsignedReflectableBase<Bool, ALLOC>
{
private:
    using Base = UnsignedReflectableBase<Bool, ALLOC>;

public:
    explicit BoolReflectable(Bool value) :
            Base(typeInfo<zserio::Bool>(), value)
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
class Int8Reflectable : public SignedReflectableBase<T, ALLOC>
{
private:
    using Base = SignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<int8_t, typename T::ValueType>, "T must be based on int8_t!");

public:
    explicit Int8Reflectable(T value) :
            Base(typeInfo<T>(), value)
    {}

    int8_t getInt8() const override
    {
        return Base::getValue();
    }
};

template <typename T, typename ALLOC>
class Int16Reflectable : public SignedReflectableBase<T, ALLOC>
{
private:
    using Base = SignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<int16_t, typename T::ValueType>, "T must be based on int16_t!");

public:
    explicit Int16Reflectable(T value) :
            Base(typeInfo<T>(), value)
    {}

    int16_t getInt16() const override
    {
        return Base::getValue();
    }
};

template <typename T, typename ALLOC>
class Int32Reflectable : public SignedReflectableBase<T, ALLOC>
{
private:
    using Base = SignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<int32_t, typename T::ValueType>, "T must be based on int32_t!");

public:
    explicit Int32Reflectable(T value) :
            Base(typeInfo<T>(), value)
    {}

    int32_t getInt32() const override
    {
        return Base::getValue();
    }
};

template <typename T, typename ALLOC>
class Int64Reflectable : public SignedReflectableBase<T, ALLOC>
{
private:
    using Base = SignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<int64_t, typename T::ValueType>, "T must be based on int64_t!");

public:
    explicit Int64Reflectable(T value) :
            Base(typeInfo<T>(), value)
    {}

    int64_t getInt64() const override
    {
        return Base::getValue();
    }
};

template <typename T, typename ALLOC>
class UInt8Reflectable : public UnsignedReflectableBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<uint8_t, typename T::ValueType>, "T must be based on uint8_t!");

public:
    explicit UInt8Reflectable(T value) :
            Base(typeInfo<T>(), value)
    {}

    uint8_t getUInt8() const override
    {
        return Base::getValue();
    }
};

template <typename T, typename ALLOC>
class UInt16Reflectable : public UnsignedReflectableBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<uint16_t, typename T::ValueType>, "T must be based on uint16_t!");

public:
    explicit UInt16Reflectable(T value) :
            Base(typeInfo<T>(), value)
    {}

    uint16_t getUInt16() const override
    {
        return Base::getValue();
    }
};

template <typename T, typename ALLOC>
class UInt32Reflectable : public UnsignedReflectableBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<uint32_t, typename T::ValueType>, "T must be based on uint32_t!");

public:
    explicit UInt32Reflectable(T value) :
            Base(typeInfo<T>(), value)
    {}

    uint32_t getUInt32() const override
    {
        return Base::getValue();
    }
};

template <typename T, typename ALLOC>
class UInt64Reflectable : public UnsignedReflectableBase<T, ALLOC>
{
private:
    using Base = UnsignedReflectableBase<T, ALLOC>;

    static_assert(std::is_same_v<uint64_t, typename T::ValueType>, "T must be based on uint64_t!");

public:
    explicit UInt64Reflectable(T value) :
            Base(typeInfo<T>(), value)
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
class FloatingPointReflectableBase : public BuiltinReflectableBase<T, ALLOC>
{
protected:
    static_assert(std::is_floating_point_v<typename T::ValueType>, "T must be a floating point type!");

    using Base = BuiltinReflectableBase<T, ALLOC>;
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
class FloatReflectable : public FloatingPointReflectableBase<T, ALLOC>
{
private:
    using Base = FloatingPointReflectableBase<T, ALLOC>;

public:
    explicit FloatReflectable(T value) :
            Base(typeInfo<T>(), value)
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
class DoubleReflectable : public FloatingPointReflectableBase<Float64, ALLOC>
{
private:
    using Base = FloatingPointReflectableBase<Float64, ALLOC>;

public:
    explicit DoubleReflectable(Float64 value) :
            Base(typeInfo<Float64>(), value)
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
class BytesReflectable : public BuiltinReflectableBase<BytesView, ALLOC>
{
private:
    using Base = BuiltinReflectableBase<BytesView, ALLOC>;

public:
    explicit BytesReflectable(BytesView value) :
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
class StringReflectable : public BuiltinReflectableBase<std::string_view, ALLOC>
{
private:
    using Base = BuiltinReflectableBase<std::string_view, ALLOC>;

public:
    explicit StringReflectable(std::string_view value) :
            Base(typeInfo<BasicString<ALLOC>>(), value)
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
class BitBufferReflectable : public BuiltinReflectableBase<std::reference_wrapper<const BitBuffer>, ALLOC>
{
private:
    using Base = BuiltinReflectableBase<std::reference_wrapper<const BitBuffer>, ALLOC>;

public:
    explicit BitBufferReflectable(std::reference_wrapper<const BitBuffer> value) :
            Base(typeInfo<BasicBitBuffer<ALLOC>>(), value)
    {}

    const BasicBitBuffer<ALLOC>& getBitBuffer() const override
    {
        return Base::getValue();
    }
};

namespace detail
{

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> getFieldFromObject(
        const IBasicReflectable<ALLOC>& object, std::string_view name)
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
IBasicReflectablePtr<ALLOC> getFieldFromObject(IBasicReflectable<ALLOC>& object, std::string_view name)
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
IBasicReflectableConstPtr<ALLOC> getParameterFromObject(
        const IBasicReflectable<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& parameters = typeInfo.getParameters();
        auto parametersIt = std::find_if(
                parameters.begin(), parameters.end(), [name](const BasicParameterInfo<ALLOC>& parameterInfo) {
                    return parameterInfo.schemaName == name;
                });
        if (parametersIt != parameters.end())
        {
            return object.getParameter(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> getParameterFromObject(IBasicReflectable<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& parameters = typeInfo.getParameters();
        auto parametersIt = std::find_if(
                parameters.begin(), parameters.end(), [name](const BasicParameterInfo<ALLOC>& parameterInfo) {
                    return parameterInfo.schemaName == name;
                });
        if (parametersIt != parameters.end())
        {
            return object.getParameter(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> callFunctionInObject(
        const IBasicReflectable<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& functions = typeInfo.getFunctions();
        auto functionsIt = std::find_if(
                functions.begin(), functions.end(), [name](const BasicFunctionInfo<ALLOC>& functionInfo) {
                    return functionInfo.schemaName == name;
                });
        if (functionsIt != functions.end())
        {
            return object.callFunction(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> callFunctionInObject(IBasicReflectable<ALLOC>& object, std::string_view name)
{
    const auto& typeInfo = object.getTypeInfo();
    if (TypeInfoUtil::isCompound(typeInfo.getSchemaType()))
    {
        const auto& functions = typeInfo.getFunctions();
        auto functionsIt = std::find_if(
                functions.begin(), functions.end(), [name](const BasicFunctionInfo<ALLOC>& functionInfo) {
                    return functionInfo.schemaName == name;
                });
        if (functionsIt != functions.end())
        {
            return object.callFunction(name);
        }
    }

    return nullptr;
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> getFromObject(
        const IBasicReflectable<ALLOC>& object, std::string_view path, size_t pos)
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

        auto parameter = getParameterFromObject(object, name);
        if (parameter)
        {
            return isLast ? std::move(parameter) : getFromObject(*parameter, path, dotPos + 1);
        }

        auto functionResult = callFunctionInObject(object, name);
        if (functionResult)
        {
            return isLast ? std::move(functionResult) : getFromObject(*functionResult, path, dotPos + 1);
        }
    }
    catch (const CppRuntimeException&)
    {}

    return nullptr;
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> getFromObject(IBasicReflectable<ALLOC>& object, std::string_view path, size_t pos)
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

        auto parameter = getParameterFromObject(object, name);
        if (parameter)
        {
            return isLast ? std::move(parameter) : getFromObject(*parameter, path, dotPos + 1);
        }

        auto functionResult = callFunctionInObject(object, name);
        if (functionResult)
        {
            return isLast ? std::move(functionResult) : getFromObject(*functionResult, path, dotPos + 1);
        }
    }
    catch (const CppRuntimeException&)
    {}

    return nullptr;
}

} // namespace detail

/**
 * Base class for reflectable which needs to hold an allocator.
 */
template <typename ALLOC>
class ReflectableAllocatorHolderBase : public ReflectableBase<ALLOC>, public AllocatorHolder<ALLOC>
{
public:
    ReflectableAllocatorHolderBase(const IBasicTypeInfo<ALLOC>& typeInfo, const ALLOC& allocator) :
            ReflectableBase<ALLOC>(typeInfo),
            AllocatorHolder<ALLOC>(allocator)
    {}
};

/**
 * Base class for constant reflectable which needs to hold an allocator.
 *
 * Overrides non constant methods and throws exception with info about constness.
 */
template <typename ALLOC>
class ReflectableConstAllocatorHolderBase : public ReflectableAllocatorHolderBase<ALLOC>
{
private:
    using Base = ReflectableAllocatorHolderBase<ALLOC>;

public:
    using Base::Base;
    using Base::getTypeInfo;

    IBasicReflectablePtr<ALLOC> getField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;
    IBasicReflectablePtr<ALLOC> getParameter(std::string_view name) override;
    IBasicReflectablePtr<ALLOC> callFunction(std::string_view name) override;

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override;
};

/**
 * Base class for reflectable arrays.
 *
 * Overrides all generic methods with default throw behaviour.
 */
template <typename ALLOC>
class ReflectableArrayBase : public ReflectableAllocatorHolderBase<ALLOC>
{
private:
    using Base = ReflectableAllocatorHolderBase<ALLOC>;

public:
    using Base::Base;
    using Base::getTypeInfo;

    bool isArray() const override
    {
        return true;
    }

    IBasicReflectableConstPtr<ALLOC> getField(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> getField(std::string_view name) override;
    IBasicReflectablePtr<ALLOC> createField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;

    IBasicReflectableConstPtr<ALLOC> operator[](size_t index) const override;
    IBasicReflectablePtr<ALLOC> operator[](size_t index) override;

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
class ReflectableConstArrayBase : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

public:
    using Base::Base;
    using Base::getTypeInfo;

    void resize(size_t index) override;
    IBasicReflectablePtr<ALLOC> at(size_t index) override;
    IBasicReflectablePtr<ALLOC> operator[](size_t index) override;
    void setAt(const BasicAny<ALLOC>& value, size_t index) override;
    void append(const BasicAny<ALLOC>& value) override;

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) override;
};

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(Bool value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<BoolReflectable<ALLOC>>(allocator, value);
}

template <typename T, BitSize BIT_SIZE, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(detail::IntWrapper<T, BIT_SIZE> value, const ALLOC& allocator = ALLOC())
{
    using Type = detail::IntWrapper<T, BIT_SIZE>;

    if constexpr (std::is_signed_v<T>)
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<Int64Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<Int32Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<Int16Reflectable<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<Int8Reflectable<Type, ALLOC>>(allocator, value);
        }
    }
    else
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<UInt64Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<UInt32Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<UInt16Reflectable<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<UInt8Reflectable<Type, ALLOC>>(allocator, value);
        }
    }
}

template <typename T, BitSize BIT_SIZE, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(
        detail::DynIntWrapper<T, BIT_SIZE> value, const ALLOC& allocator = ALLOC())
{
    using Type = detail::DynIntWrapper<T, BIT_SIZE>;

    if constexpr (std::is_signed_v<T>)
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<Int64Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<Int32Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<Int16Reflectable<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<Int8Reflectable<Type, ALLOC>>(allocator, value);
        }
    }
    else
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<UInt64Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<UInt32Reflectable<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<UInt16Reflectable<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<UInt8Reflectable<Type, ALLOC>>(allocator, value);
        }
    }
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarInt16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int16Reflectable<VarInt16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarInt32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int32Reflectable<VarInt32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarInt64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int64Reflectable<VarInt64, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarInt value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int64Reflectable<VarInt, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarUInt16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt16Reflectable<VarUInt16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarUInt32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt32Reflectable<VarUInt32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarUInt64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt64Reflectable<VarUInt64, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarUInt value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt64Reflectable<VarUInt, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(VarSize value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt32Reflectable<VarSize, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(Float16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<FloatReflectable<Float16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(Float32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<FloatReflectable<Float32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(Float64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<DoubleReflectable<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(const BasicBytes<ALLOC>& value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<BytesReflectable<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(BytesView value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<BytesReflectable<ALLOC>>(allocator, value);
}

template <typename STRING_ALLOC, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(
        const BasicString<STRING_ALLOC>& value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<StringReflectable<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(std::string_view value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<StringReflectable<ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(const BasicBitBuffer<ALLOC>& value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<BitBufferReflectable<ALLOC>>(allocator, value);
}

/**
 * Reflectable for arrays of builtin types (except dynamic bit field arrays).
 */
/** \{ */
template <typename RAW_ARRAY, typename ALLOC>
class ReflectableConstArray : public ReflectableConstArrayBase<ALLOC>
{
private:
    using Base = ReflectableConstArrayBase<ALLOC>;

public:
    using Base::at;
    using Base::getTypeInfo;
    using Base::operator[];
    using Base::getAnyValue;

    ReflectableConstArray(const ALLOC& allocator, const RAW_ARRAY& rawArray) :
            Base(typeInfo<typename RAW_ARRAY::value_type>(), allocator),
            m_rawArray(rawArray)
    {}

    size_t size() const override
    {
        return m_rawArray.size();
    }

    IBasicReflectableConstPtr<ALLOC> at(size_t index) const override
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
class ReflectableArray : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

public:
    using Base::getTypeInfo;

    ReflectableArray(const ALLOC& allocator, RAW_ARRAY& rawArray) :
            Base(typeInfo<typename RAW_ARRAY::value_type>(), allocator),
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

    IBasicReflectableConstPtr<ALLOC> at(size_t index) const override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return reflectable(m_rawArray[index], Base::get_allocator());
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
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

        using ElementType = typename RAW_ARRAY::value_type;

        if constexpr (is_bitmask_v<ElementType>)
        {
            using ElementZserioType = typename ElementType::ZserioType;
            using ElementUnderlyingType = typename ElementZserioType::ValueType;

            if (value.template isType<ElementType>())
            {
                m_rawArray[index] = value.template get<ElementType>();
            }
            else if (value.template isType<ElementZserioType>())
            {
                m_rawArray[index] = value.template get<ElementZserioType>();
            }
            else
            {
                m_rawArray[index] = valueToEnum<ElementType>(value.template get<ElementUnderlyingType>());
            }
        }
        else if constexpr (std::is_enum_v<ElementType>)
        {
            using ElementZserioType = typename EnumTraits<ElementType>::ZserioType;
            using ElementUnderlyingType = std::underlying_type_t<ElementType>;

            if (value.template isType<ElementType>())
            {
                m_rawArray[index] = value.template get<ElementType>();
            }
            else if (value.template isType<ElementZserioType>())
            {
                m_rawArray[index] = valueToEnum<ElementType>(value.template get<ElementZserioType>());
            }
            else
            {
                m_rawArray[index] = valueToEnum<ElementType>(value.template get<ElementUnderlyingType>());
            }
        }
        else
        {
            m_rawArray[index] = value.template get<ElementType>();
        }
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        using ElementType = typename RAW_ARRAY::value_type;

        if constexpr (is_bitmask_v<ElementType>)
        {
            using ElementZserioType = typename ElementType::ZserioType;
            using ElementUnderlyingType = typename ElementZserioType::ValueType;

            if (value.template isType<ElementType>())
            {
                m_rawArray.push_back(value.template get<ElementType>());
            }
            else if (value.template isType<ElementZserioType>())
            {
                m_rawArray.push_back(value.template get<ElementZserioType>());
            }
            else
            {
                m_rawArray.push_back(valueToEnum<ElementType>(value.template get<ElementUnderlyingType>()));
            }
        }
        else if constexpr (std::is_enum_v<ElementType>)
        {
            using ElementZserioType = typename EnumTraits<ElementType>::ZserioType;
            using ElementUnderlyingType = std::underlying_type_t<ElementType>;

            if (value.template isType<ElementType>())
            {
                m_rawArray.push_back(value.template get<ElementType>());
            }
            else if (value.template isType<ElementZserioType>())
            {
                m_rawArray.push_back(valueToEnum<ElementType>(value.template get<ElementZserioType>()));
            }
            else
            {
                m_rawArray.push_back(valueToEnum<ElementType>(value.template get<ElementUnderlyingType>()));
            }
        }
        else
        {
            m_rawArray.push_back(value.template get<ElementType>());
        }
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

template <typename T, typename VECTOR_ALLOC, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectableConstPtr<ALLOC> reflectableArray(
        const std::vector<T, VECTOR_ALLOC>& array, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<ReflectableConstArray<std::vector<T, VECTOR_ALLOC>, ALLOC>>(
            allocator, allocator, array);
}

template <typename T, typename VECTOR_ALLOC, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectableArray(
        std::vector<T, VECTOR_ALLOC>& array, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<ReflectableArray<std::vector<T, VECTOR_ALLOC>, ALLOC>>(
            allocator, allocator, array);
}

// implementation of base classes methods

template <typename ALLOC>
ReflectableBase<ALLOC>::ReflectableBase(const IBasicTypeInfo<ALLOC>& typeInfo) :
        m_typeInfo(typeInfo)
{}

template <typename ALLOC>
ReflectableBase<ALLOC>::~ReflectableBase() = default;

template <typename ALLOC>
const IBasicTypeInfo<ALLOC>& ReflectableBase<ALLOC>::getTypeInfo() const
{
    return m_typeInfo;
}

template <typename ALLOC>
bool ReflectableBase<ALLOC>::isArray() const
{
    return false;
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableBase<ALLOC>::getField(std::string_view) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no fields to get!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::getField(std::string_view)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no fields to get!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::createField(std::string_view)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no fields to create!";
}

template <typename ALLOC>
void ReflectableBase<ALLOC>::setField(std::string_view, const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no fields to set!";
}

template <typename ALLOC>
size_t ReflectableBase<ALLOC>::size() const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
void ReflectableBase<ALLOC>::resize(size_t)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableBase<ALLOC>::at(size_t) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::at(size_t)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableBase<ALLOC>::operator[](size_t) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::operator[](size_t)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
void ReflectableBase<ALLOC>::setAt(const BasicAny<ALLOC>&, size_t)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
void ReflectableBase<ALLOC>::append(const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not an array!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableBase<ALLOC>::getAnyValue(const ALLOC&) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableBase<ALLOC>::getAnyValue(const ALLOC&)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableBase<ALLOC>::getAnyValue() const
{
    return getAnyValue(ALLOC());
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableBase<ALLOC>::getAnyValue()
{
    return getAnyValue(ALLOC());
}

template <typename ALLOC>
bool ReflectableBase<ALLOC>::getBool() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not boolean type!";
}

template <typename ALLOC>
int8_t ReflectableBase<ALLOC>::getInt8() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not int8 type!";
}

template <typename ALLOC>
int16_t ReflectableBase<ALLOC>::getInt16() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not int16 type!";
}

template <typename ALLOC>
int32_t ReflectableBase<ALLOC>::getInt32() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not int32 type!";
}

template <typename ALLOC>
int64_t ReflectableBase<ALLOC>::getInt64() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not int64 type!";
}

template <typename ALLOC>
uint8_t ReflectableBase<ALLOC>::getUInt8() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not uint8 type!";
}

template <typename ALLOC>
uint16_t ReflectableBase<ALLOC>::getUInt16() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not uint16 type!";
}

template <typename ALLOC>
uint32_t ReflectableBase<ALLOC>::getUInt32() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not uint32 type!";
}

template <typename ALLOC>
uint64_t ReflectableBase<ALLOC>::getUInt64() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not uint64 type!";
}

template <typename ALLOC>
float ReflectableBase<ALLOC>::getFloat() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not float type!";
}

template <typename ALLOC>
double ReflectableBase<ALLOC>::getDouble() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not double type!";
}

template <typename ALLOC>
BytesView ReflectableBase<ALLOC>::getBytes() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not bytes type!";
}

template <typename ALLOC>
std::string_view ReflectableBase<ALLOC>::getStringView() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not string type!";
}

template <typename ALLOC>
const BasicBitBuffer<ALLOC>& ReflectableBase<ALLOC>::getBitBuffer() const
{
    throw CppRuntimeException("'") << getTypeInfo().getSchemaName() << "' is not an extern type!";
}

template <typename ALLOC>
int64_t ReflectableBase<ALLOC>::toInt() const
{
    throw CppRuntimeException("Conversion from '")
            << getTypeInfo().getSchemaName() << "' to signed integer is not available!";
}

template <typename ALLOC>
uint64_t ReflectableBase<ALLOC>::toUInt() const
{
    throw CppRuntimeException("Conversion from '")
            << getTypeInfo().getSchemaName() << "' to unsigned integer is not available!";
}

template <typename ALLOC>
double ReflectableBase<ALLOC>::toDouble() const
{
    throw CppRuntimeException("Conversion from '")
            << getTypeInfo().getSchemaName() << "' to double is not available!";
}

template <typename ALLOC>
BasicString<RebindAlloc<ALLOC, char>> ReflectableBase<ALLOC>::toString(const ALLOC&) const
{
    throw CppRuntimeException("Conversion from '")
            << getTypeInfo().getSchemaName() << "' to string is not available!";
}

template <typename ALLOC>
BasicString<RebindAlloc<ALLOC, char>> ReflectableBase<ALLOC>::toString() const
{
    return toString(ALLOC());
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableConstAllocatorHolderBase<ALLOC>::getField(std::string_view)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
void ReflectableConstAllocatorHolderBase<ALLOC>::setField(std::string_view, const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableConstAllocatorHolderBase<ALLOC>::getParameter(std::string_view)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableConstAllocatorHolderBase<ALLOC>::callFunction(std::string_view)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableConstAllocatorHolderBase<ALLOC>::getAnyValue(const ALLOC&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is constant!";
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableArrayBase<ALLOC>::getField(std::string_view) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableArrayBase<ALLOC>::getField(std::string_view)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableArrayBase<ALLOC>::createField(std::string_view)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
void ReflectableArrayBase<ALLOC>::setField(std::string_view, const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableArrayBase<ALLOC>::operator[](size_t index) const
{
    return this->at(index);
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableArrayBase<ALLOC>::operator[](size_t index)
{
    return this->at(index);
}

template <typename ALLOC>
bool ReflectableArrayBase<ALLOC>::getBool() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int8_t ReflectableArrayBase<ALLOC>::getInt8() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int16_t ReflectableArrayBase<ALLOC>::getInt16() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int32_t ReflectableArrayBase<ALLOC>::getInt32() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int64_t ReflectableArrayBase<ALLOC>::getInt64() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint8_t ReflectableArrayBase<ALLOC>::getUInt8() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint16_t ReflectableArrayBase<ALLOC>::getUInt16() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint32_t ReflectableArrayBase<ALLOC>::getUInt32() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint64_t ReflectableArrayBase<ALLOC>::getUInt64() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
float ReflectableArrayBase<ALLOC>::getFloat() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
double ReflectableArrayBase<ALLOC>::getDouble() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
BytesView ReflectableArrayBase<ALLOC>::getBytes() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
std::string_view ReflectableArrayBase<ALLOC>::getStringView() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
const BasicBitBuffer<ALLOC>& ReflectableArrayBase<ALLOC>::getBitBuffer() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
int64_t ReflectableArrayBase<ALLOC>::toInt() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
uint64_t ReflectableArrayBase<ALLOC>::toUInt() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
double ReflectableArrayBase<ALLOC>::toDouble() const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
BasicString<RebindAlloc<ALLOC, char>> ReflectableArrayBase<ALLOC>::toString(const ALLOC&) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
void ReflectableConstArrayBase<ALLOC>::resize(size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableConstArrayBase<ALLOC>::at(size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableConstArrayBase<ALLOC>::operator[](size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
void ReflectableConstArrayBase<ALLOC>::setAt(const BasicAny<ALLOC>&, size_t)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
void ReflectableConstArrayBase<ALLOC>::append(const BasicAny<ALLOC>&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

template <typename ALLOC>
BasicAny<ALLOC> ReflectableConstArrayBase<ALLOC>::getAnyValue(const ALLOC&)
{
    throw CppRuntimeException("Reflectable '") << getTypeInfo().getSchemaName() << "' is a constant array!";
}

} // namespace zserio

#endif // ZSERIO_REFLECTABLE_H_INC
