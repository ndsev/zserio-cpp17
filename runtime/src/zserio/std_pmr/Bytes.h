#ifndef ZSERIO_STD_PMR_BYTES_H_INC
#define ZSERIO_STD_PMR_BYTES_H_INC

#include <memory_resource>

#include "zserio/Bytes.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef for Zserio Bytes type provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>.
 */
using Bytes = BasicBytes<std::pmr::polymorphic_allocator<uint8_t>>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_BYTES_H_INC
