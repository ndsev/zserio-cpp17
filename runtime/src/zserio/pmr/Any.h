#ifndef ZSERIO_PMR_ANY_H_INC
#define ZSERIO_PMR_ANY_H_INC

#include <memory_resource>

#include "zserio/Any.h"

namespace zserio
{
namespace pmr
{

/** Typedef to Any provided for convenience */
using Any = BasicAny<std::pmr::polymorphic_allocator<uint8_t>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_ANY_H_INC
