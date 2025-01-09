#include "gtest/gtest.h"
#include "indexed_offsets/compound_indexed_offset_array/CompoundIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace compound_indexed_offset_array
{

class CompoundIndexedOffsetArrayTest : public ::testing::Test
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
            writer.writeUnsignedBits32(i, 32);
            writer.writeUnsignedBits32(i % 8, 3);
            if ((i + 1) != NUM_ELEMENTS)
            {
                writer.writeUnsignedBits32(0, ALIGNED_ELEMENT_SIZE - ELEMENT_SIZE);
            }
        }
    }

    static void checkOffsets(
            const CompoundIndexedOffsetArray& data, const zserio::View<CompoundIndexedOffsetArray>& view)
    {
        uint32_t expectedOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            ASSERT_EQ(expectedOffset, data.offsets.at(i));
            ASSERT_EQ(expectedOffset, view.offsets().at(i));
            expectedOffset += ALIGNED_ELEMENT_BYTE_SIZE;
        }
    }

    static void fillData(CompoundIndexedOffsetArray& data, bool setOffsets = false)
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
            Compound compound(i, i % 8);
            data.data.push_back(compound);
        }
    }

    static constexpr uint8_t NUM_ELEMENTS = 5;

    static constexpr uint32_t ELEMENT0_OFFSET = NUM_ELEMENTS * sizeof(uint32_t) + sizeof(uint8_t);
    static constexpr uint8_t ELEMENT_SIZE = 35;
    static constexpr uint8_t ALIGNED_ELEMENT_SIZE = 5 * 8;
    static constexpr uint8_t ALIGNED_ELEMENT_BYTE_SIZE = ALIGNED_ELEMENT_SIZE / 8;

    static constexpr uint8_t SPACER_VALUE = 1;

    static constexpr size_t COMPOUND_INDEXED_OFFSET_ARRAY_BIT_SIZE =
            ELEMENT0_OFFSET * 8 + (NUM_ELEMENTS - 1) * ALIGNED_ELEMENT_SIZE + ELEMENT_SIZE;
};

TEST_F(CompoundIndexedOffsetArrayTest, read)
{
    CompoundIndexedOffsetArray data;
    fillData(data, true);
    zserio::View view(data);

    test_utils::readTest(writeData, data);
}

TEST_F(CompoundIndexedOffsetArrayTest, writeRead)
{
    CompoundIndexedOffsetArray data;
    fillData(data);
    zserio::View view(data);

    for (auto offset : data.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

} // namespace compound_indexed_offset_array
} // namespace indexed_offsets
