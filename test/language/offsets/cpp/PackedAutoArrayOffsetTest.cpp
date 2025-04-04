#include "gtest/gtest.h"
#include "offsets/packed_auto_array_offset/AutoArrayHolder.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace offsets
{
namespace packed_auto_array_offset
{

using allocator_type = AutoArrayHolder::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

class PackedAutoArrayOffsetTest : public ::testing::Test
{
protected:
    static void fillData(AutoArrayHolder& data, bool setOffset = false)
    {
        if (setOffset)
        {
            data.autoArrayOffset = AUTO_ARRAY_OFFSET;
        }

        data.forceAlignment = FORCED_ALIGNMENT_VALUE;

        VectorType<zserio::Int7> autoArray;
        autoArray.reserve(AUTO_ARRAY_LENGTH);
        for (size_t i = 0; i < AUTO_ARRAY_LENGTH; ++i)
        {
            autoArray.push_back(static_cast<int8_t>(i));
        }
        data.autoArray = autoArray;
    }

    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(AUTO_ARRAY_OFFSET, 32);

        writer.writeUnsignedBits32(FORCED_ALIGNMENT_VALUE, 1);
        writer.alignTo(8);

        writer.writeVarSize(static_cast<uint32_t>(AUTO_ARRAY_LENGTH));

        writer.writeBool(true);
        writer.writeUnsignedBits32(PACKED_ARRAY_MAX_BIT_NUMBER, 6);
        writer.writeUnsignedBits32(0, 7);
        for (size_t i = 0; i < AUTO_ARRAY_LENGTH - 1; ++i)
        {
            writer.writeSignedBits32(PACKED_ARRAY_DELTA, static_cast<uint8_t>(PACKED_ARRAY_MAX_BIT_NUMBER + 1));
        }
    }

    static size_t getAutoArrayHolderBitSize()
    {
        size_t bitSize = 32; // field: autoArrayOffset
        bitSize += 1; // field: forceAlignment
        bitSize += 7; // padding because of alignment
        bitSize += 8; // auto varsize
        bitSize += 1; // packing descriptor: isPacked
        bitSize += 6; // packing descriptor: maxBitNumber
        bitSize += 7; // first element
        bitSize += (AUTO_ARRAY_LENGTH - 1) * (PACKED_ARRAY_MAX_BIT_NUMBER + 1U); // all deltas

        return bitSize;
    }

    static constexpr std::string_view BLOB_NAME = "language/offsets/packed_auto_array_offset.blob";

    static constexpr size_t AUTO_ARRAY_LENGTH = 5;
    static constexpr uint8_t FORCED_ALIGNMENT_VALUE = 0;

    static constexpr uint32_t AUTO_ARRAY_OFFSET = 5;

    static constexpr int8_t PACKED_ARRAY_DELTA = 1;
    static constexpr uint8_t PACKED_ARRAY_MAX_BIT_NUMBER = 1;
};

TEST_F(PackedAutoArrayOffsetTest, read)
{
    AutoArrayHolder data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(PackedAutoArrayOffsetTest, bitSizeOf)
{
    AutoArrayHolder data;
    fillData(data, true);
    zserio::View view(data);

    ASSERT_EQ(getAutoArrayHolderBitSize(), zserio::detail::bitSizeOf(view));
}

TEST_F(PackedAutoArrayOffsetTest, writeRead)
{
    AutoArrayHolder data;
    fillData(data);
    zserio::View view(data);

    EXPECT_EQ(0, data.autoArrayOffset);
    EXPECT_EQ(0, view.autoArrayOffset());

    test_utils::writeReadTest(data);
    ASSERT_EQ(AUTO_ARRAY_OFFSET, data.autoArrayOffset);
    ASSERT_EQ(AUTO_ARRAY_OFFSET, view.autoArrayOffset());
}

TEST_F(PackedAutoArrayOffsetTest, writeReadFile)
{
    AutoArrayHolder data;
    fillData(data);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

} // namespace packed_auto_array_offset
} // namespace offsets
