#include "gtest/gtest.h"
#include "templates/instantiate_with_instantiate_template_argument/InstantiateWithInstantiateTemplateArgument.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_with_instantiate_template_argument
{

TEST(InstantiateWithInstantiateTemplateArgumentTest, writeRead)
{
    InstantiateWithInstantiateTemplateArgument data;
    data.other8 = Other8(Data8(13));
    data.other32 = Other32(Data32(0xCAFE));

    test_utils::writeReadTest(data);
}

} // namespace instantiate_with_instantiate_template_argument
} // namespace templates
