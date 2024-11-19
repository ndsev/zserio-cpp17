#include <utility>

#include "array_types/fixed_array_uint8/FixedArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/BitStreamWriter.h"
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
    static void fillData(FixedArray& data, size_t length = FIXED_ARRAY_LENGTH)
    {
        VectorType<zserio::UInt8>& uint8Array = data.uint8Array;
        uint8Array.reserve(FIXED_ARRAY_LENGTH);
        for (size_t i = 0; i < length; ++i)
        {
            uint8Array.push_back(static_cast<uint8_t>(i));
        }
    }

    static void writeData(zserio::BitStreamWriter& writer)
    {
        for (size_t i = 0; i < FIXED_ARRAY_LENGTH; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 8);
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/fixed_array_uint8.blob";
    static constexpr zserio::BitSize FIXED_ARRAY_LENGTH = 5;
};

TEST_F(FixedArrayUInt8Test, bitSizeOf)
{
    FixedArray data;
    fillData(data);
    const zserio::View<FixedArray> view(data);

    const size_t bitPosition = 2;
    ASSERT_EQ(FIXED_ARRAY_LENGTH * 8, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(FixedArrayUInt8Test, read)
{
    FixedArray data;
    fillData(data);

    test_utils::readTest(writeData, data);
}

TEST_F(FixedArrayUInt8Test, writeRead)
{
    FixedArray data;
    fillData(data);

    test_utils::writeReadTest(data);
}

TEST_F(FixedArrayUInt8Test, writeReadFile)
{
    FixedArray data;
    fillData(data);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(FixedArrayUInt8Test, writeWrongArray)
{
    FixedArray data;
    fillData(data, FIXED_ARRAY_LENGTH + 1); // wrong length

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);
}

} // namespace fixed_array_uint8
} // namespace array_types
