#ifndef TEST_UTILS_COMPARISON_OPERATORS_TEST_H_INC
#define TEST_UTILS_COMPARISON_OPERATORS_TEST_H_INC

#include "gtest/gtest.h"

namespace test_utils
{

namespace detail
{

template <typename T>
void operatorEqualToTest(const T& value, const T& equalValue)
{
    ASSERT_TRUE(value == equalValue);
}

template <typename T>
void operatorEqualToTest(const T& value, const T& equalValue, const T& lessThanValue)
{
    operatorEqualToTest(value, equalValue);
    ASSERT_FALSE(value == lessThanValue);
    ASSERT_FALSE(equalValue == lessThanValue);
}

template <typename T>
void operatorLessThanTest(const T& value, const T& equalValue)
{
    ASSERT_FALSE(value < equalValue);
}

template <typename T>
void operatorLessThanTest(const T& value, const T& equalValue, const T& lessThanValue)
{
    operatorLessThanTest(value, equalValue);
    ASSERT_TRUE(lessThanValue < value);
    ASSERT_FALSE(equalValue < lessThanValue);
}

template <typename T>
void operatorNotEqualTest(const T& value, const T& equalValue)
{
    ASSERT_FALSE(value != equalValue);
}

template <typename T>
void operatorNotEqualTest(const T& value, const T& equalValue, const T& lessThanValue)
{
    operatorNotEqualTest(value, equalValue);
    ASSERT_TRUE(value != lessThanValue);
    ASSERT_TRUE(equalValue != lessThanValue);
}

template <typename T>
void operatorGreaterThanTest(const T& value, const T& equalValue)
{
    ASSERT_FALSE(value > equalValue);
}

template <typename T>
void operatorGreaterThanTest(const T& value, const T& equalValue, const T& lessThanValue)
{
    operatorGreaterThanTest(value, equalValue);
    ASSERT_FALSE(lessThanValue > value);
    ASSERT_TRUE(equalValue > lessThanValue);
}

template <typename T>
void operatorLessThanOrEqualToTest(const T& value, const T& equalValue)
{
    ASSERT_TRUE(value <= equalValue);
}

template <typename T>
void operatorLessThanOrEqualToTest(const T& value, const T& equalValue, const T& lessThanValue)
{
    operatorLessThanOrEqualToTest(value, equalValue);
    ASSERT_TRUE(lessThanValue <= value);
    ASSERT_FALSE(equalValue <= lessThanValue);
}

template <typename T>
void operatorGreaterThanOrEqualTest(const T& value, const T& equalValue)
{
    ASSERT_TRUE(value >= equalValue);
}

template <typename T>
void operatorGreaterThanOrEqualTest(const T& value, const T& equalValue, const T& lessThanValue)
{
    operatorGreaterThanOrEqualTest(value, equalValue);
    ASSERT_FALSE(lessThanValue >= value);
    ASSERT_TRUE(equalValue >= lessThanValue);
}

} // namespace detail

template <typename T>
void comparisonOperatorsTest(const T& value, const T& equalValue)
{
    detail::operatorEqualToTest(value, equalValue);
    detail::operatorLessThanTest(value, equalValue);
    detail::operatorNotEqualTest(value, equalValue);
    detail::operatorGreaterThanTest(value, equalValue);
    detail::operatorLessThanOrEqualToTest(value, equalValue);
    detail::operatorGreaterThanOrEqualTest(value, equalValue);
}

template <typename T>
void comparisonOperatorsTest(const T& value, const T& equalValue, const T& lessThanValue)
{
    detail::operatorEqualToTest(value, equalValue, lessThanValue);
    detail::operatorLessThanTest(value, equalValue, lessThanValue);
    detail::operatorNotEqualTest(value, equalValue, lessThanValue);
    detail::operatorGreaterThanTest(value, equalValue, lessThanValue);
    detail::operatorLessThanOrEqualToTest(value, equalValue, lessThanValue);
    detail::operatorGreaterThanOrEqualTest(value, equalValue, lessThanValue);
}

} // namespace test_utils

#endif // TEST_UTILS_COMPARISON_OPERATORS_TEST_H_INC
