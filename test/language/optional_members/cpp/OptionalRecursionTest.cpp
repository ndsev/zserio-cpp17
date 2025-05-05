#include <array>

#include "gtest/gtest.h"
#include "optional_members/optional_recursion/Block.h"
#include "test_utils/TestUtility.h"
#include "zserio/ArrayLengthException.h"
#include "zserio/RebindAlloc.h"

namespace optional_members
{
namespace optional_recursion
{

using AllocatorType = Block::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class OptionalRecursionTest : public ::testing::Test
{
protected:
    template <size_t SIZE>
    void fillData(Block& block, const std::array<zserio::UInt8, SIZE>& blockData)
    {
        VectorType<zserio::UInt8> dataBytes;
        dataBytes.reserve(blockData.size());
        dataBytes.assign(blockData.begin(), blockData.end());
        block.dataBytes = dataBytes;

        block.blockTerminator = 0;
    }

    template <size_t SIZE1, size_t SIZE2>
    void fillData(Block& data1, const std::array<zserio::UInt8, SIZE1>& block1Data,
            const std::array<zserio::UInt8, SIZE2>& block2Data)
    {
        Block data2;
        fillData(data2, block2Data);

        VectorType<zserio::UInt8> dataBytes;
        dataBytes.reserve(block1Data.size());
        dataBytes.assign(block1Data.begin(), block1Data.end());
        data1.dataBytes = dataBytes;

        data1.blockTerminator = static_cast<uint8_t>(block2Data.size());
        data1.nextData = data2;
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
    static void writeData(zserio::BitStreamWriter& writer, const std::array<zserio::UInt8, SIZE>& blockData)
    {
        for (zserio::UInt8 data : blockData)
        {
            writer.writeUnsignedBits32(data, 8);
        }

        writer.writeUnsignedBits32(0, 8);
    }

    template <size_t SIZE1, size_t SIZE2>
    static void writeData(zserio::BitStreamWriter& writer, const std::array<zserio::UInt8, SIZE1>& block1Data,
            const std::array<zserio::UInt8, SIZE2>& block2Data)
    {
        for (zserio::UInt8 data1 : block1Data)
        {
            writer.writeUnsignedBits32(data1, 8);
        }

        writer.writeUnsignedBits32(static_cast<uint8_t>(block2Data.size()), 8);

        writeData(writer, block2Data);
    }

    static constexpr std::array<zserio::UInt8, 6> BLOCK1_DATA = {1, 2, 3, 4, 5, 6};
    static constexpr std::array<zserio::UInt8, 4> BLOCK2_DATA = {10, 9, 8, 7};
};

TEST_F(OptionalRecursionTest, nextData)
{
    Block data1;
    fillData(data1, BLOCK1_DATA);
    zserio::View view1(data1, zserio::UInt8(static_cast<uint8_t>(data1.dataBytes.size())));
    ASSERT_FALSE(view1.nextData());

    Block data12;
    fillData(data12, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View view12(data12, zserio::UInt8(static_cast<uint8_t>(data12.dataBytes.size())));
    ASSERT_TRUE(view12.nextData());
}

TEST_F(OptionalRecursionTest, comparisionOperators)
{
    Block data;
    fillData(data, BLOCK1_DATA, BLOCK2_DATA);
    Block equalData;
    fillData(equalData, BLOCK1_DATA, BLOCK2_DATA);
    Block lessThanData;
    fillData(lessThanData, BLOCK1_DATA);

    test_utils::comparisonOperatorsTest(data, equalData, lessThanData);

    zserio::View view(data, zserio::UInt8(static_cast<uint8_t>(data.dataBytes.size())));
    zserio::View equalView(equalData, zserio::UInt8(static_cast<uint8_t>(equalData.dataBytes.size())));
    zserio::View lessThanView(lessThanData, zserio::UInt8(static_cast<uint8_t>(lessThanData.dataBytes.size())));

    test_utils::comparisonOperatorsTest(view, equalView, lessThanView);
}

TEST_F(OptionalRecursionTest, bitSizeOf)
{
    Block data1;
    fillData(data1, BLOCK1_DATA);
    zserio::View view1(data1, zserio::UInt8(static_cast<uint8_t>(data1.dataBytes.size())));
    ASSERT_EQ(getBlockBitSize(BLOCK1_DATA.size()), zserio::detail::bitSizeOf(view1));

    Block data12;
    fillData(data12, BLOCK1_DATA, BLOCK2_DATA);
    zserio::View view12(data12, zserio::UInt8(static_cast<uint8_t>(data12.dataBytes.size())));
    ASSERT_EQ(getBlockBitSize(BLOCK1_DATA.size(), BLOCK2_DATA.size()), zserio::detail::bitSizeOf(view12));
}

TEST_F(OptionalRecursionTest, writeBlock1)
{
    Block data;
    fillData(data, BLOCK1_DATA);

    test_utils::writeReadTest(data, zserio::UInt8(static_cast<uint8_t>(data.dataBytes.size())));
}

TEST_F(OptionalRecursionTest, writeBlock12)
{
    Block data;
    fillData(data, BLOCK1_DATA, BLOCK2_DATA);

    test_utils::writeReadTest(data, zserio::UInt8(static_cast<uint8_t>(data.dataBytes.size())));
}

TEST_F(OptionalRecursionTest, readBlock1)
{
    Block data;
    fillData(data, BLOCK1_DATA);

    test_utils::readTest(std::bind(writeData<BLOCK1_DATA.size()>, std::placeholders::_1, BLOCK1_DATA), data,
            zserio::UInt8(static_cast<uint8_t>(data.dataBytes.size())));
}

TEST_F(OptionalRecursionTest, readBlock12)
{
    Block data;
    fillData(data, BLOCK1_DATA, BLOCK2_DATA);

    test_utils::readTest(std::bind(writeData<BLOCK1_DATA.size(), BLOCK2_DATA.size()>, std::placeholders::_1,
                                 BLOCK1_DATA, BLOCK2_DATA),
            data, zserio::UInt8(static_cast<uint8_t>(data.dataBytes.size())));
}

TEST_F(OptionalRecursionTest, stdHash)
{
    Block data;
    fillData(data, BLOCK1_DATA, BLOCK2_DATA);
    const size_t dataHash = 1801594417;
    Block equalData;
    fillData(equalData, BLOCK1_DATA, BLOCK2_DATA);
    Block diffData;
    fillData(diffData, BLOCK1_DATA);
    const size_t diffDataHash = 5066880;

    test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);

    zserio::View view(data, zserio::UInt8(static_cast<uint8_t>(data.dataBytes.size())));
    const size_t viewHash = 3311721331;
    zserio::View equalView(equalData, zserio::UInt8(static_cast<uint8_t>(equalData.dataBytes.size())));
    zserio::View diffView(diffData, zserio::UInt8(static_cast<uint8_t>(diffData.dataBytes.size())));
    const size_t diffViewHash = 4058428634;

    test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);
}

} // namespace optional_recursion
} // namespace optional_members
