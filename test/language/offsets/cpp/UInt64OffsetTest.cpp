#include "gtest/gtest.h"
#include "offsets/uint64_offset/UInt64Offset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace uint64_offset
{

class UInt64OffsetTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        // offset
        writer.writeUnsignedBits64(OFFSET, 64);
        writer.writeVarSize(static_cast<uint32_t>(ARRAY_SIZE));
        for (size_t i = 0; i < ARRAY_SIZE; ++i)
        {
            writer.writeSignedBits32(0, 8);
        }
        writer.writeSignedBits32(0, 32);
    }

    static void fillData(UInt64Offset& data, bool setOffset = false)
    {
        data.array.resize(ARRAY_SIZE);
        if (setOffset)
        {
            data.offset = OFFSET;
        }
    }

    static constexpr size_t ARRAY_SIZE = 13;
    static constexpr size_t OFFSET = 8 + 8 /* bitsize of ARRAY_SIZE*/ / 8 + ARRAY_SIZE;
    static constexpr size_t BIT_SIZE = (OFFSET + 4) * 8;

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(UInt64OffsetTest, read)
{
    UInt64Offset data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(UInt64OffsetTest, bitSizeOf)
{
    UInt64Offset data;
    fillData(data, true);
    zserio::View view(data);

    ASSERT_EQ(BIT_SIZE, zserio::detail::bitSizeOf(view));
}

TEST_F(UInt64OffsetTest, write)
{
    UInt64Offset data;
    fillData(data);
    zserio::View view(data);

    EXPECT_EQ(0, data.offset);
    EXPECT_EQ(0, view.offset());

    test_utils::writeReadTest(data);
    ASSERT_EQ(OFFSET, data.offset);
    ASSERT_EQ(OFFSET, view.offset());
}

} // namespace uint64_offset
} // namespace offsets
