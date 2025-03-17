#ifndef ZSERIO_I_INTROSPECTABLE_H_INC
#define ZSERIO_I_INTROSPECTABLE_H_INC

#include <memory>
#include <string_view>

#include "zserio/BitStreamWriter.h"

namespace zserio
{

/**
 * Interface for introspect the Zserio objects.
 *
 * This is read-only interface which offers all information about Zserio objects including parameters,
 * functions or writing.
 *
 * In another words, this interface is a read-only "reflections" implemented about read-only View abstraction.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicIntrospectable
{
public:
    /**
     * Destructor.
     */
    virtual ~IBasicIntrospectable() = default;

    /** Shared pointer to the constant introspectable interface. */
    using ConstPtr = std::shared_ptr<const IBasicIntrospectable>;

    /**
     * Initializes indexed offsets of the introspectable compound object.
     *
     * This method actually breaks the read-only concepts because it sets the offsets in the underlying data.
     *
     * \param bitPosition The bit stream position to be used for calculation.
     *
     * \throw CppRuntimeException When the introspectable object is not a compound.
     *
     * \return Updated bit position which points to the first bit after the compound.
     */
    virtual size_t initializeOffsets(size_t bitPosition) = 0;

    /**
     * Initializes indexed offsets of the introspectable compound object.
     *
     * This method actually breaks the read-only concepts because it sets the offsets in the underlying data.
     * The bit stream position to be used for calculation is defaulted to zero.
     *
     * \throw CppRuntimeException When the introspectable object is not a compound.
     *
     * \return Updated bit position which points to the first bit after the compound.
     */
    virtual size_t initializeOffsets() = 0;

    /**
     * Gets the number of bits needed for serialization of the introspectable object.
     *
     * \note Works for all introspectable types except arrays!
     *
     * \param bitPosition The bit stream position to be used for calculation.
     *
     * \throw CppRuntimeException When the introspectable object is an array.
     *
     * \return The size of the serialized introspectable object in bits.
     */
    virtual size_t bitSizeOf(size_t bitPosition) const = 0;

    /**
     * Gets the number of bits needed for serialization of the introspectable object.
     *
     * The bit stream position to be used for calculation is defaulted to zero.
     *
     * \note Works for all introspectable types except arrays!
     *
     * \throw CppRuntimeException When the introspectable object is an array.
     *
     * \return The size of the serialized introspectable object in bits.
     */
    virtual size_t bitSizeOf() const = 0;

    /**
     * Writes the introspectable object to a bit stream using the given bit stream writer.
     *
     * \note Works for all introspectable types except arrays!
     *
     * \param writer Bit stream writer to use.
     *
     * \throw CppRuntimeException When the introspectable object is an array.
     */
    virtual void write(BitStreamWriter& writer) const = 0;

    /**
     * Gets introspectable to the parameter (i.e. member) with the given schema name.
     *
     * \note Can be called only when the introspectable object is a zserio compound type.
     *
     * \param name Parameter schema name.
     *
     * \return Reflectable to the requested parameter.
     *
     * \throw CppRuntimeException When the introspectable object is not a compound type or when the parameter
     *                            with the given name doesn't exist or when the parameter getter itself throws.
     */
    virtual ConstPtr getParameter(std::string_view name) const = 0;

    /**
     * Calls function on the introspectable zserio object and gets introspectable view to its result.
     *
     * \note Can be called only when the introspectable object is a zserio compound type.
     *
     * \param name Function schema name.
     *
     * \return Reflectable view to the value returns from the called function.
     *
     * \throw CppRuntimeException When the introspectable object is not a compound type or when the function
     *                            with the given name doesn't exist or the the function call itself throws.
     */
    virtual ConstPtr callFunction(std::string_view name) const = 0;

    /**
     * Gets name of the field which is active in the introspectable choice type.
     *
     * \note Applicable only on zserio unions and choices.
     *
     * \return Name of the active field (i.e. currently selected choice).
     *
     * \throw CppRuntimeException When the introspectable object is not a choice type (or union).
     */
    virtual std::string_view getChoice() const = 0;

    /**
     * Universal accessor to zserio entities within the zserio sub-tree represented by the introspectable
     * object.
     *
     * Supports dot notation corresponding to the tree defined in zserio language. Can access fields or
     * parameters or call functions within the zserio sub-tree.
     *
     * Examples:
     * * 'fieldA.param' - Gets introspectable view to parameter 'param' within the parameterized field 'fieldA'.
     * * 'child.getValue' - Gets introspectable view to result of the function called on field ̈́'child'.
     * * 'child.nonexisting.field' - Gets nullptr since the path doesn't represent a valid entity.
     *
     * \param path Dot notation corresponding to the zserio tree.
     *
     * \return Introspectable to the result of the given path. Returns nullptr when the path doesn't exist
     *         or when the requested operation throws CppRuntimeException.
     */
    virtual ConstPtr find(std::string_view path) const = 0;

    /**
     * \copydoc IBasicIntrospectable::find
     *
     * Overloaded method provided for convenience.
     */
    virtual ConstPtr operator[](std::string_view path) const = 0;
};

/** Typedef to inspectable smart pointer needed for convenience in generated code. */
template <typename ALLOC = std::allocator<uint8_t>>
using IBasicIntrospectableConstPtr = typename IBasicIntrospectable<ALLOC>::ConstPtr;

/** Typedef to inspectable interface provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using IIntrospectable = IBasicIntrospectable<>;
using IIntrospectableConstPtr = IBasicIntrospectableConstPtr<>;
/** \} */

} // namespace zserio

#endif // ZSERIO_I_INTROSPECTABLE_H_INC
