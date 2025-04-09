#ifndef ZSERIO_STD_PMR_ANY_H_INC
#define ZSERIO_STD_PMR_ANY_H_INC

#include <memory_resource>

#include "zserio/Any.h"

namespace zserio
{
namespace std_pmr
{

/** Typedef to Any provided for convenience */
using Any = BasicAny<std::pmr::polymorphic_allocator<uint8_t>>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_ANY_H_INC
