#ifndef ZSERIO_TRAITS_H_INC
#define ZSERIO_TRAITS_H_INC

#include <cstddef>
#include <type_traits>

namespace zserio
{

// forward declarations
template <typename, std::size_t>
class Span;

template <typename T>
class View;

template <typename T, typename ARRAY_TRAITS>
class ArrayView;

template <typename ALLOC, typename T>
class BasicOptional;

template <typename T>
class Extended;

namespace detail
{

template <typename VALUE_TYPE>
class NumericTypeWrapper;

template <typename VALUE_TYPE>
class DynIntWrapper;

template <typename T, typename = void>
struct is_allocator_impl : std::false_type
{};

template <typename T>
struct is_allocator_impl<T,
        std::void_t<typename T::value_type, decltype(std::declval<T>().allocate(0)),
                decltype(std::declval<T>().deallocate(std::declval<typename T::value_type*>(), 0))>>
        : std::true_type
{};

template <typename T>
struct is_array_view : std::false_type
{};

template <typename T, typename ARRAY_TRAITS>
struct is_array_view<ArrayView<T, ARRAY_TRAITS>> : std::true_type
{};

template <typename T>
constexpr bool is_array_view_v = is_array_view<T>::value;

template <typename T>
struct is_view : std::false_type
{};

template <typename T>
struct is_view<View<T>> : std::true_type
{};

template <typename T>
constexpr bool is_view_v = is_view<T>::value;

template <typename T>
struct needs_offset_reference : std::negation<is_view<T>>
{};

template <typename T>
struct needs_offset_reference<Extended<T>>
{
    static constexpr bool value = needs_offset_reference<T>::value;
};

template <typename ALLOC, typename T>
struct needs_offset_reference<BasicOptional<ALLOC, T>>
{
    static constexpr bool value = needs_offset_reference<T>::value;
};

template <typename T>
constexpr bool needs_offset_reference_v = needs_offset_reference<T>::value;

} // namespace detail

/**
 * Trait used to check whether the type T is an allocator.
 * \{
 */
template <typename T>
struct is_allocator : detail::is_allocator_impl<std::decay_t<T>>
{};

template <typename T>
inline constexpr bool is_allocator_v = is_allocator<T>::value;
/** \} */

/**
 * Trait used to check whether the first type of ARGS is an allocator.
 * \{
 */
template <typename... ARGS>
struct is_first_allocator : std::false_type
{};

template <typename T, typename... ARGS>
struct is_first_allocator<T, ARGS...> : is_allocator<T>
{};

template <typename... ARGS>
inline constexpr bool is_first_allocator_v = is_first_allocator<ARGS...>::value;
/** \} */

/**
 * Trait used to check whether the type has an allocator_type.
 * \{
 */
template <typename T, typename = void>
struct has_allocator : std::false_type
{};

template <typename T>
struct has_allocator<T, std::void_t<typename T::allocator_type>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool has_allocator_v = has_allocator<T, V>::value;
/** \} */

/**
 * Trait used to check whether the type T is a zserio bitmask.
 * \{
 */
template <typename T, typename = void>
struct is_bitmask : std::false_type
{};

template <typename T>
struct is_bitmask<T, std::void_t<decltype(std::declval<T>().getValue()), typename T::ZserioType>>
        : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool is_bitmask_v = is_bitmask<T, V>::value;
/**
 * \}
 */

/**
 * Trait used to check whether the type T is a Span.
 * \{
 */
template <typename>
struct is_span : std::false_type
{};

template <typename T, size_t Extent>
struct is_span<Span<T, Extent>> : std::true_type
{};

template <typename T>
inline constexpr bool is_span_v = is_span<T>::value;
/**
 * \}
 */

/**
 * Trait used to check whether the type T is a zserio numeric wrapper.
 */
template <typename T, typename = void>
struct is_numeric_wrapper : std::false_type
{};

template <typename T>
struct is_numeric_wrapper<T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::ValueType>, T>>>
        : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool is_numeric_wrapper_v = is_numeric_wrapper<T, V>::value;

/**
 * Trait used to check whether the type T is a zserio dynamic integer wrapper.
 */
template <typename T, typename = void>
struct is_dyn_int_wrapper : std::false_type
{};

template <typename T>
struct is_dyn_int_wrapper<T,
        std::enable_if_t<std::is_base_of_v<detail::DynIntWrapper<typename T::ValueType>, T>>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool is_dyn_int_wrapper_v = is_dyn_int_wrapper<T, V>::value;

/**
 * Trait used to check whether the type T is complete (defined)
 */
/** \{ */
template <typename T, typename = void>
struct is_complete : std::false_type
{};

template <typename T>
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type
{};

template <typename T>
constexpr bool is_complete_v = is_complete<T>::value;
/** \} */

/**
 * Traits used to get proper View type.
 * Results in View<T> when the View specialization exists, otherwise remains T.
 */
/** \{ */
template <typename T, typename = void>
struct view_type
{
    using type = T;
};

template <typename T>
struct view_type<T, std::enable_if_t<is_complete_v<View<T>>>>
{
    using type = View<T>;
};

template <typename T, typename V = void>
using view_type_t = typename view_type<T, V>::type;
/** \} */

/**
 * Trait used to get reference for fields which are used as offsets when the reference is needed.
 * When the underlying type is an View, reference is not needed. Otherwise the reference is necessary.
 */
/** \{ */
template <typename T, typename = void>
struct offset_field_reference
{
    using type = T;
};

template <typename T>
struct offset_field_reference<T, std::enable_if_t<detail::needs_offset_reference_v<T>>>
{
    using type = T&;
};

template <typename T, typename V = void>
using offset_field_reference_t = typename offset_field_reference<T, V>::type;
/** \} */

/**
 * Helper function to construct object of generic type T with allocator argument if supported.
 * When the type T is not constructible with allocator, the allocator argument is ignored.
 *
 * \param allocator Allocator to use if constructor with an allocator exists.
 * \param args Other arguments to constructor.
 */
template <typename T, typename ALLOC, typename... ARGS>
constexpr T constructWithAllocator(const ALLOC& allocator, ARGS&&... args)
{
    if constexpr (std::is_constructible_v<T, ARGS..., ALLOC>)
    {
        return T(std::forward<ARGS>(args)..., allocator);
    }
    else
    {
        return T(std::forward<ARGS>(args)...);
    }
}

/**
 * Overload needed to suppress conversion warnings when constructing numeric types.
 *
 * \param allocator Allocator which is always ignored in this overload.
 * \param value Value of type U convertible to the ValueType of numeric wrapper T.
 */
template <typename T, typename ALLOC, typename U, std::enable_if_t<is_numeric_wrapper_v<T>, int> = 0>
constexpr T constructWithAllocator(const ALLOC&, U value)
{
    return T(static_cast<typename T::ValueType>(value));
}

} // namespace zserio

#endif // ifndef ZSERIO_TRAITS_H_INC
