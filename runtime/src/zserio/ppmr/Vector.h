#ifndef ZSERIO_PPMR_VECTOR_H_INC
#define ZSERIO_PPMR_VECTOR_H_INC

#include "zserio/Vector.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to std::vector provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename T>
using Vector = std::vector<T, PropagatingPolymorphicAllocator<T>>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_VECTOR_H_INC
