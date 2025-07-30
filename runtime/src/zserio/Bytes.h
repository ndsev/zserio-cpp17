#ifndef ZSERIO_BYTES_H_INC
#define ZSERIO_BYTES_H_INC

#include <algorithm>
#include <cstdint>
#include <string_view>
#include <vector>

#include "zserio/BitSize.h"
#include "zserio/Span.h"
#include "zserio/Traits.h"

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
using BytesView = Span<const uint8_t>;

template <typename ALLOC>
struct view_type<BasicBytes<ALLOC>>
{
    using type = BytesView;
};

inline bool operator==(const BytesView& lhs, const BytesView& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

inline bool operator!=(const BytesView& lhs, const BytesView& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const BytesView& lhs, const BytesView& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

inline bool operator>(const BytesView& lhs, const BytesView& rhs)
{
    return rhs < lhs;
}

inline bool operator<=(const BytesView& lhs, const BytesView& rhs)
{
    return !(rhs < lhs);
}

inline bool operator>=(const BytesView& lhs, const BytesView& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

void validate(BytesView stringValue, std::string_view fieldName);
BitSize bitSizeOf(BytesView bytesValue, BitSize bitPosition = 0);
BitSize initializeOffsets(BytesView bytesValue, BitSize bitPosition);

} // namespace detail

} // namespace zserio

#endif // ZSERIO_BYTES_H_INC
