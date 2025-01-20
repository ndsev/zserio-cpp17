#include "expressions/full_enumeration_type/FullEnumerationTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace full_enumeration_type
{

TEST(FullEnumerationTypeTest, bitSizeOfWithOptional)
{
    FullEnumerationTypeExpression data;
    data.color = Color::RED;
    data.isColorRed = true;

    zserio::View view(data);

    const size_t fullEnumerationTypeExpressionBitSizeWithOptional = 9;
    ASSERT_EQ(fullEnumerationTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(FullEnumerationTypeTest, bitSizeOfWithoutOptional)
{
    FullEnumerationTypeExpression data;
    data.color = Color::BLUE;

    zserio::View view(data);

    const size_t fullEnumerationTypeExpressionBitSizeWithoutOptional = 8;
    ASSERT_EQ(fullEnumerationTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace full_enumeration_type
} // namespace expressions
