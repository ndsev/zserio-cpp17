#ifndef ZSERIO_I_INTROSPECTABLE_VIEW_INC
#define ZSERIO_I_INTROSPECTABLE_VIEW_INC

#include <memory>
#include <string_view>

#include "zserio/IIntrospectableData.h"

namespace zserio
{

/**
 * Interface to introspect View abstraction of the Zserio objects.
 *
 * This is read-only interface which offers all information available from the View abstraction of the Zserio
 * objects including parameters, functions and serialization feature.
 */
template <typename ALLOC = std::allocator<uint8_t>>
class IBasicIntrospectableView : public IIntrospectableData<IBasicIntrospectableView<ALLOC>, ALLOC>
{
public:
    /** Shared pointer to the constant introspectable interface. */
    using ConstPtr = typename IIntrospectableData<IBasicIntrospectableView<ALLOC>, ALLOC>::ConstPtr;

    /**
     * Destructor.
     */
    virtual ~IBasicIntrospectableView() = default;

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
    virtual size_t initializeOffsets(size_t bitPosition) const = 0;

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
    virtual size_t initializeOffsets() const = 0;

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
};

/** Typedef to introspectable smart pointer needed for convenience in generated code. */
template <typename ALLOC = std::allocator<uint8_t>>
using IBasicIntrospectableViewConstPtr = typename IBasicIntrospectableView<ALLOC>::ConstPtr;

/** Typedef to introspectable interface provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using IIntrospectableView = IBasicIntrospectableView<>;
using IIntrospectableViewConstPtr = IBasicIntrospectableViewConstPtr<>;
/** \} */

} // namespace zserio

#endif // ZSERIO_I_INTROSPECTABLE_VIEW_INC
