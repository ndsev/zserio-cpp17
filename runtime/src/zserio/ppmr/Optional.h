#ifndef ZSERIO_PPMR_OPTIONAL_H_INC
#define ZSERIO_PPMR_OPTIONAL_H_INC

#include "zserio/Optional.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to Optional provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename T>
using Optional = zserio::BasicOptional<PropagatingPolymorphicAllocator<uint8_t>, T>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_OPTIONAL_H_INC
