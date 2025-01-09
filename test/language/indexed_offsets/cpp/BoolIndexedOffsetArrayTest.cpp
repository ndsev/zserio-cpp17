#include "gtest/gtest.h"
#include "indexed_offsets/bool_indexed_offset_array/BoolIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace bool_indexed_offset_array
{

class BoolIndexedOffsetArrayTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        uint32_t currentOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeUnsignedBits32(currentOffset, 32);
            currentOffset += ALIGNED_ELEMENT_BYTE_SIZE;
        }

        writer.writeUnsignedBits32(SPACER_VALUE, 1);
        writer.writeUnsignedBits32(0, 7);

        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeUnsignedBits32(i & 0x01U, ELEMENT_SIZE);
            if ((i + 1) != NUM_ELEMENTS)
            {
                writer.writeUnsignedBits32(0, ALIGNED_ELEMENT_SIZE - ELEMENT_SIZE);
            }
        }
    }

    static void checkOffsets(
            const BoolIndexedOffsetArray& data, const zserio::View<BoolIndexedOffsetArray>& view)
    {
        uint32_t expectedOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            ASSERT_EQ(expectedOffset, data.offsets.at(i));
            ASSERT_EQ(expectedOffset, view.offsets().at(i));
            expectedOffset += ALIGNED_ELEMENT_BYTE_SIZE;
        }
    }

    void fillData(BoolIndexedOffsetArray& data, bool setOffsets = false)
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
            data.data.push_back((i & 0x01U) != 0);
        }
    }

    static constexpr uint8_t NUM_ELEMENTS = 5;

    static constexpr uint32_t ELEMENT0_OFFSET = NUM_ELEMENTS * sizeof(uint32_t) + sizeof(uint8_t);
    static constexpr uint8_t ELEMENT_SIZE = 1;
    static constexpr uint8_t ALIGNED_ELEMENT_SIZE = 8;
    static constexpr uint8_t ALIGNED_ELEMENT_BYTE_SIZE = ALIGNED_ELEMENT_SIZE / 8;

    static constexpr uint8_t SPACER_VALUE = 1;

    static constexpr size_t BOOL_indexed_offset_ARRAY_BIT_SIZE =
            ELEMENT0_OFFSET * 8 + (NUM_ELEMENTS - 1) * ALIGNED_ELEMENT_SIZE + ELEMENT_SIZE;
};

TEST_F(BoolIndexedOffsetArrayTest, read)
{
    BoolIndexedOffsetArray data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(BoolIndexedOffsetArrayTest, writeRead)
{
    BoolIndexedOffsetArray data;
    fillData(data);
    zserio::View view(data);

    for (auto offset : data.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

} // namespace bool_indexed_offset_array
} // namespace indexed_offsets
