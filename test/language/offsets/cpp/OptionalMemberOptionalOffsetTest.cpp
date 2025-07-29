#include "gtest/gtest.h"
#include "offsets/optional_member_optional_offset/OptionalMemberOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace optional_member_optional_offset
{

using AllocatorType = OptionalMemberOffset::allocator_type;
using OptField = zserio::BasicOptional<AllocatorType, zserio::UInt32>;
using OptFieldOffset = zserio::BasicOptional<AllocatorType, zserio::Int32>;

TEST(OptionalMemberOptionalOffsetTest, withoutOptionals)
{
    OptionalMemberOffset data(false, {}, {}, 11);
    test_utils::writeReadTest(data);
}

TEST(OptionalMemberOptionalOffsetTest, withOptionals)
{
    OptionalMemberOffset data(true, OptField(0), OptFieldOffset(20), 11);
    test_utils::writeReadTest(data);
    ASSERT_EQ(data.optionalFieldOffset, 5); // hasOptional + optionalFieldOffset
}

TEST(OptionalMemberOptionalOffsetTest, hash)
{
    OptionalMemberOffset data(true, OptField(0), OptFieldOffset(20), 11);
    ASSERT_EQ(std::hash<OptionalMemberOffset>()(data), static_cast<size_t>(43157107));
}

} // namespace optional_member_optional_offset
} // namespace offsets
