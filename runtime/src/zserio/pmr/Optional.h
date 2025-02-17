#ifndef ZSERIO_PMR_OPTIONAL_H_INC
#define ZSERIO_PMR_OPTIONAL_H_INC

#include "zserio/Optional.h"
#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to Optional provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename T>
using Optional = zserio::BasicOptional<PropagatingPolymorphicAllocator<uint8_t>, T>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_OPTIONAL_H_INC
