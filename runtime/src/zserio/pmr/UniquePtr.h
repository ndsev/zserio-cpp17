#ifndef ZSERIO_PMR_UNIQUE_PTR_H_INC
#define ZSERIO_PMR_UNIQUE_PTR_H_INC

#include <memory_resource>

#include "zserio/UniquePtr.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to zserio::UniquePtr provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>.
 */
template <typename T>
using UniquePtr = zserio::UniquePtr<T, std::pmr::polymorphic_allocator<T>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_UNIQUE_PTR_H_INC
