#ifndef ZSERIO_PMR_BYTES_H_INC
#define ZSERIO_PMR_BYTES_H_INC

#include "zserio/Bytes.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef for Zserio Bytes type provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>.
 */
using Bytes = BasicBytes<PropagatingPolymorphicAllocator<uint8_t>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_BYTES_H_INC
