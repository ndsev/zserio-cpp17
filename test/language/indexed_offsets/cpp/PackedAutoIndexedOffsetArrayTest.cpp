#include "gtest/gtest.h"
#include "indexed_offsets/packed_auto_indexed_offset_array/AutoIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace packed_auto_indexed_offset_array
{

class PackedAutoIndexedOffsetArrayTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeVarSize(NUM_ELEMENTS);
        uint32_t currentOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeUnsignedBits32(currentOffset, 32);
            currentOffset += (i == 0 ? ALIGNED_FIRST_ELEMENT_BYTE_SIZE : ALIGNED_ELEMENT_BYTE_SIZE);
        }

        writer.writeUnsignedBits32(SPACER_VALUE, 3);

        writer.writeVarSize(NUM_ELEMENTS);

        writer.alignTo(8);
        writer.writeBool(true);
        writer.writeUnsignedBits32(PACKED_ARRAY_MAX_BIT_NUMBER, 6);
        writer.writeUnsignedBits32(0, ELEMENT_SIZE);
        for (uint8_t i = 1; i < NUM_ELEMENTS; ++i)
        {
            writer.alignTo(8);
            writer.writeSignedBits32(PACKED_ARRAY_DELTA, PACKED_ARRAY_MAX_BIT_NUMBER + 1);
        }
    }

    static void checkOffsets(
            const AutoIndexedOffsetArray& data, const zserio::View<AutoIndexedOffsetArray>& view)
    {
        uint32_t expectedOffset = ELEMENT0_OFFSET;
        for (size_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            ASSERT_EQ(expectedOffset, data.offsets.at(i));
            ASSERT_EQ(expectedOffset, view.offsets().at(i));
            expectedOffset += (i == 0 ? ALIGNED_FIRST_ELEMENT_BYTE_SIZE : ALIGNED_ELEMENT_BYTE_SIZE);
        }
    }

    void fillData(AutoIndexedOffsetArray& data, bool setOffsets = false)
    {
        data.offsets.resize(NUM_ELEMENTS);
        if (setOffsets)
        {
            uint32_t currentOffset = ELEMENT0_OFFSET;
            for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
            {
                data.offsets[i] = currentOffset;
                currentOffset += (i == 0 ? ALIGNED_FIRST_ELEMENT_BYTE_SIZE : ALIGNED_ELEMENT_BYTE_SIZE);
            }
        }

        data.spacer = SPACER_VALUE;

        data.data.reserve(NUM_ELEMENTS);
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            data.data.push_back(i);
        }
    }

    static constexpr std::string_view BLOB_NAME =
            "language/indexed_offsets/packed_auto_indexed_offset_array.blob";

    static constexpr uint8_t NUM_ELEMENTS = 5;

    static constexpr size_t AUTO_ARRAY_LENGTH_BYTE_SIZE = 1;
    static constexpr uint32_t ELEMENT0_OFFSET = AUTO_ARRAY_LENGTH_BYTE_SIZE + NUM_ELEMENTS * 4 +
            (3 + AUTO_ARRAY_LENGTH_BYTE_SIZE * 8 + 5 /* alignment */) / 8;
    static constexpr uint8_t ELEMENT_SIZE = 5;
    static constexpr uint8_t ALIGNED_FIRST_ELEMENT_SIZE = 1 + 6 + ELEMENT_SIZE + 4 /* alignment */;
    static constexpr uint8_t ALIGNED_FIRST_ELEMENT_BYTE_SIZE = ALIGNED_FIRST_ELEMENT_SIZE / 8;
    static constexpr uint8_t ALIGNED_ELEMENT_SIZE = 8;
    static constexpr uint8_t ALIGNED_ELEMENT_BYTE_SIZE = ALIGNED_ELEMENT_SIZE / 8;

    static constexpr uint8_t SPACER_VALUE = 7;

    static constexpr int8_t PACKED_ARRAY_DELTA = 1;
    static constexpr uint8_t PACKED_ARRAY_MAX_BIT_NUMBER = 1;

    static constexpr size_t AUTO_INDEXED_OFFSET_ARRAY_BIT_SIZE = ELEMENT0_OFFSET * 8 +
            ALIGNED_FIRST_ELEMENT_SIZE + (NUM_ELEMENTS - 2) * ALIGNED_ELEMENT_SIZE +
            PACKED_ARRAY_MAX_BIT_NUMBER + 1;
};

TEST_F(PackedAutoIndexedOffsetArrayTest, read)
{
    AutoIndexedOffsetArray data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(PackedAutoIndexedOffsetArrayTest, writeRead)
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

TEST_F(PackedAutoIndexedOffsetArrayTest, writeReadFile)
{
    AutoIndexedOffsetArray data;
    fillData(data);
    zserio::View view(data);

    for (auto offset : data.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadFileTest(BLOB_NAME, data);
    checkOffsets(data, view);
}

} // namespace packed_auto_indexed_offset_array
} // namespace indexed_offsets
