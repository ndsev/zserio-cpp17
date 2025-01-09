#include "gtest/gtest.h"
#include "indexed_offsets/empty_indexed_offset_array/EmptyIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace empty_indexed_offset_array
{

class EmptyIndexedOffsetArrayTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(SPACER_VALUE, 1);
        writer.writeUnsignedBits32(FIELD_VALUE, 6);
    }

    static void checkOffsets(
            const EmptyIndexedOffsetArray& data, const zserio::View<EmptyIndexedOffsetArray>& view)
    {
        const size_t expectedNumElements = NUM_ELEMENTS;
        ASSERT_EQ(expectedNumElements, data.offsets.size());
        ASSERT_EQ(expectedNumElements, view.offsets().size());
    }

    void fillData(EmptyIndexedOffsetArray& data)
    {
        data.spacer = SPACER_VALUE;
        data.field = FIELD_VALUE;
    }

    static constexpr uint8_t NUM_ELEMENTS = 0;

    static constexpr uint8_t SPACER_VALUE = 1;
    static constexpr uint8_t FIELD_VALUE = 63;

    static constexpr size_t EMPTY_INDEXED_OFFSET_ARRAY_BIT_SIZE = 1 + 6;
};

TEST_F(EmptyIndexedOffsetArrayTest, read)
{
    EmptyIndexedOffsetArray data;
    fillData(data);

    test_utils::readTest(writeData, data);
}

TEST_F(EmptyIndexedOffsetArrayTest, writeRead)
{
    EmptyIndexedOffsetArray data;
    fillData(data);
    zserio::View view(data);

    test_utils::writeReadTest(data);
    checkOffsets(data, view);
}

} // namespace empty_indexed_offset_array
} // namespace indexed_offsets
