#ifndef ZSERIO_VIEW_H_INC
#define ZSERIO_VIEW_H_INC

#include <string_view>
#include <tuple>

#include "zserio/BitSize.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/Traits.h"

namespace zserio
{

/**
 * Zserio View provides schema logic for data generated from Zserio structures, choices and unions.
 *
 * Zserio View contains
 * - simple parameters stored by value
 * - compound parameters stored by reference
 * - reference to the underlying data
 *
 * This information is provided via specializations of the View structure.
 */
template <typename T>
class View;

// template argument deduction guide for View constructor
template <typename T, typename... ARGS>
View(T, ARGS&&...) -> View<T>;

namespace detail
{

/**
 * Default object traits. Concrete implementation is provided via specialization of this template structure.
 */
template <typename T>
struct ObjectTraits;

// TODO[Mi-L@]: Do we need to have U&& here? It should take either simple type, or view (or string_view, etc.).
template <size_t I, typename T, typename U>
view_type_t<std::tuple_element_t<I, typename ObjectTraits<T>::Parameters>> makeParameter(U arg)
{
    using ParamType = std::tuple_element_t<I, typename ObjectTraits<T>::Parameters>;
    if constexpr (is_dyn_int_wrapper_v<ParamType>)
    {
        return View<ParamType>(ParamType(static_cast<typename ParamType::ValueType>(arg)), 64);
    }
    else if constexpr (is_numeric_wrapper_v<ParamType>)
    {
        return ParamType(static_cast<typename ParamType::ValueType>(arg));
    }
    else
    {
        return arg;
    }
}

/**
 * Global function for validation provided via specialization.
 *
 * \param view Zserio View to use for validation.
 *
 * \throw CppRuntimeException In case of any validation error.
 */
template <typename T>
void validate(const View<T>& view, std::string_view fieldName = "")
{
    ObjectTraits<T>::validate(view, fieldName);
}

/**
 * Global function for bit size provided via specialization.
 *
 * \param view Zserio View to use.
 * \param bitPosition Bit position to use.
 *
 * \return Bit size of the Zserio object.
 */
template <typename T>
BitSize bitSizeOf(const View<T>& view, BitSize bitPosition = 0)
{
    return ObjectTraits<T>::bitSizeOf(view, bitPosition);
}

template <typename T, typename = void>
struct has_initialize_offsets : std::false_type
{};

template <typename T>
struct has_initialize_offsets<T,
        std::void_t<decltype(ObjectTraits<T>().initializeOffsets(std::declval<const View<T>&>(), 0))>>
        : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool has_initialize_offsets_v = has_initialize_offsets<T, V>::value;

template <typename T>
BitSize initializeOffsets(const View<T>& view, BitSize bitPosition)
{
    if constexpr (has_initialize_offsets_v<T>)
    {
        return ObjectTraits<T>::initializeOffsets(view, bitPosition);
    }
    else
    {
        return ObjectTraits<T>::bitSizeOf(view, bitPosition);
    }
}

/**
 * Global function for writing provided via specialization.
 *
 * \param writer Bit stream writer to use for writing.
 * \param view Zserio View to use for writing.
 *
 * \throw CppRuntimeException In case of any write error.
 */
template <typename T>
void write(BitStreamWriter& writer, const View<T>& view)
{
    ObjectTraits<T>::write(writer, view);
}

/**
 * Global function for reading provided via specialization.
 *
 * \param read Bit stream reader to read from.
 * \param data Zserio Data to fill with read data.
 * \param arguments All parameters in case of Zserio parameterized type.
 *
 * \return View with read data.
 *
 * \throw CppRuntimeException In case of any read error.
 */
template <typename T, typename... ARGS>
View<T> read(BitStreamReader& reader, T& data, ARGS&&... args)
{
    return ObjectTraits<T>::read(reader, data, std::forward<ARGS>(args)...);
}

} // namespace detail

template <typename VALUE_TYPE>
class View<detail::DynIntWrapper<VALUE_TYPE>>
{
public:
    using ValueType = VALUE_TYPE;

    View(detail::DynIntWrapper<VALUE_TYPE> value, uint8_t numBits) :
            m_value(value),
            m_numBits(numBits)
    {}

    uint8_t numBits() const
    {
        return m_numBits;
    }

    detail::DynIntWrapper<VALUE_TYPE> value() const
    {
        return m_value;
    }

    operator VALUE_TYPE() const
    {
        return m_value;
    }

private:
    detail::DynIntWrapper<VALUE_TYPE> m_value;
    uint8_t m_numBits;
};

template <typename VALUE_TYPE>
bool operator==(
        const View<detail::DynIntWrapper<VALUE_TYPE>>& lhs, const View<detail::DynIntWrapper<VALUE_TYPE>>& rhs)
{
    return lhs.numBits() == rhs.numBits() && lhs.value() == rhs.value();
}

template <typename VALUE_TYPE>
bool operator!=(
        const View<detail::DynIntWrapper<VALUE_TYPE>>& lhs, const View<detail::DynIntWrapper<VALUE_TYPE>>& rhs)
{
    return !(lhs == rhs);
}

template <typename VALUE_TYPE>
bool operator<(
        const View<detail::DynIntWrapper<VALUE_TYPE>>& lhs, const View<detail::DynIntWrapper<VALUE_TYPE>>& rhs)
{
    if (lhs.numBits() != rhs.numBits())
    {
        return lhs.numBits() < rhs.numBits();
    }
    if (lhs.value() != rhs.value())
    {
        return lhs.value() < rhs.value();
    }

    return false;
}

template <typename VALUE_TYPE>
bool operator>(
        const View<detail::DynIntWrapper<VALUE_TYPE>>& lhs, const View<detail::DynIntWrapper<VALUE_TYPE>>& rhs)
{
    return rhs < lhs;
}

template <typename VALUE_TYPE>
bool operator<=(
        const View<detail::DynIntWrapper<VALUE_TYPE>>& lhs, const View<detail::DynIntWrapper<VALUE_TYPE>>& rhs)
{
    return !(rhs < lhs);
}

template <typename VALUE_TYPE>
bool operator>=(
        const View<detail::DynIntWrapper<VALUE_TYPE>>& lhs, const View<detail::DynIntWrapper<VALUE_TYPE>>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <typename VALUE_TYPE>
void validate(const View<detail::DynIntWrapper<VALUE_TYPE>>& view, std::string_view fieldName)
{
    return validate(view.value(), view.numBits(), fieldName);
}

template <typename VALUE_TYPE>
BitSize bitSizeOf(const View<detail::DynIntWrapper<VALUE_TYPE>>& view, BitSize = 0)
{
    return view.numBits();
}

template <typename VALUE_TYPE>
void write(BitStreamWriter& writer, const View<detail::DynIntWrapper<VALUE_TYPE>>& view)
{
    write(writer, view.value(), view.numBits());
}

} // namespace detail

} // namespace zserio

namespace std
{

template <typename VALUE_TYPE>
struct hash<zserio::View<zserio::detail::DynIntWrapper<VALUE_TYPE>>>
{
    size_t operator()(const zserio::View<zserio::detail::DynIntWrapper<VALUE_TYPE>>& view) const
    {
        uint32_t result = zserio::HASH_SEED;
        result = zserio::calcHashCode(result, view.numBits());
        result = zserio::calcHashCode(result, view.value());
        return result;
    }
};

} // namespace std

#endif // ifndef ZSERIO_VIEW_H_INC
