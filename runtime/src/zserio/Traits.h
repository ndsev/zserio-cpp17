#ifndef ZSERIO_TRAITS_H_INC
#define ZSERIO_TRAITS_H_INC

#include <type_traits>

namespace zserio
{

namespace detail
{

// declval is needed because otherwise MSVC 2015 states that std::allocator<int> does NOT have allocate method!
template <typename T, typename U = decltype(std::declval<T>().allocate(0))>
struct decltype_allocate
{
    using type = U;
};

template <typename T, typename U = decltype(std::declval<T>().deallocate(nullptr, 0))>
struct decltype_deallocate
{
    using type = U;
};

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
        std::void_t<typename detail::decltype_allocate<T>::type, typename detail::decltype_deallocate<T>::type>>
        : std::true_type
{};
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
/** \} */

} // namespace zserio

#endif // ifndef ZSERIO_TRAITS_H_INC
