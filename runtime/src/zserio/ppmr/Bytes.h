#ifndef ZSERIO_PPMR_BYTES_H_INC
#define ZSERIO_PPMR_BYTES_H_INC

#include "zserio/Bytes.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef for Zserio Bytes type provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>.
 */
using Bytes = BasicBytes<PropagatingPolymorphicAllocator<uint8_t>>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_BYTES_H_INC
