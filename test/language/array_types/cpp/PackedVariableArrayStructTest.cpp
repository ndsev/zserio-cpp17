#include "array_types/packed_variable_array_struct/PackedVariableArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace packed_variable_array_struct
{

using allocator_type = PackedVariableArray::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;
using BitBufferType = zserio::BasicBitBuffer<allocator_type>;

class PackedVariableArrayStructTest : public ::testing::Test
{
protected:
    static void fillData(PackedVariableArray& data, uint32_t numElements)
    {
        VectorType<TestStructure> testStructureArray;
        testStructureArray.reserve(numElements);
        for (uint32_t i = 0; i < numElements; ++i)
        {
            testStructureArray.push_back(createTestStructure(i));
        }

        data.numElements = numElements;
        data.testUnpackedArray = TestUnpackedArray{testStructureArray};
        data.testPackedArray = TestPackedArray{testStructureArray};
    }

    static TestStructure createTestStructure(uint32_t index)
    {
        TestStructure testStructure;
        testStructure.id = index;
        testStructure.name = "name" + zserio::toString<allocator_type>(index);
        testStructure.data = BitBufferType{VectorType<uint8_t>{{0xCD, 0xC0}}, 10};
        testStructure.bytesData = VectorType<uint8_t>{{0xCD, 0xC0}};
        testStructure.testChoice = createTestChoice(index);
        testStructure.testUnion = createTestUnion(index);
        testStructure.testEnum = index % 2 == 0 ? TestEnum::DARK_RED : TestEnum::DARK_GREEN;
        testStructure.testBitmask = index % 2 == 0 ? TestBitmask::Values::READ : TestBitmask::Values::CREATE;
        if (index % 2 == 0)
        {
            testStructure.testOptional = static_cast<uint16_t>(index);
        }
        testStructure.testDynamicBitfield = index % 3;
        VectorType<zserio::VarUInt> values;
        for (uint64_t value = 1; value < 18; value += 3)
        {
            values.push_back(value);
        }
        testStructure.numValues = static_cast<uint32_t>(values.size());
        testStructure.unpackedValues = values;
        testStructure.packedValues = values;
        VectorType<Empty> empties(values.size());
        testStructure.packedEmpties = empties;

        return testStructure;
    }

    static TestChoice createTestChoice(uint32_t index)
    {
        TestChoice testChoice;
        if (index == 0 || index == 2 || index == 4)
        {
            testChoice.emplace<TestChoice::Tag::value16>(zserio::UInt16(static_cast<uint16_t>(index)));
        }
        else if (index == 5)
        {
            testChoice.emplace<TestChoice::Tag::array32>(
                    VectorType<zserio::UInt32>{{index * 2, index * 2 + 1}});
        }
        else
        {
            testChoice.emplace<TestChoice::Tag::value32>(Value32{index * 2});
        }

        return testChoice;
    }

    static TestUnion createTestUnion(uint32_t index)
    {
        TestUnion testUnion;
        if (index % 2 == 0)
        {
            testUnion.emplace<TestUnion::Tag::value16>(zserio::UInt16(static_cast<uint16_t>(index)));
        }
        else if (index == 5)
        {
            testUnion.emplace<TestUnion::Tag::array32>(VectorType<zserio::UInt32>{{index * 2, index * 2 + 1}});
        }
        else
        {
            testUnion.emplace<TestUnion::Tag::value32>(Value32{index * 2});
        }

        return testUnion;
    }

    static void checkBitSizeOf(uint32_t numElements)
    {
        PackedVariableArray data;
        fillData(data, numElements);
        const zserio::View view(data);

        const double unpackedBitSize = static_cast<double>(zserio::detail::bitSizeOf(view.testUnpackedArray()));
        const double packedBitSize = static_cast<double>(zserio::detail::bitSizeOf(view.testPackedArray()));
        const double minCompressionRatio = 0.622;
        ASSERT_GT(unpackedBitSize * minCompressionRatio, packedBitSize)
                << "Unpacked array has " << std::to_string(unpackedBitSize) << " bits, packed array has "
                << std::to_string(packedBitSize) << " bits, compression ratio is "
                << std::to_string(packedBitSize / unpackedBitSize * 100) << "%!";
    }

    static void checkWriteRead(uint32_t numElements)
    {
        PackedVariableArray data;
        fillData(data, numElements);

        test_utils::writeReadTest(data);
    }

    static void checkWriteReadFile(uint32_t numElements)
    {
        PackedVariableArray data;
        fillData(data, numElements);

        const std::string fileName = std::string(BLOB_NAME_BASE) + std::to_string(numElements) + ".blob";
        test_utils::writeReadFileTest(fileName, data);
    }

    static constexpr std::string_view BLOB_NAME_BASE = "language/array_types/packed_variable_array_struct_";
    static constexpr uint32_t VARIABLE_ARRAY_LENGTH1 = 25;
    static constexpr uint32_t VARIABLE_ARRAY_LENGTH2 = 50;
    static constexpr uint32_t VARIABLE_ARRAY_LENGTH3 = 100;
    static constexpr uint32_t VARIABLE_ARRAY_LENGTH4 = 1000;
};

TEST_F(PackedVariableArrayStructTest, bitSizeOfLength1)
{
    checkBitSizeOf(VARIABLE_ARRAY_LENGTH1);
}

TEST_F(PackedVariableArrayStructTest, bitSizeOfLength2)
{
    checkBitSizeOf(VARIABLE_ARRAY_LENGTH2);
}

TEST_F(PackedVariableArrayStructTest, bitSizeOfLength3)
{
    checkBitSizeOf(VARIABLE_ARRAY_LENGTH3);
}

TEST_F(PackedVariableArrayStructTest, bitSizeOfLength4)
{
    checkBitSizeOf(VARIABLE_ARRAY_LENGTH4);
}

TEST_F(PackedVariableArrayStructTest, writeReadLength1)
{
    checkWriteRead(VARIABLE_ARRAY_LENGTH1);
}

TEST_F(PackedVariableArrayStructTest, writeReadLength2)
{
    checkWriteRead(VARIABLE_ARRAY_LENGTH2);
}

TEST_F(PackedVariableArrayStructTest, writeReadLength3)
{
    checkWriteRead(VARIABLE_ARRAY_LENGTH3);
}

TEST_F(PackedVariableArrayStructTest, writeReadLength4)
{
    checkWriteRead(VARIABLE_ARRAY_LENGTH4);
}

TEST_F(PackedVariableArrayStructTest, writeReadFileLength1)
{
    checkWriteReadFile(VARIABLE_ARRAY_LENGTH1);
}

TEST_F(PackedVariableArrayStructTest, writeReadFileLength2)
{
    checkWriteReadFile(VARIABLE_ARRAY_LENGTH2);
}

TEST_F(PackedVariableArrayStructTest, writeReadFileLength3)
{
    checkWriteReadFile(VARIABLE_ARRAY_LENGTH3);
}

TEST_F(PackedVariableArrayStructTest, writeReadFileLength4)
{
    checkWriteReadFile(VARIABLE_ARRAY_LENGTH4);
}

} // namespace packed_variable_array_struct
} // namespace array_types
