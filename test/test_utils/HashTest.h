#ifndef TEST_UTILS_HASH_TEST_H_INC
#define TEST_UTILS_HASH_TEST_H_INC

#include <memory>

#include "gtest/gtest.h"

namespace test_utils
{

template <typename T>
void hashTest(const T& value, size_t hashValue, const T& equalValue, const T& diffValue, size_t diffHashValue)
{
    std::hash<T> hasher;

    EXPECT_NE(hashValue, diffHashValue);
    EXPECT_EQ(hashValue, hasher(value));
    EXPECT_EQ(hashValue, hasher(equalValue));
    EXPECT_EQ(diffHashValue, hasher(diffValue));
}

} // namespace test_utils

#endif // TEST_UTILS_HASH_TEST_H_INC
