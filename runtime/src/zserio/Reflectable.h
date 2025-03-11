#ifndef ZSERIO_REFLECTABLE_H_INC
#define ZSERIO_REFLECTABLE_H_INC

#include <functional>
#include <type_traits>

#include "zserio/AllocatorHolder.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/IReflectable.h"
#include "zserio/Span.h"
#include "zserio/StringConvertUtil.h"
#include "zserio/Traits.h"
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

    size_t initializeOffsets(size_t bitPosition) override;
    size_t initializeOffsets() override;
    size_t bitSizeOf(size_t bitPosition) const override;
    size_t bitSizeOf() const override;
    void write(BitStreamWriter& writer) const override;

    IBasicReflectableConstPtr<ALLOC> getField(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> getField(std::string_view name) override;
    IBasicReflectablePtr<ALLOC> createField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;
    IBasicReflectableConstPtr<ALLOC> getParameter(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> getParameter(std::string_view name) override;
    IBasicReflectableConstPtr<ALLOC> callFunction(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> callFunction(std::string_view name) override;

    std::string_view getChoice() const override;

    IBasicReflectableConstPtr<ALLOC> find(std::string_view path) const override;
    IBasicReflectablePtr<ALLOC> find(std::string_view path) override;
    IBasicReflectableConstPtr<ALLOC> operator[](std::string_view path) const override;
    IBasicReflectablePtr<ALLOC> operator[](std::string_view path) override;

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
    size_t bitSizeOf(size_t) const override
    {
        return zserio::detail::bitSizeOf(m_value);
    }

    void write(BitStreamWriter& writer) const override
    {
        zserio::detail::write(writer, m_value);
    }

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

template <typename INT_REFLECTABLE>
class DynIntReflectable : public INT_REFLECTABLE
{
private:
    using Base = INT_REFLECTABLE;
    using Type = typename Base::Type;

public:
    DynIntReflectable(Type value, uint8_t dynamicBitSize) :
            Base(value),
            m_dynamicBitSize(dynamicBitSize)
    {}

    size_t bitSizeOf(size_t) const override
    {
        return m_dynamicBitSize;
    }

    void write(BitStreamWriter& writer) const override
    {
        zserio::detail::write(writer, Base::getValue(), m_dynamicBitSize);
    }

private:
    uint8_t m_dynamicBitSize;
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

    size_t initializeOffsets(size_t bitPosition) override;
    size_t bitSizeOf(size_t bitPosition) const override;
    void write(BitStreamWriter& writer) const override;

    IBasicReflectableConstPtr<ALLOC> getField(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> getField(std::string_view name) override;
    IBasicReflectablePtr<ALLOC> createField(std::string_view name) override;
    void setField(std::string_view name, const BasicAny<ALLOC>& value) override;
    IBasicReflectableConstPtr<ALLOC> getParameter(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> getParameter(std::string_view name) override;
    IBasicReflectableConstPtr<ALLOC> callFunction(std::string_view name) const override;
    IBasicReflectablePtr<ALLOC> callFunction(std::string_view name) override;

    std::string_view getChoice() const override;

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

#if 0

template <typename ALLOC, typename RAW_ARRAY, typename ELEMENT_REFLECTABLE>
class BuiltinReflectableArray : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

    using ElementReflectable = ELEMENT_REFLECTABLE;

public:
    using Base::getTypeInfo;

    BuiltinReflectableArray(const ALLOC& allocator, RAW_ARRAY& rawArray) :
            Base(ElementReflectable::typeInfo(), allocator),
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

        return std::allocate_shared<ElementReflectable>(Base::get_allocator(), m_rawArray[index]);
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return std::allocate_shared<ElementReflectable>(Base::get_allocator(), m_rawArray[index]);
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        m_rawArray[index] = value.template get<typename RAW_ARRAY::value_type>();
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        m_rawArray.push_back(value.template get<typename RAW_ARRAY::value_type>());
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
/** \} */

/**
 * Reflectable for arrays of fixed bit fields.
 */
/** \{ */
template <typename ALLOC, typename RAW_ARRAY, typename ELEMENT_REFLECTABLE>
class FixedBitFieldReflectableConstArray : public ReflectableConstArrayBase<ALLOC>
{
private:
    using Base = ReflectableConstArrayBase<ALLOC>;

    using ElementReflectable = ELEMENT_REFLECTABLE;

public:
    using Base::at;
    using Base::getTypeInfo;
    using Base::operator[];
    using Base::getAnyValue;

    FixedBitFieldReflectableConstArray(const ALLOC& allocator, const RAW_ARRAY& rawArray, uint8_t bitSize) :
            Base(ElementReflectable::typeInfo(bitSize), allocator),
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

        return std::allocate_shared<ElementReflectable>(
                Base::get_allocator(), m_rawArray[index], getTypeInfo().getBitSize());
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

private:
    const RAW_ARRAY& m_rawArray;
};

template <typename ALLOC, typename RAW_ARRAY, typename ELEMENT_REFLECTABLE>
class FixedBitFieldReflectableArray : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

    using ElementReflectable = ELEMENT_REFLECTABLE;

public:
    using Base::getTypeInfo;

    FixedBitFieldReflectableArray(const ALLOC& allocator, RAW_ARRAY& rawArray, uint8_t bitSize) :
            Base(ElementReflectable::typeInfo(bitSize), allocator),
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

        return std::allocate_shared<ElementReflectable>(
                Base::get_allocator(), m_rawArray[index], getTypeInfo().getBitSize());
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return std::allocate_shared<ElementReflectable>(
                Base::get_allocator(), m_rawArray[index], getTypeInfo().getBitSize());
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        m_rawArray[index] = value.template get<typename RAW_ARRAY::value_type>();
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        m_rawArray.push_back(value.template get<typename RAW_ARRAY::value_type>());
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
/** \} */

/**
 * Reflectable for arrays of dynamic bit fields.
 */
/** \{ */
template <typename ALLOC, typename RAW_ARRAY, typename ELEMENT_REFLECTABLE>
class DynamicBitFieldReflectableConstArray : public ReflectableConstArrayBase<ALLOC>
{
private:
    using Base = ReflectableConstArrayBase<ALLOC>;

    using ElementReflectable = ELEMENT_REFLECTABLE;

public:
    using Base::at;
    using Base::getTypeInfo;
    using Base::operator[];
    using Base::getAnyValue;

    DynamicBitFieldReflectableConstArray(
            const ALLOC& allocator, const RAW_ARRAY& rawArray, uint8_t maxBitSize, uint8_t dynamicBitSize) :
            Base(ElementReflectable::typeInfo(maxBitSize), allocator),
            m_rawArray(rawArray),
            m_maxBitSize(maxBitSize),
            m_dynamicBitSize(dynamicBitSize)
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

        return std::allocate_shared<ElementReflectable>(
                Base::get_allocator(), m_rawArray[index], m_maxBitSize, m_dynamicBitSize);
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

private:
    const RAW_ARRAY& m_rawArray;
    const uint8_t m_maxBitSize;
    const uint8_t m_dynamicBitSize;
};

template <typename ALLOC, typename RAW_ARRAY, typename ELEMENT_REFLECTABLE>
class DynamicBitFieldReflectableArray : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

    using ElementReflectable = ELEMENT_REFLECTABLE;

public:
    using Base::getTypeInfo;

    DynamicBitFieldReflectableArray(
            const ALLOC& allocator, RAW_ARRAY& rawArray, uint8_t maxBitSize, uint8_t dynamicBitSize) :
            Base(ElementReflectable::typeInfo(maxBitSize), allocator),
            m_rawArray(rawArray),
            m_maxBitSize(maxBitSize),
            m_dynamicBitSize(dynamicBitSize)
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

        return std::allocate_shared<ElementReflectable>(
                Base::get_allocator(), m_rawArray[index], m_maxBitSize, m_dynamicBitSize);
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return std::allocate_shared<ElementReflectable>(
                Base::get_allocator(), m_rawArray[index], m_maxBitSize, m_dynamicBitSize);
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        m_rawArray[index] = value.template get<typename RAW_ARRAY::value_type>();
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        m_rawArray.push_back(value.template get<typename RAW_ARRAY::value_type>());
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
    const uint8_t m_maxBitSize;
    const uint8_t m_dynamicBitSize;
};
/** \} */

/**
 * Typedef to a builtin array.
 */
/** \{ */
template <typename ALLOC, typename RAW_ARRAY>
using BoolReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, BoolReflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using BoolReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, BoolReflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using Int8ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, Int8Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using Int8ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, Int8Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using Int16ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, Int16Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using Int16ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, Int16Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using Int32ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, Int32Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using Int32ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, Int32Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using Int64ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, Int64Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using Int64ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, Int64Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using UInt8ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, UInt8Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using UInt8ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, UInt8Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using UInt16ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, UInt16Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using UInt16ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, UInt16Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using UInt32ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, UInt32Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using UInt32ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, UInt32Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using UInt64ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, UInt64Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using UInt64ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, UInt64Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using FixedSignedBitFieldReflectableConstArray = FixedBitFieldReflectableConstArray<ALLOC, RAW_ARRAY,
        FixedSignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;
template <typename ALLOC, typename RAW_ARRAY>
using FixedSignedBitFieldReflectableArray = FixedBitFieldReflectableArray<ALLOC, RAW_ARRAY,
        FixedSignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;

template <typename ALLOC, typename RAW_ARRAY>
using FixedUnsignedBitFieldReflectableConstArray = FixedBitFieldReflectableConstArray<ALLOC, RAW_ARRAY,
        FixedUnsignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;
template <typename ALLOC, typename RAW_ARRAY>
using FixedUnsignedBitFieldReflectableArray = FixedBitFieldReflectableArray<ALLOC, RAW_ARRAY,
        FixedUnsignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;

template <typename ALLOC, typename RAW_ARRAY>
using DynamicSignedBitFieldReflectableConstArray = DynamicBitFieldReflectableConstArray<ALLOC, RAW_ARRAY,
        DynamicSignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;
template <typename ALLOC, typename RAW_ARRAY>
using DynamicSignedBitFieldReflectableArray = DynamicBitFieldReflectableArray<ALLOC, RAW_ARRAY,
        DynamicSignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;

template <typename ALLOC, typename RAW_ARRAY>
using DynamicUnsignedBitFieldReflectableConstArray = DynamicBitFieldReflectableConstArray<ALLOC, RAW_ARRAY,
        DynamicUnsignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;
template <typename ALLOC, typename RAW_ARRAY>
using DynamicUnsignedBitFieldReflectableArray = DynamicBitFieldReflectableArray<ALLOC, RAW_ARRAY,
        DynamicUnsignedBitFieldReflectable<ALLOC, typename RAW_ARRAY::value_type>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarInt16ReflectableConstArray =
        BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarInt16Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarInt16ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarInt16Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarInt32ReflectableConstArray =
        BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarInt32Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarInt32ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarInt32Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarInt64ReflectableConstArray =
        BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarInt64Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarInt64ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarInt64Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarIntReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarIntReflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarIntReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarIntReflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarUInt16ReflectableConstArray =
        BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarUInt16Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarUInt16ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarUInt16Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarUInt32ReflectableConstArray =
        BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarUInt32Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarUInt32ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarUInt32Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarUInt64ReflectableConstArray =
        BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarUInt64Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarUInt64ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarUInt64Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarUIntReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarUIntReflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarUIntReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarUIntReflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using VarSizeReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, VarSizeReflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using VarSizeReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, VarSizeReflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using Float16ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, Float16Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using Float16ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, Float16Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using Float32ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, Float32Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using Float32ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, Float32Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using Float64ReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, Float64Reflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using Float64ReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, Float64Reflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using BytesReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, BytesReflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using BytesReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, BytesReflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using StringReflectableConstArray = BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, StringReflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using StringReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, StringReflectable<ALLOC>>;

template <typename ALLOC, typename RAW_ARRAY>
using BitBufferReflectableConstArray =
        BuiltinReflectableConstArray<ALLOC, RAW_ARRAY, BitBufferReflectable<ALLOC>>;
template <typename ALLOC, typename RAW_ARRAY>
using BitBufferReflectableArray = BuiltinReflectableArray<ALLOC, RAW_ARRAY, BitBufferReflectable<ALLOC>>;
/** \} */

/**
 * Reflectable for arrays of compound types.
 */
/** \{ */
template <typename ALLOC, typename RAW_ARRAY>
class CompoundReflectableConstArray : public ReflectableConstArrayBase<ALLOC>
{
private:
    using Base = ReflectableConstArrayBase<ALLOC>;

    using ElementType = typename RAW_ARRAY::value_type;

public:
    using Base::at;
    using Base::getTypeInfo;
    using Base::operator[];
    using Base::getAnyValue;

    CompoundReflectableConstArray(const ALLOC& allocator, const RAW_ARRAY& rawArray) :
            Base(ElementType::typeInfo(), allocator),
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

        return m_rawArray[index].reflectable(Base::get_allocator());
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

private:
    const RAW_ARRAY& m_rawArray;
};

template <typename ALLOC, typename RAW_ARRAY>
class CompoundReflectableArray : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

    using ElementType = typename RAW_ARRAY::value_type;

public:
    using Base::getTypeInfo;

    CompoundReflectableArray(const ALLOC& allocator, RAW_ARRAY& rawArray) :
            Base(ElementType::typeInfo(), allocator),
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

        return m_rawArray[index].reflectable(Base::get_allocator());
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return m_rawArray[index].reflectable(Base::get_allocator());
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        m_rawArray[index] = value.template get<typename RAW_ARRAY::value_type>();
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        m_rawArray.push_back(value.template get<typename RAW_ARRAY::value_type>());
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
/** \} */

/** Reflectable for arrays of bitmask types. */
/** \{ */
template <typename ALLOC, typename RAW_ARRAY>
class BitmaskReflectableConstArray : public ReflectableConstArrayBase<ALLOC>
{
private:
    using Base = ReflectableConstArrayBase<ALLOC>;

    using ElementType = typename RAW_ARRAY::value_type;

public:
    using Base::at;
    using Base::getTypeInfo;
    using Base::operator[];
    using Base::getAnyValue;

    BitmaskReflectableConstArray(const ALLOC& allocator, const RAW_ARRAY& rawArray) :
            Base(ElementType::typeInfo(), allocator),
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

        return m_rawArray[index].reflectable(Base::get_allocator());
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

private:
    const RAW_ARRAY& m_rawArray;
};

template <typename ALLOC, typename RAW_ARRAY>
class BitmaskReflectableArray : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

    using ElementType = typename RAW_ARRAY::value_type;
    using UnderlyingElementType = typename ElementType::underlying_type;

public:
    using Base::getTypeInfo;

    BitmaskReflectableArray(const ALLOC& allocator, RAW_ARRAY& rawArray) :
            Base(ElementType::typeInfo(), allocator),
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

        return m_rawArray[index].reflectable(Base::get_allocator());
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return m_rawArray[index].reflectable(Base::get_allocator());
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        if (value.template isType<ElementType>())
        {
            m_rawArray[index] = value.template get<ElementType>();
        }
        else
        {
            m_rawArray[index] = ElementType(value.template get<UnderlyingElementType>());
        }
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        if (value.template isType<ElementType>())
        {
            m_rawArray.push_back(value.template get<ElementType>());
        }
        else
        {
            m_rawArray.push_back(ElementType(value.template get<UnderlyingElementType>()));
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
/** \} */

/** Reflectable for arrays of enum types. */
/** \{ */
template <typename ALLOC, typename RAW_ARRAY>
class EnumReflectableConstArray : public ReflectableConstArrayBase<ALLOC>
{
private:
    using Base = ReflectableConstArrayBase<ALLOC>;

    using ElementType = typename RAW_ARRAY::value_type;

public:
    using Base::at;
    using Base::getTypeInfo;
    using Base::operator[];
    using Base::getAnyValue;

    EnumReflectableConstArray(const ALLOC& allocator, const RAW_ARRAY& rawArray) :
            Base(enumTypeInfo<ElementType, ALLOC>(), allocator),
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

        return enumReflectable(m_rawArray[index], Base::get_allocator());
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

private:
    const RAW_ARRAY& m_rawArray;
};

template <typename ALLOC, typename RAW_ARRAY>
class EnumReflectableArray : public ReflectableArrayBase<ALLOC>
{
private:
    using Base = ReflectableArrayBase<ALLOC>;

    using ElementType = typename RAW_ARRAY::value_type;
    using UnderlyingElementType = typename std::underlying_type<ElementType>::type;

public:
    using Base::getTypeInfo;

    EnumReflectableArray(const ALLOC& allocator, RAW_ARRAY& rawArray) :
            Base(enumTypeInfo<ElementType, ALLOC>(), allocator),
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

        return enumReflectable(m_rawArray[index], Base::get_allocator());
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return enumReflectable(m_rawArray[index], Base::get_allocator());
    }

    void setAt(const BasicAny<ALLOC>& value, size_t index) override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for reflectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        if (value.template isType<ElementType>())
        {
            m_rawArray[index] = value.template get<ElementType>();
        }
        else
        {
            m_rawArray[index] = valueToEnum<ElementType>(value.template get<UnderlyingElementType>());
        }
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        if (value.template isType<ElementType>())
        {
            m_rawArray.push_back(value.template get<ElementType>());
        }
        else
        {
            m_rawArray.push_back(valueToEnum<ElementType>(value.template get<UnderlyingElementType>()));
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
/** \} */

/**
 * Wrapper around reflectable which actually owns the reflected object.
 *
 * This is needed in ZserioTreeCreator to be able to generically create the new instance of a zserio object.
 */
template <typename T, typename ALLOC = typename T::allocator_type>
class ReflectableOwner : public IBasicReflectable<ALLOC>
{
public:
    ReflectableOwner() :
            ReflectableOwner(ALLOC())
    {}

    explicit ReflectableOwner(const ALLOC& allocator) :
            m_object(allocator),
            m_reflectable(m_object.reflectable(allocator))
    {}

    const IBasicTypeInfo<ALLOC>& getTypeInfo() const override
    {
        return m_reflectable->getTypeInfo();
    }

    bool isArray() const override
    {
        return m_reflectable->isArray();
    }

    size_t initializeOffsets(size_t bitPosition) override
    {
        return m_reflectable->initializeOffsets(bitPosition);
    }

    size_t initializeOffsets() override
    {
        return initializeOffsets(0);
    }

    size_t bitSizeOf(size_t bitPosition) const override
    {
        return m_reflectable->bitSizeOf(bitPosition);
    }

    size_t bitSizeOf() const override
    {
        return bitSizeOf(0);
    }

    void write(BitStreamWriter& writer) const override
    {
        m_reflectable->write(writer);
    }

    IBasicReflectableConstPtr<ALLOC> getField(std::string_view name) const override
    {
        return m_reflectable->getField(name);
    }

    IBasicReflectablePtr<ALLOC> getField(std::string_view name) override
    {
        return m_reflectable->getField(name);
    }

    IBasicReflectablePtr<ALLOC> createField(std::string_view name) override
    {
        return m_reflectable->createField(name);
    }

    void setField(std::string_view name, const BasicAny<ALLOC>& value) override
    {
        m_reflectable->setField(name, value);
    }

    IBasicReflectableConstPtr<ALLOC> getParameter(std::string_view name) const override
    {
        return m_reflectable->getParameter(name);
    }

    IBasicReflectablePtr<ALLOC> getParameter(std::string_view name) override
    {
        return m_reflectable->getParameter(name);
    }

    IBasicReflectableConstPtr<ALLOC> callFunction(std::string_view name) const override
    {
        return m_reflectable->callFunction(name);
    }

    IBasicReflectablePtr<ALLOC> callFunction(std::string_view name) override
    {
        return m_reflectable->callFunction(name);
    }

    std::string_view getChoice() const override
    {
        return m_reflectable->getChoice();
    }

    IBasicReflectableConstPtr<ALLOC> find(std::string_view path) const override
    {
        return m_reflectable->find(path);
    }

    IBasicReflectablePtr<ALLOC> find(std::string_view path) override
    {
        return m_reflectable->find(path);
    }

    IBasicReflectableConstPtr<ALLOC> operator[](std::string_view path) const override
    {
        return m_reflectable->operator[](path);
    }

    IBasicReflectablePtr<ALLOC> operator[](std::string_view path) override
    {
        return m_reflectable->operator[](path);
    }

    size_t size() const override
    {
        return m_reflectable->size();
    }

    void resize(size_t size) override
    {
        m_reflectable->resize(size);
    }

    IBasicReflectableConstPtr<ALLOC> at(size_t index) const override
    {
        return m_reflectable->at(index);
    }

    IBasicReflectablePtr<ALLOC> at(size_t index) override
    {
        return m_reflectable->at(index);
    }

    IBasicReflectableConstPtr<ALLOC> operator[](size_t index) const override
    {
        return m_reflectable->operator[](index);
    }

    IBasicReflectablePtr<ALLOC> operator[](size_t index) override
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
    BytesView getBytes() const override
    {
        return m_reflectable->getBytes();
    }
    std::string_view getstd::string_view() const override
    {
        return m_reflectable->getstd::string_view();
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
    string<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const override
    {
        return m_reflectable->toString(allocator);
    }
    string<RebindAlloc<ALLOC, char>> toString() const override
    {
        return toString(ALLOC());
    }

    const ParsingInfo& parsingInfo() const override
    {
        return m_reflectable->parsingInfo();
    }

private:
    T m_object;
    IBasicReflectablePtr<ALLOC> m_reflectable;
};

/**
 * Factory used to make it easier to create reflectable instances.
 *
 * Creates reflectables for all builtin types and for arrays.
 */
template <typename ALLOC>
class BasicReflectableFactory
{
public:
    static IBasicReflectablePtr<ALLOC> getBool(bool value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BoolReflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getInt8(int8_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int8Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getInt16(int16_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int16Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getInt32(int32_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int32Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getInt64(int64_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int64Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getUInt8(uint8_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt8Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getUInt16(uint16_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt16Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getUInt32(uint32_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt32Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getUInt64(uint64_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt64Reflectable<ALLOC>>(allocator, value);
    }

    template <typename T, typename std::enable_if<std::is_signed<T>::value, int>::type = 0>
    static IBasicReflectablePtr<ALLOC> getFixedSignedBitField(
            T value, uint8_t bitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<FixedSignedBitFieldReflectable<ALLOC, T>>(allocator, value, bitSize);
    }

    template <typename T, typename std::enable_if<std::is_unsigned<T>::value, int>::type = 0>
    static IBasicReflectablePtr<ALLOC> getFixedUnsignedBitField(
            T value, uint8_t bitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<FixedUnsignedBitFieldReflectable<ALLOC, T>>(allocator, value, bitSize);
    }

    template <typename T, typename std::enable_if<std::is_signed<T>::value, int>::type = 0>
    static IBasicReflectablePtr<ALLOC> getDynamicSignedBitField(
            T value, uint8_t maxBitSize, uint8_t dynamicBitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<DynamicSignedBitFieldReflectable<ALLOC, T>>(
                allocator, value, maxBitSize, dynamicBitSize);
    }

    // for dynamic signed bit field given by a type reference (e.g. parameter, function return type)
    static IBasicReflectablePtr<ALLOC> getDynamicSignedBitField(
            int64_t value, uint8_t maxBitSize, const ALLOC& allocator = ALLOC())
    {
        if (maxBitSize != 64)
        {
            throw CppRuntimeException("ReflectableFactory::getDynamicSignedBitField - ")
                    << "maxBitSize != 64 for referenced dynamic bit field!";
        }

        return getDynamicSignedBitField(value, maxBitSize, maxBitSize, allocator);
    }

    template <typename T, typename std::enable_if<std::is_unsigned<T>::value, int>::type = 0>
    static IBasicReflectablePtr<ALLOC> getDynamicUnsignedBitField(
            T value, uint8_t maxBitSize, uint8_t dynamicBitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<DynamicUnsignedBitFieldReflectable<ALLOC, T>>(
                allocator, value, maxBitSize, dynamicBitSize);
    }

    // for dynamic unsigned bit field given a by type reference (e.g. parameter, function return type)
    static IBasicReflectablePtr<ALLOC> getDynamicUnsignedBitField(
            uint64_t value, uint8_t maxBitSize, const ALLOC& allocator = ALLOC())
    {
        if (maxBitSize != 64)
        {
            throw CppRuntimeException("ReflectableFactory::getDynamicUnsignedBitField - ")
                    << "maxBitSize != 64 for referenced dynamic bit field!";
        }

        return getDynamicUnsignedBitField(value, maxBitSize, maxBitSize, allocator);
    }

    static IBasicReflectablePtr<ALLOC> getVarInt16(int16_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt16Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarInt32(int32_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt32Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarInt64(int64_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt64Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarInt(int64_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarIntReflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarUInt16(uint16_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt16Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarUInt32(uint32_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt32Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarUInt64(uint64_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt64Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarUInt(uint64_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUIntReflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getVarSize(uint32_t value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarSizeReflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getFloat16(float value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float16Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getFloat32(float value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float32Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getFloat64(double value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float64Reflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getBytes(BytesView value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BytesReflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getString(std::string_view value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<StringReflectable<ALLOC>>(allocator, value);
    }

    static IBasicReflectablePtr<ALLOC> getBitBuffer(
            const BasicBitBuffer<ALLOC>& value, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BitBufferReflectable<ALLOC>>(allocator, value);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getBoolArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BoolReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getBoolArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BoolReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getInt8Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int8ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getInt8Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int8ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getInt16Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int16ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getInt16Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int16ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getInt32Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int32ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getInt32Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int32ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getInt64Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int64ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getInt64Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Int64ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getUInt8Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt8ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getUInt8Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt8ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getUInt16Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt16ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getUInt16Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt16ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getUInt32Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt32ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getUInt32Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt32ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getUInt64Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt64ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getUInt64Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<UInt64ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getFixedSignedBitFieldArray(
            const RAW_ARRAY& rawArray, uint8_t bitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<FixedSignedBitFieldReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, bitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getFixedSignedBitFieldArray(
            RAW_ARRAY& rawArray, uint8_t bitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<FixedSignedBitFieldReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, bitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getFixedUnsignedBitFieldArray(
            const RAW_ARRAY& rawArray, uint8_t bitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<FixedUnsignedBitFieldReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, bitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getFixedUnsignedBitFieldArray(
            RAW_ARRAY& rawArray, uint8_t bitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<FixedUnsignedBitFieldReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, bitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getDynamicSignedBitFieldArray(const RAW_ARRAY& rawArray,
            uint8_t maxBitSize, uint8_t dynamicBitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<DynamicSignedBitFieldReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, maxBitSize, dynamicBitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getDynamicSignedBitFieldArray(
            RAW_ARRAY& rawArray, uint8_t maxBitSize, uint8_t dynamicBitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<DynamicSignedBitFieldReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, maxBitSize, dynamicBitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getDynamicUnsignedBitFieldArray(const RAW_ARRAY& rawArray,
            uint8_t maxBitSize, uint8_t dynamicBitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<DynamicUnsignedBitFieldReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, maxBitSize, dynamicBitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getDynamicUnsignedBitFieldArray(
            RAW_ARRAY& rawArray, uint8_t maxBitSize, uint8_t dynamicBitSize, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<DynamicUnsignedBitFieldReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray, maxBitSize, dynamicBitSize);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarInt16Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt16ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarInt16Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt16ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarInt32Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt32ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarInt32Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt32ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarInt64Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt64ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarInt64Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarInt64ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarIntArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarIntReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarIntArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarIntReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarUInt16Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt16ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarUInt16Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt16ReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarUInt32Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt32ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarUInt32Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt32ReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarUInt64Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt64ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarUInt64Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUInt64ReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarUIntArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUIntReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarUIntArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarUIntReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getVarSizeArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarSizeReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getVarSizeArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<VarSizeReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getFloat16Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float16ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getFloat16Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float16ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getFloat32Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float32ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getFloat32Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float32ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getFloat64Array(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float64ReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getFloat64Array(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<Float64ReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getBytesArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BytesReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getBytesArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BytesReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getStringArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<StringReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getStringArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<StringReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getBitBufferArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BitBufferReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getBitBufferArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BitBufferReflectableArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getCompoundArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<CompoundReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getCompoundArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<CompoundReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getBitmaskArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BitmaskReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getBitmaskArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<BitmaskReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectableConstPtr<ALLOC> getEnumArray(
            const RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<EnumReflectableConstArray<ALLOC, RAW_ARRAY>>(
                allocator, allocator, rawArray);
    }

    template <typename RAW_ARRAY>
    static IBasicReflectablePtr<ALLOC> getEnumArray(RAW_ARRAY& rawArray, const ALLOC& allocator = ALLOC())
    {
        return std::allocate_shared<EnumReflectableArray<ALLOC, RAW_ARRAY>>(allocator, allocator, rawArray);
    }
};

/** Typedef to the reflectable factory provided for convenience - using default std::allocator<uint8_t>. */
using ReflectableFactory = BasicReflectableFactory<std::allocator<uint8_t>>;

#endif

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
size_t ReflectableBase<ALLOC>::initializeOffsets(size_t)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
size_t ReflectableBase<ALLOC>::initializeOffsets()
{
    return initializeOffsets(0);
}

template <typename ALLOC>
size_t ReflectableBase<ALLOC>::bitSizeOf(size_t) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
size_t ReflectableBase<ALLOC>::bitSizeOf() const
{
    return bitSizeOf(0);
}

template <typename ALLOC>
void ReflectableBase<ALLOC>::write(BitStreamWriter&) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is not implemented!";
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
IBasicReflectableConstPtr<ALLOC> ReflectableBase<ALLOC>::getParameter(std::string_view) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no parameters to get!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::getParameter(std::string_view)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no parameters to get!";
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableBase<ALLOC>::callFunction(std::string_view) const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no functions to call!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::callFunction(std::string_view)
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' has no functions to call!";
}

template <typename ALLOC>
std::string_view ReflectableBase<ALLOC>::getChoice() const
{
    throw CppRuntimeException("Type '") << getTypeInfo().getSchemaName() << "' is neither choice nor union!";
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableBase<ALLOC>::find(std::string_view path) const
{
    return detail::getFromObject(*this, path, 0);
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::find(std::string_view path)
{
    return detail::getFromObject(*this, path, 0);
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableBase<ALLOC>::operator[](std::string_view path) const
{
    return find(path);
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableBase<ALLOC>::operator[](std::string_view path)
{
    return find(path);
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
size_t ReflectableArrayBase<ALLOC>::initializeOffsets(size_t)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
size_t ReflectableArrayBase<ALLOC>::bitSizeOf(size_t) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
void ReflectableArrayBase<ALLOC>::write(BitStreamWriter&) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
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
IBasicReflectableConstPtr<ALLOC> ReflectableArrayBase<ALLOC>::getParameter(std::string_view) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableArrayBase<ALLOC>::getParameter(std::string_view)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectableConstPtr<ALLOC> ReflectableArrayBase<ALLOC>::callFunction(std::string_view) const
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
IBasicReflectablePtr<ALLOC> ReflectableArrayBase<ALLOC>::callFunction(std::string_view)
{
    throw CppRuntimeException("Reflectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
}

template <typename ALLOC>
std::string_view ReflectableArrayBase<ALLOC>::getChoice() const
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

template <typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(Bool value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<BoolReflectable<ALLOC>>(allocator, value);
}

template <typename T, BitSize BIT_SIZE, typename ALLOC = std::allocator<uint8_t>,
        std::enable_if_t<is_allocator_v<ALLOC>, int> = 0>
IBasicReflectablePtr<ALLOC> reflectable(detail::IntWrapper<T, BIT_SIZE> value, const ALLOC& allocator = ALLOC())
{
    static_assert(BIT_SIZE != 0, "Variable dynamic bit fields not allowed here!");
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

template <typename T, typename ALLOC = std::allocator<uint8_t>,
        std::enable_if_t<is_allocator_v<ALLOC>, int> = 0>
IBasicReflectablePtr<ALLOC> reflectable(
        detail::DynIntWrapper<T, 0> value, uint8_t bitSize, const ALLOC& allocator = ALLOC())
{
    using Type = detail::DynIntWrapper<T, 0>;

    if constexpr (std::is_signed_v<T>)
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<DynIntReflectable<Int64Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<DynIntReflectable<Int32Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<DynIntReflectable<Int16Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
        }
        else
        {
            return std::allocate_shared<DynIntReflectable<Int8Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
        }
    }
    else
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<DynIntReflectable<UInt64Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<DynIntReflectable<UInt32Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<DynIntReflectable<UInt16Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
        }
        else
        {
            return std::allocate_shared<DynIntReflectable<UInt8Reflectable<Type, ALLOC>>>(
                    allocator, value, bitSize);
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

        m_rawArray[index] = value.template get<typename RAW_ARRAY::value_type>();
    }

    void append(const BasicAny<ALLOC>& value) override
    {
        m_rawArray.push_back(value.template get<typename RAW_ARRAY::value_type>());
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

} // namespace zserio

#endif // ZSERIO_REFLECTABLE_H_INC
