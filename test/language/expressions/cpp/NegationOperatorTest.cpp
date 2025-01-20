#include "expressions/negation_operator/NegationOperatorExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace negation_operator
{

TEST(NumBitsOperatorTest, NegatedValue)
{
    NegationOperatorExpression data;
    data.value = true;
    zserio::View view(data);

    ASSERT_FALSE(view.negatedValue());

    data.value = false;
    ASSERT_TRUE(view.negatedValue());
}

} // namespace negation_operator
} // namespace expressions
