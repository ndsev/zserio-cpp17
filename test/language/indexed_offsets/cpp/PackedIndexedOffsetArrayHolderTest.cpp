#include "gtest/gtest.h"
#include "indexed_offsets/packed_indexed_offset_array_holder/AutoIndexedOffsetArray.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace indexed_offsets
{
namespace packed_indexed_offset_array_holder
{

class PackedIndexedOffsetArrayHolderTest : public ::testing::Test
{
protected:
    void fillData(AutoIndexedOffsetArray& data, size_t numElements)
    {
        auto& offsetArray = data.offsetArray;
        auto& offsetHolders = offsetArray.offsetHolders;
        offsetHolders.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            offsetHolders.emplace_back(UINT32_C(0), UINT32_C(0), static_cast<uint32_t>(i));
        }

        auto& data1 = data.data1;
        data1.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            data1.push_back(static_cast<int32_t>(i));
        }

        auto& data2 = data.data2;
        data2.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            data2.push_back(static_cast<int32_t>(i * 2));
        }
    }

    size_t getAutoIndexedOffsetArrayBitSize(size_t numElements)
    {
        size_t bitSize = 0;
        for (size_t i = 0; i < numElements; ++i)
        {
            bitSize += 32; // offset1[i]
            bitSize += 32; // offset2[i]
            bitSize += 32; // value[i]
        }

        for (size_t i = 0; i < numElements; ++i)
        {
            bitSize += 32; // data1[i]
        }

        for (size_t i = 0; i < numElements; ++i)
        {
            bitSize += 32; // data2[i]
        }

        return bitSize;
    }

    void checkBitSizeOf(size_t numElements)
    {
        AutoIndexedOffsetArray data;
        fillData(data, numElements);
        zserio::View view(data);

        const double unpackedBitSize = static_cast<double>(getAutoIndexedOffsetArrayBitSize(numElements));
        const double packedBitSize = static_cast<double>(zserio::detail::bitSizeOf(view));
        const double minCompressionRation = 0.83;
        ASSERT_GT(unpackedBitSize * minCompressionRation, packedBitSize)
                << "Unpacked array has " << std::to_string(unpackedBitSize) << " bits, packed array has "
                << std::to_string(packedBitSize) << " bits, compression ratio is "
                << std::to_string(packedBitSize / unpackedBitSize * 100) << "%!";
    }

    void checkWriteRead(size_t numElements)
    {
        AutoIndexedOffsetArray data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    void checkWriteReadFile(size_t numElements)
    {
        AutoIndexedOffsetArray data;
        fillData(data, numElements);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE =
            "language/indexed_offsets/packed_indexed_offset_array_holder_";

    static constexpr size_t NUM_ELEMENTS1 = 50;
    static constexpr size_t NUM_ELEMENTS2 = 100;
    static constexpr size_t NUM_ELEMENTS3 = 1000;
};

TEST_F(PackedIndexedOffsetArrayHolderTest, bitSizeOfLength1)
{
    checkBitSizeOf(NUM_ELEMENTS1);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, bitSizeOfLength2)
{
    checkBitSizeOf(NUM_ELEMENTS2);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, bitSizeOfLength3)
{
    checkBitSizeOf(NUM_ELEMENTS3);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, writeReadLength1)
{
    checkWriteRead(NUM_ELEMENTS1);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, writeReadLength2)
{
    checkWriteRead(NUM_ELEMENTS2);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, writeReadLength3)
{
    checkWriteRead(NUM_ELEMENTS3);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, writeReadFileLength1)
{
    checkWriteReadFile(NUM_ELEMENTS1);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, writeReadFileLength2)
{
    checkWriteReadFile(NUM_ELEMENTS2);
}

TEST_F(PackedIndexedOffsetArrayHolderTest, writeReadFileLength3)
{
    checkWriteReadFile(NUM_ELEMENTS3);
}

} // namespace packed_indexed_offset_array_holder
} // namespace indexed_offsets
