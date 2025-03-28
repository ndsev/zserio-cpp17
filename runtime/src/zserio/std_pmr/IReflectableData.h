#ifndef ZSERIO_STD_PMR_I_REFLECTABLE_DATA_H_INC
#define ZSERIO_STD_PMR_I_REFLECTABLE_DATA_H_INC

#include "zserio/IReflectableData.h"
#include <memory_resource>

// needed to have proper typedefs
#include "zserio/std_pmr/Any.h"
#include "zserio/std_pmr/BitBuffer.h"
#include "zserio/std_pmr/String.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to reflectable interface provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>.
 */
/** \{ */
using IReflectableData = IBasicReflectableData<std::pmr::polymorphic_allocator<uint8_t>>;
using IReflectableDataPtr = IBasicReflectableDataPtr<std::pmr::polymorphic_allocator<uint8_t>>;
using IReflectableDataConstPtr = IBasicReflectableDataConstPtr<std::pmr::polymorphic_allocator<uint8_t>>;
/** \} */

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_I_REFLECTABLE_DATA_H_INC
