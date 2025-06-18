#ifndef ZSERIO_I_INTROSPECTABLE_VIEW_INC
#define ZSERIO_I_INTROSPECTABLE_VIEW_INC

#include <memory>
#include <string_view>

#include "zserio/BitSize.h"
#include "zserio/IIntrospectableData.h"
#include "zserio/View.h"

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
    ~IBasicIntrospectableView() override = default;

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
     * Serializes the introspectable compound object.
     *
     * \param allocator Allocator to use for the value allocation.
     *
     * \throw CppRuntimeException When the introspectable object is not a compound.
     *
     * \return Allocated bit buffer containing the serialized compound object.
     */
    /** \{ */
    virtual BasicBitBuffer<ALLOC> serialize(const ALLOC& allocator) const = 0;
    virtual BasicBitBuffer<ALLOC> serialize() const = 0;
    /** \} */
};

/** Typedef to introspectable smart pointer needed for convenience in generated code. */
template <typename ALLOC = std::allocator<uint8_t>>
using IBasicIntrospectableViewConstPtr = typename IBasicIntrospectableView<ALLOC>::ConstPtr;

/** Typedef to introspectable interface provided for convenience - using default std::allocator<uint8_t>. */
/** \{ */
using IIntrospectableView = IBasicIntrospectableView<>;
using IIntrospectableViewConstPtr = IBasicIntrospectableViewConstPtr<>;
/** \} */

namespace detail
{

template <typename T, typename ALLOC>
struct Introspectable;

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
template <typename T, typename ALLOC = typename T::allocator_type>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(const View<T>& view, const ALLOC& allocator = ALLOC())
{
    return detail::Introspectable<T, ALLOC>::create(view, allocator);
}

template <typename T, typename ALLOC = std::allocator<uint8_t>,
        std::enable_if_t<std::is_enum_v<T> || is_bitmask_v<T>, int> = 0>
IBasicIntrospectableViewConstPtr<ALLOC> introspectable(T value, const ALLOC& allocator = ALLOC())
{
    return detail::Introspectable<T, ALLOC>::create(value, allocator);
}
/** \} */

} // namespace zserio

#endif // ZSERIO_I_INTROSPECTABLE_VIEW_INC
