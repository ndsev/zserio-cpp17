#ifndef TEST_UTILS_WRITE_READ_TEST_H_INC
#define TEST_UTILS_WRITE_READ_TEST_H_INC

#include <utility>

#include "gtest/gtest.h"
#include "zserio/SerializeUtil.h"

namespace test_utils
{

namespace detail
{

template <typename T, typename... ARGS>
void writeReadTestDetail(const T& data, ARGS&&... arguments)
{
    zserio::View view(data, arguments...);
    zserio::detail::validate(view);

    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);

    zserio::BitBuffer bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    T readData;
    const zserio::View readView = zserio::detail::read(reader, readData, arguments...);
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(readView, view);
}

template <typename T, typename... ARGS>
void writeReadTestSerializeData(const T& data, ARGS&&... arguments)
{
    zserio::View view(data, arguments...);
    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);

    auto bitBuffer = zserio::serialize(data, arguments...);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());

    T readData;
    const zserio::View readView = zserio::deserialize(bitBuffer, readData, arguments...);
    ASSERT_EQ(view, readView);
}

template <typename T, typename... ARGS>
void writeReadTestSerializeView(const T& data, ARGS&&... arguments)
{
    zserio::View view(data, arguments...);
    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);

    auto bitBuffer = zserio::serialize(view);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());

    T readData;
    const zserio::View readView = zserio::deserialize(bitBuffer, readData, arguments...);
    ASSERT_EQ(view, readView);
}

template <typename T, typename... ARGS>
void writeReadTestSerializeDataView(const T& data, ARGS&&... arguments)
{
    zserio::DataView dataView(data, arguments...);
    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(dataView);

    auto bitBuffer = zserio::serialize(dataView);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());

    const zserio::DataView readDataView = zserio::deserialize<T>(bitBuffer, arguments...);
    ASSERT_EQ(dataView, readDataView);
}

} // namespace detail

template <typename T, typename... ARGS>
void writeReadTest(const T& data, ARGS&&... arguments)
{
    detail::writeReadTestDetail(data, arguments...);
    detail::writeReadTestSerializeData(data, arguments...);
    detail::writeReadTestSerializeView(data, arguments...);
    detail::writeReadTestSerializeDataView(data, arguments...);
}

} // namespace test_utils

#endif // TEST_UTILS_WRITE_READ_TEST_H_INC
