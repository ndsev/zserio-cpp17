#include <utility>

#include "array_types/variable_array_struct_cast_int8/VariableArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace variable_array_struct_cast_int8
{

using AllocatorType = VariableArray::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class VariableArrayStructCastInt8Test : public ::testing::Test
{
protected:
    static VariableArray createData(size_t numElements, bool wrong = false)
    {
        VectorType<TestStructure> compoundArray;
        compoundArray.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            TestStructure testStructure;
            testStructure.id = static_cast<uint32_t>(i);
            testStructure.name = StringType("Name") + zserio::toString<AllocatorType>(i);
            compoundArray.push_back(testStructure);
        }
        VariableArray variableArray;
        variableArray.numElements = static_cast<int8_t>(wrong ? numElements + 1 : numElements);
        variableArray.compoundArray = compoundArray;
        return variableArray;
    }

    static void writeData(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeSignedBits32(static_cast<int8_t>(numElements), 8);
        for (size_t i = 0; i < numElements; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 32);
            writer.writeString(std::string("Name") + std::to_string(i));
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/variable_array_struct_cast_int8.blob";
};

TEST_F(VariableArrayStructCastInt8Test, bitSizeOf)
{
    const size_t numElements = 33;
    auto data = createData(numElements);
    const zserio::View<VariableArray> view(data);

    const size_t bitPosition = 2;
    const size_t numOneNumberIndexes = 10; // (numElements > 9) ? 10 : numElements
    const zserio::BitSize expectedBitSize = (1 + numElements * (4 + 7) - numOneNumberIndexes) * 8;
    ASSERT_EQ(expectedBitSize, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(VariableArrayStructCastInt8Test, read)
{
    const size_t numElements = 59;
    auto data = createData(numElements);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, numElements), data);
}

TEST_F(VariableArrayStructCastInt8Test, writeRead)
{
    const size_t numElements = 33;
    auto data = createData(numElements);

    test_utils::writeReadTest(data);
}

TEST_F(VariableArrayStructCastInt8Test, writeReadFile)
{
    const size_t numElements = 33;
    auto data = createData(numElements);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(VariableArrayStructCastInt8Test, writeWrongArray)
{
    const size_t numElements = 33;
    auto data = createData(numElements, true);

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);
}

} // namespace variable_array_struct_cast_int8
} // namespace array_types