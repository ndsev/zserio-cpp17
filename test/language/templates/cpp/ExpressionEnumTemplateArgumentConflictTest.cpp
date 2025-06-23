#include "gtest/gtest.h"
#include "templates/expression_enum_template_argument_conflict/EnumTemplateArgumentConflictHolder.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace expression_enum_template_argument_conflict
{

TEST(ExpressionEnumTemplateArgumentConflictTest, writeRead)
{
    const EnumTemplateArgumentConflict<Letters> enumTemplateArgumentConflict_Letters(false, zserio::Int32(10));
    ASSERT_TRUE(zserio::View(enumTemplateArgumentConflict_Letters).expressionField());
    EnumTemplateArgumentConflictHolder data(enumTemplateArgumentConflict_Letters);

    test_utils::writeReadTest(data);
}

} // namespace expression_enum_template_argument_conflict
} // namespace templates
