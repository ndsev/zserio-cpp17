#include "gtest/gtest.h"
#include "parameterized_types/packed_array_element_param/Database.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace packed_array_element_param
{

class PackedArrayElementParamTest : public ::testing::Test
{
protected:
    void fillData(Database& data, uint16_t numBlocks)
    {
        data.numBlocks = numBlocks;
        auto& headers = data.headers;
        for (uint16_t i = 0; i < numBlocks; ++i)
        {
            BlockHeader blockHeader;
            blockHeader.numItems = static_cast<uint16_t>(i + 1);
            blockHeader.offset = 0;
            headers.push_back(blockHeader);
        }

        auto& blocks = data.blocks;
        for (const auto& header : headers)
        {
            Block block;
            auto& items = block.items;
            const uint16_t numItems = header.numItems;
            block.value = numItems;
            for (uint16_t j = 0; j < numItems; ++j)
            {
                items.push_back(j * 2);
            }
            blocks.push_back(block);
        }
    }

    size_t getUnpackedDatabaseBitSize(uint16_t numBlocks)
    {
        size_t bitSize = 16; // numBlocks
        bitSize += static_cast<size_t>(numBlocks) * (16 + 32); // headers
        for (size_t i = 0; i < numBlocks; ++i)
        {
            bitSize += 64 + (i + 1) * 64; // blocks[i]
        }

        return bitSize;
    }

    void checkBitSizeOf(uint16_t numBlocks)
    {
        Database data;
        fillData(data, numBlocks);
        zserio::View view(data);

        const double unpackedBitSize = static_cast<double>(getUnpackedDatabaseBitSize(numBlocks));
        const double packedBitSize = static_cast<double>(zserio::detail::bitSizeOf(view));
        const double minCompressionRatio = 0.12;
        ASSERT_GT(unpackedBitSize * minCompressionRatio, packedBitSize)
                << "Unpacked array has " << std::to_string(unpackedBitSize) << " bits, packed array has "
                << std::to_string(packedBitSize) << " bits, compression ratio is "
                << std::to_string(packedBitSize / unpackedBitSize * 100) << "%!";
    }

    void checkWriteRead(uint16_t numBlocks)
    {
        Database data;
        fillData(data, numBlocks);

        test_utils::writeReadTest(data);
    }

    void checkWriteReadFile(uint16_t numBlocks)
    {
        Database data;
        fillData(data, numBlocks);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numBlocks) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE =
            "language/parameterized_types/packed_array_element_param_";
    static constexpr uint16_t NUM_BLOCKS1 = 50;
    static constexpr uint16_t NUM_BLOCKS2 = 100;
    static constexpr uint16_t NUM_BLOCKS3 = 1000;
};

TEST_F(PackedArrayElementParamTest, bitSizeOfLength1)
{
    checkBitSizeOf(NUM_BLOCKS1);
}

TEST_F(PackedArrayElementParamTest, bitSizeOfLength2)
{
    checkBitSizeOf(NUM_BLOCKS2);
}

TEST_F(PackedArrayElementParamTest, bitSizeOfLength3)
{
    checkBitSizeOf(NUM_BLOCKS3);
}

TEST_F(PackedArrayElementParamTest, writeReadLength1)
{
    checkWriteRead(NUM_BLOCKS1);
}

TEST_F(PackedArrayElementParamTest, writeReadLength2)
{
    checkWriteRead(NUM_BLOCKS2);
}

TEST_F(PackedArrayElementParamTest, writeReadLength3)
{
    checkWriteRead(NUM_BLOCKS3);
}

TEST_F(PackedArrayElementParamTest, writeReadFileLength1)
{
    checkWriteReadFile(NUM_BLOCKS1);
}

TEST_F(PackedArrayElementParamTest, writeReadFileLength2)
{
    checkWriteReadFile(NUM_BLOCKS2);
}

TEST_F(PackedArrayElementParamTest, writeReadFileLength3)
{
    checkWriteReadFile(NUM_BLOCKS3);
}

} // namespace packed_array_element_param
} // namespace parameterized_types
