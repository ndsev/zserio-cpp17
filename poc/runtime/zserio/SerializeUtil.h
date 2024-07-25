/**
 * \file
 * It provides help methods for serialization and deserialization of generated objects.
 *
 * These utilities are not used by generated code and they are provided only for user convenience.
 *
 * \note Please note that file operations allocate memory as needed and are not designed to use allocators.
 */

#ifndef ZSERIO_SERIALIZE_UTIL_H_INC
#define ZSERIO_SERIALIZE_UTIL_H_INC

#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/FileUtil.h"
#include "zserio/Traits.h"
#include "zserio/Vector.h"
#include "zserio/DataView.h"

namespace zserio
{

namespace detail
{
    struct dummy;

    template<typename T, typename... ARGS>
    dummy& initializeOffsets(T& data, size_t endBitPosition, ARGS&&... arguments);

    template<typename T, typename... ARGS>
    constexpr bool is_initialize_offsets_defined(T& data, size_t endBitPosition, ARGS&&... arguments)
    {
        using returnedType = decltype(initializeOffsets(data, endBitPosition,
                ::std::forward<ARGS>(arguments)...));
        return std::is_same<returnedType, size_t>();
    }
} // namespace detail

template <typename... ARGS>
struct is_first_alloc : is_first_allocator<::std::decay_t<ARGS>...>
{};

template <typename T, typename ALLOC, typename... ARGS,
    typename = ::std::enable_if_t<!::std::is_enum_v<T> && is_allocator<ALLOC>::value &&
        !T::needs_initialize_offsets::value>>
BasicBitBuffer<ALLOC> serialize(const T& data, const ALLOC& allocator, ARGS&&... arguments)
{
    const View<T> view(data, ::std::forward<ARGS>(arguments)...);
    size_t bitSize = detail::bitSizeOf(view, 0);
    BasicBitBuffer<ALLOC> buffer(bitSize, allocator);
    BitStreamWriter writer(buffer);
    detail::validate(view);
    detail::write(writer, view);
    return buffer;
}

template <typename T, typename ALLOC, typename... ARGS,
    typename = ::std::enable_if_t<!::std::is_enum_v<T> && is_allocator<ALLOC>::value &&
        T::needs_initialize_offsets::value>>
BasicBitBuffer<ALLOC> serialize(T& data, const ALLOC& allocator, ARGS&&... arguments)
{
    size_t bitSize = detail::initializeOffsets(data, 0, ::std::forward<ARGS>(arguments)...);
    BasicBitBuffer<ALLOC> buffer(bitSize, allocator);
    BitStreamWriter writer(buffer);
    const View<T> view(data, ::std::forward<ARGS>(arguments)...);
    detail::validate(view);
    detail::write(writer, view);
    return buffer;
}

template <typename T, typename... ARGS,
    typename ::std::enable_if_t<!::std::is_enum_v<T> && !is_first_alloc<ARGS...>::value, int> = 0>
BasicBitBuffer<typename T::allocator_type> serialize(T& data, ARGS&&... arguments)
{
    return serialize(data, typename T::allocator_type(), ::std::forward<ARGS>(arguments)...);
}

// allocator variant is not needed as allocator is already embed in buffer
template <typename T, typename ALLOC, typename... ARGS,
    typename ::std::enable_if_t<!::std::is_enum<T>::value, int> = 0>
typename ::zserio::View<T> deserialize(const BasicBitBuffer<ALLOC>& buffer, T& data, ARGS&&... arguments)
{
    BitStreamReader reader(buffer);
    return detail::read(reader, data, ::std::forward<ARGS>(arguments)...);
}

template <typename T, typename ALLOC>
BasicBitBuffer<ALLOC> serialize(const DataView<T>& dataView, const ALLOC& allocator)
{
    // there is no need to set offsets or call validation here, DataView is already consistent
    size_t bitSize = detail::bitSizeOf(dataView, 0);
    BasicBitBuffer<ALLOC> buffer(bitSize, allocator);
    BitStreamWriter writer(buffer);

    detail::write(writer, dataView);
    return buffer;
}

template <typename T>
BasicBitBuffer<typename T::allocator_type> serialize(const DataView<T>& dataView)
{
    return serialize(dataView, typename T::allocator_type());
}

template <typename T, typename ALLOC, typename... ARGS,
    typename = ::std::enable_if_t<!::std::is_enum<T>::value>>
DataView<T> deserialize(const BasicBitBuffer<ALLOC>& buffer, ARGS&&... arguments)
{
    T data;
    deserialize(buffer, data, ::std::forward<ARGS>(arguments)...);

    // there is no need to call validation here, everything is consistent after reading
    return DataView(data, detail::SkipValidation, ::std::forward<ARGS>(arguments)...);
}

} // namespace zserio

#endif // ZSERIO_SERIALIZE_UTIL_H_INC
