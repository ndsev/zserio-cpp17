#ifndef ZSERIO_PPMR_STRING_H_INC
#define ZSERIO_PPMR_STRING_H_INC

#include "zserio/String.h"
#include "zserio/ppmr/PropagatingPolymorphicAllocator.h"

namespace zserio
{
namespace ppmr
{

/**
 * Typedef to std::string provided for convenience - using PropagatingPolymorphicAllocator<char>.
 */
using String = zserio::BasicString<PropagatingPolymorphicAllocator<char>>;

} // namespace ppmr
} // namespace zserio

#endif // ZSERIO_PPMR_STRING_H_INC
