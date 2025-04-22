#ifndef ZSERIO_PPMR_MAP_H_INC
#define ZSERIO_PPMR_MAP_H_INC

#include <map>

#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to std::map provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename KEY, typename T, typename COMPARE = std::less<KEY>>
using Map = std::map<KEY, T, COMPARE, PropagatingPolymorphicAllocator<std::pair<const KEY, T>>>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_MAP_H_INC
