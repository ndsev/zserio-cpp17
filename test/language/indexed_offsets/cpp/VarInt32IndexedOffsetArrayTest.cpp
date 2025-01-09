#include "gtest/gtest.h"
#include "indexed_offsets/varint32_indexed_offset_array/VarInt32IndexedOffsetArray.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace varint32_indexed_offset_array
{

class VarInt32IndexedOffsetArrayTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        uint32_t currentOffset = ELEMENT0_OFFSET;
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeUnsignedBits32(currentOffset, 32);
            currentOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(zserio::VarInt32(i)) / 8);
        }

        writer.writeUnsignedBits32(SPACER_VALUE, 1);
        writer.writeUnsignedBits32(0, 7);

        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeVarInt32(i);
        }
    }

    static void checkOffsets(
            const VarInt32IndexedOffsetArray& data, const zserio::View<VarInt32IndexedOffsetArray>& view)
    {
        uint32_t expectedOffset = ELEMENT0_OFFSET;
        for (size_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            ASSERT_EQ(expectedOffset, data.offsets.at(i));
            ASSERT_EQ(expectedOffset, view.offsets().at(i));
            expectedOffset +=
                    static_cast<uint32_t>(zserio::detail::bitSizeOf(zserio::VarInt32(data.data[i])) / 8);
        }
    }

    void fillData(VarInt32IndexedOffsetArray& data, bool setOffsets = false)
    {
        data.offsets.resize(NUM_ELEMENTS);
        if (setOffsets)
        {
            uint32_t currentOffset = ELEMENT0_OFFSET;
            for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
            {
                data.offsets[i] = currentOffset;
                currentOffset += static_cast<uint32_t>(zserio::detail::bitSizeOf(zserio::VarInt32(i)) / 8);
            }
        }

        data.spacer = SPACER_VALUE;

        data.data.reserve(NUM_ELEMENTS);
        for (uint8_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            data.data.push_back(i);
        }
    }

    static constexpr int8_t NUM_ELEMENTS = 5;
    static constexpr int32_t ELEMENT0_OFFSET = NUM_ELEMENTS * sizeof(uint32_t) + sizeof(uint8_t);
    static constexpr int8_t SPACER_VALUE = 1;
};

TEST_F(VarInt32IndexedOffsetArrayTest, read)
{
    VarInt32IndexedOffsetArray data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(VarInt32IndexedOffsetArrayTest, writeRead)
{
    VarInt32IndexedOffsetArray data;
    fillData(data);
    zserio::View view(data);

    for (auto offset : data.offsets)
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

} // namespace varint32_indexed_offset_array
} // namespace indexed_offsets
