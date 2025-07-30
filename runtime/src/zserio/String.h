#ifndef ZSERIO_STRING_H_INC
#define ZSERIO_STRING_H_INC

#include <string>
#include <string_view>

#include "zserio/BitSize.h"
#include "zserio/Traits.h"

namespace zserio
{

/**
 * Typedef to std::basic_string provided for convenience - using char.
 */
template <typename ALLOC>
using BasicString = std::basic_string<char, std::char_traits<char>, ALLOC>;

/**
 * Typedef to std::string provided for convenience - using std::allocator<uint8_t>.
 */
using String = BasicString<std::allocator<char>>;

template <typename ALLOC>
struct view_type<BasicString<ALLOC>>
{
    using type = std::string_view;
};

namespace detail
{

void validate(std::string_view stringValue, std::string_view fieldName);

BitSize bitSizeOf(std::string_view stringValue, BitSize = 0);

BitSize initializeOffsets(std::string_view stringValue, BitSize bitPosition);

} // namespace detail

} // namespace zserio

#endif // ZSERIO_STRING_H_INC
