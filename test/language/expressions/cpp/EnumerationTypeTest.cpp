#include "expressions/enumeration_type/EnumerationTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace enumeration_type
{

TEST(EnumerationTypeTest, bitSizeOfWithOptional)
{
    EnumerationTypeExpression data;
    data.color = Color::RED;
    data.isColorRed = true;

    zserio::View view(data);

    const size_t enumerationTypeExpressionBitSizeWithOptional = 9;
    ASSERT_EQ(enumerationTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(EnumerationTypeTest, bitSizeOfWithoutOptional)
{
    EnumerationTypeExpression data;
    data.color = Color::BLUE;

    zserio::View view(data);

    const size_t enumerationTypeExpressionBitSizeWithoutOptional = 8;
    ASSERT_EQ(enumerationTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace enumeration_type
} // namespace expressions
