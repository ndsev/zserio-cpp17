#include "gtest/gtest.h"
#include "templates/expression_enum_template_argument/EnumTemplateArgumentHolder.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace expression_enum_template_argument
{

TEST(ExpressionEnumTemplateArgumentTest, readWrite)
{
    const EnumTemplateArgument<Color> enumTemplateArgument_Color(false, zserio::Int32(10));
    ASSERT_TRUE(zserio::View(enumTemplateArgument_Color).expressionField());

    EnumTemplateArgumentHolder data(enumTemplateArgument_Color);
    test_utils::writeReadTest(data);
}

} // namespace expression_enum_template_argument
} // namespace templates
