#ifndef ZSERIO_ARRAY_TRAIT_H_INC
#define ZSERIO_ARRAY_TRAIT_H_INC

#include <cstddef>
#include <type_traits>

#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Types.h"
#include "zserio/View.h"

namespace zserio
{

/**
 * Array traits provides various functionality for all zserio and user generated types.
 *
 * This information is provided via specializations of the ArrayTraits strucure.
 */
template <typename T, typename = void>
struct ArrayTraits;

namespace detail
{

struct DummyArrayOwner
{};

template <typename ARRAY_TRAITS, typename = void>
struct array_owner_type
{
    using type = DummyArrayOwner;
};

template <typename ARRAY_TRAITS>
struct array_owner_type<ARRAY_TRAITS, std::void_t<typename ARRAY_TRAITS::OwnerType>>
{
    using type = typename ARRAY_TRAITS::OwnerType;
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
// TODO[Mi-L@]: Remove if not needed.
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

template <typename T, typename>
struct ArrayTraits
{
    static View<T> at(const detail::DummyArrayOwner&, const T& element, size_t)
    {
        return View<T>(element);
    }

    static void read(zserio::BitStreamReader& reader, const detail::DummyArrayOwner&, T& element, size_t)
    {
        (void)detail::read(reader, element);
    }
};

template <typename T>
struct NativeArrayTraits
{
    static constexpr T at(const detail::DummyArrayOwner&, T element, size_t)
    {
        return element;
    }

    static void read(zserio::BitStreamReader& reader, const detail::DummyArrayOwner&, T& element, size_t)
    {
        detail::read(reader, element);
    }

    static BitSize bitSizeOf()
    {
        return detail::bitSizeOf(T{});
    }
};

template <>
struct ArrayTraits<detail::BoolWrapper> : NativeArrayTraits<detail::BoolWrapper>
{};

template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct ArrayTraits<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>
        : NativeArrayTraits<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>
{};

template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct ArrayTraits<detail::DynIntWrapper<VALUE_TYPE, BIT_SIZE>, std::enable_if_t<(BIT_SIZE > 0)>>
        : NativeArrayTraits<detail::DynIntWrapper<VALUE_TYPE, BIT_SIZE>>
{};

template <typename VALUE_TYPE, typename detail::VarIntType VAR_TYPE>
struct ArrayTraits<detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
        : NativeArrayTraits<detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
{};

template <typename VALUE_TYPE, detail::FloatType FLOAT_TYPE>
struct ArrayTraits<detail::FloatWrapper<VALUE_TYPE, FLOAT_TYPE>>
        : NativeArrayTraits<detail::FloatWrapper<VALUE_TYPE, FLOAT_TYPE>>
{};

template <>
struct ArrayTraits<Bytes>
{
    static constexpr Span<const uint8_t> at(const detail::DummyArrayOwner&, const Bytes& element, size_t)
    {
        return element;
    }

    static void read(zserio::BitStreamReader& reader, const detail::DummyArrayOwner&, Bytes& element, size_t)
    {
        detail::read(reader, element);
    }
};

template <typename ALLOC>
struct ArrayTraits<std::basic_string<char, std::char_traits<char>, ALLOC>>
{
    static constexpr std::string_view at(
            const detail::DummyArrayOwner&, const basic_string<ALLOC>& element, size_t)
    {
        return element;
    }

    static void read(zserio::BitStreamReader& reader, const detail::DummyArrayOwner&,
            basic_string<ALLOC>& element, size_t)
    {
        detail::read(reader, element);
    }
};

} // namespace zserio

#endif // ZSERIO_ARRAY_TRAIT_H_INC
