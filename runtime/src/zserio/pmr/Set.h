#ifndef ZSERIO_PMR_SET_H_INC
#define ZSERIO_PMR_SET_H_INC

#include <set>

#include "zserio/pmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to std::set provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename T, typename COMPARE = std::less<T>>
using Set = std::set<T, COMPARE, PropagatingPolymorphicAllocator<T>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_SET_H_INC
