#include "gtest/gtest.h"
#include "templates/instantiate_template_argument/InstantiateTemplateArgument.h"
#include "templates/instantiate_template_argument/Str.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_template_argument
{

TEST(InstantiateTemplateArgumentTest, writeRead)
{
    InstantiateTemplateArgument data{Other<Str>{Str{"test"}}};

    test_utils::writeReadTest(data);
}

} // namespace instantiate_template_argument
} // namespace templates
