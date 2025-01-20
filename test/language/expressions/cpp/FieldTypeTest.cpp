#include "expressions/field_type/FieldTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace field_type
{

TEST(FieldTypeTest, bitSizeOfWithOptional)
{
    ContainedType containedType;
    containedType.needsExtraValue = true;
    FieldTypeExpression data;
    data.containedType = containedType;
    data.extraValue = 0x02;

    zserio::View view(data);

    const size_t fieldTypeExpressionBitSizeWithOptional = 4;
    ASSERT_EQ(fieldTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(FieldTypeTest, bitSizeOfWithoutOptional)
{
    ContainedType containedType;
    containedType.needsExtraValue = false;
    FieldTypeExpression data;
    data.containedType = containedType;

    zserio::View view(data);

    const size_t fieldTypeExpressionBitSizeWithoutOptional = 1;
    ASSERT_EQ(fieldTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace field_type
} // namespace expressions
