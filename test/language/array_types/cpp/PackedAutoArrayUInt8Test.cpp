#include "array_types/packed_auto_array_uint8/PackedAutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace packed_auto_array_uint8
{

class PackedAutoArrayUInt8Test : public ::testing::Test
{
protected:
    void fillData(PackedAutoArray& data, size_t numElements)
    {
        auto& uint8Array = data.uint8Array;
        uint8Array.reserve(numElements);
        uint8_t value = PACKED_ARRAY_ELEMENT0;
        uint8Array.push_back(value);
        value = static_cast<uint8_t>(value + PACKED_ARRAY_DELTA);
        for (size_t i = 0; i < numElements - 1; ++i)
        {
            value = static_cast<uint8_t>(value + PACKED_ARRAY_DELTA);
            uint8Array.push_back(value);
        }
    }

    zserio::BitSize getBitSize(size_t numElements)
    {
        zserio::BitSize bitSize = 8; // auto array size: varsize
        bitSize += 1; // packing descriptor: isPacked
        if (numElements > 1)
        {
            bitSize += 6; // packing descriptor: maxBitNumber
        }
        bitSize += 8; // first element
        // all deltas
        bitSize += (static_cast<zserio::BitSize>(numElements) - 1) * (PACKED_ARRAY_MAX_BIT_NUMBER + 1U);

        return bitSize;
    }

    void writeData(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeVarSize(static_cast<uint32_t>(numElements));
        const bool isPacked = numElements > 1;
        writer.writeBool(isPacked);
        if (isPacked)
        {
            writer.writeUnsignedBits32(PACKED_ARRAY_MAX_BIT_NUMBER, 6);
        }
        writer.writeUnsignedBits32(PACKED_ARRAY_ELEMENT0, 8);
        if (numElements > 1)
        {
            writer.writeSignedBits32(
                    PACKED_ARRAY_DELTA * 2, static_cast<uint8_t>(PACKED_ARRAY_MAX_BIT_NUMBER + 1));
            for (size_t i = 0; i < numElements - 2; ++i)
            {
                writer.writeSignedBits32(
                        PACKED_ARRAY_DELTA, static_cast<uint8_t>(PACKED_ARRAY_MAX_BIT_NUMBER + 1));
            }
        }
    }

    void checkBitSizeOf(size_t numElements)
    {
        PackedAutoArray data;
        fillData(data, numElements);
        const zserio::View<PackedAutoArray> view(data);

        const size_t bitPosition = 2;
        const zserio::BitSize autoArrayBitSize = getBitSize(numElements);
        ASSERT_EQ(autoArrayBitSize, zserio::detail::bitSizeOf(view, bitPosition));
    }

    void checkRead(size_t numElements)
    {
        PackedAutoArray data;
        fillData(data, numElements);
        const zserio::View<PackedAutoArray> view(data);

        const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
        zserio::BitBuffer bitBuffer(bitSize);
        zserio::BitStreamWriter writer(bitBuffer);
        writeData(writer, numElements);

        test_utils::readTest(writer, data);
    }

    void checkWriteRead(size_t numElements)
    {
        PackedAutoArray data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    void checkWriteReadFile(size_t numElements)
    {
        PackedAutoArray data;
        fillData(data, numElements);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/array_types/packed_auto_array_uint8_";

    static constexpr size_t AUTO_ARRAY_LENGTH1 = 1;
    static constexpr size_t AUTO_ARRAY_LENGTH2 = 5;
    static constexpr size_t AUTO_ARRAY_LENGTH3 = 10;

    static constexpr uint8_t PACKED_ARRAY_ELEMENT0 = 255;
    static constexpr int16_t PACKED_ARRAY_DELTA = -2;
    static constexpr uint8_t PACKED_ARRAY_MAX_BIT_NUMBER = 3;
};

TEST_F(PackedAutoArrayUInt8Test, bitSizeOfLength1)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayUInt8Test, bitSizeOfLength2)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayUInt8Test, bitSizeOfLength3)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH3);
}

TEST_F(PackedAutoArrayUInt8Test, readLength1)
{
    checkRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayUInt8Test, readLength2)
{
    checkRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayUInt8Test, readLength3)
{
    checkRead(AUTO_ARRAY_LENGTH3);
}

TEST_F(PackedAutoArrayUInt8Test, writeReadLength1)
{
    checkWriteRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayUInt8Test, writeReadLength2)
{
    checkWriteRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayUInt8Test, writeReadLength3)
{
    checkWriteRead(AUTO_ARRAY_LENGTH3);
}

TEST_F(PackedAutoArrayUInt8Test, writeReadFileLength1)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH1);
}

TEST_F(PackedAutoArrayUInt8Test, writeReadFileLength2)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH2);
}

TEST_F(PackedAutoArrayUInt8Test, writeReadFileLength3)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH3);
}

} // namespace packed_auto_array_uint8
} // namespace array_types
