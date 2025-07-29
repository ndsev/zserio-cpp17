#include "gtest/gtest.h"
#include "offsets/optional_offset_in_template/OptionalMemberOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace optional_offset_in_template
{

TEST(OptionalOffsetInTemplateTest, withoutOptional)
{
    OptionalMemberOffset data({false, {}}, {}, 11);
    test_utils::writeReadTest(data);
}

TEST(OptionalOffsetInTemplateTest, withOptional)
{
    OptionalMemberOffset data({true, zserio::UInt32(0)}, zserio::Int32(10), 11);
    test_utils::writeReadTest(data);
    ASSERT_EQ(data.offsetHolder.optionalFieldOffset, 5U); // hasOptional + optionalFieldOffset
}

TEST(OptionalOffsetInTemplateTest, hash)
{
    OptionalMemberOffset data;
    ASSERT_EQ(std::hash<OptionalMemberOffset>()(data), static_cast<size_t>(62974));
}

} // namespace optional_offset_in_template
} // namespace offsets
