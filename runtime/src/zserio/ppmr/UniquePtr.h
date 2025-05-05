#ifndef ZSERIO_PPMR_UNIQUE_PTR_H_INC
#define ZSERIO_PPMR_UNIQUE_PTR_H_INC

#include "zserio/UniquePtr.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to zserio::UniquePtr provided for convenience - using PropagatingPolymorphicAllocator<uint8_t>.
 */
template <typename T>
using UniquePtr = zserio::UniquePtr<T, PropagatingPolymorphicAllocator<T>>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_UNIQUE_PTR_H_INC
