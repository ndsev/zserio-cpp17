#include "array_types/variable_array_ternary_operator/VariableArray.h"
#include "gtest/gtest.h"
#include "zserio/RebindAlloc.h"
#include "zserio/SerializeUtil.h"

namespace array_types
{
namespace variable_array_ternary_operator
{

using allocator_type = VariableArray::allocator_type;
template <typename T>
using vector_type = zserio::vector<T, allocator_type>;

class VariableArrayTernaryOperator : public ::testing::Test
{
protected:
    void fillVariableArray(VariableArray& data, bool isFirstSizeUsed)
    {
        data.isFirstSizeUsed = isFirstSizeUsed;
        const zserio::UInt6 currentSize = (isFirstSizeUsed) ? data.firstSize : data.secondSize;
        vector_type<VariableArrayElement>& array = data.array;
        array.clear();
        const size_t arraySize = static_cast<size_t>(currentSize) * static_cast<size_t>(currentSize);
        for (size_t i = 0; i < arraySize; ++i)
        {
            VariableArrayElement element;
            element.element = i;
            array.push_back(element);
        }
    }

    void checkVariableArray(const zserio::View<VariableArray>& view, bool isFirstSizeUsed)
    {
        ASSERT_EQ(isFirstSizeUsed, view.isFirstSizeUsed());
        const zserio::UInt6 currentSize = (isFirstSizeUsed) ? FIRST_SIZE : SECOND_SIZE;
        const auto& array = view.array();
        const size_t arraySize = static_cast<size_t>(currentSize) * static_cast<size_t>(currentSize);
        for (size_t i = 0; i < arraySize; ++i)
        {
            const auto& element = array[i];
            ASSERT_EQ(currentSize, element.bitSize());
            ASSERT_EQ(i, element.element().value());
        }
    }

    void testWriteReadFile(bool isFirstSizeUsed)
    {
        VariableArray data;
        fillVariableArray(data, isFirstSizeUsed);
        zserio::View<VariableArray> view(data);

        const std::string blobName = std::string((isFirstSizeUsed) ? BLOB_NAME_FIRST : BLOB_NAME_SECOND);
        zserio::serializeToFile(view, blobName);

        VariableArray readData;
        auto readView = zserio::deserializeFromFile(blobName, readData);
        checkVariableArray(readView, isFirstSizeUsed);
        ASSERT_EQ(data, readData);
        ASSERT_EQ(view, readView);
    }

    static constexpr uint8_t FIRST_SIZE = 10;
    static constexpr uint8_t SECOND_SIZE = 20;

    static constexpr std::string_view BLOB_NAME_FIRST =
            "language/array_types/variable_array_ternary_operator1.blob";
    static constexpr std::string_view BLOB_NAME_SECOND =
            "language/array_types/variable_array_ternary_operator1.blob";

    zserio::BitBuffer bitBuffer = zserio::BitBuffer(1024 * 8);
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
