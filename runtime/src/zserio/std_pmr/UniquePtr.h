#ifndef ZSERIO_STD_PMR_UNIQUE_PTR_H_INC
#define ZSERIO_STD_PMR_UNIQUE_PTR_H_INC

#include <memory_resource>

#include "zserio/UniquePtr.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to zserio::UniquePtr provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>.
 */
template <typename T>
using UniquePtr = zserio::UniquePtr<T, std::pmr::polymorphic_allocator<T>>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_UNIQUE_PTR_H_INC
