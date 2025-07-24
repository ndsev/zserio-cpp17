#include "gtest/gtest.h"
#include "offsets/optional_offset_in_template/OptionalMemberOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace optional_offset_in_template
{

TEST(OptionalOffsetInTemplateTest, writeRead)
{
    OptionalMemberOffset data;
    test_utils::writeReadTest(data);
}

TEST(OptionalOffsetInTemplateTest, hash)
{
    OptionalMemberOffset data;
    ASSERT_EQ(std::hash<OptionalMemberOffset>()(data), static_cast<size_t>(62974));
}

} // namespace optional_offset_in_template
} // namespace offsets
