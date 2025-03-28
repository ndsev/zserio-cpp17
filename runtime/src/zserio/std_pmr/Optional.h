#ifndef ZSERIO_STD_PMR_OPTIONAL_H_INC
#define ZSERIO_STD_PMR_OPTIONAL_H_INC

#include <memory_resource>

#include "zserio/Optional.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to Optional provided for convenience - using std::pmr::polymorphic_allocator.
 */
template <typename T>
using Optional = zserio::BasicOptional<std::pmr::polymorphic_allocator<uint8_t>, T>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_OPTIONAL_H_INC
