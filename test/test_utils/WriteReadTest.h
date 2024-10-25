#ifndef TEST_UTILS_WRITE_READ_TEST_H_INC
#define TEST_UTILS_WRITE_READ_TEST_H_INC

#include <utility>

#include "gtest/gtest.h"
#include "zserio/BitBuffer.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/View.h"

namespace test_utils
{

template <typename T, typename... ARGS>
void writeReadTest(T& data, ARGS&&... arguments)
{
    zserio::View<T> view(data, ::std::forward<ARGS>(arguments)...);

    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view, 0);
    zserio::BitBuffer bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    T readData;
    const zserio::View<T> readView = zserio::detail::read(reader, readData, ::std::forward<ARGS>(arguments)...);
    ASSERT_EQ(readView, view);
}

} // namespace test_utils

#endif // TEST_UTILS_WRITE_READ_TEST_H_INC
