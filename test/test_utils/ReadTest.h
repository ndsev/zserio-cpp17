#ifndef TEST_UTILS_READ_TEST_H_INC
#define TEST_UTILS_READ_TEST_H_INC

#include <utility>

#include "gtest/gtest.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/View.h"

namespace test_utils
{

template <typename T, typename... ARGS>
void readTest(const zserio::BitStreamWriter& writer, T& expectedReadData, ARGS&&... arguments)
{
    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    T readData;
    zserio::View<T> readView = zserio::detail::read(reader, readData, ::std::forward<ARGS>(arguments)...);

    ASSERT_EQ(expectedReadData, readData);

    zserio::View<T> expectedReadView(expectedReadData, ::std::forward<ARGS>(arguments)...);
    ASSERT_EQ(expectedReadView, readView);
}

} // namespace test_utils

#endif // TEST_UTILS_READ_TEST_H_INC
