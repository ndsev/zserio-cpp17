#ifndef ZSERIO_PPMR_SET_H_INC
#define ZSERIO_PPMR_SET_H_INC

#include <set>

#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to std::set provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename T, typename COMPARE = std::less<T>>
using Set = std::set<T, COMPARE, PropagatingPolymorphicAllocator<T>>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_SET_H_INC
