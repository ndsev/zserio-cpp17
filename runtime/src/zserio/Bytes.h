#ifndef ZSERIO_BYTES_H_INC
#define ZSERIO_BYTES_H_INC

#include <cstdint>

#include "zserio/BitSize.h"
#include "zserio/Span.h"

namespace zserio
{

/**
 * Typedef for Zserio Bytes type - vector with uint8_t value.
 */
template <typename ALLOC = std::allocator<uint8_t>>
using BasicBytes = std::vector<uint8_t, ALLOC>;

/**
 * Typedef for Zserio Bytes type provided for convenience - using std::allocator<uint8_t>.
 */
using Bytes = BasicBytes<>;

/**
 * Typedef for View on Zserio Byte s type.
 */
using BytesView = zserio::Span<const uint8_t>;

namespace detail
{

BitSize bitSizeOf(BytesView bytesValue);

} // namespace detail

} // namespace zserio

#endif // ZSERIO_BYTES_H_INC
