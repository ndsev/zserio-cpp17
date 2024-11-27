#include <array>
#include <utility>

#include "functions/structure_array_param/ChildStructure.h"
#include "functions/structure_array_param/ParentStructure.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace structure_array_param
{

class StructureArrayParamTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(static_cast<uint32_t>(VALUES.size()), 8);

        for (uint64_t value : VALUES)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(value), CHILD_BIT_SIZE);
        }

        writer.writeUnsignedBits32(static_cast<uint32_t>(ANOTHER_VALUES.size()), 8);

        for (uint64_t anotherValue : ANOTHER_VALUES)
        {
            writer.writeUnsignedBits32(static_cast<uint32_t>(anotherValue), ANOTHER_CHILD_BIT_SIZE);
        }
    }

    static void fillData(ParentStructure& data)
    {
        data.numChildren = static_cast<uint8_t>(VALUES.size());

        auto& children = data.children;
        for (uint64_t value : VALUES)
        {
            ChildStructure child;
            child.value = value;
            children.push_back(child);
        }

        data.numAnotherChildren = static_cast<uint8_t>(ANOTHER_VALUES.size());

        auto& anotherChildren = data.anotherChildren;
        for (uint64_t anotherValue : ANOTHER_VALUES)
        {
            ChildStructure child;
            child.value = anotherValue;
            anotherChildren.push_back(child);
        }
    }

    static constexpr uint8_t CHILD_BIT_SIZE = 19;
    static constexpr uint8_t ANOTHER_CHILD_BIT_SIZE = 17;

private:
    static constexpr std::array<uint64_t, 2> VALUES = {0xAABB, 0xCCDD};
    static constexpr std::array<uint64_t, 2> ANOTHER_VALUES = {0xAABB, 0xCCDD};
};

TEST_F(StructureArrayParamTest, checkParentStructure)
{
    ParentStructure data;
    fillData(data);
    zserio::View view(data);

    ASSERT_EQ(CHILD_BIT_SIZE, view.getChildBitSize());
    ASSERT_EQ(ANOTHER_CHILD_BIT_SIZE, view.notLeftMost().getAnotherChildBitSize());

    test_utils::readTest(writeData, data);
    test_utils::writeReadTest(data);
}

} // namespace structure_array_param
} // namespace functions
