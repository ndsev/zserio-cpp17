#include "expressions/string_type/CHOOSER.h"
#include "expressions/string_type/STRING_CONSTANT.h"
#include "expressions/string_type/StringTypeExpression.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"

namespace expressions
{
namespace string_type
{

using AllocatorType = StringTypeExpression::AllocatorType;

class StringTypeTest : public ::testing::Test
{
protected:
    static StringTypeExpression createData(bool hasValue)
    {
        StringTypeExpression data;
        data.hasValue = hasValue;
        if (hasValue)
        {
            data.value = zserio::toString<AllocatorType>(VALUE);
        }

        return data;
    }

    static constexpr std::string_view VALUE = "value";
    static constexpr std::string_view APPEND = "append";
    static constexpr std::string_view IEKS = "ix";
    static constexpr std::string_view LITERAL = "literal";
    static constexpr std::string_view EXPRESSION = "expression";
    static constexpr std::string_view FALSE = "false";
    static constexpr std::string_view CHOSEN = "chosen";
    static constexpr std::string_view SPACE = " ";
    static constexpr std::string_view UNDERSCORE = "_";
};

TEST_F(StringTypeTest, returnValue)
{
    const StringTypeExpression data = createData(true);
    zserio::View view(data);
    ASSERT_EQ(VALUE, view.returnValue());
}

TEST_F(StringTypeTest, returnDefaultValue)
{
    const StringTypeExpression data = createData(true);
    zserio::View view(data);
    ASSERT_EQ(CHOOSER ? zserio::toString(STRING_CONSTANT)
                      : zserio::toString(FALSE) + zserio::toString(SPACE) + zserio::toString(STRING_CONSTANT),
            zserio::toString(view.returnDefaultValue()));
}

TEST_F(StringTypeTest, returnDefaultChosen)
{
    const StringTypeExpression data = createData(true);
    zserio::View view(data);
    ASSERT_EQ(CHOOSER ? zserio::toString(CHOSEN) + zserio::toString(SPACE) + zserio::toString(STRING_CONSTANT)
                      : std::string(),
            zserio::toString(view.returnDefaultChosen()));
}

TEST_F(StringTypeTest, appendix)
{
    const StringTypeExpression data = createData(false);
    zserio::View view(data);
    ASSERT_EQ(zserio::toString(APPEND) + zserio::toString(IEKS), zserio::toString(view.appendix()));
}

TEST_F(StringTypeTest, appendToConst)
{
    const StringTypeExpression data = createData(false);
    zserio::View view(data);
    ASSERT_EQ(zserio::toString(STRING_CONSTANT) + zserio::toString(UNDERSCORE) + zserio::toString(APPEND) +
                    zserio::toString(IEKS),
            zserio::toString(view.appendToConst()));
}

TEST_F(StringTypeTest, valueOrLiteral)
{
    StringTypeExpression data1 = createData(true);
    zserio::View view1(data1);
    ASSERT_EQ(VALUE, view1.valueOrLiteral());
    StringTypeExpression data2 = createData(false);
    zserio::View view2(data2);
    ASSERT_EQ(LITERAL, view2.valueOrLiteral());
}

TEST_F(StringTypeTest, valueOrLiteralExpression)
{
    StringTypeExpression data1 = createData(true);
    zserio::View view1(data1);
    ASSERT_EQ(VALUE, view1.valueOrLiteralExpression());
    StringTypeExpression data2 = createData(false);
    zserio::View view2(data2);
    ASSERT_EQ(zserio::toString(LITERAL) + zserio::toString(SPACE) + zserio::toString(EXPRESSION),
            zserio::toString(view2.valueOrLiteralExpression()));
}

TEST_F(StringTypeTest, valueOrConst)
{
    StringTypeExpression data1 = createData(true);
    zserio::View view1(data1);
    ASSERT_EQ(VALUE, view1.valueOrConst());
    StringTypeExpression data2 = createData(false);
    zserio::View view2(data2);
    ASSERT_EQ(STRING_CONSTANT, view2.valueOrConst());
}

TEST_F(StringTypeTest, valueOrConstExpression)
{
    StringTypeExpression data1 = createData(true);
    zserio::View view1(data1);
    ASSERT_EQ(VALUE, view1.valueOrConstExpression());
    StringTypeExpression data2 = createData(false);
    zserio::View view2(data2);
    ASSERT_EQ(zserio::toString(STRING_CONSTANT) + zserio::toString(SPACE) + zserio::toString(EXPRESSION),
            zserio::toString(view2.valueOrConstExpression()));
}

} // namespace string_type
} // namespace expressions
