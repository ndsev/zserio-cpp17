#ifndef ZSERIO_ARRAY_TRAIT_H_INC
#define ZSERIO_ARRAY_TRAIT_H_INC

#include <cstddef>
#include <string_view>
#include <type_traits>

#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Bitmasks.h"
#include "zserio/DeltaContext.h"
#include "zserio/Enums.h"
#include "zserio/Traits.h"
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
struct is_dummy_array_owner : std::is_same<DummyArrayOwner, T>
{};

template <typename T>
inline constexpr bool is_dummy_array_owner_v = is_dummy_array_owner<T>::value;

template <typename T, typename = void>
struct is_packable : std::false_type
{};

template <typename T>
struct is_packable<T, std::enable_if_t<is_numeric_wrapper_v<T> || is_bitmask_v<T> || std::is_enum_v<T>>>
        : std::true_type
{};

template <typename T>
struct is_packable<T, std::enable_if_t<is_complete_v<typename ObjectTraits<std::decay_t<T>>::PackingContext>>>
        : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool is_packable_v = is_packable<T, V>::value;

template <typename T>
struct is_delta_context : std::is_same<DeltaContext, T>
{};

template <typename T>
inline constexpr bool is_delta_context_v = is_delta_context<T>::value;

template <typename T>
struct NumericArrayTraits
{
    static const T& at(const detail::DummyArrayOwner&, const T& element, size_t)
    {
        return element;
    }

    static T& at(const detail::DummyArrayOwner&, T& element, size_t)
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

} // namespace detail

// for all generated objects which don't need an owner
template <typename OBJECT, typename>
struct ArrayTraits
{
    template <typename OBJECT_ = OBJECT,
            std::enable_if_t<std::is_constructible_v<View<OBJECT_>, const OBJECT&>, int> = 0>
    static View<OBJECT> at(const detail::DummyArrayOwner&, const OBJECT& element, size_t)
    {
        return View<OBJECT>(element);
    }

    template <typename OBJECT_ = OBJECT,
            std::enable_if_t<std::is_constructible_v<View<OBJECT_>, OBJECT&>, int> = 0>
    static View<OBJECT> at(const detail::DummyArrayOwner&, OBJECT& element, size_t)
    {
        return View<OBJECT>(element);
    }

    static void read(BitStreamReader& reader, const detail::DummyArrayOwner&, OBJECT& element, size_t)
    {
        (void)detail::read(reader, element);
    }

    template <typename OBJECT_ = OBJECT>
    static std::enable_if_t<!detail::is_delta_context_v<detail::packing_context_type_t<OBJECT_>>> read(
            typename detail::ObjectTraits<OBJECT_>::PackingContext& packingContext, BitStreamReader& reader,
            const detail::DummyArrayOwner&, OBJECT& element, size_t)
    {
        detail::read(packingContext, reader, element);
    }
};

template <>
struct ArrayTraits<detail::BoolWrapper> : detail::IntegralArrayTraits<detail::BoolWrapper>
{};

template <BitSize BIT_SIZE, bool IS_SIGNED>
struct ArrayTraits<detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>>
        : detail::IntegralArrayTraits<detail::FixedIntWrapper<BIT_SIZE, IS_SIGNED>>
{};

template <typename VALUE_TYPE, typename detail::VarIntType VAR_TYPE>
struct ArrayTraits<detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
        : detail::IntegralArrayTraits<detail::VarIntWrapper<VALUE_TYPE, VAR_TYPE>>
{};

template <typename VALUE_TYPE, detail::FloatType FLOAT_TYPE>
struct ArrayTraits<detail::FloatWrapper<VALUE_TYPE, FLOAT_TYPE>>
        : detail::NumericArrayTraits<detail::FloatWrapper<VALUE_TYPE, FLOAT_TYPE>>
{};

template <typename ALLOC>
struct ArrayTraits<BasicBytes<ALLOC>>
{
    static constexpr BytesView at(const detail::DummyArrayOwner&, const BasicBytes<ALLOC>& element, size_t)
    {
        return element;
    }

    static void read(
            BitStreamReader& reader, const detail::DummyArrayOwner&, BasicBytes<ALLOC>& element, size_t)
    {
        detail::read(reader, element);
    }
};

template <typename ALLOC>
struct ArrayTraits<BasicBitBuffer<ALLOC>>
{
    static constexpr BasicBitBufferView<ALLOC> at(
            const detail::DummyArrayOwner&, const BasicBitBuffer<ALLOC>& element, size_t)
    {
        return element;
    }

    static void read(
            BitStreamReader& reader, const detail::DummyArrayOwner&, BasicBitBuffer<ALLOC>& element, size_t)
    {
        detail::read(reader, element);
    }
};

template <typename ALLOC>
struct ArrayTraits<BasicString<ALLOC>>
{
    static constexpr std::string_view at(const detail::DummyArrayOwner&, std::string_view element, size_t)
    {
        return element;
    }

    static void read(
            BitStreamReader& reader, const detail::DummyArrayOwner&, BasicString<ALLOC>& element, size_t)
    {
        detail::read(reader, element);
    }
};

template <typename T>
struct ArrayTraits<T, std::enable_if_t<std::is_enum_v<T>>>
{
    static T at(const detail::DummyArrayOwner&, T element, size_t)
    {
        return element;
    }

    static void read(BitStreamReader& reader, const detail::DummyArrayOwner&, T& element, size_t)
    {
        detail::read(reader, element);
    }

    static void read(detail::DeltaContext& context, BitStreamReader& reader, const detail::DummyArrayOwner&,
            T& element, size_t)
    {
        detail::read(context, reader, element);
    }
};

template <typename T>
struct ArrayTraits<T, std::enable_if_t<zserio::is_bitmask_v<T>>>
{
    static T at(const detail::DummyArrayOwner&, T element, size_t)
    {
        return element;
    }

    static void read(BitStreamReader& reader, const detail::DummyArrayOwner&, T& element, size_t)
    {
        detail::read(reader, element);
    }

    static void read(detail::DeltaContext& context, BitStreamReader& reader, const detail::DummyArrayOwner&,
            T& element, size_t)
    {
        detail::read(context, reader, element);
    }
};

} // namespace zserio

#endif // ZSERIO_ARRAY_TRAIT_H_INC
