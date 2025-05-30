#ifndef ZSERIO_ITEMS_ACCESSOR_INC_H
#define ZSERIO_ITEMS_ACCESSOR_INC_H

#include "Traits.h"

namespace zserio
{

template <typename T, typename = void>
struct ItemsAccessor;

template <typename T>
struct ItemsAccessor<T, std::enable_if_t<std::is_enum_v<T>>>
{
    using Items = T;
};

template <typename T>
struct ItemsAccessor<T, std::enable_if_t<zserio::is_bitmask_v<T>>>
{
    using Items = typename T::Values;
};

} // namespace zserio

#endif // ZSERIO_ITEMS_ACCESSOR_INC_H
