#ifndef ZSERIO_STRING_H_INC
#define ZSERIO_STRING_H_INC

#include <string>
#include <string_view>

#include "zserio/BitSize.h"

namespace zserio
{

/**
 * Typedef to std::basic_string provided for convenience - using char.
 */
template <typename ALLOC>
using BasicString = std::basic_string<char, std::char_traits<char>, ALLOC>;

/**
 * Typedef to std::string_view to hide it under zserio specific name.
 */
using StringView = std::string_view;

/**
 * Typedef to std::string provided for convenience - using std::allocator<uint8_t>.
 */
using String = BasicString<std::allocator<char>>;

namespace detail
{

void validate(StringView stringValue, std::string_view fieldName);

BitSize bitSizeOf(StringView stringValue, BitSize = 0);

} // namespace detail

} // namespace zserio

#endif // ZSERIO_STRING_H_INC
