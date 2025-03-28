#ifndef ZSERIO_STD_PMR_VARIANT_H_INC
#define ZSERIO_STD_PMR_VARIANT_H_INC

#include <memory_resource>

#include "zserio/Variant.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to Variant provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename INDEX, typename... T>
using Variant = BasicVariant<std::pmr::polymorphic_allocator<uint8_t>, INDEX, T...>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_VARIANT_H_INC
