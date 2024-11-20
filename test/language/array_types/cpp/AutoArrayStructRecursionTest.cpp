#include <utility>

#include "array_types/auto_array_struct_recursion/AutoArrayRecursion.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace array_types
{
namespace auto_array_struct_recursion
{

class AutoArrayStructRecursionTest : public ::testing::Test
{
protected:
    static void fillData(AutoArrayRecursion& data, size_t numElements)
    {
        data.id = 0;

        auto& autoArray = data.autoArrayRecursion;
        for (size_t i = 1; i <= numElements; ++i)
        {
            AutoArrayRecursion element;
            element.id = static_cast<uint8_t>(i);
            autoArray.push_back(element);
        }
    }

    static zserio::BitSize getBitSize(size_t numElements)
    {
        return 8 + 8 + static_cast<zserio::BitSize>(numElements) * (8 + 8);
    }

    static void writeData(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeUnsignedBits32(0, 8);
        writer.writeVarSize(static_cast<uint32_t>(numElements));
        for (size_t i = 1; i <= numElements; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 8);
            writer.writeVarSize(0);
        }
    }

    static void checkBitSizeOf(size_t numElements)
    {
        AutoArrayRecursion autoArrayRecursion;
        fillData(autoArrayRecursion, numElements);
        const zserio::View<AutoArrayRecursion> view(autoArrayRecursion);

        const size_t bitPosition = 2;
        const zserio::BitSize autoArrayBitSize = getBitSize(numElements);
        ASSERT_EQ(autoArrayBitSize, zserio::detail::bitSizeOf(view, bitPosition));
    }

    static void checkRead(size_t numElements)
    {
        AutoArrayRecursion data;
        fillData(data, numElements);

        test_utils::readTest(std::bind(writeData, std::placeholders::_1, numElements), data);
    }

    static void checkWriteRead(size_t numElements)
    {
        AutoArrayRecursion data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    static void checkWriteReadFile(size_t numElements)
    {
        AutoArrayRecursion data;
        fillData(data, numElements);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/array_types/auto_array_struct_recursion_";
    static constexpr size_t AUTO_ARRAY_LENGTH1 = 5;
    static constexpr size_t AUTO_ARRAY_LENGTH2 = 10;
};

TEST_F(AutoArrayStructRecursionTest, bitSizeOfLength1)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayStructRecursionTest, bitSizeOfLength2)
{
    checkBitSizeOf(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArrayStructRecursionTest, readLength1)
{
    checkRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayStructRecursionTest, readLength2)
{
    checkRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArrayStructRecursionTest, writeReadLength1)
{
    checkWriteRead(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayStructRecursionTest, writeReadLength2)
{
    checkWriteRead(AUTO_ARRAY_LENGTH2);
}

TEST_F(AutoArrayStructRecursionTest, writeReadFileLength1)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayStructRecursionTest, writeReadFileLength2)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH2);
}

} // namespace auto_array_struct_recursion
} // namespace array_types
