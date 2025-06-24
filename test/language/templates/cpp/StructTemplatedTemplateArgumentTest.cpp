#include "gtest/gtest.h"
#include "templates/struct_templated_template_argument/StructTemplatedTemplateArgument.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_templated_template_argument
{

TEST(StructTemplatedTemplateArgumentTest, writeRead)
{
    StructTemplatedTemplateArgument data;
    data.compoundField = Field<Compound<zserio::UInt32>>{Compound<zserio::UInt32>{42}};

    test_utils::writeReadTest(data);
}

} // namespace struct_templated_template_argument
} // namespace templates
