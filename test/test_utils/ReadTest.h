#ifndef TEST_UTILS_READ_TEST_H_INC
#define TEST_UTILS_READ_TEST_H_INC

#include <functional>
#include <utility>

#include "gtest/gtest.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/View.h"

namespace test_utils
{

template <typename T, typename... ARGS>
void readTest(
        std::function<void(zserio::BitStreamWriter&)> writeData, const T& expectedReadData, ARGS&&... arguments)
{
    const zserio::View<T> view(expectedReadData, arguments...);
    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    zserio::BitBuffer bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    T readData;
    zserio::View<T> readView = zserio::detail::read(reader, readData, arguments...);

    ASSERT_EQ(expectedReadData, readData);

    zserio::View<T> expectedReadView(expectedReadData, arguments...);
    ASSERT_EQ(expectedReadView, readView);
}

} // namespace test_utils

#endif // TEST_UTILS_READ_TEST_H_INC
