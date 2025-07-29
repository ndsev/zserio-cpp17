#include "gtest/gtest.h"
#include "offsets/optional_member_optional_offset/OptionalMemberOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace optional_member_optional_offset
{

TEST(OptionalMemberOptionalOffsetTest, withoutOptionals)
{
    OptionalMemberOffset data(false, {}, {}, 11);
    test_utils::writeReadTest(data);
}

TEST(OptionalMemberOptionalOffsetTest, withOptionals)
{
    OptionalMemberOffset data(true, zserio::UInt32(0), zserio::Int32(20), 11);
    test_utils::writeReadTest(data);
    ASSERT_EQ(data.optionalFieldOffset, 5U); // hasOptional + optionalFieldOffset
}

TEST(OptionalMemberOptionalOffsetTest, hash)
{
    OptionalMemberOffset data(true, zserio::UInt32(0), zserio::Int32(20), 11);
    ASSERT_EQ(std::hash<OptionalMemberOffset>()(data), static_cast<size_t>(43157107));
}

} // namespace optional_member_optional_offset
} // namespace offsets
