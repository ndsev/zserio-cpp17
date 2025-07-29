#include "extended_members/extended_optional_field_in_template/ExtendedCompound.h"
#include "extended_members/extended_optional_field_in_template/ExtendedSimple.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace extended_members
{
namespace extended_optional_field_in_template
{

TEST(ExtendedOptionalFieldInTemplateTest, writeRead)
{
    ExtendedSimple data;
    test_utils::writeReadTest(data);
    ExtendedCompound data2;
    test_utils::writeReadTest(data2);
}

TEST(ExtendedOptionalFieldInTemplateTest, hash
{
    ExtendedSimple data;
    ASSERT_EQ(std::hash<ExtendedSimple>()(data), static_cast<size_t>(851));
    ExtendedCompound data2;
    ASSERT_EQ(std::hash<ExtendedCompound>()(data2), static_cast<size_t>(851));
}

} // namespace extended_optional_field_in_template
} // namespace extended_members
