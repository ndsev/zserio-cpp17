#include <utility>

#include "array_types/auto_array_uint8/AutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/BitStreamWriter.h"

namespace array_types
{
namespace auto_array_uint8
{

using AllocatorType = AutoArray::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, AllocatorType>;

class AutoArrayUInt8Test : public ::testing::Test
{
protected:
    static void fillData(AutoArray& autoArray, size_t numElements)
    {
        auto& uint8Array = autoArray.uint8Array;
        uint8Array.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            uint8Array.push_back(static_cast<uint8_t>(i));
        }
    }

    static void writeData(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeVarSize(static_cast<uint32_t>(numElements));
        for (size_t i = 0; i < numElements; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 8);
        }
    }

    static void checkBitSizeOf(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);
        const zserio::View<AutoArray> view(data);

        const size_t bitPosition = 2;
        const zserio::BitSize autoArrayBitSize = 8 + static_cast<zserio::BitSize>(numElements) * 8;
        ASSERT_EQ(autoArrayBitSize, zserio::detail::bitSizeOf(view, bitPosition));
    }

    static void checkRead(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, numElements), data);
    }

    static void checkWriteRead(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    static void checkWriteReadFile(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/array_types/auto_array_uint8_";
    static constexpr size_t AUTO_ARRAY_LENGTH1 = 5;
    static constexpr size_t AUTO_ARRAY_LENGTH2 = 10;
};

TEST_F(AutoArrayUInt8Test, bitSizeOfLength1)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayUInt8Test, bitSizeOfLength2)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArrayUInt8Test, readLength1)
{
    checkRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayUInt8Test, readLength2)
{
    checkRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArrayUInt8Test, writeReadLength1)
{
    checkWriteRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayUInt8Test, writeReadLength2)
{
    checkWriteRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArrayUInt8Test, writeReadFileLength1)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayUInt8Test, writeReadFileLength2)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH2);
}

} // namespace auto_array_uint8
} // namespace array_types
