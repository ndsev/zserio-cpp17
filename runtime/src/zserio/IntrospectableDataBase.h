#ifndef ZSERIO_INTROSPECTABLE_DATA_BASE_H_INC
#define ZSERIO_INTROSPECTABLE_DATA_BASE_H_INC

#include "zserio/TypeInfo.h"

namespace zserio
{
namespace detail
{

/**
 * Base class for all introspectable data implementations.
 */
template <typename I, typename ALLOC>
class IntrospectableDataBase : public I
{
public:
    /** Shared pointer to the constant introspectable interface. */
    using ConstPtr = typename I::ConstPtr;

    using I::at;
    using I::getField;
    using I::operator[];
    using I::find;
    using I::getAnyValue;

    /**
     * Constructor.
     *
     * \param typeInfo Type info of the object.
     */
    explicit IntrospectableDataBase(const IBasicTypeInfo<ALLOC>& typeInfo);

    /** Destructor. */
    ~IntrospectableDataBase() override = default;

    /**
     * Copying and moving is disallowed!
     * \{
     */
    IntrospectableDataBase(const IntrospectableDataBase&) = delete;
    IntrospectableDataBase& operator=(const IntrospectableDataBase&) = delete;

    IntrospectableDataBase(const IntrospectableDataBase&&) = delete;
    IntrospectableDataBase& operator=(const IntrospectableDataBase&&) = delete;
    /**
     * \}
     */

    const IBasicTypeInfo<ALLOC>& getTypeInfo() const override;
    bool isArray() const override;

    ConstPtr getField(std::string_view name) const override;
    std::string_view getChoice() const override;

    size_t size() const override;
    ConstPtr at(size_t index) const override;
    ConstPtr operator[](size_t index) const override;

    BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const override;
    BasicAny<ALLOC> getAnyValue() const override;

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

    ConstPtr find(std::string_view path) const override;
    ConstPtr operator[](std::string_view path) const override;

private:
    const IBasicTypeInfo<ALLOC>& m_typeInfo;
};

// implementation of base classes methods

template <typename I, typename ALLOC>
IntrospectableDataBase<I, ALLOC>::IntrospectableDataBase(const IBasicTypeInfo<ALLOC>& typeInfo) :
        m_typeInfo(typeInfo)
{}

template <typename I, typename ALLOC>
const IBasicTypeInfo<ALLOC>& IntrospectableDataBase<I, ALLOC>::getTypeInfo() const
{
    return m_typeInfo;
}

template <typename I, typename ALLOC>
bool IntrospectableDataBase<I, ALLOC>::isArray() const
{
    return false;
}

template <typename I, typename ALLOC>
typename IntrospectableDataBase<I, ALLOC>::ConstPtr IntrospectableDataBase<I, ALLOC>::getField(
        std::string_view) const
{
    throw CppRuntimeException("Type '") << m_typeInfo.getSchemaName() << "' has no fields to get!";
}

template <typename I, typename ALLOC>
std::string_view IntrospectableDataBase<I, ALLOC>::getChoice() const
{
    throw CppRuntimeException("Type '") << m_typeInfo.getSchemaName() << "' is neither choice nor union!";
}

template <typename I, typename ALLOC>
size_t IntrospectableDataBase<I, ALLOC>::size() const
{
    throw CppRuntimeException("Type '") << m_typeInfo.getSchemaName() << "' is not an array!";
}

template <typename I, typename ALLOC>
typename IntrospectableDataBase<I, ALLOC>::ConstPtr IntrospectableDataBase<I, ALLOC>::at(size_t) const
{
    throw CppRuntimeException("Type '") << m_typeInfo.getSchemaName() << "' is not an array!";
}

template <typename I, typename ALLOC>
typename IntrospectableDataBase<I, ALLOC>::ConstPtr IntrospectableDataBase<I, ALLOC>::operator[](size_t) const
{
    throw CppRuntimeException("Type '") << m_typeInfo.getSchemaName() << "' is not an array!";
}

template <typename I, typename ALLOC>
BasicAny<ALLOC> IntrospectableDataBase<I, ALLOC>::getAnyValue(const ALLOC&) const
{
    throw CppRuntimeException("Type '") << m_typeInfo.getSchemaName() << "' is not implemented!";
}

template <typename I, typename ALLOC>
BasicAny<ALLOC> IntrospectableDataBase<I, ALLOC>::getAnyValue() const
{
    return getAnyValue(ALLOC());
}

template <typename I, typename ALLOC>
bool IntrospectableDataBase<I, ALLOC>::getBool() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not boolean type!";
}

template <typename I, typename ALLOC>
int8_t IntrospectableDataBase<I, ALLOC>::getInt8() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not int8 type!";
}

template <typename I, typename ALLOC>
int16_t IntrospectableDataBase<I, ALLOC>::getInt16() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not int16 type!";
}

template <typename I, typename ALLOC>
int32_t IntrospectableDataBase<I, ALLOC>::getInt32() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not int32 type!";
}

template <typename I, typename ALLOC>
int64_t IntrospectableDataBase<I, ALLOC>::getInt64() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not int64 type!";
}

template <typename I, typename ALLOC>
uint8_t IntrospectableDataBase<I, ALLOC>::getUInt8() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not uint8 type!";
}

template <typename I, typename ALLOC>
uint16_t IntrospectableDataBase<I, ALLOC>::getUInt16() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not uint16 type!";
}

template <typename I, typename ALLOC>
uint32_t IntrospectableDataBase<I, ALLOC>::getUInt32() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not uint32 type!";
}

template <typename I, typename ALLOC>
uint64_t IntrospectableDataBase<I, ALLOC>::getUInt64() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not uint64 type!";
}

template <typename I, typename ALLOC>
float IntrospectableDataBase<I, ALLOC>::getFloat() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not float type!";
}

template <typename I, typename ALLOC>
double IntrospectableDataBase<I, ALLOC>::getDouble() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not double type!";
}

template <typename I, typename ALLOC>
BytesView IntrospectableDataBase<I, ALLOC>::getBytes() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not bytes type!";
}

template <typename I, typename ALLOC>
std::string_view IntrospectableDataBase<I, ALLOC>::getStringView() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not string type!";
}

template <typename I, typename ALLOC>
const BasicBitBuffer<ALLOC>& IntrospectableDataBase<I, ALLOC>::getBitBuffer() const
{
    throw CppRuntimeException("'") << m_typeInfo.getSchemaName() << "' is not an extern type!";
}

template <typename I, typename ALLOC>
int64_t IntrospectableDataBase<I, ALLOC>::toInt() const
{
    throw CppRuntimeException("Conversion from '")
            << m_typeInfo.getSchemaName() << "' to signed integer is not available!";
}

template <typename I, typename ALLOC>
uint64_t IntrospectableDataBase<I, ALLOC>::toUInt() const
{
    throw CppRuntimeException("Conversion from '")
            << m_typeInfo.getSchemaName() << "' to unsigned integer is not available!";
}

template <typename I, typename ALLOC>
double IntrospectableDataBase<I, ALLOC>::toDouble() const
{
    throw CppRuntimeException("Conversion from '")
            << m_typeInfo.getSchemaName() << "' to double is not available!";
}

template <typename I, typename ALLOC>
BasicString<RebindAlloc<ALLOC, char>> IntrospectableDataBase<I, ALLOC>::toString(const ALLOC&) const
{
    throw CppRuntimeException("Conversion from '")
            << m_typeInfo.getSchemaName() << "' to string is not available!";
}

template <typename I, typename ALLOC>
BasicString<RebindAlloc<ALLOC, char>> IntrospectableDataBase<I, ALLOC>::toString() const
{
    return toString(ALLOC());
}

template <typename I, typename ALLOC>
typename IntrospectableDataBase<I, ALLOC>::ConstPtr IntrospectableDataBase<I, ALLOC>::find(
        std::string_view) const
{
    throw CppRuntimeException("Find is not available for '") << m_typeInfo.getSchemaName() << "'!";
}

template <typename I, typename ALLOC>
typename IntrospectableDataBase<I, ALLOC>::ConstPtr IntrospectableDataBase<I, ALLOC>::operator[](
        std::string_view path) const
{
    return find(path);
}

} // namespace detail
} // namespace zserio

#endif // ZSERIO_INTROSPECTABLE_DATA_BASE_H_INC
