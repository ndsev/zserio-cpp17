#ifndef ZSERIO_PMR_REFLECTABLE_DATA_H_INC
#define ZSERIO_PMR_REFLECTABLE_DATA_H_INC

#include "zserio/ReflectableData.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to the reflectable factory provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>.
 */
using ReflectableFactory = BasicReflectableFactory<PropagatingPolymorphicAllocator<uint8_t>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_REFLECTABLE_DATA_H_INC
