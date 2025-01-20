#include "expressions/parameter_type/ParameterTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace parameter_type
{

TEST(ParameterTypeTest, bitSizeOfWithOptional)
{
    ParameterTypeExpression data;
    data.value = 0x3F;
    data.isParameterRed = true;

    zserio::View view(data, Color::RED);

    const size_t parameterTypeExpressionBitSizeWithOptional = 8;
    ASSERT_EQ(parameterTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(ParameterTypeTest, bitSizeOfWithoutOptional)
{
    ParameterTypeExpression data;
    data.value = 0x3F;

    zserio::View view(data, Color::BLUE);

    const size_t parameterTypeExpressionBitSizeWithoutOptional = 7;
    ASSERT_EQ(parameterTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace parameter_type
} // namespace expressions
