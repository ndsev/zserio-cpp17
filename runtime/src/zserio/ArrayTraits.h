#ifndef ZSERIO_ARRAY_TRAIT_H_INC
#define ZSERIO_ARRAY_TRAIT_H_INC

#include "zserio/Types.h"

namespace zserio
{

/**
 * Array traits provides various functionality for all zserio and user generated types.
 *
 * This information is provided via specializations of the ArrayTraits strucure.
 */
template <typename VALUE_TYPE, typename = void>
struct ArrayTraits
{};

namespace detail
{

struct DummyArrayOwner
{};

template <typename T, typename = void>
struct array_owner_type
{
    using type = DummyArrayOwner;
};

template <typename T>
struct array_owner_type<T, std::void_t<typename ArrayTraits<T>::OwnerType>>
{
    using type = typename ArrayTraits<T>::OwnerType;
};

template <typename T, typename V = void>
using array_owner_type_t = typename array_owner_type<T, V>::type;

template <typename T>
struct is_dummy : std::is_same<T, DummyArrayOwner>
{};

template <typename T>
inline constexpr bool is_dummy_v = is_dummy<T>::value;

template <typename T, typename = void>
struct has_equal_method : std::false_type
{};

template <typename T>
struct has_equal_method<T, std::void_t<decltype(&T::equal)>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool has_equal_method_v = has_equal_method<T, V>::value;

/**
 * Trait used to check whether the type has an OwnerType.
 * \{
 */
template <typename T, typename = void>
struct has_view_type : std::false_type
{};

template <typename T>
struct has_view_type<T, std::void_t<typename T::ViewType>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool has_view_type_v = has_view_type<T, V>::value;
/** \} */

} // namespace detail

template <typename T>
struct NativeArrayTraits
{
    static constexpr T at(detail::DummyArrayOwner, T element, size_t)
    {
        return element;
    }
};

template <typename T>
struct ArrayTraits<T,
        std::enable_if_t<std::is_base_of_v<detail::NumericTypeWrapper<typename T::value_type>, T>>>
        : NativeArrayTraits<T>
{};

} // namespace zserio

#endif // ZSERIO_ARRAY_TRAIT_H_INC
