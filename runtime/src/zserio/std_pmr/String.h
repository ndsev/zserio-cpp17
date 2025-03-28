#ifndef ZSERIO_STD_PMR_STRING_H_INC
#define ZSERIO_STD_PMR_STRING_H_INC

#include <memory_resource>

#include "zserio/String.h"

namespace zserio
{
namespace std_pmr
{

/**
 * Typedef to std::string provided for convenience - using std::pmr::polymorphic_allocator<char>.
 */
using String = zserio::BasicString<std::pmr::polymorphic_allocator<char>>;

} // namespace std_pmr
} // namespace zserio

#endif // ZSERIO_STD_PMR_STRING_H_INC
