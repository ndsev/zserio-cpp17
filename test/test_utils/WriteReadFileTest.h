#ifndef TEST_UTILS_WRITE_READ_FILE_TEST_H_INC
#define TEST_UTILS_WRITE_READ_FILE_TEST_H_INC

#include <string>
#include <utility>

#include "gtest/gtest.h"
#include "zserio/SerializeUtil.h"
#include "zserio/View.h"

namespace test_utils
{

template <typename T, typename... ARGS>
void writeReadFileTest(const std::string& fileName, T& data, ARGS&&... arguments)
{
    zserio::View<T> view(data, ::std::forward<ARGS>(arguments)...);
    zserio::serializeToFile(view, fileName);

    T readData;
    const zserio::View<T> readView =
            zserio::deserializeFromFile(fileName, readData, ::std::forward<ARGS>(arguments)...);
    ASSERT_EQ(readData, data);
    ASSERT_EQ(readView, view);
}

} // namespace test_utils

#endif // TEST_UTILS_WRITE_READ_FILE_TEST_H_INC
