#ifndef ZSERIO_STD_PMR_MAP_H_INC
#define ZSERIO_STD_PMR_MAP_H_INC

#include <map>
#include <memory_resource>

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to std::map provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename KEY, typename T, typename COMPARE = std::less<KEY>>
using Map = std::map<KEY, T, COMPARE, std::pmr::polymorphic_allocator<std::pair<const KEY, T>>>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_MAP_H_INC
