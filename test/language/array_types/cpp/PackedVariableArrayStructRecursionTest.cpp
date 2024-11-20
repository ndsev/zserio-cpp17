#include "array_types/packed_variable_array_struct_recursion/PackedVariableArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace packed_variable_array_struct_recursion
{

class PackedVariableArrayStructRecursionTest : public ::testing::Test
{
protected:
    static void fillData(PackedVariableArray& data, size_t numElements)
    {
        const zserio::UInt8 byteCount = 1;
        data.byteCount = byteCount;
        data.numElements = static_cast<uint32_t>(numElements);
        auto& blocks = data.packedBlocks;
        blocks.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            blocks.push_back(createBlock(byteCount, false));
        }
    }

    static Block createBlock(uint8_t byteCount, bool isLast)
    {
        Block block;
        auto& dataBytes = block.dataBytes;
        dataBytes.reserve(byteCount);
        for (uint8_t i = 0; i < byteCount; ++i)
        {
            dataBytes.push_back(i);
        }

        if (isLast)
        {
            block.blockTerminator = 0;
            return block;
        }
        else
        {
            const zserio::UInt8 blockTerminator = static_cast<uint8_t>(byteCount + 1);
            block.blockTerminator = blockTerminator;
            block.nextData = createBlock(blockTerminator, blockTerminator > 5);
            return block;
        }
    }

    static zserio::BitSize getUnpackedVariableArrayBitSize(size_t numElements)
    {
        zserio::BitSize bitSize = 8; // byteCount
        bitSize += 8; // numElements
        const uint8_t byteCount = 1;
        for (size_t i = 0; i < numElements; ++i)
        {
            bitSize += getUnpackedBlockBitSize(byteCount, false);
        }

        return bitSize;
    }

    static zserio::BitSize getUnpackedBlockBitSize(uint8_t byteCount, bool isLast)
    {
        zserio::BitSize bitSize = 8U * byteCount; // dataBytes[byteCount]
        bitSize += 8; // blockTerminator
        if (!isLast)
        {
            const uint8_t blockTerminator = static_cast<uint8_t>(byteCount + 1);
            bitSize += getUnpackedBlockBitSize(blockTerminator, blockTerminator > 5);
        }

        return bitSize;
    }

    static void checkBitSizeOf(size_t numElements)
    {
        PackedVariableArray data;
        fillData(data, numElements);
        const zserio::View<PackedVariableArray> view(data);

        const double unpackedBitSize = static_cast<double>(getUnpackedVariableArrayBitSize(numElements));
        const double packedBitSize = static_cast<double>(zserio::detail::bitSizeOf(view));
        const double minCompressionRatio = 0.9;
        ASSERT_GT(unpackedBitSize * minCompressionRatio, packedBitSize)
                << "Unpacked array has " << std::to_string(unpackedBitSize) << " bits, packed array has "
                << std::to_string(packedBitSize) << " bits, compression ratio is "
                << std::to_string(packedBitSize / unpackedBitSize * 100) << "%!";
    }

    static void checkWriteRead(size_t numElements)
    {
        PackedVariableArray data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    static void checkWriteReadFile(size_t numElements)
    {
        PackedVariableArray data;
        fillData(data, numElements);

        test_utils::writeReadFileTest(
                std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob", data);
    }

    static constexpr std::string_view BLOB_NAME_BASE =
            "language/array_types/packed_variable_array_struct_recursion_";

    static constexpr size_t VARIABLE_ARRAY_LENGTH1 = 100;
    static constexpr size_t VARIABLE_ARRAY_LENGTH2 = 500;
    static constexpr size_t VARIABLE_ARRAY_LENGTH3 = 1000;
};

TEST_F(PackedVariableArrayStructRecursionTest, bitSizeOfLength1)
{
    checkBitSizeOf(VARIABLE_ARRAY_LENGTH1);
}

TEST_F(PackedVariableArrayStructRecursionTest, bitSizeOfLength2)
{
    checkBitSizeOf(VARIABLE_ARRAY_LENGTH2);
}

TEST_F(PackedVariableArrayStructRecursionTest, bitSizeOfLength3)
{
    checkBitSizeOf(VARIABLE_ARRAY_LENGTH3);
}

TEST_F(PackedVariableArrayStructRecursionTest, writeReadLength1)
{
    checkWriteRead(VARIABLE_ARRAY_LENGTH1);
}

TEST_F(PackedVariableArrayStructRecursionTest, writeReadLength2)
{
    checkWriteRead(VARIABLE_ARRAY_LENGTH2);
}

TEST_F(PackedVariableArrayStructRecursionTest, writeReadLength3)
{
    checkWriteRead(VARIABLE_ARRAY_LENGTH3);
}

TEST_F(PackedVariableArrayStructRecursionTest, writeReadFileLength1)
{
    checkWriteReadFile(VARIABLE_ARRAY_LENGTH1);
}

TEST_F(PackedVariableArrayStructRecursionTest, writeReadFileLength2)
{
    checkWriteReadFile(VARIABLE_ARRAY_LENGTH2);
}

TEST_F(PackedVariableArrayStructRecursionTest, writeReadFileLength3)
{
    checkWriteReadFile(VARIABLE_ARRAY_LENGTH3);
}

} // namespace packed_variable_array_struct_recursion
} // namespace array_types
