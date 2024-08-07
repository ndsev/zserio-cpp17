#ifndef ZSERIO_VIEW_H_INC
#define ZSERIO_VIEW_H_INC

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>

namespace zserio
{

template <typename T>
class View;

namespace detail
{

template <typename T>
void validate(const T& value);

template <typename T>
void write(::zserio::BitStreamWriter& writer, const T& value);

template <typename T, typename... ARGS>
View<T> read(
        ::zserio::BitStreamReader& reader, T& data, const typename T::allocator_type& alloc, ARGS&&... args);

template <typename T, typename... ARGS>
View<T> read(::zserio::BitStreamReader& reader, typename T::ZserioPackingContext& context, T& data,
        const typename T::allocator_type& alloc, ARGS&&... args);

template <typename T>
size_t bitSizeOf(const T& value, size_t bitPosition);

template <typename T, typename... ARGS>
size_t initializeOffsets(T& data, size_t endBitPosition, ARGS&&... args);

template <typename T>
void initPackingContext(const T& value, typename T::ZserioPackingContext& context);

} // namespace detail

} // namespace zserio

#endif // ifndef ZSERIO_VIEW_H_INC
