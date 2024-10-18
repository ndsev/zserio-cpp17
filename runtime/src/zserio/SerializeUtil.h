/**
 * \file
 * It provides methods for serialization and deserialization of Zserio generated objects.
 *
 * \note Please note that file operations allocate memory as needed and are not designed to use allocators.
 */

#ifndef ZSERIO_SERIALIZE_UTIL_H_INC
#define ZSERIO_SERIALIZE_UTIL_H_INC

#include <type_traits>
#include <utility>

#include "zserio/BitBuffer.h"
#include "zserio/BitSize.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/FileUtil.h"
#include "zserio/Traits.h"
#include "zserio/View.h"

namespace zserio
{

template <typename... ARGS>
struct is_first_alloc : is_first_allocator<::std::decay_t<ARGS>...>
{};

/**
 * Serializes Zserio object using View to bit buffer using given allocator.
 *
 * Before serialization, the method properly calls `validate()` method on the given Zserio object.
 *
 * Example:
 * \code{.cpp}
 *     #include <memory_resource>
 *     #include <zserio/SerializeUtil.h>
 *
 *     const std::pmr::PolymorphicAllocator<> allocator;
 *     SomeZserioObject objectData;
 *     zserio::View<SomeZserioObject> objectView(objectData);
 *     const zserio::BasicBitBuffer<zserio::pmr::PolymorphicAllocator<>> bitBuffer =
 *             zserio::serialize(objectView, allocator);
 * \endcode
 *
 * \param view View of Zserio object to serialize.
 * \param allocator Allocator to use to allocate bit buffer.
 *
 * \return Bit buffer containing the serialized object.
 *
 * \throw CppRuntimeException When serialization fails.
 */
template <typename T, typename ALLOC>
BasicBitBuffer<ALLOC> serialize(const View<T>& view, const ALLOC& allocator)
{
    const BitSize bitSize = detail::bitSizeOf(view, 0);
    BasicBitBuffer<ALLOC> buffer(bitSize, allocator);
    BitStreamWriter writer(buffer);
    detail::validate(view);
    detail::write(writer, view);

    return buffer;
}

/**
 * Serializes Zserio object using View to bit buffer.
 *
 * The memory for serialization is allocated using new allocator of the type defined by the Zserio object.
 *
 * Before serialization, the method properly calls `validate()` method on the given Zserio object.
 *
 * Example:
 * \code{.cpp}
 *     #include <zserio/SerializeUtil.h>
 *
 *     SomeZserioObject objectData;
 *     zserio::View<SomeZserioObject> objectView(objectData);
 *     const auto bitBuffer = zserio::serialize(objectView);
 * \endcode
 *
 * \param view View of Zserio object to serialize.
 *
 * \return Bit buffer containing the serialized object.
 *
 * \throw CppRuntimeException When serialization fails.
 */
template <typename T>
BasicBitBuffer<typename T::allocator_type> serialize(const View<T>& view)
{
    return serialize(view, typename T::allocator_type());
}

/**
 * Serializes Zserio object using Data to bit buffer using given allocator.
 *
 * Before serialization, the method properly calls `validate()` method on the given Zserio object.
 *
 * Example:
 * \code{.cpp}
 *     #include <memory_resource>
 *     #include <zserio/SerializeUtil.h>
 *
 *     const std::pmr::PolymorphicAllocator<> allocator;
 *     SomeZserioObject objectData;
 *     const zserio::BasicBitBuffer<zserio::pmr::PolymorphicAllocator<>> bitBuffer =
 *             zserio::serialize(objectData, allocator);
 * \endcode
 *
 * \param data Data of Zserio object to serialize.
 * \param allocator Allocator to use to allocate bit buffer.
 * \param arguments All parameters in case of Zserio parameterized type.
 *
 * \return Bit buffer containing the serialized object.
 *
 * \throw CppRuntimeException When serialization fails.
 */
template <typename T, typename ALLOC, typename... ARGS,
        typename = ::std::enable_if_t<is_allocator<ALLOC>::value>>
BasicBitBuffer<ALLOC> serialize(const T& data, const ALLOC& allocator, ARGS&&... arguments)
{
    const View<T> view(data, ::std::forward<ARGS>(arguments)...);

    return serialize(view, allocator);
}

/**
 * Serializes Zserio object using Data to bit buffer.
 *
 * The memory for serialization is allocated using new allocator of the type defined by the Zserio object.
 *
 * Before serialization, the method properly calls `validate()` method on the given Zserio object.
 *
 * Example:
 * \code{.cpp}
 *     #include <zserio/SerializeUtil.h>
 *
 *     SomeZserioObject objectData;
 *     const auto bitBuffer = zserio::serialize(objectData);
 * \endcode
 *
 * \param data Data of Zserio object to serialize.
 * \param arguments All parameters in case of Zserio parameterized type.
 *
 * \return Bit buffer containing the serialized object.
 *
 * \throw CppRuntimeException When serialization fails.
 */
template <typename T, typename... ARGS, typename ::std::enable_if_t<!is_first_alloc<ARGS...>::value, int> = 0>
BasicBitBuffer<typename T::allocator_type> serialize(const T& data, ARGS&&... arguments)
{
    return serialize(data, typename T::allocator_type(), ::std::forward<ARGS>(arguments)...);
}

/**
 * Deserializes bit buffer to instance of generated object.
 *
 * Example:
 * \code{.cpp}
 *     #include <zserio/SerializeUtil.h>
 *
 *     SomeZserioObject objectData;
 *     const zserio::BitBuffer bitBuffer = zserio::serialize(objectData);
 *     zserio::View<SomeZserioObject> objectView = zserio::deserialize(bitBuffer, objectData);
 * \endcode
 *
 * \param bitBuffer Bit buffer to use.
 * \param data Data of Zserio object to fill.
 * \param arguments All parameters in case of Zserio parameterized type.
 *
 * \return View of Zserio object created from the given bit buffer.
 *
 * \throw CppRuntimeException When deserialization fails.
 */
template <typename T, typename ALLOC, typename... ARGS,
        typename ::std::enable_if_t<!is_first_alloc<ARGS...>::value, int> = 0>
typename ::zserio::View<T> deserialize(const BasicBitBuffer<ALLOC>& buffer, T& data, ARGS&&... arguments)
{
    BitStreamReader reader(buffer);

    return detail::read(reader, data, ::std::forward<ARGS>(arguments)...);
}

/**
 * Serializes Zserio object using View to the file.
 *
 * Example:
 * \code{.cpp}
 *     #include <zserio/SerializeUtil.h>
 *
 *     SomeZserioObject objectData;
 *     zserio::View<SomeZserioObject> objectView(objectData);
 *     zserio::serializeToFile(objectView, "FileName.bin");
 * \endcode
 *
 * \param view View of Zserio object to serialize.
 * \param fileName File name to write.
 *
 * \throw CppRuntimeException When serialization fails.
 */
template <typename T>
void serializeToFile(const View<T>& view, const std::string& fileName)
{
    const auto bitBuffer = serialize(view);
    writeBufferToFile(bitBuffer, fileName);
}

/**
 * Serializes Zserio object using Data to the file.
 *
 * Example:
 * \code{.cpp}
 *     #include <zserio/SerializeUtil.h>
 *
 *     SomeZserioObject objectData;
 *     zserio::serializeToFile(objectData, "FileName.bin");
 * \endcode
 *
 * \param data Data of Zserio object to serialize.
 * \param fileName File name to write.
 * \param arguments All parameters in case of Zserio parameterized type.
 *
 * \throw CppRuntimeException When serialization fails.
 */
template <typename T, typename... ARGS>
void serializeToFile(const T& data, const std::string& fileName, ARGS&&... arguments)
{
    const View<T> view(data, ::std::forward<ARGS>(arguments)...);
    serializeToFile(view, fileName);
}

/**
 * Deserializes given file contents to instance of Zserio object.
 *
 * Example:
 * \code{.cpp}
 *     #include <zserio/SerializeUtil.h>
 *
 *     const std::string fileName = "FileName.bin";
 *     SomeZserioObject objectData;
 *     zserio::serializeToFile(objectData, fileName);
 *     zserio::View<SomeZserioObject> objectView = zserio::deserializeFromFile(fileName, objectData);
 * \endcode
 *
 * \note Please note that BitBuffer is always allocated using 'std::allocator<uint8_t>'.
 *
 * \param fileName File to use.
 * \param arguments All parameters in case of Zserio parameterized type.
 *
 * \return View of Zserio object created from the given file.
 *
 * \throw CppRuntimeException When deserialization fails.
 */
template <typename T, typename... ARGS>
::zserio::View<T> deserializeFromFile(const std::string& fileName, T& data, ARGS&&... arguments)
{
    const BitBuffer bitBuffer = readBufferFromFile(fileName);
    return deserialize(bitBuffer, data, std::forward<ARGS>(arguments)...);
}

} // namespace zserio

#endif // ZSERIO_SERIALIZE_UTIL_H_INC