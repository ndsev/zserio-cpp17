#include "expressions/expressions_in_templates/ExpressionsInTemplates.h"
#include "gtest/gtest.h"
#include "test_utils/WriteReadTest.h"

namespace expressions
{
namespace expressions_in_templates
{

using AllocatorType = ExpressionsInTemplates::allocator_type;
using BytesType = zserio::BasicBytes<AllocatorType>;

TEST(ExpressionsInTemplatesTest, writeRead)
{
    ExpressionsInTemplates data;
    data.arrayExpressions.array = {13, 15};
    data.arrayExpressions.isZeroArrayValid = {};
    data.constExpressions.value = Constant;
    data.colorExpressions.color = ColorE::BLUE;
    data.floatExpressions.extra = zserio::Float64(1.0);
    data.functionExpressions.color = ColorE::BLUE;
    data.lengthOfExpressions.strField = "shalom";
    data.lengthOfExpressions.bytesField = BytesType{0x10, 0x20};
    data.parameterizedArrayExpressions.holder.array.resize(2);
    data.parameterizedArrayExpressions.isValueZero = true;

    test_utils::writeReadTest(data);
}

TEST(ExpressionsInTemplatesTest, hash)
{
    ExpressionsInTemplates data;

    ASSERT_EQ(std::hash<ExpressionsInTemplates>()(data), static_cast<size_t>(556366779ULL));
}

} // namespace expressions_in_templates
} // namespace expressions
