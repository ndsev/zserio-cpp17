#include "expressions/valueof_operator/ValueOfFunctions.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace valueof_operator
{

TEST(ValueOfOperatorTest, GetValueOfWhiteColor)
{
    ValueOfFunctions data;
    data.color = Color::WHITE;
    zserio::View view(data);

    const uint8_t whiteColor = 1;
    ASSERT_EQ(whiteColor, view.getValueOfColor());
    ASSERT_EQ(whiteColor, view.getValueOfWhiteColor());
}

TEST(ValueOfOperatorTest, GetValueOfBlackColor)
{
    ValueOfFunctions data;
    data.color = Color::BLACK;
    zserio::View view(data);

    const uint8_t blackColor = 2;
    ASSERT_EQ(blackColor, view.getValueOfColor());
    ASSERT_EQ(blackColor, view.getValueOfBlackColor());
}

} // namespace valueof_operator
} // namespace expressions
