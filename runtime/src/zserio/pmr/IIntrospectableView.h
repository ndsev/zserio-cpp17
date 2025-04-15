#ifndef ZSERIO_PMR_I_INTROSPECTABLE_VIEW_H_INC
#define ZSERIO_PMR_I_INTROSPECTABLE_VIEW_H_INC

#include "zserio/IIntrospectableView.h"
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
using IIntrospectableView = IBasicIntrospectableView<PropagatingPolymorphicAllocator<uint8_t>>;
using IIntrospectableViewConstPtr = IBasicIntrospectableViewConstPtr<PropagatingPolymorphicAllocator<uint8_t>>;
/** \} */

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_I_INTROSPECTABLE_VIEW_H_INC
