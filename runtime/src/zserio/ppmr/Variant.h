#ifndef ZSERIO_PPMR_VARIANT_H_INC
#define ZSERIO_PPMR_VARIANT_H_INC

#include "zserio/Variant.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to Variant provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename INDEX, typename... T>
using Variant = BasicVariant<PropagatingPolymorphicAllocator<uint8_t>, INDEX, T...>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_VARIANT_H_INC
