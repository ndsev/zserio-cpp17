#include "gtest/gtest.h"
#include "templates/expression_bitmask_template_argument/BitmaskTemplateArgumentHolder.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace expression_bitmask_template_argument
{

TEST(ExpressionBitmaskTemplateArgumentTest, writeRead)
{
    const BitmaskTemplateArgument<Permission> bitmaskTemplateArgument_Permission(false, zserio::Int32(10));
    ASSERT_TRUE(zserio::View(bitmaskTemplateArgument_Permission).expressionField());
    BitmaskTemplateArgumentHolder data(bitmaskTemplateArgument_Permission);

    test_utils::writeReadTest(data);
}

} // namespace expression_bitmask_template_argument
} // namespace templates
