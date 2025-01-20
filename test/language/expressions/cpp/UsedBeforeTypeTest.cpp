#include "expressions/used_before_type/UsedBeforeTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace used_before_type
{

TEST(UsedBeforeTypeTest, bitSizeOfWithOptional)
{
    UsedBeforeTypeExpression data;
    data.color = Color::RED;
    data.isRedColorLight = true;

    zserio::View view(data);

    const size_t usedBeforeTypeExpressionBitSizeWithOptional = 8;
    ASSERT_EQ(usedBeforeTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(UsedBeforeTypeTest, bitSizeOfWithoutOptional)
{
    UsedBeforeTypeExpression data;
    data.color = Color::BLUE;

    zserio::View view(data);

    const size_t usedBeforeTypeExpressionBitSizeWithoutOptional = 7;
    ASSERT_EQ(usedBeforeTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace used_before_type
} // namespace expressions
