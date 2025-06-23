#include "gtest/gtest.h"
#include "templates/expression_full_template_argument/FullTemplateArgumentHolder.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace expression_full_template_argument
{

TEST(ExpressionFullTemplateArgumentTest, writeRead)
{
    const FullTemplateArgument<Color> colorInternal(false, zserio::Int32(10));
    ASSERT_TRUE(zserio::View(colorInternal).expressionField());
    const FullTemplateArgument<import_color::Color> colorExternal(false, std::nullopt);
    ASSERT_FALSE(zserio::View(colorExternal).expressionField());
    FullTemplateArgumentHolder data(colorInternal, colorExternal);

    test_utils::writeReadTest(data);
}

} // namespace expression_full_template_argument
} // namespace templates
