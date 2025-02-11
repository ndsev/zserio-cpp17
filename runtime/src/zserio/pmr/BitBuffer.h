#ifndef ZSERIO_PMR_BIT_BUFFER_H_INC
#define ZSERIO_PMR_BIT_BUFFER_H_INC

#include "zserio/BitBuffer.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/** Typedef to BitBuffer provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>. */
using BitBuffer = BasicBitBuffer<PropagatingPolymorphicAllocator<uint8_t>>;

/** Typedef to BitBufferView provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>. */
using BitBufferView = std::reference_wrapper<const BitBuffer>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_BIT_BUFFER_H_INC
