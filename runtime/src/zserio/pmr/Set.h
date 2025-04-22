#ifndef ZSERIO_PMR_SET_H_INC
#define ZSERIO_PMR_SET_H_INC

#include <memory_resource>
#include <set>

namespace zserio
{
namespace pmr
{

/**
 * Typedef to std::set provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename T, typename COMPARE = std::less<T>>
using Set = std::set<T, COMPARE, std::pmr::polymorphic_allocator<T>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_SET_H_INC
