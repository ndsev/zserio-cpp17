#include "gtest/gtest.h"
#include "templates/struct_template_clash_other_type/InstantiationNameClashOtherType.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace struct_template_clash_other_type
{

TEST(StructTemplateClashOtherTypeTest, writeRead)
{
    InstantiationNameClashOtherType data(struct_template_clash_other_type::Test<zserio::UInt32>(42));

    test_utils::writeReadTest(data);
}

} // namespace struct_template_clash_other_type
} // namespace templates
