#include "gtest/gtest.h"
#include "offsets/bit_offset/BitOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace bit_offset
{

class BitOffsetTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(FIELD1_OFFSET, 8);
        writer.writeUnsignedBits32(FIELD2_OFFSET, 16);
        writer.writeUnsignedBits32(FIELD3_OFFSET, 32);
        writer.writeUnsignedBits32(FIELD4_OFFSET, 8);
        writer.writeUnsignedBits32(FIELD5_OFFSET, 15);
        writer.writeUnsignedBits32(FIELD6_OFFSET, 18);
        writer.writeUnsignedBits32(FIELD7_OFFSET, 23);
        writer.writeUnsignedBits32(FIELD8_OFFSET, 8);

        writer.writeUnsignedBits32(FIELD1_VALUE, 1);

        writer.writeUnsignedBits32(0, 7);
        writer.writeUnsignedBits32(FIELD2_VALUE, 2);

        writer.writeUnsignedBits32(0, 6);
        writer.writeUnsignedBits32(FIELD3_VALUE, 3);

        writer.writeUnsignedBits32(0, 5);
        writer.writeUnsignedBits32(FIELD4_VALUE, 4);

        writer.writeUnsignedBits32(0, 4);
        writer.writeUnsignedBits32(FIELD5_VALUE, 5);

        writer.writeUnsignedBits32(0, 3);
        writer.writeUnsignedBits32(FIELD6_VALUE, 6);

        writer.writeUnsignedBits32(0, 2);
        writer.writeUnsignedBits32(FIELD7_VALUE, 7);

        writer.writeUnsignedBits32(0, 1);
        writer.writeUnsignedBits32(FIELD8_VALUE, 8);
    }

    static void checkOffsets(const BitOffset& data, const zserio::View<BitOffset>& view)
    {
        ASSERT_EQ(FIELD1_OFFSET, data.field1Offset);
        ASSERT_EQ(FIELD2_OFFSET, data.field2Offset);
        ASSERT_EQ(FIELD3_OFFSET, data.field3Offset);
        ASSERT_EQ(FIELD4_OFFSET, data.field4Offset);
        ASSERT_EQ(FIELD5_OFFSET, data.field5Offset);
        ASSERT_EQ(FIELD6_OFFSET, data.field6Offset);
        ASSERT_EQ(FIELD7_OFFSET, data.field7Offset);
        ASSERT_EQ(FIELD8_OFFSET, data.field8Offset);

        ASSERT_EQ(FIELD1_OFFSET, view.field1Offset());
        ASSERT_EQ(FIELD2_OFFSET, view.field2Offset());
        ASSERT_EQ(FIELD3_OFFSET, view.field3Offset());
        ASSERT_EQ(FIELD4_OFFSET, view.field4Offset());
        ASSERT_EQ(FIELD5_OFFSET, view.field5Offset());
        ASSERT_EQ(FIELD6_OFFSET, view.field6Offset());
        ASSERT_EQ(FIELD7_OFFSET, view.field7Offset());
        ASSERT_EQ(FIELD8_OFFSET, view.field8Offset());
    }

    static void fillData(BitOffset& data, bool setOffsets = false)
    {
        if (setOffsets)
        {
            data.field1Offset = FIELD1_OFFSET;
            data.field2Offset = FIELD2_OFFSET;
            data.field3Offset = FIELD3_OFFSET;
            data.field4Offset = FIELD4_OFFSET;
            data.field5Offset = FIELD5_OFFSET;
            data.field6Offset = FIELD6_OFFSET;
            data.field7Offset = FIELD7_OFFSET;
            data.field8Offset = FIELD8_OFFSET;
        }

        data.field1 = FIELD1_VALUE;
        data.field2 = FIELD2_VALUE;
        data.field3 = FIELD3_VALUE;
        data.field4 = FIELD4_VALUE;
        data.field5 = FIELD5_VALUE;
        data.field6 = FIELD6_VALUE;
        data.field7 = FIELD7_VALUE;
        data.field8 = FIELD8_VALUE;
    }

    static constexpr size_t BIT_OFFSET_BIT_SIZE = 192;

    static constexpr uint8_t WRONG_FIELD1_OFFSET = 0;
    static constexpr uint16_t WRONG_FIELD2_OFFSET = 0;
    static constexpr uint32_t WRONG_FIELD3_OFFSET = 0;
    static constexpr uint8_t WRONG_FIELD4_OFFSET = 0;
    static constexpr uint16_t WRONG_FIELD5_OFFSET = 0;
    static constexpr uint32_t WRONG_FIELD6_OFFSET = 0;
    static constexpr uint32_t WRONG_FIELD7_OFFSET = 0;
    static constexpr uint8_t WRONG_FIELD8_OFFSET = 0;

    static constexpr uint8_t FIELD1_OFFSET = 16;
    static constexpr uint16_t FIELD2_OFFSET = 17;
    static constexpr uint32_t FIELD3_OFFSET = 18;
    static constexpr uint8_t FIELD4_OFFSET = 19;
    static constexpr uint16_t FIELD5_OFFSET = 20;
    static constexpr uint32_t FIELD6_OFFSET = 21;
    static constexpr uint32_t FIELD7_OFFSET = 22;
    static constexpr uint8_t FIELD8_OFFSET = 23;

    static const uint8_t FIELD1_VALUE = 1;
    static const uint8_t FIELD2_VALUE = 2;
    static const uint8_t FIELD3_VALUE = 5;
    static const uint8_t FIELD4_VALUE = 13;
    static const uint8_t FIELD5_VALUE = 26;
    static const uint8_t FIELD6_VALUE = 56;
    static const uint8_t FIELD7_VALUE = 88;
    static const uint8_t FIELD8_VALUE = 222;
};

TEST_F(BitOffsetTest, read)
{
    BitOffset data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(BitOffsetTest, writeRead)
{
    BitOffset data;
    fillData(data);
    zserio::View view(data);

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

} // namespace bit_offset
} // namespace offsets
