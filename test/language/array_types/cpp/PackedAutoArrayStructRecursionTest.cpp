#include <utility>

#include "array_types/packed_auto_array_struct_recursion/PackedAutoArrayRecursion.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace packed_auto_array_struct_recursion
{

class PackedAutoArrayStructRecursionTest : public ::testing::Test
{
protected:
    static void fillData(PackedAutoArrayRecursion& data, size_t numElements)
    {
        data.id = 0;

        auto& autoArray = data.packedAutoArrayRecursion;
        for (size_t i = 1; i <= numElements; ++i)
        {
            PackedAutoArrayRecursion element;
            element.id = static_cast<uint8_t>(i);
            autoArray.push_back(element);
        }
    }

    static zserio::BitSize getBitSize(size_t numElements)
    {
        zserio::BitSize bitSize = 8; // id
        bitSize += 8; // varsize (length of auto array)
        bitSize += 1; // packing descriptor: isPacked
        if (numElements > 1)
        {
            bitSize += 6; // packing descriptor: maxBitNumber
        }
        bitSize += 8 + 8; // first element
        bitSize += (static_cast<zserio::BitSize>(numElements) - 1) * (8 + 2); // all deltas

        return bitSize;
    }

    static void writeData(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeUnsignedBits32(0, 8);
        writer.writeVarSize(static_cast<uint32_t>(numElements));
        const bool isPacked = numElements > 1;
        writer.writeBool(isPacked);
        const uint8_t maxBitNumber = 1;
        if (isPacked)
        {
            writer.writeUnsignedBits32(maxBitNumber, 6);
        }
        writer.writeUnsignedBits32(1, 8);
        writer.writeVarSize(0);
        for (size_t i = 1; i < numElements; ++i)
        {
            writer.writeSignedBits32(1, maxBitNumber + 1);
            writer.writeVarSize(0);
        }
    }

    static void checkBitSizeOf(size_t numElements)
    {
        PackedAutoArrayRecursion data;
        fillData(data, numElements);
        const zserio::View view(data);

        const size_t bitPosition = 2;
        const zserio::BitSize autoArrayBitSize = getBitSize(numElements);
        ASSERT_EQ(autoArrayBitSize, zserio::detail::bitSizeOf(view, bitPosition));
    }

    static void checkRead(size_t numElements)
    {
        PackedAutoArrayRecursion data;
        fillData(data, numElements);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, numElements), data);
    }

    static void checkWriteRead(size_t numElements)
    {
        PackedAutoArrayRecursion data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    static void checkWriteReadFile(size_t numElements)
    {
        PackedAutoArrayRecursion data;
        fillData(data, numElements);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE =
            "language/array_types/packed_auto_array_struct_recursion_";
    static constexpr size_t AUTO_ARRAY_LENGTH1 = 1;
    static constexpr size_t AUTO_ARRAY_LENGTH2 = 5;
    static constexpr size_t AUTO_ARRAY_LENGTH3 = 10;
};

TEST_F(PackedAutoArrayStructRecursionTest, bitSizeOfLength1)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayStructRecursionTest, bitSizeOfLength2)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayStructRecursionTest, bitSizeOfLength3)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayStructRecursionTest, readLength1)
{
    checkRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayStructRecursionTest, readLength2)
{
    checkRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayStructRecursionTest, readLength3)
{
    checkRead(AUTO_ARRAY_LENGTH3);
}

TEST_F(PackedAutoArrayStructRecursionTest, writeReadLength1)
{
    checkWriteRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayStructRecursionTest, writeReadLength2)
{
    checkWriteRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayStructRecursionTest, writeReadLength3)
{
    checkWriteRead(AUTO_ARRAY_LENGTH3);
}

TEST_F(PackedAutoArrayStructRecursionTest, writeReadFileLength1)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayStructRecursionTest, writeReadFileLength2)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayStructRecursionTest, writeReadFileLength3)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH3);
}

} // namespace packed_auto_array_struct_recursion
} // namespace array_types
