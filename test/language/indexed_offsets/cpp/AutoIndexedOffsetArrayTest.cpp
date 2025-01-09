#include "gtest/gtest.h"
#include "indexed_offsets/auto_indexed_offset_array/AutoIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace auto_indexed_offset_array
{

class AutoIndexedOffsetArrayTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeVarSize(NUM_ELEMENTS);
        uint32_t currentOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeUnsignedBits32(currentOffset, 32);
            currentOffset += ALIGNED_ELEMENT_BYTE_SIZE;
        }

        writer.writeUnsignedBits32(SPACER_VALUE, 1);

        writer.writeVarSize(NUM_ELEMENTS);
        writer.writeUnsignedBits32(0, 7);
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeUnsignedBits32(i % 64, ELEMENT_SIZE);
            if ((i + 1) != NUM_ELEMENTS)
            {
                writer.writeUnsignedBits32(0, ALIGNED_ELEMENT_SIZE - ELEMENT_SIZE);
            }
        }
    }

    static void fillData(AutoIndexedOffsetArray& data, bool setOffsets = false)
    {
        data.offsets.resize(NUM_ELEMENTS);
        if (setOffsets)
        {
            uint32_t currentOffset = ELEMENT0_OFFSET;
            for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
            {
                data.offsets[i] = currentOffset;
                currentOffset += ALIGNED_ELEMENT_BYTE_SIZE;
            }
        }

        data.spacer = SPACER_VALUE;

        data.data.reserve(NUM_ELEMENTS);
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            data.data.push_back(i % 64);
        }
    }

    static void checkOffsets(
            const AutoIndexedOffsetArray& data, const zserio::View<AutoIndexedOffsetArray>& view)
    {
        uint32_t currentOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            ASSERT_EQ(currentOffset, data.offsets.at(i));
            ASSERT_EQ(currentOffset, view.offsets().at(i));
            currentOffset += ALIGNED_ELEMENT_BYTE_SIZE;
        }
    }

    static constexpr uint8_t NUM_ELEMENTS = 5;

    static constexpr size_t AUTO_ARRAY_LENGTH_BYTE_SIZE = 1;
    static constexpr uint32_t ELEMENT0_OFFSET = AUTO_ARRAY_LENGTH_BYTE_SIZE + NUM_ELEMENTS * sizeof(uint32_t) +
            sizeof(uint8_t) + AUTO_ARRAY_LENGTH_BYTE_SIZE;
    static constexpr uint8_t ELEMENT_SIZE = 5;
    static constexpr uint8_t ALIGNED_ELEMENT_SIZE = 8;
    static constexpr uint8_t ALIGNED_ELEMENT_BYTE_SIZE = ALIGNED_ELEMENT_SIZE / 8;

    static constexpr uint8_t SPACER_VALUE = 1;

    static constexpr size_t AUTO_INDEXED_OFFSET_ARRAY_BIT_SIZE =
            ELEMENT0_OFFSET * 8 + (NUM_ELEMENTS - 1) * ALIGNED_ELEMENT_SIZE + ELEMENT_SIZE;
};

TEST_F(AutoIndexedOffsetArrayTest, read)
{
    AutoIndexedOffsetArray data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(AutoIndexedOffsetArrayTest, writeRead)
{
    AutoIndexedOffsetArray data;
    fillData(data);
    zserio::View view(data);

    for (auto offset : data.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

} // namespace auto_indexed_offset_array
} // namespace indexed_offsets
