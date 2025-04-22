#ifndef ZSERIO_PMR_STRING_H_INC
#define ZSERIO_PMR_STRING_H_INC

#include <memory_resource>

#include "zserio/String.h"

namespace zserio
{
namespace pmr
{

/**
 * Typedef to std::string provided for convenience - using std::pmr::polymorphic_allocator<char>.
 */
using String = zserio::BasicString<std::pmr::polymorphic_allocator<char>>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_STRING_H_INC
