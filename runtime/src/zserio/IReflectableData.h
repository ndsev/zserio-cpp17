#ifndef ZSERIO_I_REFLECTABLE_DATA_H_INC
#define ZSERIO_I_REFLECTABLE_DATA_H_INC

#include <memory>
#include <string_view>

#include "zserio/IIntrospectableData.h"

namespace zserio
{

/**
 * Interface for reflection of the Zserio objects using Data abstraction.
 *
 * This interface allows modification of all information available in the Data abstraction of the Zserio
 * objects.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicReflectableData : public IIntrospectableData<IBasicReflectableData<ALLOC>, ALLOC>
{
public:
    /** Shared pointer to the reflectable interface. */
    using Ptr = std::shared_ptr<IBasicReflectableData>;

    /** Shared pointer to the constant reflectable interface. */
    using ConstPtr = typename IIntrospectableData<IBasicReflectableData<ALLOC>, ALLOC>::ConstPtr;

    using IIntrospectableData<IBasicReflectableData<ALLOC>, ALLOC>::getField;
    using IIntrospectableData<IBasicReflectableData<ALLOC>, ALLOC>::at;
    using IIntrospectableData<IBasicReflectableData<ALLOC>, ALLOC>::operator[];
    using IIntrospectableData<IBasicReflectableData<ALLOC>, ALLOC>::getAnyValue;
    using IIntrospectableData<IBasicReflectableData<ALLOC>, ALLOC>::find;

    /**
     * Destructor.
     */
    ~IBasicReflectableData() override = default;

    /**
     * Gets reflectable to the field (i.e. member) with the given schema name.
     *
     * \note Can be called only when the reflected object is a zserio compound type.
     *
     * \param name Field schema name.
     *
     * \return Reflectable to the requested field.
     *
     * \throw CppRuntimeException When the reflected object is not a compound type or when the field with
     *                            the given name doesn't exist or when the field getter itself throws.
     */
    virtual Ptr getField(std::string_view name) = 0;

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
     * Gets reflectable to an array element.
     *
     * \note Can be called only when the reflected object is an array.
     *
     * \return Reflectable to an array element on the given index.
     *
     * \throw CppRuntimeException When the reflected object is not an array or when the given index is
     *                            out of bounds of the underlying array.
     */
    virtual Ptr at(size_t index) = 0;

    /**
     * \copydoc IBasicReflectableData::at
     *
     * Overloaded method provided for convenience.
     */
    virtual Ptr operator[](size_t index) = 0;

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
    virtual BasicAny<ALLOC> getAnyValue(const ALLOC& allocator) = 0;
    virtual BasicAny<ALLOC> getAnyValue() = 0;
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
    virtual Ptr find(std::string_view path) = 0;

    /**
     * \copydoc IIntrospectableData::find
     *
     * Overloaded method provided for convenience.
     */
    virtual Ptr operator[](std::string_view path) = 0;
};

/** Typedef to reflectable smart pointer needed for convenience in generated code. */
/** \{ */
template <typename ALLOC = std::allocator<uint8_t>>
using IBasicReflectableDataPtr = typename IBasicReflectableData<ALLOC>::Ptr;

template <typename ALLOC = std::allocator<uint8_t>>
using IBasicReflectableDataConstPtr = typename IBasicReflectableData<ALLOC>::ConstPtr;
/** \} */

/** Typedef to reflectable interface provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using IReflectableData = IBasicReflectableData<>;
using IReflectableDataPtr = IBasicReflectableDataPtr<>;
using IReflectableDataConstPtr = IBasicReflectableDataConstPtr<>;
/** \} */

namespace detail
{

template <typename T, typename ALLOC>
struct Reflectable;

} // namespace detail

/**
 * Gets reflectable for the given object.
 *
 * \param value Object value to reflect.
 * \param allocator Allocator to use for reflectable allocation.
 *
 * \return Reflectable to the given object.
 */
/** \{ */
template <typename T, typename ALLOC = typename T::allocator_type,
        std::enable_if_t<is_complete_v<View<T>> && has_allocator_v<std::decay_t<T>>, int> = 0>
IBasicReflectableDataConstPtr<ALLOC> reflectable(const T& value, const ALLOC& allocator = ALLOC())
{
    return detail::Reflectable<T, ALLOC>::create(value, allocator);
}

template <typename T, typename ALLOC = typename T::allocator_type,
        std::enable_if_t<is_complete_v<View<T>> && has_allocator_v<std::decay_t<T>>, int> = 0>
IBasicReflectableDataPtr<ALLOC> reflectable(T& value, const ALLOC& allocator = ALLOC())
{
    return detail::Reflectable<T, ALLOC>::create(value, allocator);
}

template <typename T, typename ALLOC = std::allocator<uint8_t>,
        std::enable_if_t<(std::is_enum_v<T> || is_bitmask_v<T>) && !has_allocator_v<std::decay_t<T>>, int> = 0>
IBasicReflectableDataPtr<ALLOC> reflectable(T value, const ALLOC& allocator = ALLOC())
{
    return detail::Reflectable<T, ALLOC>::create(value, allocator);
}
/** \} */

} // namespace zserio

#endif // ZSERIO_I_REFLECTABLE_DATA_H_INC
