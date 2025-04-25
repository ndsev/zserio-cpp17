#include "array_types/variable_array_ternary_operator/VariableArray.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace array_types
{
namespace variable_array_ternary_operator
{

using AllocatorType = VariableArray::allocator_type;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

class VariableArrayTernaryOperator : public ::testing::Test
{
protected:
    static void fillData(VariableArray& data, bool isFirstSizeUsed)
    {
        data.isFirstSizeUsed = isFirstSizeUsed;
        const zserio::UInt6 currentSize = (isFirstSizeUsed) ? data.firstSize : data.secondSize;
        VectorType<VariableArrayElement>& array = data.array;
        array.clear();
        const size_t arraySize = static_cast<size_t>(currentSize) * static_cast<size_t>(currentSize);
        for (size_t i = 0; i < arraySize; ++i)
        {
            VariableArrayElement element;
            element.element = i;
            array.push_back(element);
        }
    }

    static void testWriteReadFile(bool isFirstSizeUsed)
    {
        VariableArray data;
        fillData(data, isFirstSizeUsed);

        const std::string_view blobName = (isFirstSizeUsed) ? BLOB_NAME_FIRST : BLOB_NAME_SECOND;
        test_utils::writeReadFileTest(blobName, data);
    }

    static constexpr uint8_t FIRST_SIZE = 10;
    static constexpr uint8_t SECOND_SIZE = 20;

    static constexpr std::string_view BLOB_NAME_FIRST =
            "language/array_types/variable_array_ternary_operator1.blob";
    static constexpr std::string_view BLOB_NAME_SECOND =
            "language/array_types/variable_array_ternary_operator2.blob";
};

TEST_F(VariableArrayTernaryOperator, firstWriteReadFile)
{
    const bool isFirstOffsetUsed = true;
    testWriteReadFile(isFirstOffsetUsed);
}

TEST_F(VariableArrayTernaryOperator, secondWriteReadFile)
{
    const bool isFirstOffsetUsed = false;
    testWriteReadFile(isFirstOffsetUsed);
}

} // namespace variable_array_ternary_operator
} // namespace array_types
