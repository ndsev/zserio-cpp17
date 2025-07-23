#include "gtest/gtest.h"
#include "offsets/parameter_offset_in_template/ParameterOffsetInTemplate.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace parameter_offset_in_template
{

TEST(ParameterOffsetInTemplateTest, writeReadTest)
{
    ParameterOffsetInTemplate data;
    test_utils::writeReadTest(data);
}

TEST(ParameterOffsetInTemplateTest, hashTest)
{
    ParameterOffsetInTemplate data;
    ASSERT_EQ(std::hash<ParameterOffsetInTemplate>()(data), static_cast<size_t>(1198208));
}

} // namespace parameter_offset_in_template
} // namespace offsets
