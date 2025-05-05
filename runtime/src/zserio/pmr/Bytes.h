#ifndef ZSERIO_PMR_BYTES_H_INC
#define ZSERIO_PMR_BYTES_H_INC

#include <memory_resource>

#include "zserio/Bytes.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef for Zserio Bytes type provided for convenience - using std::pmr::polymorphic_allocator<uint8_t>.
 */
using Bytes = BasicBytes<std::pmr::polymorphic_allocator<uint8_t>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_BYTES_H_INC
