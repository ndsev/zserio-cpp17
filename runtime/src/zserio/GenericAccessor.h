#ifndef ZSERIO_GENERIC_ACCESSOR_INC_H
#define ZSERIO_GENERIC_ACCESSOR_INC_H

#include "zserio/Extended.h"
#include "zserio/Optional.h"
#include "zserio/Traits.h"

namespace zserio
{

namespace detail
{

template <typename T, typename = void>
struct generic_accessor;

template <typename T>
struct generic_accessor<T, std::enable_if_t<std::is_enum_v<T>>>
{
    using type = T;
};

template <typename T>
struct generic_accessor<T, std::enable_if_t<is_bitmask_v<T>>>
{
    using type = typename T::Values;
};

template <typename T, typename U>
void setGenericOffset(T& offsetField, U bytesOffset)
{
    offsetField = static_cast<typename T::ValueType>(bytesOffset);
}

} // namespace detail

template <typename T>
using generic_accessor_t = typename detail::generic_accessor<T>::type;

template <typename T>
const T& genericAccessor(const T& value)
{
    return value;
}

template <typename ALLOC, typename T>
const auto& genericAccessor(const BasicOptional<ALLOC, T>& optionalValue)
{
    return optionalValue.value();
}

template <typename T>
const auto& genericAccessor(const Extended<T>& extendedValue)
{
    return genericAccessor(extendedValue.value());
}

template <typename T>
T& genericAccessor(T& value)
{
    return value;
}

template <typename ALLOC, typename T>
auto& genericAccessor(BasicOptional<ALLOC, T>& optionalValue)
{
    return optionalValue.value();
}

template <typename T>
auto& genericAccessor(Extended<T>& extendedValue)
{
    return genericAccessor(extendedValue.value());
}

} // namespace zserio

#endif // ZSERIO_GENERIC_ACCESSOR_INC_H
