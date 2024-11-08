#include <array>

#include "gtest/gtest.h"
#include "optional_members/optional_recursion/Block.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"
#include "zserio/RebindAlloc.h"

namespace optional_members
{
namespace optional_recursion
{

using AllocatorType = Block::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, AllocatorType>;

class OptionalRecursionTest : public ::testing::Test
{
protected:
    template <size_t SIZE>
    void fillBlock(Block& block, const std::array<zserio::UInt8, SIZE>& blockData)
    {
        VectorType<zserio::UInt8> dataBytes;
        dataBytes.reserve(blockData.size());
        dataBytes.assign(blockData.begin(), blockData.end());
        block.dataBytes = dataBytes;

        block.blockTerminator = 0;
    }

    template <size_t SIZE1, size_t SIZE2>
    void fillBlock(Block& block1, const std::array<zserio::UInt8, SIZE1>& block1Data,
            const std::array<zserio::UInt8, SIZE2>& block2Data)
    {
        Block block2;
        fillBlock(block2, block2Data);

        VectorType<zserio::UInt8> dataBytes;
        dataBytes.reserve(block1Data.size());
        dataBytes.assign(block1Data.begin(), block1Data.end());
        block1.dataBytes = dataBytes;

        block1.blockTerminator = static_cast<uint8_t>(block2Data.size());
        block1.nextData = block2;
    }

    size_t getBlockBitSize(size_t blockDataSize)
    {
        return 8 * blockDataSize + 8;
    }

    size_t getBlockBitSize(size_t block1DataSize, size_t block2DataSize)
    {
        return getBlockBitSize(block1DataSize) + getBlockBitSize(block2DataSize);
    }

    template <size_t SIZE>
    void checkBlockInBitStream(
            zserio::BitStreamReader& reader, const std::array<zserio::UInt8, SIZE>& blockData)
    {
        for (zserio::UInt8 data : blockData)
        {
            ASSERT_EQ(data, reader.readUnsignedBits32(8));
        }

        ASSERT_EQ(0, reader.readUnsignedBits32(8));
    }

    template <size_t SIZE1, size_t SIZE2>
    void checkBlockInBitStream(zserio::BitStreamReader& reader,
            const std::array<zserio::UInt8, SIZE1>& block1Data,
            const std::array<zserio::UInt8, SIZE2>& block2Data)
    {
        for (zserio::UInt8 data1 : block1Data)
        {
            ASSERT_EQ(data1, reader.readUnsignedBits32(8));
        }
        ASSERT_EQ(block2Data.size(), reader.readUnsignedBits32(8));

        checkBlockInBitStream(reader, block2Data);
    }

    static constexpr std::array<zserio::UInt8, 6> BLOCK1_DATA = {1, 2, 3, 4, 5, 6};
    static constexpr std::array<zserio::UInt8, 4> BLOCK2_DATA = {10, 9, 8, 7};

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(OptionalRecursionTest, nextData)
{
    Block block1;
    fillBlock(block1, BLOCK1_DATA);
    zserio::View<Block> view1(block1, zserio::UInt8(static_cast<uint8_t>(block1.dataBytes.size())));
    ASSERT_FALSE(view1.nextData());

    Block block12;
    fillBlock(block12, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12(block12, zserio::UInt8(static_cast<uint8_t>(block12.dataBytes.size())));
    ASSERT_TRUE(view12.nextData());
}

TEST_F(OptionalRecursionTest, operatorEquality)
{
    Block block1;
    fillBlock(block1, BLOCK1_DATA);
    zserio::View<Block> view1(block1, zserio::UInt8(static_cast<uint8_t>(block1.dataBytes.size())));

    Block block2;
    fillBlock(block2, BLOCK1_DATA);
    zserio::View<Block> view2(block2, zserio::UInt8(static_cast<uint8_t>(block2.dataBytes.size())));
    ASSERT_TRUE(block2 == block1);
    ASSERT_TRUE(view2 == view1);

    Block block12_1;
    fillBlock(block12_1, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12_1(block12_1, zserio::UInt8(static_cast<uint8_t>(block12_1.dataBytes.size())));
    ASSERT_FALSE(block12_1 == block1);
    ASSERT_FALSE(view12_1 == view1);

    Block block12_2;
    fillBlock(block12_2, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12_2(block12_2, zserio::UInt8(static_cast<uint8_t>(block12_2.dataBytes.size())));
    ASSERT_TRUE(block12_1 == block12_2);
    ASSERT_TRUE(view12_1 == view12_2);

    block12_1.blockTerminator = 0; // set but not used
    ASSERT_FALSE(block12_1 == block12_2);
    ASSERT_FALSE(view12_1 == view12_2);

    block12_2.blockTerminator = 0; // set but not used
    ASSERT_TRUE(block12_1 == block12_2);
    ASSERT_TRUE(view12_1 == view12_2);
}

TEST_F(OptionalRecursionTest, operatorLessThan)
{
    Block block1;
    fillBlock(block1, BLOCK1_DATA);
    zserio::View<Block> view1(block1, zserio::UInt8(static_cast<uint8_t>(block1.dataBytes.size())));

    Block block2;
    fillBlock(block2, BLOCK1_DATA);
    zserio::View<Block> view2(block2, zserio::UInt8(static_cast<uint8_t>(block2.dataBytes.size())));
    ASSERT_FALSE(block1 < block2);
    ASSERT_FALSE(block2 < block1);
    ASSERT_FALSE(view1 < view2);
    ASSERT_FALSE(view2 < view1);

    Block block12_1;
    fillBlock(block12_1, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12_1(block12_1, zserio::UInt8(static_cast<uint8_t>(block12_1.dataBytes.size())));
    ASSERT_TRUE(block1 < block12_1);
    ASSERT_FALSE(block12_1 < block1);
    ASSERT_TRUE(view1 < view12_1);
    ASSERT_FALSE(view12_1 < view1);

    Block block12_2;
    fillBlock(block12_2, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12_2(block12_2, zserio::UInt8(static_cast<uint8_t>(block12_2.dataBytes.size())));
    ASSERT_FALSE(block12_1 < block12_2);
    ASSERT_FALSE(block12_2 < block12_1);
    ASSERT_FALSE(view12_1 < view12_2);
    ASSERT_FALSE(view12_2 < view12_1);

    block12_1.blockTerminator = 0; // set but not used
    ASSERT_TRUE(block12_1 < block12_2);
    ASSERT_FALSE(block12_2 < block12_1);
    ASSERT_TRUE(view12_1 < view12_2);
    ASSERT_FALSE(view12_2 < view12_1);

    block12_2.blockTerminator = 0; // set but not used
    ASSERT_FALSE(block12_1 < block12_2);
    ASSERT_FALSE(block12_2 < block12_1);
    ASSERT_FALSE(view12_1 < view12_2);
    ASSERT_FALSE(view12_2 < view12_1);
}

TEST_F(OptionalRecursionTest, stdHash)
{
    std::hash<Block> blockHasher;
    std::hash<zserio::View<Block>> viewHasher;

    Block block1;
    fillBlock(block1, BLOCK1_DATA);
    zserio::View<Block> view1(block1, zserio::UInt8(static_cast<uint8_t>(block1.dataBytes.size())));

    Block block2;
    fillBlock(block2, BLOCK1_DATA);
    zserio::View<Block> view2(block2, zserio::UInt8(static_cast<uint8_t>(block2.dataBytes.size())));
    ASSERT_EQ(blockHasher(block2), blockHasher(block1));
    ASSERT_EQ(viewHasher(view2), viewHasher(view1));

    Block block12_1;
    fillBlock(block12_1, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12_1(block12_1, zserio::UInt8(static_cast<uint8_t>(block12_1.dataBytes.size())));
    ASSERT_NE(blockHasher(block12_1), blockHasher(block1));
    ASSERT_NE(viewHasher(view12_1), viewHasher(view1));

    Block block12_2;
    fillBlock(block12_2, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12_2(block12_2, zserio::UInt8(static_cast<uint8_t>(block12_2.dataBytes.size())));
    ASSERT_EQ(blockHasher(block12_1), blockHasher(block12_2));
    ASSERT_EQ(viewHasher(view12_1), viewHasher(view12_2));

    block12_1.blockTerminator = 0; // set but not used
    ASSERT_NE(blockHasher(block12_1), blockHasher(block12_2));
    ASSERT_NE(viewHasher(view12_1), viewHasher(view12_2));

    // TODO[Mi-L@]: Hardcoded hash values have changed in C++17 generator!

    block12_2.blockTerminator = 0; // set but not used
    ASSERT_EQ(blockHasher(block12_1), blockHasher(block12_2));
    ASSERT_EQ(viewHasher(view12_1), viewHasher(view12_2));
}

TEST_F(OptionalRecursionTest, bitSizeOf)
{
    Block block1;
    fillBlock(block1, BLOCK1_DATA);
    zserio::View<Block> view1(block1, zserio::UInt8(static_cast<uint8_t>(block1.dataBytes.size())));
    ASSERT_EQ(getBlockBitSize(BLOCK1_DATA.size()), zserio::detail::bitSizeOf(view1));

    Block block12;
    fillBlock(block12, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12(block12, zserio::UInt8(static_cast<uint8_t>(block12.dataBytes.size())));
    ASSERT_EQ(getBlockBitSize(BLOCK1_DATA.size(), BLOCK2_DATA.size()), zserio::detail::bitSizeOf(view12));

    block12.blockTerminator = 0; // set but not used
    ASSERT_EQ(getBlockBitSize(BLOCK1_DATA.size()), zserio::detail::bitSizeOf(view12));
}

TEST_F(OptionalRecursionTest, writeBlock1)
{
    Block block1;
    fillBlock(block1, BLOCK1_DATA);
    zserio::View<Block> view1(block1, zserio::UInt8(static_cast<uint8_t>(block1.dataBytes.size())));

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view1);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    checkBlockInBitStream(reader, BLOCK1_DATA);
    reader.setBitPosition(0);

    Block readBlock1;
    zserio::View<Block> readView1 =
            zserio::detail::read(reader, readBlock1, zserio::UInt8(static_cast<uint8_t>(BLOCK1_DATA.size())));
    ASSERT_EQ(block1, readBlock1);
    ASSERT_EQ(view1, readView1);
}

TEST_F(OptionalRecursionTest, writeBlock12)
{
    Block block12;
    fillBlock(block12, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View<Block> view12(block12, zserio::UInt8(static_cast<uint8_t>(block12.dataBytes.size())));

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view12);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    checkBlockInBitStream(reader, BLOCK1_DATA, BLOCK2_DATA);
    reader.setBitPosition(0);

    Block readBlock12;
    zserio::View<Block> readView12 =
            zserio::detail::read(reader, readBlock12, zserio::UInt8(static_cast<uint8_t>(BLOCK1_DATA.size())));
    ASSERT_EQ(block12, readBlock12);
    ASSERT_EQ(view12, readView12);

    block12.blockTerminator = 0; // set but not used
    zserio::BitStreamWriter writer2(bitBuffer);
    zserio::detail::write(writer2, view12);

    zserio::BitStreamReader reader2(writer2.getWriteBuffer(), writer2.getBitPosition(), zserio::BitsTag());
    checkBlockInBitStream(reader2, BLOCK1_DATA);
    reader2.setBitPosition(0);

    Block readBlock12_2;
    zserio::View<Block> readView12_2 = zserio::detail::read(
            reader2, readBlock12_2, zserio::UInt8(static_cast<uint8_t>(BLOCK1_DATA.size())));
    // block12 now has different data than readBlock12_2
    ASSERT_EQ(view12, readView12_2);
}

} // namespace optional_recursion
} // namespace optional_members
