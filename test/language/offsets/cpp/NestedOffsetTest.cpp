#include "gtest/gtest.h"
#include "offsets/nested_offset/NestedOffset.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace nested_offset
{

class NestedOffsetTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(TERMINATOR_OFFSET, 32);
        writer.writeBool(BOOL_VALUE);
        // union's choice tag
        writer.writeVarSize(static_cast<uint32_t>(NestedOffsetUnion::Tag::nestedOffsetArrayStructure - 1));
        writer.writeUnsignedBits32(NUM_ELEMENTS, 8);
        for (uint32_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            writer.writeUnsignedBits32(FIRST_DATA_OFFSET + i * 8, 32);
            writer.writeUnsignedBits32(0, (i == 0) ? 7 : 1);
            writer.writeUnsignedBits32(i, 31);
        }

        writer.alignTo(8);
        writer.writeUnsignedBits32(TERMINATOR_VALUE, 7);
    }

    static void checkOffsets(const zserio::View<NestedOffset>& view)
    {
        ASSERT_EQ(TERMINATOR_OFFSET, view.terminatorOffset());
        ASSERT_EQ(BOOL_VALUE, view.boolValue());

        const auto& nestedOffsetChoice = view.nestedOffsetChoice();
        ASSERT_EQ(BOOL_VALUE, nestedOffsetChoice.type());

        const auto& nestedOffsetUnion = nestedOffsetChoice.nestedOffsetUnion();
        ASSERT_EQ(NestedOffsetUnion::Tag::nestedOffsetArrayStructure, nestedOffsetUnion.zserioChoiceTag());

        const auto& nestedOffsetArrayStructure = nestedOffsetUnion.nestedOffsetArrayStructure();
        ASSERT_EQ(NUM_ELEMENTS, nestedOffsetArrayStructure.numElements());

        const auto& nestedOffsetStructureList = nestedOffsetArrayStructure.nestedOffsetStructureList();
        ASSERT_EQ(NUM_ELEMENTS, nestedOffsetStructureList.size());
        for (uint32_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            const auto& nestedOffsetStructure = nestedOffsetStructureList[i];
            ASSERT_EQ(FIRST_DATA_OFFSET + i * 8, nestedOffsetStructure.dataOffset());
            ASSERT_EQ(i, nestedOffsetStructure.data());
        }

        ASSERT_EQ(TERMINATOR_VALUE, view.terminator());
    }

    static void fillData(NestedOffset& data, bool setOffsets = false)
    {
        if (setOffsets)
        {
            data.terminatorOffset = TERMINATOR_OFFSET;
        }
        data.boolValue = BOOL_VALUE;
        NestedOffsetChoice& nestedOffsetChoice = data.nestedOffsetChoice;
        fillNestedOffsetChoice(nestedOffsetChoice, setOffsets);
        data.terminator = TERMINATOR_VALUE;
    }

    static void fillNestedOffsetChoice(NestedOffsetChoice& nestedOffsetChoice, bool setOffsets)
    {
        NestedOffsetUnion nestedOffsetUnion;
        NestedOffsetArrayStructure nestedOffsetArrayStructure;
        nestedOffsetArrayStructure.numElements = NUM_ELEMENTS;
        auto& nestedOffsetStructureList = nestedOffsetArrayStructure.nestedOffsetStructureList;
        nestedOffsetStructureList.reserve(NUM_ELEMENTS);
        for (uint32_t i = 0; i < NUM_ELEMENTS; ++i)
        {
            NestedOffsetStructure nestedOffsetStructure;
            if (setOffsets)
            {
                nestedOffsetStructure.dataOffset = FIRST_DATA_OFFSET + i * 8;
            }
            nestedOffsetStructure.data = i;
            nestedOffsetStructureList.push_back(nestedOffsetStructure);
        }
        nestedOffsetUnion.emplace<NestedOffsetUnion::Tag::nestedOffsetArrayStructure>(
                nestedOffsetArrayStructure);
        nestedOffsetChoice.emplace<NestedOffsetChoice::Tag::nestedOffsetUnion>(nestedOffsetUnion);
    }

    static constexpr bool BOOL_VALUE = true;
    static constexpr uint8_t NUM_ELEMENTS = 2;

    static constexpr uint32_t TERMINATOR_OFFSET = 7 + NUM_ELEMENTS * 8;

    static constexpr uint32_t FIRST_DATA_OFFSET = 7 + 4;

    static constexpr uint8_t TERMINATOR_VALUE = 0x45;

    static constexpr size_t NESTED_OFFSET_BIT_SIZE = TERMINATOR_OFFSET * 8 + 7;
};

TEST_F(NestedOffsetTest, read)
{
    NestedOffset data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(NestedOffsetTest, writeRead)
{
    NestedOffset data;
    fillData(data);
    zserio::View view(data);

    test_utils::writeReadTest(data);
    checkOffsets(view);
}

} // namespace nested_offset
} // namespace offsets
