#ifndef ZSERIO_PMR_ANY_H_INC
#define ZSERIO_PMR_ANY_H_INC

#include "zserio/Any.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/** Typedef to Any provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>. */
using Any = BasicAny<PropagatingPolymorphicAllocator<uint8_t>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_ANY_H_INC
