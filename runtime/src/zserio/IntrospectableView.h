#ifndef ZSERIO_INTROSPECTABLE_VIEW_H_INC
#define ZSERIO_INTROSPECTABLE_VIEW_H_INC

#include "zserio/IIntrospectableView.h"
#include "zserio/IntrospectableDataBase.h"
#include "zserio/TypeInfoUtil.h"

namespace zserio
{

/**
 * Base class for all introspectable view implementations.
 *
 * Holds the value.
 */
template <typename ALLOC>
class IntrospectableViewBase : public IntrospectableDataBase<IBasicIntrospectableView<ALLOC>, ALLOC>
{
public:
    /** Shared pointer to the constant introspectable interface. */
    using ConstPtr = typename IBasicIntrospectableView<ALLOC>::ConstPtr;

    /**
     * Constructor.
     *
     * \param typeInfo Type info of the object.
     */
    explicit IntrospectableViewBase(const IBasicTypeInfo<ALLOC>& typeInfo);

    /** Destructor. */
    ~IntrospectableViewBase() override = 0;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    IntrospectableViewBase(const IntrospectableViewBase&) = delete;
    IntrospectableViewBase& operator=(const IntrospectableViewBase&) = delete;

    IntrospectableViewBase(const IntrospectableViewBase&&) = delete;
    IntrospectableViewBase& operator=(const IntrospectableViewBase&&) = delete;
    /**
     * \}
     */

    ConstPtr find(std::string_view path) const override;

    ConstPtr getParameter(std::string_view name) const override;
    ConstPtr callFunction(std::string_view name) const override;

    BitSize initializeOffsets(BitSize bitPosition) const override;
    BitSize initializeOffsets() const override;
    BitSize bitSizeOf(BitSize bitPosition) const override;
    BitSize bitSizeOf() const override;
    void write(BitStreamWriter& writer) const override;

private:
    ConstPtr getFieldFromObject(const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const;
    ConstPtr getParameterFromObject(const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const;
    ConstPtr callFunctionInObject(const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const;
    ConstPtr getFromObject(
            const IBasicIntrospectableView<ALLOC>& object, std::string_view path, size_t pos) const;
};

/**
 * Base class for introspectable numeric (arithmetic) types, string view and span.
 *
 * It holds the value.
 */
template <typename T, typename ALLOC>
class BuiltinIntrospectableViewBase : public IntrospectableViewBase<ALLOC>
{
private:
    using Base = IntrospectableViewBase<ALLOC>;

protected:
    BuiltinIntrospectableViewBase(const IBasicTypeInfo<ALLOC>& typeInfo, T value) :
            IntrospectableViewBase<ALLOC>(typeInfo),
            m_value(value)
    {}

    T getValue() const
    {
        return m_value;
    }

public:
    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(getValue(), allocator);
    }

    BitSize bitSizeOf(BitSize bitPosition) const override
    {
        return detail::bitSizeOf(m_value, bitPosition);
    }

    void write(BitStreamWriter& writer) const override
    {
        detail::write(writer, m_value);
    }

private:
    T m_value;
};

/**
 * Base class for integral introspectables.
 *
 * Implements toString() and toDouble() conversions.
 */
template <typename T, typename ALLOC>
class IntegralIntrospectableViewBase : public BuiltinIntrospectableViewBase<T, ALLOC>
{
protected:
    static_assert(std::is_integral_v<typename T::ValueType>, "T must be a signed integral type!");

    using Base = BuiltinIntrospectableViewBase<T, ALLOC>;

public:
    IntegralIntrospectableViewBase(const IBasicTypeInfo<ALLOC>& typeInfo, T value) :
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
 * Base class for signed integral introspectables.
 *
 * Implements toInt() conversion.
 */
template <typename T, typename ALLOC>
class SignedIntrospectableViewBase : public IntegralIntrospectableViewBase<T, ALLOC>
{
protected:
    static_assert(std::is_signed_v<typename T::ValueType>, "T must be a signed integral type!");

    using Base = IntegralIntrospectableViewBase<T, ALLOC>;

    using Base::Base;

public:
    int64_t toInt() const override
    {
        return Base::getValue();
    }
};

/**
 * Base class for unsigned integral introspectables.
 *
 * Implements toUInt() conversion.
 */
template <typename T, typename ALLOC>
class UnsignedIntrospectableViewBase : public IntegralIntrospectableViewBase<T, ALLOC>
{
protected:
    static_assert(std::is_unsigned<typename T::ValueType>::value, "T must be an unsigned integral type!");

    using Base = IntegralIntrospectableViewBase<T, ALLOC>;

    using Base::Base;

public:
    uint64_t toUInt() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for values of bool type.
 */
template <typename ALLOC>
class BoolIntrospectableView : public UnsignedIntrospectableViewBase<Bool, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<Bool, ALLOC>;

public:
    explicit BoolIntrospectableView(Bool value) :
            Base(typeInfo<zserio::Bool, ALLOC>(), value)
    {}

    bool getBool() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for int8 type.
 */
template <typename T, typename ALLOC>
class Int8IntrospectableView : public SignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<int8_t, typename T::ValueType>, "T must be based on int8_t!");

public:
    explicit Int8IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int8_t getInt8() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for int16 type.
 */
template <typename T, typename ALLOC>
class Int16IntrospectableView : public SignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<int16_t, typename T::ValueType>, "T must be based on int16_t!");

public:
    explicit Int16IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int16_t getInt16() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for int32 type.
 */
template <typename T, typename ALLOC>
class Int32IntrospectableView : public SignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<int32_t, typename T::ValueType>, "T must be based on int32_t!");

public:
    explicit Int32IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int32_t getInt32() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for int64 type.
 */
template <typename T, typename ALLOC>
class Int64IntrospectableView : public SignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<int64_t, typename T::ValueType>, "T must be based on int64_t!");

public:
    explicit Int64IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int64_t getInt64() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for uint8 type.
 */
template <typename T, typename ALLOC>
class UInt8IntrospectableView : public UnsignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<uint8_t, typename T::ValueType>, "T must be based on uint8_t!");

public:
    explicit UInt8IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint8_t getUInt8() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for uint16 type.
 */
template <typename T, typename ALLOC>
class UInt16IntrospectableView : public UnsignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<uint16_t, typename T::ValueType>, "T must be based on uint16_t!");

public:
    explicit UInt16IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint16_t getUInt16() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for uint32 type.
 */
template <typename T, typename ALLOC>
class UInt32IntrospectableView : public UnsignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<uint32_t, typename T::ValueType>, "T must be based on uint32_t!");

public:
    explicit UInt32IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint32_t getUInt32() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for uint64 type.
 */
template <typename T, typename ALLOC>
class UInt64IntrospectableView : public UnsignedIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<T, ALLOC>;

    static_assert(std::is_same_v<uint64_t, typename T::ValueType>, "T must be based on uint64_t!");

public:
    explicit UInt64IntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint64_t getUInt64() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of int8 type.
 */
template <typename T, typename ALLOC>
class DynInt8IntrospectableView : public SignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<int8_t, typename T::ValueType>, "T must be based on int8_t!");

public:
    explicit DynInt8IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int8_t getInt8() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of int16 type.
 */
template <typename T, typename ALLOC>
class DynInt16IntrospectableView : public SignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<int16_t, typename T::ValueType>, "T must be based on int16_t!");

public:
    explicit DynInt16IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int16_t getInt16() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of int32 type.
 */
template <typename T, typename ALLOC>
class DynInt32IntrospectableView : public SignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<int32_t, typename T::ValueType>, "T must be based on int32_t!");

public:
    explicit DynInt32IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int32_t getInt32() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of int64 type.
 */
template <typename T, typename ALLOC>
class DynInt64IntrospectableView : public SignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = SignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<int64_t, typename T::ValueType>, "T must be based on int64_t!");

public:
    explicit DynInt64IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    int64_t getInt64() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of uint8 type.
 */
template <typename T, typename ALLOC>
class DynUInt8IntrospectableView : public UnsignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<uint8_t, typename T::ValueType>, "T must be based on uint8_t!");

public:
    explicit DynUInt8IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint8_t getUInt8() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of uint16 type.
 */
template <typename T, typename ALLOC>
class DynUInt16IntrospectableView : public UnsignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<uint16_t, typename T::ValueType>, "T must be based on uint16_t!");

public:
    explicit DynUInt16IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint16_t getUInt16() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of uint32 type.
 */
template <typename T, typename ALLOC>
class DynUInt32IntrospectableView : public UnsignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<uint32_t, typename T::ValueType>, "T must be based on uint32_t!");

public:
    explicit DynUInt32IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint32_t getUInt32() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for dynamic bitfield of uint64 type.
 */
template <typename T, typename ALLOC>
class DynUInt64IntrospectableView : public UnsignedIntrospectableViewBase<View<T>, ALLOC>
{
private:
    using Base = UnsignedIntrospectableViewBase<View<T>, ALLOC>;

    static_assert(std::is_same_v<uint64_t, typename T::ValueType>, "T must be based on uint64_t!");

public:
    explicit DynUInt64IntrospectableView(const View<T>& value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    uint64_t getUInt64() const override
    {
        return Base::getValue();
    }
};

/**
 * Base class for floating point introspectables.
 */
template <typename T, typename ALLOC>
class FloatingPointIntrospectableViewBase : public BuiltinIntrospectableViewBase<T, ALLOC>
{
protected:
    static_assert(std::is_floating_point_v<typename T::ValueType>, "T must be a floating point type!");

    using Base = BuiltinIntrospectableViewBase<T, ALLOC>;
    using Base::Base;

public:
    double toDouble() const override
    {
        return static_cast<double>(Base::getValue());
    }
};

/**
 * Introspectable for values of float type.
 */
template <typename T, typename ALLOC>
class FloatIntrospectableView : public FloatingPointIntrospectableViewBase<T, ALLOC>
{
private:
    using Base = FloatingPointIntrospectableViewBase<T, ALLOC>;

public:
    explicit FloatIntrospectableView(T value) :
            Base(typeInfo<T, ALLOC>(), value)
    {}

    float getFloat() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for values of double type.
 */
template <typename ALLOC>
class DoubleInstrospectableData : public FloatingPointIntrospectableViewBase<Float64, ALLOC>
{
private:
    using Base = FloatingPointIntrospectableViewBase<Float64, ALLOC>;

public:
    explicit DoubleInstrospectableData(Float64 value) :
            Base(typeInfo<Float64, ALLOC>(), value)
    {}

    double getDouble() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for values of bytes type.
 */
template <typename ALLOC>
class BytesIntrospectableView : public BuiltinIntrospectableViewBase<BytesView, ALLOC>
{
private:
    using Base = BuiltinIntrospectableViewBase<BytesView, ALLOC>;

public:
    explicit BytesIntrospectableView(BytesView value) :
            Base(typeInfo<BasicBytes<ALLOC>>(), value)
    {}

    BytesView getBytes() const override
    {
        return Base::getValue();
    }
};

/**
 * Instrospectable for values of string type.
 */
template <typename ALLOC>
class StringIntrospectableView : public BuiltinIntrospectableViewBase<std::string_view, ALLOC>
{
private:
    using Base = BuiltinIntrospectableViewBase<std::string_view, ALLOC>;

public:
    explicit StringIntrospectableView(std::string_view value) :
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
 * Introspectable for values of bit buffer type.
 */
template <typename ALLOC>
class BitBufferInstrospectableData
        : public BuiltinIntrospectableViewBase<std::reference_wrapper<const BasicBitBuffer<ALLOC>>, ALLOC>
{
private:
    using Base = BuiltinIntrospectableViewBase<std::reference_wrapper<const BasicBitBuffer<ALLOC>>, ALLOC>;

public:
    explicit BitBufferInstrospectableData(std::reference_wrapper<const BasicBitBuffer<ALLOC>> value) :
            Base(typeInfo<BasicBitBuffer<ALLOC>>(), value)
    {}

    const BasicBitBuffer<ALLOC>& getBitBuffer() const override
    {
        return Base::getValue();
    }
};

/**
 * Introspectable for arrays.
 */
template <typename RAW_ARRAY, typename ALLOC>
class IntrospectableViewArray : public IntrospectableViewBase<ALLOC>, public AllocatorHolder<ALLOC>
{
private:
    using Base = IntrospectableViewBase<ALLOC>;

public:
    using Base::getTypeInfo;

    IntrospectableViewArray(const ALLOC& allocator, const RAW_ARRAY& rawArray) :
            Base(typeInfo<typename RAW_ARRAY::value_type, ALLOC>(), allocator),
            m_rawArray(rawArray)
    {}

    bool isArray() const override
    {
        return true;
    }

    IBasicIntrospectableViewConstPtr<ALLOC> getField(std::string_view) const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    std::string_view getChoice() const
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    size_t size() const override
    {
        return m_rawArray.size();
    }

    IBasicIntrospectableViewConstPtr<ALLOC> at(size_t index) const override
    {
        if (index >= size())
        {
            throw CppRuntimeException("Index ")
                    << index << " out of range for introspectable array '" << getTypeInfo().getSchemaName()
                    << "' of size " << size() << "!";
        }

        return introspectable(m_rawArray[index], Base::get_allocator());
    }

    IBasicIntrospectableViewConstPtr<ALLOC> operator[](size_t index) const override
    {
        return this->at(index);
    }

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override
    {
        return BasicAny<ALLOC>(std::cref(m_rawArray), allocator);
    }

    bool getBool() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    int8_t getInt8() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    int16_t getInt16() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    int32_t getInt32() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    int64_t getInt64() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    uint8_t getUInt8() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    uint16_t getUInt16() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    uint32_t getUInt32() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    uint64_t getUInt64() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    float getFloat() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    double getDouble() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    BytesView getBytes() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    std::string_view getStringView() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    const BasicBitBuffer<ALLOC>& getBitBuffer() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    int64_t toInt() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    uint64_t toUInt() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    double toDouble() const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC&) const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    IBasicIntrospectableViewConstPtr<ALLOC> getParameter(std::string_view) const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    IBasicIntrospectableViewConstPtr<ALLOC> callFunction(std::string_view) const override
    {
        throw CppRuntimeException("Introspectable is an array '") << getTypeInfo().getSchemaName() << "[]'!";
    }

    BitSize initializeOffsets(BitSize bitPosition) const override
    {
        return detail::initializeOffsets(m_rawArray, bitPosition);
    }

    BitSize bitSizeOf(BitSize bitPosition) const override
    {
        return detail::bitSizeOf(m_rawArray, bitPosition);
    }

    void write(BitStreamWriter& writer) const override
    {
        detail::write(writer, m_rawArray);
    }

private:
    const RAW_ARRAY& m_rawArray;
};

// implementation of base classes methods

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::IntrospectableViewBase(const IBasicTypeInfo<ALLOC>& typeInfo) :
        IntrospectableDataBase<IBasicIntrospectableView<ALLOC>, ALLOC>(typeInfo)
{}

template <typename ALLOC>
IntrospectableViewBase<ALLOC>::~IntrospectableViewBase() = default;

template <typename ALLOC>
IBasicIntrospectableViewConstPtr<ALLOC> IntrospectableViewBase<ALLOC>::find(std::string_view path) const
{
    return getFromObject(*this, path, 0);
}

template <typename ALLOC>
IBasicIntrospectableViewConstPtr<ALLOC> IntrospectableViewBase<ALLOC>::getParameter(std::string_view) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' has no parameters to get!";
}

template <typename ALLOC>
IBasicIntrospectableViewConstPtr<ALLOC> IntrospectableViewBase<ALLOC>::callFunction(std::string_view) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' has no functions to call!";
}

template <typename ALLOC>
BitSize IntrospectableViewBase<ALLOC>::initializeOffsets(BitSize) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not a compound type!";
}

template <typename ALLOC>
BitSize IntrospectableViewBase<ALLOC>::initializeOffsets() const
{
    return initializeOffsets(0);
}

template <typename ALLOC>
BitSize IntrospectableViewBase<ALLOC>::bitSizeOf(BitSize) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
BitSize IntrospectableViewBase<ALLOC>::bitSizeOf() const
{
    return bitSizeOf(0);
}

template <typename ALLOC>
void IntrospectableViewBase<ALLOC>::write(BitStreamWriter&) const
{
    throw CppRuntimeException("Type '")
            << IntrospectableViewBase<ALLOC>::getTypeInfo().getSchemaName() << "' is not implemented!";
}

template <typename ALLOC>
IBasicIntrospectableViewConstPtr<ALLOC> IntrospectableViewBase<ALLOC>::getFieldFromObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const
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
IBasicIntrospectableViewConstPtr<ALLOC> IntrospectableViewBase<ALLOC>::getParameterFromObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const
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
IBasicIntrospectableViewConstPtr<ALLOC> IntrospectableViewBase<ALLOC>::callFunctionInObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view name) const
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
IBasicIntrospectableViewConstPtr<ALLOC> IntrospectableViewBase<ALLOC>::getFromObject(
        const IBasicIntrospectableView<ALLOC>& object, std::string_view path, size_t pos) const
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

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(Bool value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<BoolIntrospectableView<ALLOC>>(allocator, value);
}

template <typename T, BitSize BIT_SIZE, typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(
        detail::IntWrapper<T, BIT_SIZE> value, const ALLOC& allocator = ALLOC())
{
    using Type = detail::IntWrapper<T, BIT_SIZE>;

    if constexpr (std::is_signed_v<T>)
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<Int64IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<Int32IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<Int16IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<Int8IntrospectableView<Type, ALLOC>>(allocator, value);
        }
    }
    else
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<UInt64IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<UInt32IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<UInt16IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<UInt8IntrospectableView<Type, ALLOC>>(allocator, value);
        }
    }
}

template <typename T, typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(
        const View<detail::DynIntWrapper<T, 0>>& value, const ALLOC& allocator = ALLOC())
{
    using Type = detail::DynIntWrapper<T, 0>;

    if constexpr (std::is_signed_v<T>)
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<DynInt64IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<DynInt32IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<DynInt16IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<DynInt8IntrospectableView<Type, ALLOC>>(allocator, value);
        }
    }
    else
    {
        if constexpr (sizeof(T) > 4)
        {
            return std::allocate_shared<DynUInt64IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 2)
        {
            return std::allocate_shared<DynUInt32IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else if constexpr (sizeof(T) > 1)
        {
            return std::allocate_shared<DynUInt16IntrospectableView<Type, ALLOC>>(allocator, value);
        }
        else
        {
            return std::allocate_shared<DynUInt8IntrospectableView<Type, ALLOC>>(allocator, value);
        }
    }
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarInt16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int16IntrospectableView<VarInt16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarInt32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int32IntrospectableView<VarInt32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarInt64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int64IntrospectableView<VarInt64, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarInt value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<Int64IntrospectableView<VarInt, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarUInt16 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt16IntrospectableView<VarUInt16, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarUInt32 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt32IntrospectableView<VarUInt32, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarUInt64 value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt64IntrospectableView<VarUInt64, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarUInt value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt64IntrospectableView<VarUInt, ALLOC>>(allocator, value);
}

template <typename ALLOC = std::allocator<uint8_t>>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(VarSize value, const ALLOC& allocator = ALLOC())
{
    return std::allocate_shared<UInt32IntrospectableView<VarSize, ALLOC>>(allocator, value);
}

} // namespace zserio

#endif // ZSERIO_INTROSPECTABLE_VIEW_H_INC
