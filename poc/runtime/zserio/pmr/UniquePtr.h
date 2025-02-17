#ifndef ZSERIO_PMR_UNIQUE_PTR_H_INC
#define ZSERIO_PMR_UNIQUE_PTR_H_INC

#include "zserio/UniquePtr.h"
#include "zserio/pmr/PolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to zserio::UniquePtr provided for convenience - using PropagatingPolymorphicAllocator.
 */
template <typename T>
using UniquePtr = zserio::UniquePtr<T, PropagatingPolymorphicAllocator<T>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_UNIQUE_PTR_H_INC
