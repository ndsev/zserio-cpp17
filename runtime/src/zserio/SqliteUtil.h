#ifndef ZSERIO_SQLITE_UTIL_H_INC
#define ZSERIO_SQLITE_UTIL_H_INC

#include "zserio/Bitmasks.h"
#include "zserio/Enums.h"
#include "zserio/Optional.h"
#include "zserio/SerializeUtil.h"
#include "zserio/Span.h"
#include "zserio/Traits.h"

#include "sqlite3.h"

namespace zserio
{
namespace detail
{

/**
 * Row of an SQL table. Implementation provided by specializations.
 */
template <typename T>
struct SqlRow;

template <typename T, typename = void>
struct ColumnTraits;

template <typename T>
struct ColumnTraits<T, std::enable_if_t<is_complete_v<View<T>>>>
{
    static constexpr std::string_view TYPE_NAME = "BLOB";
    static constexpr int TYPE = SQLITE_BLOB;
};

template <typename T>
struct ColumnTraits<T, std::enable_if_t<std::is_enum_v<T> || is_bitmask_v<T>>>
{
    static constexpr std::string_view TYPE_NAME = "INTEGER";
    static constexpr int TYPE = SQLITE_INTEGER;
};

template <typename T>
struct ColumnTraits<T, std::enable_if_t<is_numeric_wrapper_v<T> && std::is_integral_v<typename T::ValueType>>>
{
    static constexpr std::string_view TYPE_NAME = "INTEGER";
    static constexpr int TYPE = SQLITE_INTEGER;
};

template <typename T>
struct ColumnTraits<T,
        std::enable_if_t<is_numeric_wrapper_v<T> && std::is_floating_point_v<typename T::ValueType>>>
{
    static constexpr std::string_view TYPE_NAME = "REAL";
    static constexpr int TYPE = SQLITE_FLOAT;
};

template <typename ALLOC>
struct ColumnTraits<BasicString<ALLOC>>
{
    static constexpr std::string_view TYPE_NAME = "TEXT";
    static constexpr int TYPE = SQLITE_TEXT;
};

template <typename ALLOC, typename T, typename... ARGS>
std::enable_if_t<is_complete_v<View<T>>> readColumn(
        BasicOptional<ALLOC, T>& column, sqlite3_stmt& stmt, int index, const ARGS&... args)
{
    // blob
    const void* blobDataPtr = sqlite3_column_blob(&stmt, index);
    const int blobDataLength = sqlite3_column_bytes(&stmt, index);
    Span<const uint8_t> blobData(static_cast<const uint8_t*>(blobDataPtr), static_cast<size_t>(blobDataLength));
    column.emplace();
    deserializeFromBytes(blobData, *column, args...);
}

template <typename ALLOC, typename T>
std::enable_if_t<std::is_enum_v<T>> readColumn(BasicOptional<ALLOC, T>& column, sqlite3_stmt& stmt, int index)
{
    const int64_t intValue = sqlite3_column_int64(&stmt, index);
    column = valueToEnum<T>(static_cast<std::underlying_type_t<T>>(intValue));
}

template <typename ALLOC, typename T>
std::enable_if_t<is_bitmask_v<T>> readColumn(BasicOptional<ALLOC, T>& column, sqlite3_stmt& stmt, int index)
{
    const int64_t intValue = sqlite3_column_int64(&stmt, index);
    column = T(static_cast<typename T::ZserioType::ValueType>(intValue));
}

template <typename ALLOC>
void readColumn(BasicOptional<ALLOC, Bool>& column, sqlite3_stmt& stmt, int index)
{
    const int64_t intValue = sqlite3_column_int64(&stmt, index);
    column = intValue != 0;
}

template <typename ALLOC, BitSize BIT_SIZE, bool IS_SIGNED>
void readColumn(
        BasicOptional<ALLOC, FixedIntWrapper<BIT_SIZE, IS_SIGNED>>& column, sqlite3_stmt& stmt, int index)
{
    const int64_t intValue = sqlite3_column_int64(&stmt, index);
    column = static_cast<typename FixedIntWrapper<BIT_SIZE, IS_SIGNED>::ValueType>(intValue);
}

template <typename ALLOC, typename T>
void readColumn(BasicOptional<ALLOC, DynIntWrapper<T>>& column, sqlite3_stmt& stmt, int index)
{
    const int64_t intValue = sqlite3_column_int64(&stmt, index);
    column = static_cast<T>(intValue);
}

template <typename ALLOC, typename VALUE_TYPE, VarIntType VAR_TYPE>
void readColumn(
        BasicOptional<ALLOC, VarIntWrapper<VALUE_TYPE, VAR_TYPE>>& column, sqlite3_stmt& stmt, int index)
{
    const int64_t intValue = sqlite3_column_int64(&stmt, index);
    column = static_cast<VALUE_TYPE>(intValue);
}

template <typename ALLOC, typename VALUE_TYPE, FloatType FLOAT_TYPE>
void readColumn(
        BasicOptional<ALLOC, FloatWrapper<VALUE_TYPE, FLOAT_TYPE>>& column, sqlite3_stmt& stmt, int index)
{
    const double doubleValue = sqlite3_column_double(&stmt, index);
    column = static_cast<VALUE_TYPE>(doubleValue);
}

template <typename ALLOC>
void readColumn(
        BasicOptional<ALLOC, BasicString<RebindAlloc<ALLOC, char>>>& column, sqlite3_stmt& stmt, int index)
{
    const unsigned char* textValue = sqlite3_column_text(&stmt, index);
    column.emplace(reinterpret_cast<const char*>(textValue));
}

template <typename T>
BitSize prepareColumn(const View<T>& view)
{
    validate(view);
    return initializeOffsets(view, 0);
}

template <typename T>
BitSize prepareColumn(const T&)
{
    return 0;
}

template <typename ALLOC, typename T>
int bindColumn(sqlite3_stmt& stmt, int index, const View<T>& view, BitSize bitSize, const ALLOC& allocator)
{
    BasicBitBuffer<ALLOC> bitBuffer(bitSize, allocator);
    BitStreamWriter writer(bitBuffer);
    write(writer, view);
    return sqlite3_bind_blob(
            &stmt, index, bitBuffer.getBuffer(), static_cast<int>(bitBuffer.getByteSize()), SQLITE_TRANSIENT);
}

template <typename ALLOC, typename T>
std::enable_if_t<std::is_enum_v<T>, int> bindColumn(
        sqlite3_stmt& stmt, int index, T value, BitSize, const ALLOC&)
{
    return sqlite3_bind_int64(&stmt, index, static_cast<int64_t>(value));
}

template <typename ALLOC, typename T>
std::enable_if_t<is_bitmask_v<T>, int> bindColumn(sqlite3_stmt& stmt, int index, T value, BitSize, const ALLOC&)
{
    return sqlite3_bind_int64(&stmt, index, static_cast<int64_t>(value.getValue()));
}

template <typename ALLOC>
int bindColumn(sqlite3_stmt& stmt, int index, Bool value, BitSize, const ALLOC&)
{
    return sqlite3_bind_int64(&stmt, index, static_cast<int64_t>(value));
}

template <typename ALLOC, BitSize BIT_SIZE, bool IS_SIGNED>
int bindColumn(sqlite3_stmt& stmt, int index, FixedIntWrapper<BIT_SIZE, IS_SIGNED> value, BitSize, const ALLOC&)
{
    return sqlite3_bind_int64(&stmt, index, static_cast<int64_t>(value));
}

template <typename ALLOC, typename T>
int bindColumn(sqlite3_stmt& stmt, DynIntWrapper<T> value, BitSize, int index, const ALLOC&)
{
    return sqlite3_bind_int64(&stmt, index, static_cast<int64_t>(value));
}

template <typename ALLOC, typename VALUE_TYPE, VarIntType VAR_TYPE>
int bindColumn(sqlite3_stmt& stmt, int index, VarIntWrapper<VALUE_TYPE, VAR_TYPE> value, BitSize, const ALLOC&)
{
    return sqlite3_bind_int64(&stmt, index, static_cast<int64_t>(value));
}

template <typename ALLOC, typename VALUE_TYPE, FloatType FLOAT_TYPE>
int bindColumn(sqlite3_stmt& stmt, int index, FloatWrapper<VALUE_TYPE, FLOAT_TYPE> value, BitSize, const ALLOC&)
{
    return sqlite3_bind_double(&stmt, index, static_cast<double>(value));
}

template <typename ALLOC>
int bindColumn(sqlite3_stmt& stmt, int index, std::string_view value, BitSize, const ALLOC&)
{
    return sqlite3_bind_text(&stmt, index, value.data(), static_cast<int>(value.size()), SQLITE_TRANSIENT);
}

} // namespace detail
} // namespace zserio

#endif // ZSERIO_SQLITE_UTIL_H_INC
