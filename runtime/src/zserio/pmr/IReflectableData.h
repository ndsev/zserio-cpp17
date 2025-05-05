#ifndef ZSERIO_PMR_I_REFLECTABLE_DATA_H_INC
#define ZSERIO_PMR_I_REFLECTABLE_DATA_H_INC

#include <memory_resource>

#include "zserio/IReflectableData.h"

// needed to have proper typedefs
#include "zserio/pmr/Any.h"
#include "zserio/pmr/BitBuffer.h"
#include "zserio/pmr/String.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to reflectable interface provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>.
 */
/** \{ */
using IReflectableData = IBasicReflectableData<std::pmr::polymorphic_allocator<uint8_t>>;
using IReflectableDataPtr = IBasicReflectableDataPtr<std::pmr::polymorphic_allocator<uint8_t>>;
using IReflectableDataConstPtr = IBasicReflectableDataConstPtr<std::pmr::polymorphic_allocator<uint8_t>>;
/** \} */

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_I_REFLECTABLE_DATA_H_INC
