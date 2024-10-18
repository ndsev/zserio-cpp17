#include "array_types/auto_array_struct_recursion/AutoArrayRecursion.h"
#include "gtest/gtest.h"
#include "zserio/SerializeUtil.h"

namespace array_types
{
namespace auto_array_struct_recursion
{

class AutoArrayStructRecursionTest : public ::testing::Test
{
protected:
    void fillAutoArrayRecursion(AutoArrayRecursion& autoArrayRecursion, size_t numElements)
    {
        autoArrayRecursion.id = 0;

        auto& autoArray = autoArrayRecursion.autoArrayRecursion;
        for (size_t i = 1; i <= numElements; ++i)
        {
            AutoArrayRecursion element;
            element.id = static_cast<uint8_t>(i);
            autoArray.push_back(element);
        }
    }

    size_t getAutoArrayRecursionBitSizeOf(size_t numElements)
    {
        return 8 + 8 + numElements * (8 + 8);
    }

    void writeAutoArrayRecursionToByteArray(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeUnsignedBits32(0, 8);
        writer.writeVarSize(static_cast<uint32_t>(numElements));
        for (size_t i = 1; i <= numElements; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 8);
            writer.writeVarSize(0);
        }
    }

    void checkAutoArrayRecursion(const zserio::View<AutoArrayRecursion>& autoArrayRecursion, size_t numElements)
    {
        ASSERT_EQ(0, autoArrayRecursion.id());
        const auto& autoArray = autoArrayRecursion.autoArrayRecursion();
        ASSERT_EQ(numElements, autoArray.size());
        for (size_t i = 1; i <= numElements; ++i)
        {
            const auto& element = autoArray.at(i - 1);
            ASSERT_EQ(i, element.id());
            ASSERT_EQ(0, element.autoArrayRecursion().size());
        }
    }

    void checkBitSizeOf(size_t numElements)
    {
        AutoArrayRecursion autoArrayRecursion;
        fillAutoArrayRecursion(autoArrayRecursion, numElements);

        zserio::View<AutoArrayRecursion> view(autoArrayRecursion);

        const size_t bitPosition = 2;
        const size_t autoArrayBitSize = getAutoArrayRecursionBitSizeOf(numElements);
        ASSERT_EQ(autoArrayBitSize, zserio::detail::bitSizeOf(view, bitPosition));
    }

    void checkRead(size_t numElements)
    {
        zserio::BitStreamWriter writer(bitBuffer);
        writeAutoArrayRecursionToByteArray(writer, numElements);

        zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
        AutoArrayRecursion autoArrayRecursion;
        zserio::View<AutoArrayRecursion> view = zserio::detail::read(reader, autoArrayRecursion);
        checkAutoArrayRecursion(view, numElements);
    }

    void checkWriteRead(size_t numElements)
    {
        AutoArrayRecursion autoArrayRecursion;
        fillAutoArrayRecursion(autoArrayRecursion, numElements);

        zserio::View<AutoArrayRecursion> view(autoArrayRecursion);

        zserio::BitStreamWriter writer(bitBuffer);
        zserio::detail::write(writer, view);

        ASSERT_EQ(zserio::detail::bitSizeOf(view), writer.getBitPosition());

        zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
        AutoArrayRecursion readAutoArrayRecursion;
        zserio::View<AutoArrayRecursion> readView = zserio::detail::read(reader, readAutoArrayRecursion);
        checkAutoArrayRecursion(readView, numElements);
    }
    /*
    void checkWriteReadFile(size_t numElements)
    {
        AutoArrayRecursion autoArrayRecursion;
        fillAutoArrayRecursion(autoArrayRecursion, numElements);

        const std::string fileName = BLOB_NAME_BASE + std::to_string(numElements) + ".blob";
        zserio::serializeToFile(autoArrayRecursion, fileName);

        AutoArrayRecursion readAutoArrayRecursion = zserio::deserializeFromFile<AutoArrayRecursion>(fileName);
        checkAutoArrayRecursion(readAutoArrayRecursion, numElements);
    }
    */
    static constexpr std::string_view BLOB_NAME_BASE = "language/array_types/auto_array_struct_recursion_";
    static constexpr size_t AUTO_ARRAY_LENGTH1 = 5;
    static constexpr size_t AUTO_ARRAY_LENGTH2 = 10;
    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
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
/*
TEST_F(AutoArrayStructRecursionTest, writeReadFileLength1)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH1);
}

TEST_F(AutoArrayStructRecursionTest, writeReadFileLength2)
{
    checkWriteReadFile(AUTO_ARRAY_LENGTH2);
}
*/
} // namespace auto_array_struct_recursion
} // namespace array_types
