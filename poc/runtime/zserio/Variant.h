#ifndef ZSERIO_VARIANT_H_INC
#define ZSERIO_VARIANT_H_INC

#include <variant>

namespace zserio
{

// This will be replaced by our own implementation which uses SBO and allocates
// when types are too big

template <typename... ARGS>
using Variant = std::variant<ARGS...>;

namespace pmr
{

template <typename... ARGS>
using Variant = std::variant<ARGS...>;

}

} // namespace zserio

#endif