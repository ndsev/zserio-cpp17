#include "array_types/fixed_array_uint8/FixedArray.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SerializeUtil.h"

namespace array_types
{
namespace fixed_array_uint8
{

using AllocatorType = FixedArray::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, AllocatorType>;

class FixedArrayUInt8Test : public ::testing::Test
{
protected:
    void writeFixedArrayToByteArray(zserio::BitStreamWriter& writer)
    {
        for (size_t i = 0; i < FIXED_ARRAY_LENGTH; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 8);
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/fixed_array_uint8.blob";
    static constexpr size_t FIXED_ARRAY_LENGTH = 5;
    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(FixedArrayUInt8Test, bitSizeOf)
{
    VectorType<zserio::UInt8> uint8Array;
    uint8Array.reserve(FIXED_ARRAY_LENGTH);
    for (size_t i = 0; i < FIXED_ARRAY_LENGTH; ++i)
    {
        uint8Array.push_back(static_cast<uint8_t>(i));
    }
    FixedArray fixedArray(uint8Array);
    zserio::View<FixedArray> view(fixedArray);

    const size_t bitPosition = 2;
    ASSERT_EQ(FIXED_ARRAY_LENGTH * 8, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(FixedArrayUInt8Test, read)
{
    zserio::BitStreamWriter writer(bitBuffer);
    writeFixedArrayToByteArray(writer);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    FixedArray fixedArray;
    zserio::View<FixedArray> view = zserio::detail::read(reader, fixedArray);

    const auto& uint8Array = view.uint8Array();
    const size_t numElements = FIXED_ARRAY_LENGTH;
    ASSERT_EQ(numElements, uint8Array.size());
    for (size_t i = 0; i < numElements; ++i)
    {
        ASSERT_EQ(i, uint8Array.at(i));
    }
}

TEST_F(FixedArrayUInt8Test, writeRead)
{
    VectorType<zserio::UInt8> uint8Array;
    uint8Array.reserve(FIXED_ARRAY_LENGTH);
    for (size_t i = 0; i < FIXED_ARRAY_LENGTH; ++i)
    {
        uint8Array.push_back(static_cast<uint8_t>(i));
    }
    FixedArray fixedArray(uint8Array);
    zserio::View<FixedArray> view(fixedArray);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    FixedArray readFixedArray;
    zserio::View<FixedArray> readView = zserio::detail::read(reader, readFixedArray);
    const auto& readUint8Array = readView.uint8Array();
    const size_t numElements = FIXED_ARRAY_LENGTH;
    ASSERT_EQ(numElements, readUint8Array.size());
    for (size_t i = 0; i < numElements; ++i)
    {
        ASSERT_EQ(i, readUint8Array.at(i));
    }
}
/*
TEST_F(FixedArrayUInt8Test, writeReadFile)
{
    VectorType<uint8_t> uint8Array;
    uint8Array.reserve(FIXED_ARRAY_LENGTH);
    for (size_t i = 0; i < FIXED_ARRAY_LENGTH; ++i)
    {
        uint8Array.push_back(static_cast<uint8_t>(i));
    }
    FixedArray fixedArray;
    fixedArray.setUint8Array(uint8Array);

    zserio::serializeToFile(fixedArray, BLOB_NAME);

    FixedArray readFixedArray = zserio::deserializeFromFile<FixedArray>(BLOB_NAME);
    const VectorType<uint8_t>& readUint8Array = readFixedArray.getUint8Array();
    const size_t numElements = FIXED_ARRAY_LENGTH;
    ASSERT_EQ(numElements, readUint8Array.size());
    for (size_t i = 0; i < numElements; ++i)
    {
        ASSERT_EQ(i, readUint8Array[i]);
    }
}

TEST_F(FixedArrayUInt8Test, writeWrongArray)
{
    VectorType<uint8_t> uint8Array;
    const size_t wrongArrayLength = FIXED_ARRAY_LENGTH + 1;
    uint8Array.reserve(wrongArrayLength);
    for (size_t i = 0; i < wrongArrayLength; ++i)
    {
        uint8Array.push_back(static_cast<uint8_t>(i));
    }
    FixedArray fixedArray;
    fixedArray.setUint8Array(uint8Array);

    zserio::BitStreamWriter writer(bitBuffer);
    ASSERT_THROW(fixedArray.write(writer), zserio::CppRuntimeException);
}
*/
} // namespace fixed_array_uint8
} // namespace array_types
