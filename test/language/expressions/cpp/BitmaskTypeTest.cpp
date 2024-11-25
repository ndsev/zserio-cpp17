#include "expressions/bitmask_type/BitmaskTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace bitmask_type
{

TEST(BitmaskTypeTest, bitSizeOfNoColor)
{
    BitmaskTypeExpression data;
    data.colors = Colors();
    data.hasNotColorRed = true;

    ASSERT_EQ(9, zserio::detail::bitSizeOf(zserio::View<BitmaskTypeExpression>(data)));
}

TEST(BitmaskTypeTest, bitSizeOfRed)
{
    BitmaskTypeExpression data;
    data.colors = Colors::Values::RED;
    data.hasColorRed = true;

    ASSERT_EQ(9, zserio::detail::bitSizeOf(zserio::View<BitmaskTypeExpression>(data)));
}

TEST(BitmaskTypeTest, bitSizeOfGreen)
{
    BitmaskTypeExpression data;
    data.colors = Colors::Values::GREEN;
    data.hasColorGreen = true;
    data.hasNotColorRed = true;
    data.hasOtherColorThanRed = true;

    ASSERT_EQ(11, zserio::detail::bitSizeOf(zserio::View<BitmaskTypeExpression>(data)));
}

TEST(BitmaskTypeTest, bitSizeOfBlue)
{
    BitmaskTypeExpression data;
    data.colors = Colors::Values::BLUE;
    data.hasColorBlue = true;
    data.hasNotColorRed = true;
    data.hasOtherColorThanRed = true;

    ASSERT_EQ(11, zserio::detail::bitSizeOf(zserio::View<BitmaskTypeExpression>(data)));
}

TEST(BitmaskTypeTest, bitSizeOfBlueGreen)
{
    BitmaskTypeExpression data;
    data.colors = Colors::Values::BLUE | Colors::Values::GREEN;
    data.hasColorGreen = true;
    data.hasColorBlue = true;
    data.hasNotColorRed = true;
    data.hasOtherColorThanRed = true;

    ASSERT_EQ(12, zserio::detail::bitSizeOf(zserio::View<BitmaskTypeExpression>(data)));
}

TEST(BitmaskTypeTest, bitSizeOfAllColors)
{
    BitmaskTypeExpression data;
    data.colors = Colors::Values::RED | Colors::Values::GREEN | Colors::Values::BLUE;
    data.hasColorRed = true;
    data.hasColorGreen = true;
    data.hasColorBlue = true;
    data.hasAllColors = true;
    data.hasOtherColorThanRed = true;

    ASSERT_EQ(13, zserio::detail::bitSizeOf(zserio::View<BitmaskTypeExpression>(data)));
}

} // namespace bitmask_type
} // namespace expressions
