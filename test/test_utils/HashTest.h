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

    ASSERT_NE(hashValue, diffHashValue);
    ASSERT_EQ(hashValue, hasher(value));
    ASSERT_EQ(hashValue, hasher(equalValue));
    ASSERT_EQ(diffHashValue, hasher(diffValue));
}

} // namespace test_utils

#endif // TEST_UTILS_HASH_TEST_H_INC
