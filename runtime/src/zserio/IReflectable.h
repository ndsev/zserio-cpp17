#ifndef ZSERIO_I_REFLECTABLE_H_INC
#define ZSERIO_I_REFLECTABLE_H_INC

#include <memory>
#include <string_view>

#include "zserio/Any.h"
#include "zserio/BitBuffer.h"
#include "zserio/RebindAlloc.h"

namespace zserio
{

// forward declaration
template <typename ALLOC>
class IBasicTypeInfo;

/**
 * Interface for reflectable view to instances of zserio objects.
 *
 * \note Users are responsible to maintain life-time of reflected objects which must exist
 * as long as the views are used.
 *
 * \note The object in this context may be also an instance of a built-in type.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicReflectable
{
public:
    /** Shared pointer to the reflectable interface. */
    using Ptr = std::shared_ptr<IBasicReflectable>;

    /** Shared pointer to the constant reflectable interface. */
    using ConstPtr = std::shared_ptr<const IBasicReflectable>;

    /**
     * Destructor.
     */
    virtual ~IBasicReflectable() = default;

    /**
     * Gets type info for the current zserio object.
     *
     * \return Reference to the static instance of type info.
     */
    virtual const IBasicTypeInfo<ALLOC>& getTypeInfo() const = 0;

    /**
     * Gets whether the reflected object is an array.
     *
     * \return True when the object is an array, false otherwise.
     */
    virtual bool isArray() const = 0;

    /**
     * Gets reflectable view to the field (i.e. member) with the given schema name.
     *
     * \note Can be called only when the reflected object is a zserio compound type.
     *
     * \param name Field schema name.
     *
     * \return Reflectable view to the requested field.
     *
     * \throw CppRuntimeException When the reflected object is not a compound type or when the field with
     *                            the given name doesn't exist or when the field getter itself throws.
     */
    /** \{ */
    virtual ConstPtr getField(std::string_view name) const = 0;
    virtual Ptr getField(std::string_view name) = 0;
    /** \} */

    /**
     * Sets the field (i.e. member) with the given schema name.
     *
     * \note For optional fields, the value can be also nullptr of type std::nullptr_t which allows
     *       to reset an optional field.
     *
     * \param name Field schema name.
     * \param value Value to set. The type must exactly match the type of the zserio field mapped to C++!
     *
     * \throw CppRuntimeException When the reflected object is not a compound type or when the field with
     *                            the given name doesn't exist or when the provided value is of a wrong type
     *                            or when the field setter itself throws.
     */
    virtual void setField(std::string_view name, const BasicAny<ALLOC>& value) = 0;

    /**
     * Creates a default constructed field within current object and returns reflectable pointer to it.
     *
     * \note When the field already exists, it's reset with the new default constructed value.
     *
     * \param name Name of the optional field to create.
     *
     * \return Reflectable to just created object.
     *
     * \throw CppRuntimeException When the reflected object is not a compound type or when the field with
     *                            the given name doesn't exists.
     */
    virtual Ptr createField(std::string_view name) = 0;

    /**
     * Gets size of the reflected array.
     *
     * \note Can be called only when the reflected object is an array.
     *
     * \return Size of the reflected array.
     *
     * \throw CppRuntimeException When the reflected object is not an array.
     */
    virtual size_t size() const = 0;

    /**
     * Resizes the reflected array.
     *
     * \note Can be called only when the reflected object is an array.
     *
     * \param size New array size.
     *
     * \throws CppRuntimeException When the reflected object is not an array.
     */
    virtual void resize(size_t size) = 0;

    /**
     * Gets reflectable view to an array element.
     *
     * \note Can be called only when the reflected object is an array.
     *
     * \return Reflectable view to an array element on the given index.
     *
     * \throw CppRuntimeException When the reflected object is not an array or when the given index is
     *                            out of bounds of the underlying array.
     */
    /** \{ */
    virtual ConstPtr at(size_t index) const = 0;
    virtual Ptr at(size_t index) = 0;
    /** \} */

    /**
     * \copydoc IBasicReflectable::at
     *
     * Overloaded method provided for convenience.
     */
    /** \{ */
    virtual ConstPtr operator[](size_t index) const = 0;
    virtual Ptr operator[](size_t index) = 0;
    /** \} */

    /**
     * Sets an element value at the given index within the reflected array.
     *
     * \param value Value to set.
     * \param index Index of the element to set.
     *
     * \throws CppRuntimeException When the reflected object is not an array.
     */
    virtual void setAt(const BasicAny<ALLOC>& value, size_t index) = 0;

    /**
     * Appends an element at the given index within the reflected array.
     *
     * \param value Value to append.
     *
     * \throws CppRuntimeException When the reflected object is not an array.
     */
    virtual void append(const BasicAny<ALLOC>& value) = 0;

    /**
     * Gets any value within the reflected object.
     *
     * For builtin types, enums and bitmasks the value is "returned by value" - i.e. it's copied
     * into the any holder, but note that for bytes the any holder contains Span,
     * for string the any holder contains an appropriate string_view and for compounds, bit buffers and arrays
     * the value is "returned by reference" - i.e. the any holder contains std::reference_wrapper<T> with the
     * reference to the compound type or the raw array type.
     *
     * \note For bit buffers only const reference is available.
     * \note Overloads without parameter use default constructed allocator.
     *
     * \param allocator Allocator to use for the value allocation.
     *
     * \return Any value.
     */
    /** \{ */
    virtual BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) const = 0;
    virtual BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) = 0;
    virtual BasicAny<ALLOC> getAnyValue() const = 0;
    virtual BasicAny<ALLOC> getAnyValue() = 0;
    /** \} */

    /**
     * Gets bool value of the bool reflectable.
     *
     * \return Bool value.
     * \throw CppRuntimeException When the reflected object is not a bool type.
     */
    virtual bool getBool() const = 0;

    /**
     * Gets 8-bit signed integral value of the int8_t reflectable.
     *
     * \return 8-bit signed integral value.
     * \throw CppRuntimeException When the reflected object is not a int8_t type.
     */
    virtual int8_t getInt8() const = 0;

    /**
     * Gets 16-bit signed integral value of the int16_t reflectable.
     *
     * \return 16-bit signed integral value.
     * \throw CppRuntimeException When the reflected object is not a int16_t type.
     */
    virtual int16_t getInt16() const = 0;

    /**
     * Gets 32-bit signed integral value of the int32_t reflectable.
     *
     * \return 32-bit signed integral value.
     * \throw CppRuntimeException When the reflected object is not a int32_t type.
     */
    virtual int32_t getInt32() const = 0;

    /**
     * Gets 64-bit signed integral value of the int64_t reflectable.
     *
     * \return 64-bit signed integral value.
     * \throw CppRuntimeException When the reflected object is not a int64_t type.
     */
    virtual int64_t getInt64() const = 0;

    /**
     * Gets 8-bit unsigned integral value of the uint8_t reflectable.
     *
     * \return 8-bit unsigned integral value.
     * \throw CppRuntimeException When the reflected object is not a uint8_t type.
     */
    virtual uint8_t getUInt8() const = 0;

    /**
     * Gets 16-bit unsigned integral value of the uint16_t reflectable.
     *
     * \return 16-bit unsigned integral value.
     * \throw CppRuntimeException When the reflected object is not a uint16_t type.
     */
    virtual uint16_t getUInt16() const = 0;

    /**
     * Gets 32-bit unsigned integral value of the uint32_t reflectable.
     *
     * \return 32-bit unsigned integral value.
     * \throw CppRuntimeException When the reflected object is not a uint32_t type.
     */
    virtual uint32_t getUInt32() const = 0;

    /**
     * Gets 64-bit unsigned integral value of the uint64_t reflectable.
     *
     * \return 64-bit unsigned integral value.
     * \throw CppRuntimeException When the reflected object is not a uint64_t type.
     */
    virtual uint64_t getUInt64() const = 0;

    /**
     * Gets float value of the float reflectable.
     *
     * \return Float value.
     * \throw CppRuntimeException When the reflected object is not a float type.
     */
    virtual float getFloat() const = 0;

    /**
     * Gets double value of the double reflectable.
     *
     * \return Double value.
     * \throw CppRuntimeException When the reflected object is not a double type.
     */
    virtual double getDouble() const = 0;

    /**
     * Gets byte value of the bytes reflectable.
     *
     * \return Bytes value as a span.
     * \throw CppRuntimeException When the reflected object is not a bytes type.
     */
    virtual BytesView getBytes() const = 0;

    /**
     * Gets reference to the string value of the string reflectable.
     *
     * \return Reference to the string value.
     * \throw CppRuntimeException When the reflected object is not a string type.
     */
    virtual std::string_view getStringView() const = 0;

    /**
     * Gets reference to the reflected bit buffer.
     *
     * \return Reference to the bit buffer.
     * \throw CppRuntimeException When the reflected object is not a bit buffer (i.e. extern type).
     */
    virtual const BasicBitBuffer<ALLOC>& getBitBuffer() const = 0; // TODO[Mi-L@]: return BitBufferView?

    /**
     * Converts any signed integral value to 64-bit signed integer.
     *
     * Works also for enum types defined with signed underlying type.
     *
     * \return 64-bit signed integral value.
     * \throw CppRuntimeException When the reflected object cannot be converted to a signed integral value.
     */
    virtual int64_t toInt() const = 0;

    /**
     * Converts any unsigned integral value to 64-bit unsigned integer.
     *
     * Works also for bitmask and enum typed defined with unsigned underlying type.
     *
     * \return 64-bit unsigned integral value.
     * \throw CppRuntimeException When the reflected object cannot be converted to
     *                            an unsigned integral value.
     */
    virtual uint64_t toUInt() const = 0;

    /**
     * Converts any numeric value to double.
     *
     * Works also for bitmask and enum types.
     *
     * \return Double value.
     * \throw CppRuntimeException When the reflected object cannot be converted to double.
     */
    virtual double toDouble() const = 0;

    /**
     * Converts an reflected object to string.
     *
     * Works for all integral types including bool, bitmask and enum types and for string types.
     *
     * \note Floating point types are not currently supported!
     * \note The conversion to string can be implemented for more types in future versions.
     * \note Overload without parameter use default constructed allocator.
     *
     * \param allocator Allocator to use for the string allocation.
     *
     * \return String value representing the reflected object.
     */
    /** \{ */
    virtual BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const = 0;
    virtual BasicString<RebindAlloc<ALLOC, char>> toString() const = 0;
    /** \} */
};

/** Typedef to reflectable smart pointer needed for convenience in generated code. */
/** \{ */
template <typename ALLOC = std::allocator<uint8_t>>
using IBasicReflectablePtr = typename IBasicReflectable<ALLOC>::Ptr;

template <typename ALLOC = std::allocator<uint8_t>>
using IBasicReflectableConstPtr = typename IBasicReflectable<ALLOC>::ConstPtr;
/** \} */

/** Typedef to reflectable interface provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using IReflectable = IBasicReflectable<>;
using IReflectablePtr = IBasicReflectablePtr<>;
using IReflectableConstPtr = IBasicReflectableConstPtr<>;
/** \} */

/**
 * Gets reflectable view for the given enum item.
 *
 * \param value Enum value to reflect.
 * \param allocator Allocator to use for reflectable allocation.
 *
 * \return Enum reflectable view.
 */
template <typename T, typename ALLOC = std::allocator<uint8_t>>
IBasicReflectablePtr<ALLOC> reflectable(T value, const ALLOC& allocator = ALLOC());

} // namespace zserio

#endif // ZSERIO_I_REFLECTABLE_H_INC
