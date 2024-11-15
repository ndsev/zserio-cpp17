#include "array_types/packed_fixed_array_uint8/PackedFixedArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/SerializeUtil.h"

namespace array_types
{
namespace packed_fixed_array_uint8
{

class PackedFixedArrayUInt8Test : public ::testing::Test
{
protected:
    void fillData(PackedFixedArray& data, size_t length = FIXED_ARRAY_LENGTH)
    {
        auto& uint8Array = data.uint8Array;

        uint8Array.reserve(length);
        for (size_t i = 0; i < length; ++i)
        {
            uint8Array.push_back(PACKED_ARRAY_ELEMENT);
        }
    }

    zserio::BitSize getBitSize()
    {
        zserio::BitSize bitSize = 1; // packing descriptor: isPacked
        bitSize += 6; // packing descriptor: maxBitNumber
        bitSize += 8; // firstElement

        return bitSize;
    }

    void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeBool(true);
        writer.writeUnsignedBits32(PACKED_ARRAY_MAX_BIT_NUMBER, 6);
        writer.writeUnsignedBits32(PACKED_ARRAY_ELEMENT, 8);
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/packed_fixed_array_uint8.blob";

    static constexpr size_t FIXED_ARRAY_LENGTH = 5;
    static constexpr uint8_t PACKED_ARRAY_MAX_BIT_NUMBER = 0;
    static constexpr uint8_t PACKED_ARRAY_ELEMENT = 100;
};

TEST_F(PackedFixedArrayUInt8Test, bitSizeOf)
{
    PackedFixedArray data;
    fillData(data);
    const zserio::View<PackedFixedArray> view(data);

    const size_t bitPosition = 2;
    const zserio::BitSize fixedArrayBitSize = getBitSize();
    ASSERT_EQ(fixedArrayBitSize, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(PackedFixedArrayUInt8Test, read)
{
    PackedFixedArray data;
    fillData(data);
    const zserio::View<PackedFixedArray> view(data);

    const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
    zserio::BitBuffer bitBuffer(bitSize);
    zserio::BitStreamWriter writer(bitBuffer);
    writeData(writer);

    test_utils::readTest(writer, data);
}

TEST_F(PackedFixedArrayUInt8Test, writeRead)
{
    PackedFixedArray data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(PackedFixedArrayUInt8Test, writeReadFile)
{
    PackedFixedArray data;
    fillData(data);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(PackedFixedArrayUInt8Test, writeWrongArray)
{
    PackedFixedArray data;
    fillData(data, FIXED_ARRAY_LENGTH + 1);

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);
}

} // namespace packed_fixed_array_uint8
} // namespace array_types
