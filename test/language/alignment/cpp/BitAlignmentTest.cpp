#include "alignment/bit_alignment/BitAlignment.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
namespace alignment
{
namespace bit_alignment
{

class BitAlignmentTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(ALIGNED1_FIELD_VALUE, 1);

        writer.writeUnsignedBits32(0, 1);
        writer.writeUnsignedBits32(ALIGNED2_FIELD_VALUE, 2);

        writer.writeUnsignedBits32(0, 2);
        writer.writeUnsignedBits32(ALIGNED3_FIELD_VALUE, 3);

        writer.writeUnsignedBits32(0, 3);
        writer.writeUnsignedBits32(ALIGNED4_FIELD_VALUE, 4);

        writer.writeUnsignedBits32(0, 4);
        writer.writeUnsignedBits32(ALIGNED5_FIELD_VALUE, 5);

        writer.writeUnsignedBits32(0, 5);
        writer.writeUnsignedBits32(ALIGNED6_FIELD_VALUE, 6);

        writer.writeUnsignedBits32(0, 6);
        writer.writeUnsignedBits32(ALIGNED7_FIELD_VALUE, 7);

        writer.writeUnsignedBits32(0, 7);
        writer.writeUnsignedBits32(ALIGNED8_FIELD_VALUE, 8);

        writer.writeUnsignedBits32(0, 1);
        writer.writeUnsignedBits32(0, 15);
        writer.writeUnsignedBits32(ALIGNED16_FIELD_VALUE, 16);

        writer.writeUnsignedBits32(0, 1);
        writer.writeUnsignedBits32(0, 31);
        writer.writeUnsignedBits32(ALIGNED32_FIELD_VALUE, 32);

        writer.writeUnsignedBits64(0, 33);
        writer.writeUnsignedBits64(0, 63);
        writer.writeUnsignedBits64(ALIGNED64_FIELD_VALUE, 64);
    }

    static void fillData(BitAlignment& data)
    {
        data.aligned1Field = ALIGNED1_FIELD_VALUE;
        data.aligned2Field = ALIGNED2_FIELD_VALUE;
        data.aligned3Field = ALIGNED3_FIELD_VALUE;
        data.aligned4Field = ALIGNED4_FIELD_VALUE;
        data.aligned5Field = ALIGNED5_FIELD_VALUE;
        data.aligned6Field = ALIGNED6_FIELD_VALUE;
        data.aligned7Field = ALIGNED7_FIELD_VALUE;
        data.aligned8Field = ALIGNED8_FIELD_VALUE;
        data.alignment16Break = ALIGNMENT16_BREAK;
        data.aligned16Field = ALIGNED16_FIELD_VALUE;
        data.alignment32Break = ALIGNMENT32_BREAK;
        data.aligned32Field = ALIGNED32_FIELD_VALUE;
        data.alignment64Break = ALIGNMENT64_BREAK;
        data.aligned64Field = ALIGNED64_FIELD_VALUE;
    }

    // constants
    static constexpr size_t BIT_ALIGNMENT_BIT_SIZE = 320;

    static constexpr uint8_t ALIGNED1_FIELD_VALUE = 1;
    static constexpr uint8_t ALIGNED2_FIELD_VALUE = 2;
    static constexpr uint8_t ALIGNED3_FIELD_VALUE = 5;
    static constexpr uint8_t ALIGNED4_FIELD_VALUE = 13;
    static constexpr uint8_t ALIGNED5_FIELD_VALUE = 26;
    static constexpr uint8_t ALIGNED6_FIELD_VALUE = 56;
    static constexpr uint8_t ALIGNED7_FIELD_VALUE = 88;
    static constexpr uint8_t ALIGNED8_FIELD_VALUE = 222;
    static constexpr uint8_t ALIGNMENT16_BREAK = 0;
    static constexpr uint16_t ALIGNED16_FIELD_VALUE = 0xcafe;
    static constexpr uint8_t ALIGNMENT32_BREAK = 0;
    static constexpr uint32_t ALIGNED32_FIELD_VALUE = 0xcafec0de;
    static constexpr uint64_t ALIGNMENT64_BREAK = UINT64_C(0);
    static constexpr uint64_t ALIGNED64_FIELD_VALUE = UINT64_C(0xcafec0dedeadface);
};

TEST_F(BitAlignmentTest, read)
{
    BitAlignment data;
    fillData(data);

    test_utils::readTest(writeData, data);
}

TEST_F(BitAlignmentTest, bitSizeOf)
{
    BitAlignment data;
    fillData(data);
    zserio::View view(data);

    // test default argument
    ASSERT_EQ(BIT_ALIGNMENT_BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(BitAlignmentTest, bitSizeOfWithPosition)
{
    BitAlignment data;
    fillData(data);
    zserio::View view(data);

    // starting up to bit position 77, the structure still fits into original size thanks to alignments
    zserio::BitSize startBitPosition = 0;
    for (; startBitPosition < 78; ++startBitPosition)
    {
        ASSERT_EQ(BIT_ALIGNMENT_BIT_SIZE - startBitPosition, zserio::detail::bitSizeOf(view, startBitPosition));
    }
    // starting at bit position 78, also next 64bits are needed
    ASSERT_EQ(
            BIT_ALIGNMENT_BIT_SIZE - startBitPosition + 64, zserio::detail::bitSizeOf(view, startBitPosition));
}

TEST_F(BitAlignmentTest, writeRead)
{
    BitAlignment data;
    fillData(data);

    test_utils::writeReadTest(data);
}

} // namespace bit_alignment
} // namespace alignment
