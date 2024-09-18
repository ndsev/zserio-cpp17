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

template <typename T, typename V = void>
inline constexpr bool is_first_allocator_v = is_first_allocator<T, V>::value;
/** \} */

} // namespace zserio

#endif // ifndef ZSERIO_TRAITS_H_INC
