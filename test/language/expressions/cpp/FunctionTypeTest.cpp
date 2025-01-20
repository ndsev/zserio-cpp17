#include "expressions/function_type/FunctionTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace function_type
{

TEST(FunctionTypeTest, bitSizeOfWithOptional)
{
    FunctionTypeExpression data;
    data.color = Color::RED;
    data.isRedColorLight = true;

    zserio::View view(data);

    const size_t functionTypeExpressionBitSizeWithOptional = 9;
    ASSERT_EQ(functionTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(FunctionTypeTest, bitSizeOfWithoutOptional)
{
    FunctionTypeExpression data;
    data.color = Color::BLUE;

    zserio::View view(data);

    const size_t functionTypeExpressionBitSizeWithoutOptional = 8;
    ASSERT_EQ(functionTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace function_type
} // namespace expressions
