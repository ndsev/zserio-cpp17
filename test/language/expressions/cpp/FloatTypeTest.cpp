#include "expressions/float_type/FloatTypeExpression.h"
#include "gtest/gtest.h"

namespace expressions
{
namespace float_type
{

TEST(FloatTypeTest, result)
{
    FloatTypeExpression data;
    const float floatValue = 15.0;
    data.floatValue = floatValue;

    zserio::View view(data);

    const bool result = floatValue * 2.0F + 1.0F / 0.5F > 1.0F;
    ASSERT_EQ(result, view.result());
}

} // namespace float_type
} // namespace expressions
