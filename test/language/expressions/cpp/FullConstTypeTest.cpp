#include "expressions/full_const_type/FullConstTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace full_const_type
{

TEST(FullConstTypeTest, bitSizeOfWithOptional)
{
    FullConstTypeExpression data;
    const uint8_t fullValidValue = 0x01;
    const uint8_t additionalValue = 0x03;
    data.value = fullValidValue;
    data.additionalValue = additionalValue;

    zserio::View view(data);

    const size_t fullConstTypeExpressionBitSizeWithOptional = 10;
    ASSERT_EQ(fullConstTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(FullConstTypeTest, bitSizeOfWithoutOptional)
{
    FullConstTypeExpression data;
    const uint8_t fullInvalidValue = 0x00;
    data.value = fullInvalidValue;

    zserio::View view(data);

    const size_t fullConstTypeExpressionBitSizeWithoutOptional = 7;
    ASSERT_EQ(fullConstTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace full_const_type
} // namespace expressions
