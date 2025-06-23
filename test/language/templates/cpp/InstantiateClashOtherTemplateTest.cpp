#include "gtest/gtest.h"
#include "templates/instantiate_clash_other_template/InstantiateClashOtherTemplate.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_clash_other_template
{

TEST(InstantiateClashOtherTemplateTest, writeRead)
{
    InstantiateClashOtherTemplate data(instantiate_clash_other_template::Test<zserio::UInt32>(13));

    test_utils::writeReadTest(data);
}

} // namespace instantiate_clash_other_template
} // namespace templates
