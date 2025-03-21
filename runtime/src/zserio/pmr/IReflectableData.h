#ifndef ZSERIO_PMR_I_REFLECTABLE_DATA_H_INC
#define ZSERIO_PMR_I_REFLECTABLE_DATA_H_INC

#include "zserio/IReflectableData.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

// needed to have proper typedefs
#include "zserio/pmr/Any.h"
#include "zserio/pmr/BitBuffer.h"
#include "zserio/pmr/String.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to reflectable interface provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>.
 */
/** \{ */
using IReflectableData = IBasicReflectableData<PropagatingPolymorphicAllocator<uint8_t>>;
using IReflectableDataPtr = IBasicReflectableDataPtr<PropagatingPolymorphicAllocator<uint8_t>>;
using IReflectableDataConstPtr = IBasicReflectableDataConstPtr<PropagatingPolymorphicAllocator<uint8_t>>;
/** \} */

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_I_REFLECTABLE_DATA_H_INC
