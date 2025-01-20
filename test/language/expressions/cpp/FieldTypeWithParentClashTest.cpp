#include "expressions/field_type_with_parent_clash/FieldTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace field_type_with_parent_clash
{

TEST(FieldTypeWithParentClashTest, bitSizeOfWithOptional)
{
    ContainedType containedType;
    containedType.needsExtraValue = true;
    FieldTypeExpression data;
    data.fieldTypeExpression = containedType;
    data.extraValue = 0x02;

    zserio::View view(data);

    const size_t fieldTypeExpressionBitSizeWithOptional = 4;
    ASSERT_EQ(fieldTypeExpressionBitSizeWithOptional, zserio::detail::bitSizeOf(view));
}

TEST(FieldTypeWithParentClashTest, bitSizeOfWithoutOptional)
{
    ContainedType containedType;
    containedType.needsExtraValue = false;
    FieldTypeExpression data;
    data.fieldTypeExpression = containedType;

    zserio::View view(data);

    const size_t fieldTypeExpressionBitSizeWithoutOptional = 1;
    ASSERT_EQ(fieldTypeExpressionBitSizeWithoutOptional, zserio::detail::bitSizeOf(view));
}

} // namespace field_type_with_parent_clash
} // namespace expressions
