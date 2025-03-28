#ifndef ZSERIO_STD_PMR_SET_H_INC
#define ZSERIO_STD_PMR_SET_H_INC

#include <memory_resource>
#include <set>

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to std::set provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename T, typename COMPARE = std::less<T>>
using Set = std::set<T, COMPARE, std::pmr::polymorphic_allocator<T>>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_SET_H_INC
