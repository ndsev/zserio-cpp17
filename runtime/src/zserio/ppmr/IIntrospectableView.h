#ifndef ZSERIO_PPMR_I_INTROSPECTABLE_VIEW_H_INC
#define ZSERIO_PPMR_I_INTROSPECTABLE_VIEW_H_INC

#include "zserio/IIntrospectableView.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

// needed to have proper typedefs
#include "zserio/ppmr/Any.h"
#include "zserio/ppmr/BitBuffer.h"
#include "zserio/ppmr/String.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to reflectable interface provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>.
 */
/** \{ */
using IIntrospectableView = IBasicIntrospectableView<PropagatingPolymorphicAllocator<uint8_t>>;
using IIntrospectableViewConstPtr = IBasicIntrospectableViewConstPtr<PropagatingPolymorphicAllocator<uint8_t>>;
/** \} */

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_I_INTROSPECTABLE_VIEW_H_INC
