#include "expressions/question_mark/QuestionMarkExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace question_mark
{

TEST(QuestionMarkTest, firstValue)
{
    QuestionMarkExpression data;
    const uint8_t firstValue = 0x11;
    data.firstValue = firstValue;
    data.secondValue = 0x22;
    data.isFirstValueValid = true;

    zserio::View view(data);

    ASSERT_EQ(firstValue, view.validValue());
}

TEST(QuestionMarkTest, secondValue)
{
    QuestionMarkExpression data;
    data.firstValue = 0x11;
    const uint8_t secondValue = 0x22;
    data.secondValue = secondValue;
    data.isFirstValueValid = false;

    zserio::View view(data);

    ASSERT_EQ(secondValue, view.validValue());
}

} // namespace question_mark
} // namespace expressions
