#include "gtest/gtest.h"
#include "parameterized_types/array_element_param/Database.h"
#include "test_utils/TestUtility.h"

namespace parameterized_types
{
namespace array_element_param
{

using AllocatorType = Database::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class ArrayElementParamTest : public ::testing::Test
{
protected:
    static void fillData(Database& data, bool setOffsets = false)
    {
        data.numBlocks = NUM_BLOCKS;
        auto& headers = data.headers;
        uint32_t offset = FIRST_BYTE_OFFSET;
        for (uint16_t i = 0; i < NUM_BLOCKS; ++i)
        {
            BlockHeader blockHeader;
            blockHeader.numItems = static_cast<uint16_t>(i + 1);
            if (setOffsets)
            {
                blockHeader.offset = offset;
            }
            headers.push_back(blockHeader);
            offset += 8U * blockHeader.numItems;
        }

        auto& blocks = data.blocks;
        for (const auto& header : headers)
        {
            Block block;
            auto& items = block.items;
            const uint16_t numItems = header.numItems;
            for (uint16_t j = 0; j < numItems; ++j)
            {
                items.push_back(j * 2);
            }
            blocks.push_back(block);
        }
    }

    static void writeData(zserio::BitStreamWriter& writer, const Database& data)
    {
        const uint16_t numBlocks = data.numBlocks;

        writer.writeUnsignedBits32(numBlocks, 16);

        const auto& headers = data.headers;
        uint32_t offset = FIRST_BYTE_OFFSET;
        for (uint16_t i = 0; i < numBlocks; ++i)
        {
            const uint16_t numItems = headers.at(i).numItems;
            writer.writeUnsignedBits32(numItems, 16);
            writer.writeUnsignedBits32(offset, 32);
            offset += 8U * numItems;
        }

        const auto& blocks = data.blocks;
        for (uint16_t i = 0; i < numBlocks; ++i)
        {
            const uint16_t numItems = headers.at(i).numItems;
            const auto& items = blocks.at(i).items;
            for (uint16_t j = 0; j < numItems; ++j)
            {
                writer.writeSignedBits64(items.at(j), 64);
            }
        }
    }

private:
    static constexpr uint16_t NUM_BLOCKS = 3;
    static constexpr uint32_t FIRST_BYTE_OFFSET = 2 + ArrayElementParamTest::NUM_BLOCKS * (2 + 4);
};

TEST_F(ArrayElementParamTest, constructors)
{
    {
        Database data;
        ASSERT_EQ(0, data.numBlocks);
        ASSERT_TRUE(data.headers.empty());
        ASSERT_TRUE(data.blocks.empty());
    }
    {
        Database data = {};
        ASSERT_EQ(0, data.numBlocks);
        ASSERT_TRUE(data.headers.empty());
        ASSERT_TRUE(data.blocks.empty());
    }
    {
        Database data(AllocatorType{});
        ASSERT_EQ(0, data.numBlocks);
        ASSERT_TRUE(data.headers.empty());
        ASSERT_TRUE(data.blocks.empty());
    }
    {
        zserio::UInt16 numBlocks = 1;
        VectorType<BlockHeader> headers(1);
        VectorType<Block> blocks(1);
        Database data(numBlocks, headers, blocks);
        zserio::View view(data);
        ASSERT_EQ(1, view.numBlocks());
        ASSERT_EQ(1, view.headers().size());
        ASSERT_EQ(1, view.blocks().size());
    }
}

TEST_F(ArrayElementParamTest, writeRead)
{
    Database data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(ArrayElementParamTest, read)
{
    Database dataForWrite;
    fillData(dataForWrite);

    Database expectedData;
    fillData(expectedData, true);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, dataForWrite), expectedData);
}

} // namespace array_element_param
} // namespace parameterized_types
