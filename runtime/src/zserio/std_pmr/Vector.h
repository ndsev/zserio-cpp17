#ifndef ZSERIO_STD_PMR_VECTOR_H_INC
#define ZSERIO_STD_PMR_VECTOR_H_INC

#include <memory_resource>

#include "zserio/Vector.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to std::vector provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename T>
using Vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_VECTOR_H_INC
