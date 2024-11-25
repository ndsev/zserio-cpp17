#include "alignment/auto_optional_member_alignment/AutoOptionalMemberAlignment.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace alignment
{
namespace auto_optional_member_alignment
{

class AutoOptionalMemberAlignmentTest : public ::testing::Test
{
protected:
    static void writeData(
            zserio::BitStreamWriter& writer, bool hasAutoOptional, int32_t autoOptionalField, int32_t field)
    {
        writer.writeBool(hasAutoOptional);

        if (hasAutoOptional)
        {
            writer.writeUnsignedBits32(0, 31);
            writer.writeSignedBits32(autoOptionalField, 32);
        }

        writer.writeSignedBits32(field, 32);
    }

    static void fillData(
            AutoOptionalMemberAlignment& data, bool hasAutoOptional, int32_t autoOptionalField, int32_t field)
    {
        if (hasAutoOptional)
        {
            data.autoOptionalField = autoOptionalField;
        }
        data.field = field;
    }

    static constexpr size_t WITH_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE = 96;
    static constexpr size_t WITHOUT_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE = 33;
};

TEST_F(AutoOptionalMemberAlignmentTest, readWithOptional)
{
    const bool hasAutoOptional = true;
    const int32_t autoOptionalField = 0x1234;
    const int32_t field = 0x7654;
    AutoOptionalMemberAlignment data;
    fillData(data, hasAutoOptional, autoOptionalField, field);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, hasAutoOptional, autoOptionalField, field), data);
}

TEST_F(AutoOptionalMemberAlignmentTest, readWithoutOptional)
{
    const bool hasAutoOptional = false;
    const int32_t autoOptionalField = 0;
    const int32_t field = 0x2222;
    AutoOptionalMemberAlignment data;
    fillData(data, hasAutoOptional, autoOptionalField, field);

    test_utils::readTest(
            std::bind(writeData, std::placeholders::_1, hasAutoOptional, autoOptionalField, field), data);
}

TEST_F(AutoOptionalMemberAlignmentTest, bitSizeOfWithOptional)
{
    AutoOptionalMemberAlignment data;
    fillData(data, true, 0x4433, 0x1122);
    zserio::View<AutoOptionalMemberAlignment> view(data);

    ASSERT_EQ(WITH_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(AutoOptionalMemberAlignmentTest, bitSizeOfWithoutOptional)
{
    AutoOptionalMemberAlignment data;
    fillData(data, false, 0, 0x7624);
    zserio::View<AutoOptionalMemberAlignment> view(data);

    ASSERT_EQ(WITHOUT_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(AutoOptionalMemberAlignmentTest, writeWithOptional)
{
    const bool hasAutoOptional = true;
    const int32_t autoOptionalField = 0x9ADB;
    const int32_t field = 0x8ACD;
    AutoOptionalMemberAlignment data;
    fillData(data, hasAutoOptional, autoOptionalField, field);

    test_utils::writeReadTest(data);
}

TEST_F(AutoOptionalMemberAlignmentTest, writeWithoutOptional)
{
    const bool hasAutoOptional = true;
    const int32_t autoOptionalField = 0;
    const int32_t field = 0x7ACF;
    AutoOptionalMemberAlignment data;
    fillData(data, hasAutoOptional, autoOptionalField, field);

    test_utils::writeReadTest(data);
}

} // namespace auto_optional_member_alignment
} // namespace alignment
