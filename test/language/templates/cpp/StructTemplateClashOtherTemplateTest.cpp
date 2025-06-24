#include "gtest/gtest.h"
#include "templates/struct_template_clash_other_template/InstantiationNameClashOtherTemplate.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_template_clash_other_template
{

TEST(StructTemplateClashOtherTemplateTest, writeRead)
{
    InstantiationNameClashOtherTemplate data(
            Test_A<zserio::UInt32>(42), struct_template_clash_other_template::Test<A_uint32>(A_uint32(10)));

    test_utils::writeReadTest(data);
}

} // namespace struct_template_clash_other_template
} // namespace templates
