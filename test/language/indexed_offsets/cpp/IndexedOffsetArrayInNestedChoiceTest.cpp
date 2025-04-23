#include "gtest/gtest.h"
#include "indexed_offsets/indexed_offset_array_in_nested_choice/TestStruct.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace indexed_offset_array_in_nested_choice
{

class IndexedOffsetArrayInNestedChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(0);
        writer.writeVarSize(SIZE);
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            writer.writeUnsignedBits32(FIRST_OFFSET + i * 4);
        }
        writer.writeVarSize(SIZE);
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            writer.writeUnsignedBits32(i);
        }
    }

    static void fillData(TestStruct& data, bool setOffsets = false)
    {
        data.nameLength = 0;
        auto& offsetArray =
                data.choiceField.emplace<TestChoice::Tag::nestedChoice>()
                        .emplace<NestedChoice::Tag::offsetArray>();
        offsetArray.resize(SIZE);
        data.u32Array.resize(SIZE);
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            offsetArray[i] = setOffsets ? FIRST_OFFSET + i * 4 : 0;
            data.u32Array[i] = i;
        }
    }

    static void checkOffsets(const zserio::View<TestStruct>& view)
    {
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            ASSERT_EQ(FIRST_OFFSET + i * 4, view.choiceField().nestedChoice().offsetArray()[i]);
        }
    }

private:
    static constexpr uint32_t SIZE = 5;
    static constexpr uint32_t FIRST_OFFSET = 4 /* nameLength */ + 1 /* varsize */ + SIZE * 4 + 1 /* varsize */;
};

TEST_F(IndexedOffsetArrayInNestedChoiceTest, read)
{
    TestStruct data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(IndexedOffsetArrayInNestedChoiceTest, writeRead)
{
    TestStruct data;
    fillData(data);
    zserio::View view(data);

    for (auto offset : view.choiceField().nestedChoice().offsetArray())
    {
        EXPECT_EQ(0, offset);
    }

    test_utils::writeReadTest(data);
    checkOffsets(view);
}

} // namespace indexed_offset_array_in_nested_choice
} // namespace indexed_offsets
