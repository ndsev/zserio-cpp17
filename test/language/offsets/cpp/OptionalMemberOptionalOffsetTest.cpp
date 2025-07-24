#include "gtest/gtest.h"
#include "offsets/optional_member_optional_offset/OptionalMemberOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace optional_member_optional_offset
{

TEST(OptionalMemberOptionalOffsetTest, writeRead)
{
    OptionalMemberOffset data;
    test_utils::writeReadTest(data);
}

TEST(OptionalMemberOptionalOffsetTest, hash)
{
    OptionalMemberOffset data;
    ASSERT_EQ(std::hash<OptionalMemberOffset>()(data), static_cast<size_t>(31487));
}

} // namespace optional_member_optional_offset
} // namespace offsets
