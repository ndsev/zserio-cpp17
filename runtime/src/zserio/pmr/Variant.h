#ifndef ZSERIO_PMR_VARIANT_H_INC
#define ZSERIO_PMR_VARIANT_H_INC

#include "zserio/Variant.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to Variant provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename INDEX, typename... T>
using Variant = BasicVariant<PropagatingPolymorphicAllocator<uint8_t>, INDEX, T...>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_VARIANT_H_INC
