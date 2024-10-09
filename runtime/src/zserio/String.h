#ifndef ZSERIO_STRING_H_INC
#define ZSERIO_STRING_H_INC

#include <string>

#include "zserio/RebindAlloc.h"

namespace zserio
{

/**
 * Typedef to std::basic_string provided for convenience - using char.
 *
 * Automatically rebinds the given allocator.
 */
template <typename ALLOC>
using basic_string = std::basic_string<char, std::char_traits<char>, RebindAlloc<ALLOC, char>>;

/**
 * Typedef to std::string provided for convenience - using std::allocator<uint8_t>.
 */
using string = basic_string<std::allocator<char>>;

} // namespace zserio

#endif // ZSERIO_STRING_H_INC
