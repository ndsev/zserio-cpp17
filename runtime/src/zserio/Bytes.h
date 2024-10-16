#ifndef ZSERIO_BYTES_H_INC
#define ZSERIO_BYTES_H_INC

#include "zserio/BitSize.h"
#include "zserio/Span.h"

namespace zserio
{

namespace detail
{

BitSize bitSizeOf(Span<const uint8_t> bytesValue);

} // namespace detail

} // namespace zserio

#endif // ZSERIO_BYTES_H_INC
