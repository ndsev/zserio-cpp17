#include "gtest/gtest.h"
#include "offsets/parameter_offset_in_template/ParameterOffsetInTemplate.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace parameter_offset_in_template
{

using RoomType = Room<OffsetHolder>;
using SchoolType = School<OffsetHolder, RoomType>;

TEST(ParameterOffsetInTemplateTest, writeReadOffset)
{
    ParameterOffsetInTemplate data(SchoolType(155, OffsetHolder(), RoomType(11)));
    test_utils::writeReadTest(data);

    /* todo: Fix offsets in templated fields
    constexpr int OFFSET = 2 + 4; // schoolId + roomOffset
    ASSERT_EQ(data.school.offsetHolder.roomOffset, OFFSET);
    */
}

TEST(ParameterOffsetInTemplateTest, hash)
{
    ParameterOffsetInTemplate data;
    ASSERT_EQ(std::hash<ParameterOffsetInTemplate>()(data), static_cast<size_t>(1198208));
}

} // namespace parameter_offset_in_template
} // namespace offsets
