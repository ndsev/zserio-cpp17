#include "gtest/gtest.h"
#include "indexed_offsets/indexed_offset_array_in_recursive_union/TestStruct.h"
#include "test_utils/TestUtility.h"

namespace indexed_offsets
{
namespace indexed_offset_array_in_recursive_union
{

class IndexedOffsetArrayInRecursiveUnionTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeVarSize(0); // choiceTag
        writer.writeVarSize(SIZE); // varsize
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            writer.writeVarSize(1); // choiceTag
            writer.writeUnsignedBits32(FIRST_OFFSET + i * 4);
        }
        writer.writeUnsignedBits32(FIELD);
        writer.writeVarSize(5);
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            writer.writeUnsignedBits32(i);
        }
    }

    static void fillData(TestStruct& data, bool setOffsets = false)
    {
        auto& unionArray = data.unionField.emplace<TestUnion::ChoiceTag::CHOICE_unionArray>();
        unionArray.resize(SIZE);
        data.field = FIELD;
        data.array.resize(SIZE);
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            unionArray[i].emplace<TestUnion::ChoiceTag::CHOICE_offset>(setOffsets ? FIRST_OFFSET + i * 4 : 0);
            data.array[i] = i;
        }
    }

    static void checkOffsets(const zserio::View<TestStruct>& view)
    {
        for (uint32_t i = 0; i < SIZE; ++i)
        {
            ASSERT_EQ(FIRST_OFFSET + i * 4, view.unionField().unionArray()[i].offset());
        }
    }

private:
    static constexpr uint32_t SIZE = 5;
    static constexpr uint32_t FIELD = 13;
    static constexpr uint32_t FIRST_OFFSET = 1 /* choiceTag*/ + 1 /* varsize */ +
            SIZE * (1 /* choiceTag */ + 4) + 4 /* field */ + 1 /* varsize */;
};

TEST_F(IndexedOffsetArrayInRecursiveUnionTest, read)
{
    TestStruct data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(IndexedOffsetArrayInRecursiveUnionTest, writeRead)
{
    TestStruct data;
    fillData(data);
    zserio::View view(data);

    for (auto unionOffset : view.unionField().unionArray())
    {
        EXPECT_EQ(0, unionOffset.offset());
    }

    test_utils::writeReadTest(data);
    checkOffsets(view);
}

} // namespace indexed_offset_array_in_recursive_union
} // namespace indexed_offsets
