#ifndef ZSERIO_I_INTROSPECTABLE_DATA_INC
#define ZSERIO_I_INTROSPECTABLE_DATA_INC

#include <memory>
#include <string_view>

#include "zserio/Any.h"
#include "zserio/BitBuffer.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Bytes.h"
#include "zserio/RebindAlloc.h"

namespace zserio
{

// Necessary to break cyclic dependency, ITypeInfo header must include IReflectableData header as well!
template <typename ALLOC>
class IBasicTypeInfo;

/**
 * Interface to introspect Data abstraction of the Zserio objects.
 *
 * This is read-only interface which offers all information available from the Data abstraction of the Zserio
 * objects.
 */
template <typename I, typename ALLOC = std::allocator<uint8_t>>
class IIntrospectableData
{
public:
    /** Shared pointer to the constant introspectable interface. */
    using ConstPtr = std::shared_ptr<const I>;

    /**
     * Destructor.
     */
    virtual ~IIntrospectableData() = default;

    /**
     * Gets type info for the current zserio object.
     *
     * \return Reference to the static instance of type info.
     */
    virtual const IBasicTypeInfo<ALLOC>& getTypeInfo() const = 0;

    /**
     * Gets whether the introspective object is an array.
     *
     * \return True when the object is an array, false otherwise.
     */
    virtual bool isArray() const = 0;

    /**
     * Gets the introspectable to the field (i.e. member) with the given schema name.
     *
     * \note Can be called only when the object is a zserio compound type.
     *
     * \param name Field schema name.
     *
     * \return The introspectable to the requested field.
     *
     * \throw CppRuntimeException When the object is not a compound type or when the field with
     *                            the given name doesn't exist or when the field getter itself throws.
     */
    virtual ConstPtr getField(std::string_view name) const = 0;

    /**
     * Gets name of the field which is active in the introspectable choice type.
     *
     * \note Applicable only on zserio unions and choices.
     *
     * \return Name of the active field (i.e. currently selected choice). Returns empty string for empty cases.
     *
     * \throw CppRuntimeException When the introspectable object is not a choice type (or union).
     */
    virtual std::string_view getChoice() const = 0;

    /**
     * Gets size of the introspective array.
     *
     * \note Can be called only when the introspective object is an array.
     *
     * \return Size of the introspective array.
     *
     * \throw CppRuntimeException When the introspective object is not an array.
     */
    virtual size_t size() const = 0;

    /**
     * Gets the introspectable to an array element.
     *
     * \note Can be called only when the introspective object is an array.
     *
     * \return The introspectable to an array element on the given index.
     *
     * \throw CppRuntimeException When the introspective object is not an array or when the given index is
     *                            out of bounds of the underlying array.
     */
    virtual ConstPtr at(size_t index) const = 0;

    /**
     * \copydoc IIntrospectableData::at
     *
     * Overloaded method provided for convenience.
     */
    virtual ConstPtr operator[](size_t index) const = 0;

    /**
     * Gets any value within the introspective object.
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
    virtual BasicAny<ALLOC> getAnyValue() const = 0;
    /** \} */

    /**
     * Gets bool value of the bool introspectable.
     *
     * \return Bool value.
     * \throw CppRuntimeException When the introspective object is not a bool type.
     */
    virtual bool getBool() const = 0;

    /**
     * Gets 8-bit signed integral value of the int8_t introspectable.
     *
     * \return 8-bit signed integral value.
     * \throw CppRuntimeException When the introspective object is not a int8_t type.
     */
    virtual int8_t getInt8() const = 0;

    /**
     * Gets 16-bit signed integral value of the int16_t introspectable.
     *
     * \return 16-bit signed integral value.
     * \throw CppRuntimeException When the introspective object is not a int16_t type.
     */
    virtual int16_t getInt16() const = 0;

    /**
     * Gets 32-bit signed integral value of the int32_t introspectable.
     *
     * \return 32-bit signed integral value.
     * \throw CppRuntimeException When the introspective object is not a int32_t type.
     */
    virtual int32_t getInt32() const = 0;

    /**
     * Gets 64-bit signed integral value of the int64_t introspectable.
     *
     * \return 64-bit signed integral value.
     * \throw CppRuntimeException When the introspective object is not a int64_t type.
     */
    virtual int64_t getInt64() const = 0;

    /**
     * Gets 8-bit unsigned integral value of the uint8_t introspectable.
     *
     * \return 8-bit unsigned integral value.
     * \throw CppRuntimeException When the introspective object is not a uint8_t type.
     */
    virtual uint8_t getUInt8() const = 0;

    /**
     * Gets 16-bit unsigned integral value of the uint16_t introspectable.
     *
     * \return 16-bit unsigned integral value.
     * \throw CppRuntimeException When the introspective object is not a uint16_t type.
     */
    virtual uint16_t getUInt16() const = 0;

    /**
     * Gets 32-bit unsigned integral value of the uint32_t introspectable.
     *
     * \return 32-bit unsigned integral value.
     * \throw CppRuntimeException When the introspective object is not a uint32_t type.
     */
    virtual uint32_t getUInt32() const = 0;

    /**
     * Gets 64-bit unsigned integral value of the uint64_t introspectable.
     *
     * \return 64-bit unsigned integral value.
     * \throw CppRuntimeException When the introspective object is not a uint64_t type.
     */
    virtual uint64_t getUInt64() const = 0;

    /**
     * Gets float value of the float introspectable.
     *
     * \return Float value.
     * \throw CppRuntimeException When the introspective object is not a float type.
     */
    virtual float getFloat() const = 0;

    /**
     * Gets double value of the double introspectable.
     *
     * \return Double value.
     * \throw CppRuntimeException When the introspective object is not a double type.
     */
    virtual double getDouble() const = 0;

    /**
     * Gets byte value of the bytes introspectable.
     *
     * \return Bytes value as a span.
     * \throw CppRuntimeException When the introspective object is not a bytes type.
     */
    virtual BytesView getBytes() const = 0;

    /**
     * Gets reference to the string value of the string introspectable.
     *
     * \return Reference to the string value.
     * \throw CppRuntimeException When the introspective object is not a string type.
     */
    virtual std::string_view getStringView() const = 0;

    /**
     * Gets reference to the introspective bit buffer.
     *
     * \return Reference to the bit buffer.
     * \throw CppRuntimeException When the introspective object is not a bit buffer (i.e. extern type).
     */
    virtual const BasicBitBuffer<ALLOC>& getBitBuffer() const = 0; // TODO[Mi-L@]: return BitBufferView?

    /**
     * Converts any signed integral value to 64-bit signed integer.
     *
     * Works also for enum types defined with signed underlying type.
     *
     * \return 64-bit signed integral value.
     * \throw CppRuntimeException When the introspective object cannot be converted to a signed integral value.
     */
    virtual int64_t toInt() const = 0;

    /**
     * Converts any unsigned integral value to 64-bit unsigned integer.
     *
     * Works also for bitmask and enum typed defined with unsigned underlying type.
     *
     * \return 64-bit unsigned integral value.
     * \throw CppRuntimeException When the introspective object cannot be converted to
     *                            an unsigned integral value.
     */
    virtual uint64_t toUInt() const = 0;

    /**
     * Converts any numeric value to double.
     *
     * Works also for bitmask and enum types.
     *
     * \return Double value.
     * \throw CppRuntimeException When the introspective object cannot be converted to double.
     */
    virtual double toDouble() const = 0;

    /**
     * Converts an introspective object to string.
     *
     * Works for all integral types including bool, bitmask and enum types and for string types.
     *
     * \note Floating point types are not currently supported!
     * \note The conversion to string can be implemented for more types in future versions.
     * \note Overload without parameter use default constructed allocator.
     *
     * \param allocator Allocator to use for the string allocation.
     *
     * \return String value representing the introspective object.
     */
    /** \{ */
    virtual BasicString<RebindAlloc<ALLOC, char>> toString(const ALLOC& allocator) const = 0;
    virtual BasicString<RebindAlloc<ALLOC, char>> toString() const = 0;
    /** \} */

    /**
     * Universal accessor to zserio entities within the zserio sub-tree represented by the introspectable
     * object.
     *
     * Supports dot notation corresponding to the tree defined in zserio language. Can access fields or
     * parameters or call functions within the zserio sub-tree.
     *
     * Examples:
     * * 'fieldA.param' - Gets introspectable to parameter 'param' within the parameterized field 'fieldA'.
     * * 'child.getValue' - Gets introspectable to result of the function called on field ̈́'child'.
     * * 'child.nonexisting.field' - Gets nullptr since the path doesn't represent a valid entity.
     *
     * \param path Dot notation corresponding to the zserio tree.
     *
     * \return Introspectable to the result of the given path. Returns nullptr when the path doesn't exist
     *         or when the requested operation throws CppRuntimeException.
     */
    virtual ConstPtr find(std::string_view path) const = 0;

    /**
     * \copydoc IIntrospectableData::find
     *
     * Overloaded method provided for convenience.
     */
    virtual ConstPtr operator[](std::string_view path) const = 0;
};

} // namespace zserio

#endif // ZSERIO_I_INTROSPECTABLE_DATA_INC
