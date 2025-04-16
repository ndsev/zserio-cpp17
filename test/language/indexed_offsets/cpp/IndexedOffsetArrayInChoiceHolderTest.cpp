#include "gtest/gtest.h"
#include "indexed_offsets/indexed_offset_array_in_choice_holder/TestStruct.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace indexed_offset_array_in_choice_holder
{

class IndexedOffsetArrayInChoiceHolderTest : public ::testing::Test
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
        auto& choiceArray = data.choiceArray;
        choiceArray.resize(SIZE);
        data.u32Array.resize(SIZE);
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            choiceArray[i].emplace<TestChoice::ChoiceTag::CHOICE_offset>(setOffsets ? FIRST_OFFSET + i * 4 : 0);
            data.u32Array[i] = i;
        }
    }

    static void checkOffsets(const zserio::View<TestStruct>& view)
    {
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            ASSERT_EQ(FIRST_OFFSET + i * 4, view.choiceArray()[i].offset());
        }
    }

private:
    static constexpr uint32_t SIZE = 5;
    static constexpr uint32_t FIRST_OFFSET = 4 /* nameLength */ + 1 /* varsize */ + SIZE * 4 + 1 /* varsize */;
};

TEST_F(IndexedOffsetArrayInChoiceHolderTest, read)
{
    TestStruct data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(IndexedOffsetArrayInChoiceHolderTest, writeRead)
{
    TestStruct data;
    fillData(data);
    zserio::View view(data);

    for (auto choiceOffset : view.choiceArray())
    {
        EXPECT_EQ(0, choiceOffset.offset());
    }

    test_utils::writeReadTest(data);
    checkOffsets(view);
}

} // namespace indexed_offset_array_in_choice_holder
} // namespace indexed_offsets
