#include "array_types/variable_array_subtyped_struct/VariableArray.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SerializeUtil.h"
#include "zserio/StringConvertUtil.h"

namespace array_types
{
namespace variable_array_subtyped_struct
{

using AllocatorType = VariableArray::AllocatorType;
using StringType = zserio::BasicString<AllocatorType>;
template <typename T>
using VectorType = zserio::Vector<T, AllocatorType>;

class VariableArraySubtypedStructTest : public ::testing::Test
{
protected:
    void writeVariableArrayToByteArray(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeSignedBits32(static_cast<uint8_t>(numElements), 8);
        for (size_t i = 0; i < numElements; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 32);
            zserio::detail::write(writer, (std::string("Name") + std::to_string(i)));
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/variable_array_subtyped_struct.blob";
    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
};

TEST_F(VariableArraySubtypedStructTest, bitSizeOf)
{
    const size_t numElements = 33;
    VectorType<ArrayElement> compoundArray;
    compoundArray.reserve(numElements);
    for (size_t i = 0; i < numElements; ++i)
    {
        const ArrayElement arrayElement(
                static_cast<uint32_t>(i), StringType("Name") + zserio::toString<AllocatorType>(i));
        compoundArray.push_back(arrayElement);
    }
    VariableArray variableArray(static_cast<uint8_t>(numElements), compoundArray);
    zserio::View<VariableArray> view(variableArray);

    const size_t bitPosition = 2;
    const size_t numOneNumberIndexes = 10; // (numElements > 9) ? 10 : numElements
    const size_t expectedBitSize = (1 + numElements * (4 + 7) - numOneNumberIndexes) * 8;
    ASSERT_EQ(expectedBitSize, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(VariableArraySubtypedStructTest, read)
{
    const size_t numElements = 59;
    zserio::BitStreamWriter writer(bitBuffer);
    writeVariableArrayToByteArray(writer, numElements);
    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    VariableArray variableArray;
    zserio::View<VariableArray> view = zserio::detail::read(reader, variableArray);

    ASSERT_EQ(numElements, static_cast<size_t>(view.numElements()));
    const auto& compoundArray = view.compoundArray();
    ASSERT_EQ(numElements, compoundArray.size());
    for (size_t i = 0; i < numElements; ++i)
    {
        ASSERT_EQ(i, compoundArray.at(i).id());
        ASSERT_EQ(std::string("Name") + std::to_string(i), compoundArray.at(i).name());
    }
}

TEST_F(VariableArraySubtypedStructTest, writeRead)
{
    const size_t numElements = 33;
    VectorType<ArrayElement> compoundArray;
    compoundArray.reserve(numElements);
    for (size_t i = 0; i < numElements; ++i)
    {
        const ArrayElement arrayElement(
                static_cast<uint32_t>(i), StringType("Name") + zserio::toString<AllocatorType>(i));
        compoundArray.push_back(arrayElement);
    }
    VariableArray variableArray(static_cast<uint8_t>(numElements), compoundArray);
    zserio::View<VariableArray> view(variableArray);

    zserio::BitStreamWriter writer(bitBuffer);
    zserio::detail::write(writer, view);

    ASSERT_EQ(zserio::detail::bitSizeOf(view), writer.getBitPosition());

    zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
    VariableArray readVariableArray;
    zserio::View<VariableArray> readView = zserio::detail::read(reader, readVariableArray);
    const auto& readCompoundArray = readView.compoundArray();
    ASSERT_EQ(numElements, readCompoundArray.size());
    for (size_t i = 0; i < numElements; ++i)
    {
        ASSERT_EQ(i, readCompoundArray.at(i).id());
        ASSERT_EQ(std::string("Name") + std::to_string(i), readCompoundArray.at(i).name());
    }
}
/*
TEST_F(VariableArraySubtypedStructTest, writeReadFile)
{
    const size_t numElements = 33;
    VectorType<ArrayElement> compoundArray;
    compoundArray.reserve(numElements);
    for (size_t i = 0; i < numElements; ++i)
    {
        const ArrayElement arrayElement(
                static_cast<uint32_t>(i), StringType("Name") + zserio::toString<AllocatorType>(i));
        compoundArray.push_back(arrayElement);
    }
    VariableArray variableArray;
    variableArray.setNumElements(static_cast<uint8_t>(numElements));
    variableArray.setCompoundArray(compoundArray);

    zserio::serializeToFile(variableArray, BLOB_NAME);

    VariableArray readVariableArray = zserio::deserializeFromFile<VariableArray>(BLOB_NAME);
    const VectorType<ArrayElement>& readCompoundArray = readVariableArray.getCompoundArray();
    ASSERT_EQ(numElements, readCompoundArray.size());
    for (size_t i = 0; i < numElements; ++i)
    {
        ASSERT_EQ(i, readCompoundArray[i].getId());
        ASSERT_EQ(std::string("Name") + std::to_string(i), readCompoundArray[i].getName());
    }
}

TEST_F(VariableArraySubtypedStructTest, writeWrongArray)
{
    const size_t numElements = 33;
    VectorType<ArrayElement> compoundArray;
    compoundArray.reserve(numElements);
    for (size_t i = 0; i < numElements; ++i)
    {
        const ArrayElement arrayElement(
                static_cast<uint32_t>(i), StringType("Name") + zserio::toString<AllocatorType>(i));
        compoundArray.push_back(arrayElement);
    }
    VariableArray variableArray;
    variableArray.setNumElements(static_cast<uint8_t>(numElements + 1));
    variableArray.setCompoundArray(compoundArray);

    zserio::BitStreamWriter writer(bitBuffer);
    ASSERT_THROW(variableArray.write(writer), zserio::CppRuntimeException);
}
*/
} // namespace variable_array_subtyped_struct
} // namespace array_types
