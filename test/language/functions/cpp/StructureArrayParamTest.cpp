#include <array>
#include <vector>

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
    void writeData(zserio::BitStreamWriter& writer)
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

    void fillData(ParentStructure& data)
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

    static const uint8_t CHILD_BIT_SIZE = 19;
    static const uint8_t ANOTHER_CHILD_BIT_SIZE = 17;

private:
    static const std::array<uint64_t, 2> VALUES;
    static const std::array<uint64_t, 2> ANOTHER_VALUES;
};

const std::array<uint64_t, 2> StructureArrayParamTest::VALUES = {0xAABB, 0xCCDD};

const std::array<uint64_t, 2> StructureArrayParamTest::ANOTHER_VALUES = {0xAABB, 0xCCDD};

TEST_F(StructureArrayParamTest, checkParentStructure)
{
    ParentStructure data;
    fillData(data);
    zserio::View<ParentStructure> view(data);

    const uint8_t expectedChildBitSize = CHILD_BIT_SIZE;
    ASSERT_EQ(expectedChildBitSize, view.getChildBitSize());
    const uint8_t expectedAnotherChildBitSize = ANOTHER_CHILD_BIT_SIZE;
    ASSERT_EQ(expectedAnotherChildBitSize, view.notLeftMost().getAnotherChildBitSize());

    zserio::BitBuffer expectedBitBuffer = zserio::BitBuffer(1024 * 8);
    zserio::BitStreamWriter expectedWriter(expectedBitBuffer);
    writeData(expectedWriter);

    test_utils::readTest(expectedWriter, data);
    test_utils::writeReadTest(data);
}

} // namespace structure_array_param
} // namespace functions
