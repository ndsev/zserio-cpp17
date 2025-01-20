#include "expressions/parenthesis/ParenthesisExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace parenthesis
{

TEST(ParenthesisTest, firstValue)
{
    ParenthesisExpression data;
    const uint8_t firstValue = 0x11;
    data.firstValue = firstValue;
    const uint8_t secondValue = 0x22;
    data.secondValue = secondValue;

    zserio::View view(data);

    ASSERT_EQ(firstValue * (secondValue + 1), view.result());
}

} // namespace parenthesis
} // namespace expressions
