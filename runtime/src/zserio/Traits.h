#ifndef ZSERIO_TRAITS_H_INC
#define ZSERIO_TRAITS_H_INC

#include <type_traits>

namespace zserio
{

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

} // namespace zserio

#endif // ifndef ZSERIO_TRAITS_H_INC
