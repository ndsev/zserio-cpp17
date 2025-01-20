#include "expressions/const_type/ConstTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace const_type
{

TEST(ConstTypeTest, bitSizeOfWithOptional)
{
    ConstTypeExpression data;
    const uint8_t validValue = 0x01;
    const uint8_t additionalValue = 0x03;
    data.value = validValue;
    data.additionalValue = additionalValue;

    zserio::View view(data);

    const size_t constTypeExpressionBitSizeWithOptional = 10;
    ASSERT_EQ(constTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(ConstTypeTest, bitSizeOfWithoutOptional)
{
    ConstTypeExpression data;
    const uint8_t invalidValue = 0x00;
    data.value = invalidValue;

    zserio::View view(data);

    const size_t constTypeExpressionBitSizeWithoutOptional = 7;
    ASSERT_EQ(constTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace const_type
} // namespace expressions
