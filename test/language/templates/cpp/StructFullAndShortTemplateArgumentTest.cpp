#include "gtest/gtest.h"
#include "templates/struct_full_and_short_template_argument/StructFullNameTemplateArgument.h"
#include "templates/struct_full_and_short_template_argument/templated_struct/StructShortNameTemplateArgument.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_full_and_short_template_argument
{

TEST(StructFullAndShortTemplateArgumentTest, writeReadFull)
{
    StructFullNameTemplateArgument data(
            templated_struct::TemplatedStruct<templated_struct::Storage>(templated_struct::Storage("String")));

    test_utils::writeReadTest(data);
}

TEST(StructFullAndShortTemplateArgumentTest, writeReadShort)
{
    templated_struct::StructShortNameTemplateArgument data(
            templated_struct::TemplatedStruct<templated_struct::Storage>(templated_struct::Storage("String")));

    test_utils::writeReadTest(data);
}

} // namespace struct_full_and_short_template_argument
} // namespace templates
