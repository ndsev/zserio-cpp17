#include "gtest/gtest.h"
#include "optional_members/optional_recursion_in_template/OptionalRecursionInTemplate.h"
#include "test_utils/TestUtility.h"

namespace optional_members
{
namespace optional_recursion_in_template
{

TEST(OptionalRecursionInTemplateTest, writeReadTest)
{
    OptionalRecursionInTemplate data;
    ASSERT_EQ(data.block.blockTerminator, 0);
    data.block.data.resize(32);
    data.block.blockTerminator = 10;
    data.block.nextData.emplace();
    data.block.nextData->data.resize(10);
    test_utils::writeReadTest(data);
}

TEST(OptionalRecursionInTemplateTest, hashTest)
{
    OptionalRecursionInTemplate data;
    ASSERT_EQ(std::hash<OptionalRecursionInTemplate>()(data), static_cast<size_t>(1702));
}

} // namespace optional_recursion_in_template
} // namespace optional_members
