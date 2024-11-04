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
void writeReadTestDetail(T& data, ARGS&&... arguments)
{
    zserio::View<T> view(data, ::std::forward<ARGS>(arguments)...);
    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);

    zserio::BitBuffer bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);
    ASSERT_EQ(bitSize, writer.getBitPosition());

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    T readData;
    const zserio::View<T> readView = zserio::detail::read(reader, readData, ::std::forward<ARGS>(arguments)...);
    ASSERT_EQ(bitSize, reader.getBitPosition());
    ASSERT_EQ(readView, view);
}

template <typename T, typename... ARGS>
void writeReadTestSerializeData(T& data, ARGS&&... arguments)
{
    zserio::View<T> view(data, ::std::forward<ARGS>(arguments)...);
    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);

    auto bitBuffer = zserio::serialize(data, ::std::forward<ARGS>(arguments)...);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());

    T readData;
    const zserio::View<T> readView = zserio::deserialize(bitBuffer, data, ::std::forward<ARGS>(arguments)...);
    ASSERT_EQ(view, readView);
}

template <typename T, typename... ARGS>
void writeReadTestSerializeView(T& data, ARGS&&... arguments)
{
    zserio::View<T> view(data, ::std::forward<ARGS>(arguments)...);
    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);

    auto bitBuffer = zserio::serialize(view);
    ASSERT_EQ(bitSize, bitBuffer.getBitSize());

    T readData;
    const zserio::View<T> readView = zserio::deserialize(bitBuffer, data, ::std::forward<ARGS>(arguments)...);
    ASSERT_EQ(view, readView);
}

} // namespace detail

template <typename T, typename... ARGS>
void writeReadTest(T& data, ARGS&&... arguments)
{
    detail::writeReadTestDetail(data, std::forward<ARGS>(arguments)...);
    detail::writeReadTestSerializeData(data, std::forward<ARGS>(arguments)...);
    detail::writeReadTestSerializeView(data, std::forward<ARGS>(arguments)...);
}

} // namespace test_utils

#endif // TEST_UTILS_WRITE_READ_TEST_H_INC
