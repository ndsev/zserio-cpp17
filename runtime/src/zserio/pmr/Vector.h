#ifndef ZSERIO_PMR_VECTOR_H_INC
#define ZSERIO_PMR_VECTOR_H_INC

#include <memory_resource>

#include "zserio/Vector.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to std::vector provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename T>
using Vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_VECTOR_H_INC
