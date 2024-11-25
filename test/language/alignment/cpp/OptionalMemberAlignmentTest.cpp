#include "alignment/optional_member_alignment/OptionalMemberAlignment.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace alignment
{
namespace optional_member_alignment
{

class OptionalMemberAlignmentTest : public ::testing::Test
{
protected:
    static void writeData(
            zserio::BitStreamWriter& writer, bool hasOptional, int32_t optionalField, int32_t field)
    {
        writer.writeBool(hasOptional);

        if (hasOptional)
        {
            writer.writeUnsignedBits32(0, 31);
            writer.writeSignedBits32(optionalField, 32);
        }

        writer.writeSignedBits32(field, 32);
    }

    static void fillData(OptionalMemberAlignment& data, bool hasOptional, int32_t optionalField, int32_t field)
    {
        data.hasOptional = hasOptional;
        if (hasOptional)
        {
            data.optionalField = optionalField;
        }
        data.field = field;
    }

    static constexpr size_t WITH_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE = 96;
    static constexpr size_t WITHOUT_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE = 33;
};

TEST_F(OptionalMemberAlignmentTest, readWithOptional)
{
    const bool hasOptional = true;
    const int32_t optionalField = 0x1234;
    const int32_t field = 0x7654;
    OptionalMemberAlignment data;
    fillData(data, hasOptional, optionalField, field);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, hasOptional, optionalField, field), data);
}

TEST_F(OptionalMemberAlignmentTest, readWithoutOptional)
{
    const bool hasOptional = false;
    const int32_t optionalField = 0;
    const int32_t field = 0x2222;
    OptionalMemberAlignment data;
    fillData(data, hasOptional, optionalField, field);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, hasOptional, optionalField, field), data);
}

TEST_F(OptionalMemberAlignmentTest, bitSizeOfWithOptional)
{
    OptionalMemberAlignment data;
    fillData(data, true, 0x4433, 0x1122);
    zserio::View<OptionalMemberAlignment> view(data);

    ASSERT_EQ(WITH_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(OptionalMemberAlignmentTest, bitSizeOfWithoutOptional)
{
    OptionalMemberAlignment data;
    fillData(data, false, 0, 0x7624);
    zserio::View<OptionalMemberAlignment> view(data);

    ASSERT_EQ(WITHOUT_OPTIONAL_MEMBER_ALIGNMENT_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(OptionalMemberAlignmentTest, writeReadWithOptional)
{
    const bool hasOptional = true;
    const int32_t optionalField = 0x9ADB;
    const int32_t field = 0x8ACD;
    OptionalMemberAlignment data;
    fillData(data, hasOptional, optionalField, field);

    test_utils::writeReadTest(data);
}

TEST_F(OptionalMemberAlignmentTest, writeReadWithoutOptional)
{
    const bool hasOptional = true;
    const int32_t optionalField = 0;
    const int32_t field = 0x7ACF;
    OptionalMemberAlignment data;
    fillData(data, hasOptional, optionalField, field);

    test_utils::writeReadTest(data);
}

} // namespace optional_member_alignment
} // namespace alignment
