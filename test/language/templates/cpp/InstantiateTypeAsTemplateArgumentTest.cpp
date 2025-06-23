#include "gtest/gtest.h"
#include "templates/instantiate_type_as_template_argument/InstantiateTypeAsTemplateArgument.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_type_as_template_argument
{

TEST(InstantiateTypeAsTemplateArgumentTest, writeRead)
{
    InstantiateTypeAsTemplateArgument instantiateTypeAsTemplateArgument{Other<Str>{Str{"test"}}};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_type_as_template_argument
} // namespace templates
