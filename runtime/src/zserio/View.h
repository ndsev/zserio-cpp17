#ifndef ZSERIO_VIEW_H_INC
#define ZSERIO_VIEW_H_INC

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
void validate(const View<T>& view);

/**
 * Global function for writing provided via specialization.
 *
 * \param writer Bit stream writer to use for writing.
 * \param view Zserio View to use for writing.
 *
 * \throw CppRuntimeException In case of any write error.
 */
template <typename T>
void write(::zserio::BitStreamWriter& writer, const View<T>& view);

/**
 * Global function for reading provided via specialization.
 *
 * \param read Bit stream reader to read from.
 * \param data Zserio Data to fill with read data.
 * \param alloc Allocator to use.
 * \param args All parameters in case of Zserio parameterized type.
 *
 * \return View with read data.
 *
 * \throw CppRuntimeException In case of any read error.
 */
template <typename T, typename... ARGS>
View<T> read(
        ::zserio::BitStreamReader& reader, T& data, const typename T::allocator_type& alloc, ARGS&&... args);

/**
 * Global function for bit size provided via specialization.
 *
 * \param view Zserio View to use.
 * \param bitPosition Bit position to use.
 *
 * \return Bit size of the Zserio object.
 */
template <typename T>
BitSize bitSizeOf(const View<T>& view, BitSize bitPosition);

/**
 * Global function for offsets initialization provided via specialization.
 *
 * \param data Zserio Data to use for initialization.
 * \param bitPosition Bit position to use.
 * \param args All parameters in case of Zserio parameterized type.
 *
 * \return End bit position.
 */
template <typename T, typename... ARGS>
BitSize initializeOffsets(T& data, BitSize bitPosition, ARGS&&... args);

} // namespace detail

} // namespace zserio

#endif // ifndef ZSERIO_VIEW_H_INC
