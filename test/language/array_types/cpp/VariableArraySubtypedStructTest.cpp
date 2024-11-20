#include <utility>

#include "array_types/variable_array_subtyped_struct/VariableArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace variable_array_subtyped_struct
{

using AllocatorType = VariableArray::AllocatorType;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class VariableArraySubtypedStructTest : public ::testing::Test
{
protected:
    static VariableArray createData(size_t numElements, bool wrong = false)
    {
        VectorType<ArrayElement> compoundArray;
        compoundArray.reserve(numElements);
        for (size_t i = 0; i < numElements; ++i)
        {
            const ArrayElement arrayElement(
                    static_cast<uint32_t>(i), StringType("Name") + zserio::toString<AllocatorType>(i));
            compoundArray.push_back(arrayElement);
        }
        return VariableArray(static_cast<uint8_t>(wrong ? numElements + 1 : numElements), compoundArray);
    }

    static void writeData(zserio::BitStreamWriter& writer, size_t numElements)
    {
        writer.writeSignedBits32(static_cast<uint8_t>(numElements), 8);
        for (size_t i = 0; i < numElements; ++i)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(i), 32);
            zserio::detail::write(writer, (std::string("Name") + std::to_string(i)));
        }
    }

    static constexpr std::string_view BLOB_NAME = "language/array_types/variable_array_subtyped_struct.blob";
};

TEST_F(VariableArraySubtypedStructTest, bitSizeOf)
{
    const size_t numElements = 33;
    auto data = createData(numElements);
    const zserio::View<VariableArray> view(data);

    const size_t bitPosition = 2;
    const size_t numOneNumberIndexes = 10; // (numElements > 9) ? 10 : numElements
    const zserio::BitSize expectedBitSize = (1 + numElements * (4 + 7) - numOneNumberIndexes) * 8;
    ASSERT_EQ(expectedBitSize, zserio::detail::bitSizeOf(view, bitPosition));
}

TEST_F(VariableArraySubtypedStructTest, read)
{
    const size_t numElements = 59;
    auto data = createData(numElements);

    test_utils::readTest(std::bind(writeData, std::placeholders::_1, numElements), data);
}

TEST_F(VariableArraySubtypedStructTest, writeRead)
{
    const size_t numElements = 33;
    auto data = createData(numElements);

    test_utils::writeReadTest(data);
}

TEST_F(VariableArraySubtypedStructTest, writeReadFile)
{
    const size_t numElements = 33;
    auto data = createData(numElements);

    test_utils::writeReadFileTest(BLOB_NAME, data);
}

TEST_F(VariableArraySubtypedStructTest, writeWrongArray)
{
    const size_t numElements = 33;
    auto data = createData(numElements, true);

    ASSERT_THROW(zserio::serialize(data), zserio::CppRuntimeException);
}

} // namespace variable_array_subtyped_struct
} // namespace array_types
