#include "gtest/gtest.h"
#include "offsets/optional_member_offset/OptionalMemberOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace optional_member_offset
{

class OptionalMemberOffsetTest : public ::testing::Test
{
protected:
    static void writeData(
            zserio::BitStreamWriter& writer, bool hasOptional, int32_t optionalField, int32_t field)
    {
        writer.writeBool(hasOptional);

        if (hasOptional)
        {
            writer.writeUnsignedBits32(OPTIONAL_FIELD_OFFSET, 32);
            writer.writeUnsignedBits32(0, 7); // alignment
            writer.writeSignedBits32(optionalField, 32);
        }
        else
        {
            writer.writeUnsignedBits32(0, 32);
        }

        writer.writeSignedBits32(field, 32);
    }

    static void fillData(OptionalMemberOffset& data, bool hasOptional, int32_t optionalField, int32_t field,
            bool setOffset = false)
    {
        data.hasOptional = hasOptional;
        if (hasOptional)
        {
            if (setOffset)
            {
                data.optionalFieldOffset = OPTIONAL_FIELD_OFFSET;
            }
            data.optionalField = optionalField;
        }
        data.field = field;
    }

    static constexpr size_t WITH_OPTIONAL_MEMBER_OFFSET_BIT_SIZE = 104;
    static constexpr size_t WITHOUT_OPTIONAL_MEMBER_OFFSET_BIT_SIZE = 65;

    static constexpr size_t OPTIONAL_FIELD_OFFSET = 5;
};

TEST_F(OptionalMemberOffsetTest, readWithOptional)
{
    OptionalMemberOffset data;
    const bool hasOptional = true;
    const int32_t optionalField = 0x1212;
    const int32_t field = 0x2121;
    fillData(data, hasOptional, optionalField, field, true);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, hasOptional, optionalField, field), data);
}

TEST_F(OptionalMemberOffsetTest, readConstructorWithoutOptional)
{
    OptionalMemberOffset data;
    const bool hasOptional = false;
    const int32_t optionalField = 0;
    const int32_t field = 0x2121;
    fillData(data, hasOptional, optionalField, field, true);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, hasOptional, optionalField, field), data);
}

TEST_F(OptionalMemberOffsetTest, writeWithOptional)
{
    OptionalMemberOffset data;
    const bool hasOptional = true;
    const int32_t optionalField = 0x1A1A;
    const int32_t field = 0xA1A1;
    fillData(data, hasOptional, optionalField, field);
    zserio::View view(data);

    EXPECT_EQ(0, data.optionalFieldOffset);
    EXPECT_EQ(0, view.optionalFieldOffset());

    test_utils::writeReadTest(data);
    ASSERT_EQ(OPTIONAL_FIELD_OFFSET, data.optionalFieldOffset);
    ASSERT_EQ(OPTIONAL_FIELD_OFFSET, view.optionalFieldOffset());
}

TEST_F(OptionalMemberOffsetTest, writeWithoutOptional)
{
    OptionalMemberOffset data;
    const bool hasOptional = false;
    const int32_t optionalField = 0;
    const int32_t field = 0x7ACF;
    fillData(data, hasOptional, optionalField, field);
    zserio::View view(data);

    EXPECT_EQ(0, data.optionalFieldOffset);
    EXPECT_EQ(0, view.optionalFieldOffset());

    test_utils::writeReadTest(data);
    ASSERT_EQ(0, data.optionalFieldOffset);
    ASSERT_EQ(0, view.optionalFieldOffset());
}

} // namespace optional_member_offset
} // namespace offsets
