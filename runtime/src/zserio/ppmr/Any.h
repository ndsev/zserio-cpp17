#ifndef ZSERIO_PPMR_ANY_H_INC
#define ZSERIO_PPMR_ANY_H_INC

#include "zserio/Any.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/** Typedef to Any provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>. */
using Any = BasicAny<PropagatingPolymorphicAllocator<uint8_t>>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_ANY_H_INC
