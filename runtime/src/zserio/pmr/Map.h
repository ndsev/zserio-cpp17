#ifndef ZSERIO_PMR_MAP_H_INC
#define ZSERIO_PMR_MAP_H_INC

#include <map>
#include <memory_resource>

namespace zserio
{
namespace pmr
{

/**
 * Typedef to std::map provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename KEY, typename T, typename COMPARE = std::less<KEY>>
using Map = std::map<KEY, T, COMPARE, std::pmr::polymorphic_allocator<std::pair<const KEY, T>>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_MAP_H_INC
