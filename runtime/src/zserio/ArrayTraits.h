#ifndef ZSERIO_ARRAY_TRAIT_H_INC
#define ZSERIO_ARRAY_TRAIT_H_INC

#include <cstddef>
#include <type_traits>

#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/DeltaContext.h"
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

/**
 * Packed array traits provides packing related functionality for all zserio and user generated types.
 *
 * This information is provided via specializations of the PackedArrayTraits strucure.
 */
template <typename ARRAY_TRAITS, typename = void>
struct PackedArrayTraits;

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

// helper trait to choose packing context type for an array from an element type T
template <typename T, typename = void>
struct packing_context_type
{
    using type = DeltaContext;
};

template <typename T>
struct packing_context_type<T, typename std::enable_if<has_zserio_packing_context<View<T>>::value>::type>
{
    using type = typename View<T>::ZserioPackingContext;
};

template <typename T, typename V = void>
using packing_context_type_t = typename packing_context_type<T, V>::type;

} // namespace detail

// for all generated objects which don't need an owner
template <typename OBJECT, typename>
struct ArrayTraits
{
    static View<OBJECT> at(const detail::DummyArrayOwner&, const OBJECT& element, size_t)
    {
        return View<OBJECT>(element);
    }

    static void read(BitStreamReader& reader, const detail::DummyArrayOwner&, OBJECT& element, size_t)
    {
        (void)detail::read(reader, element);
    }

    template <typename OBJECT_ = OBJECT>
    static std::enable_if_t<has_zserio_packing_context_v<View<OBJECT_>>> read(
            typename View<OBJECT_>::ZserioPackingContext& packingContext, BitStreamReader& reader,
            const detail::DummyArrayOwner&, OBJECT& element, size_t)
    {
        detail::read(packingContext, reader, element);
    }
};

template <typename T>
struct NumericArrayTraits
{
    static constexpr T at(const detail::DummyArrayOwner&, T element, size_t)
    {
        return element;
    }

    static void read(BitStreamReader& reader, const detail::DummyArrayOwner&, T& element, size_t)
    {
        detail::read(reader, element);
    }

    static BitSize bitSizeOf()
    {
        return detail::bitSizeOf(T{});
    }
};

template <typename T>
struct IntegralArrayTraits : NumericArrayTraits<T>
{
    using NumericArrayTraits<T>::read;

    static void read(
            DeltaContext& context, BitStreamReader& reader, const detail::DummyArrayOwner&, T& element, size_t)
    {
        detail::read(context, reader, element);
    }
};

template <>
struct ArrayTraits<detail::BoolWrapper> : IntegralArrayTraits<detail::BoolWrapper>
{};

template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct ArrayTraits<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>
        : IntegralArrayTraits<detail::IntWrapper<VALUE_TYPE, BIT_SIZE>>
{};

template <typename VALUE_TYPE, BitSize BIT_SIZE>
struct ArrayTraits<detail::DynIntWrapper<VALUE_TYPE, BIT_SIZE>, std::enable_if_t<(BIT_SIZE > 0)>>
        : IntegralArrayTraits<detail::DynIntWrapper<VALUE_TYPE, BIT_SIZE>>
{};

template <typename VALUE_TYPE, typename detail::VarIntType VAR_TYPE>
struct ArrayTraits<detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
        : IntegralArrayTraits<detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
{};

template <typename VALUE_TYPE, detail::FloatType FLOAT_TYPE>
struct ArrayTraits<detail::FloatWrapper<VALUE_TYPE, FLOAT_TYPE>>
        : NumericArrayTraits<detail::FloatWrapper<VALUE_TYPE, FLOAT_TYPE>>
{};

template <>
struct ArrayTraits<Bytes>
{
    static constexpr Span<const uint8_t> at(const detail::DummyArrayOwner&, const Bytes& element, size_t)
    {
        return element;
    }

    static void read(BitStreamReader& reader, const detail::DummyArrayOwner&, Bytes& element, size_t)
    {
        detail::read(reader, element);
    }
};

template <typename ALLOC>
struct ArrayTraits<std::basic_string<char, std::char_traits<char>, ALLOC>>
{
    static constexpr std::string_view at(
            const detail::DummyArrayOwner&, const BasicString<ALLOC>& element, size_t)
    {
        return element;
    }

    static void read(
            BitStreamReader& reader, const detail::DummyArrayOwner&, BasicString<ALLOC>& element, size_t)
    {
        detail::read(reader, element);
    }
};

} // namespace zserio

#endif // ZSERIO_ARRAY_TRAIT_H_INC
