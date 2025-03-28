#ifndef ZSERIO_TRAITS_H_INC
#define ZSERIO_TRAITS_H_INC

#include <type_traits>

namespace zserio
{

// forward declarations
template <typename, std::size_t>
class Span;

namespace detail
{

template <typename VALUE_TYPE>
class NumericTypeWrapper;

} // namespace detail

/**
 * Trait used to check whether the type T is an allocator.
 * \{
 */
template <typename T, typename = void>
struct is_allocator : std::false_type
{};

template <typename T>
struct is_allocator<T,
        std::void_t<decltype(std::declval<T>().allocate(0)),
                decltype(std::declval<T>().deallocate(nullptr, 0))>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool is_allocator_v = is_allocator<T, V>::value;
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
struct has_std_allocator : std::false_type
{};

template <typename T>
struct has_std_allocator<T, std::void_t<typename T::allocator_type>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool has_std_allocator_v = has_std_allocator<T, V>::value;

template <typename T, typename = void>
struct has_zs_allocator : std::false_type
{};

template <typename T>
struct has_zs_allocator<T, std::void_t<typename T::AllocatorType>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool has_zs_allocator_v = has_zs_allocator<T, V>::value;

template <typename T, typename V = void>
struct has_allocator : std::disjunction<has_std_allocator<T, V>, has_zs_allocator<T, V>>
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
 * */
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

} // namespace zserio

#endif // ifndef ZSERIO_TRAITS_H_INC
