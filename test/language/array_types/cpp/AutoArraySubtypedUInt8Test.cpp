#include "array_types/auto_array_subtyped_uint8/AutoArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/BitStreamWriter.h"

namespace array_types
{
namespace auto_array_subtyped_uint8
{

using AllocatorType = AutoArray::AllocatorType;
template <typename T>
using VectorType = zserio::Vector<T, AllocatorType>;

class AutoArraySubtypedUInt8Test : public ::testing::Test
{
protected:
    void fillData(AutoArray& data, size_t numElements)
    {
        VectorType<ArrayElement> array;
        array.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            array.push_back(static_cast<ArrayElement::ValueType>(i));
        }
        data.array = array;
    }

    void writeData(zserio::BitStreamWriter& writer, size_t length)
    {
        writer.writeVarSize(static_cast<uint32_t>(length));
        for (size_t i = 0; i < length; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 8);
        }
    }

    void checkBitSizeOf(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);
        const zserio::View<AutoArray> view(data);

        const size_t bitPosition = 2;
        const zserio::BitSize subtypedBuiltinAutoArrayBitSize =
                8 + static_cast<zserio::BitSize>(numElements) * 8;
        ASSERT_EQ(subtypedBuiltinAutoArrayBitSize, zserio::detail::bitSizeOf(view, bitPosition));
    }

    void checkRead(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);
        const zserio::View<AutoArray> view(data);

        const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
        zserio::BitBuffer bitBuffer(bitSize);
        zserio::BitStreamWriter writer(bitBuffer);
        writeData(writer, numElements);

        test_utils::readTest(writer, data);
    }

    void checkWriteRead(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    void checkWriteReadFile(size_t numElements)
    {
        AutoArray data;
        fillData(data, numElements);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/array_types/auto_array_subtyped_uint8_";
    static constexpr size_t AUTO_ARRAY_LENGTH1 = 5;
    static constexpr size_t AUTO_ARRAY_LENGTH2 = 10;
};

TEST_F(AutoArraySubtypedUInt8Test, bitSizeOfLength1)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArraySubtypedUInt8Test, bitSizeOfLength2)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArraySubtypedUInt8Test, readLength1)
{
    checkRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArraySubtypedUInt8Test, readLength2)
{
    checkRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArraySubtypedUInt8Test, writeReadLength1)
{
    checkWriteRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArraySubtypedUInt8Test, writeReadLength2)
{
    checkWriteRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArraySubtypedUInt8Test, writeReadFileLength1)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArraySubtypedUInt8Test, writeReadFileLength2)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH2);
}

} // namespace auto_array_subtyped_uint8
} // namespace array_types
