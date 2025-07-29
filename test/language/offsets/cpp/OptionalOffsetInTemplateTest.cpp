#include "gtest/gtest.h"
#include "offsets/optional_offset_in_template/OptionalMemberOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace optional_offset_in_template
{

using AllocatorType = OptionalMemberOffset::allocator_type;
using OptFieldOffset = zserio::BasicOptional<AllocatorType, zserio::UInt32>;
using OptField = zserio::BasicOptional<AllocatorType, zserio::Int32>;

TEST(OptionalOffsetInTemplateTest, withoutOptional)
{
    OptionalMemberOffset data({false, {}}, {}, 11);
    test_utils::writeReadTest(data);
}

TEST(OptionalOffsetInTemplateTest, withOptional)
{
    OptionalMemberOffset data({true, OptFieldOffset(0)}, OptField(10), 11);
    test_utils::writeReadTest(data);
    ASSERT_EQ(data.offsetHolder.optionalFieldOffset, 5); // hasOptional + optionalFieldOffset
}

TEST(OptionalOffsetInTemplateTest, hash)
{
    OptionalMemberOffset data;
    ASSERT_EQ(std::hash<OptionalMemberOffset>()(data), static_cast<size_t>(62974));
}

} // namespace optional_offset_in_template
} // namespace offsets
