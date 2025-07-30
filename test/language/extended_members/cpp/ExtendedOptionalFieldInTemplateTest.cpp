#include "extended_members/extended_optional_field_in_template/ExtendedCompound.h"
#include "extended_members/extended_optional_field_in_template/ExtendedSimple.h"
#include "extended_members/extended_optional_field_in_template/Original.h"
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

    ExtendedCompound extended;
    extended.extendedValue.setPresent(true);
    extended.extendedValue.value() = Compound<zserio::UInt32>(20);
    ASSERT_TRUE(extended.extendedValue.isPresent());
    ASSERT_EQ((*extended.extendedValue)->field, zserio::UInt32(20));
    test_utils::writeReadTest(extended);
}

TEST(ExtendedOptionalFieldInTemplateTest, hash)
{
    Original orig(100);
    ASSERT_EQ(std::hash<Original>()(orig), static_cast<size_t>(951));
    ExtendedSimple simple(100, {});
    ASSERT_EQ(std::hash<ExtendedSimple>()(simple), std::hash<Original>()(orig));

    ExtendedCompound extended;
    extended.extendedValue.setPresent(true);
    extended.extendedValue.value() = Compound<zserio::UInt32>(20);
    ASSERT_EQ(std::hash<ExtendedCompound>()(extended), static_cast<size_t>(32358));
}

TEST(ExtendedOptionalFieldTemplateTest, writeOriginalReadExtended)
{
    Original orig(27);
    auto origView = zserio::View(orig);
    auto bitBuffer = zserio::serialize(orig);

    ExtendedCompound extended;
    auto extendedView = zserio::deserialize<ExtendedCompound>(bitBuffer, extended);
    ASSERT_EQ(extended.value, orig.value);
    ASSERT_FALSE(extended.extendedValue.isPresent());
    ASSERT_EQ(zserio::detail::bitSizeOf(extendedView), zserio::detail::bitSizeOf(origView));
}

TEST(ExtendedOptionalFieldTemplateTest, writeExtendedReadOriginal)
{
    ExtendedCompound extended(100, {});
    auto bitBuffer = zserio::serialize(extended);

    Original orig;
    zserio::deserialize<Original>(bitBuffer, orig);
    ASSERT_EQ(orig.value, extended.value);
}

} // namespace extended_optional_field_in_template
} // namespace extended_members
