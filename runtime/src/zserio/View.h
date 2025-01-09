#ifndef ZSERIO_VIEW_H_INC
#define ZSERIO_VIEW_H_INC

#include <string_view>

#include "zserio/BitSize.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

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
View(T, ARGS...) -> View<T>;

namespace detail
{

/**
 * Global function for validation provided via specialization.
 *
 * \param view Zserio View to use for validation.
 *
 * \throw CppRuntimeException In case of any validation error.
 */
template <typename T>
void validate(const View<T>& view, std::string_view fieldName = "");

/**
 * Global function for bit size provided via specialization.
 *
 * \param view Zserio View to use.
 * \param bitPosition Bit position to use.
 *
 * \return Bit size of the Zserio object.
 */
template <typename T>
BitSize bitSizeOf(const View<T>& view, BitSize bitPosition = 0);

/**
 * Default offset initializer. Types which need offset initialization shall provide this
 * information via specialization of this template structure.
 */
template <typename T>
struct OffsetsInitializer;

template <typename T, typename = void>
struct has_offsets_initializer : std::false_type
{};

template <typename T>
struct has_offsets_initializer<T, std::void_t<decltype(OffsetsInitializer<T>{})>> : std::true_type
{};

template <typename T, typename V = void>
inline constexpr bool has_offsets_initializer_v = has_offsets_initializer<T, V>::value;

template <typename T>
BitSize initializeOffsets(const View<T>& view, BitSize bitPosition)
{
    if constexpr (has_offsets_initializer_v<T>)
    {
        return OffsetsInitializer<T>::initialize(view, bitPosition);
    }
    else
    {
        return bitSizeOf(view, bitPosition);
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
void write(BitStreamWriter& writer, const View<T>& view);

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
View<T> read(BitStreamReader& reader, T& data, ARGS...);

} // namespace detail

template <typename VALUE_TYPE>
class View<detail::DynIntWrapper<VALUE_TYPE, 0>>
{
public:
    using ValueType = VALUE_TYPE;

    View(detail::DynIntWrapper<VALUE_TYPE, 0> value, uint8_t numBits) :
            m_value(value),
            m_numBits(numBits)
    {}

    uint8_t numBits() const
    {
        return m_numBits;
    }

    detail::DynIntWrapper<VALUE_TYPE, 0> value() const
    {
        return m_value;
    }

    operator VALUE_TYPE() const
    {
        return m_value;
    }

private:
    const detail::DynIntWrapper<VALUE_TYPE, 0> m_value;
    const uint8_t m_numBits;
};

template <typename VALUE_TYPE>
bool operator==(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& lhs,
        const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& rhs)
{
    return lhs.numBits() == rhs.numBits() && lhs.value() == rhs.value();
}

template <typename VALUE_TYPE>
bool operator!=(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& lhs,
        const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& rhs)
{
    return !(lhs == rhs);
}

template <typename VALUE_TYPE>
bool operator<(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& lhs,
        const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& rhs)
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
bool operator>(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& lhs,
        const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& rhs)
{
    return rhs < lhs;
}

template <typename VALUE_TYPE>
bool operator<=(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& lhs,
        const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& rhs)
{
    return !(rhs < lhs);
}

template <typename VALUE_TYPE>
bool operator>=(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& lhs,
        const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& rhs)
{
    return !(lhs < rhs);
}

namespace detail
{

template <typename VALUE_TYPE>
void validate(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& view, std::string_view fieldName)
{
    return validate(view.value(), view.numBits(), fieldName);
}

template <typename VALUE_TYPE>
BitSize bitSizeOf(const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& view, BitSize = 0)
{
    return view.numBits();
}

template <typename VALUE_TYPE>
void write(BitStreamWriter& writer, const View<detail::DynIntWrapper<VALUE_TYPE, 0>>& view)
{
    write(writer, view.value(), view.numBits());
}

} // namespace detail

} // namespace zserio

namespace std
{

template <typename VALUE_TYPE>
struct hash<zserio::View<zserio::detail::DynIntWrapper<VALUE_TYPE, 0>>>
{
    size_t operator()(const zserio::View<zserio::detail::DynIntWrapper<VALUE_TYPE, 0>>& view) const
    {
        uint32_t result = zserio::HASH_SEED;
        result = zserio::calcHashCode(result, view.numBits());
        result = zserio::calcHashCode(result, view.value());
        return result;
    }
};

} // namespace std

#endif // ifndef ZSERIO_VIEW_H_INC
