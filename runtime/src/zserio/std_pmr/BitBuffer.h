#ifndef ZSERIO_STD_PMR_BIT_BUFFER_H_INC
#define ZSERIO_STD_PMR_BIT_BUFFER_H_INC

#include <memory_resource>

#include "zserio/BitBuffer.h"

namespace zserio
{
namespace std_pmr
{

/** Typedef to BitBuffer provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>. */
using BitBuffer = BasicBitBuffer<std::pmr::polymorphic_allocator<uint8_t>>;

/** Typedef to BitBufferView provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>. */
using BitBufferView = std::reference_wrapper<const BitBuffer>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_BIT_BUFFER_H_INC
