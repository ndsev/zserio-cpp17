#include <array>
#include <vector>

#include "functions/structure_array/Item.h"
#include "functions/structure_array/StructureArray.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace structure_array
{

class StructureArrayTest : public ::testing::Test
{
protected:
    void writeData(zserio::BitStreamWriter& writer, uint16_t pos)
    {
        writer.writeUnsignedBits32(static_cast<uint32_t>(ITEMS.size()), 16);

        for (Item item : ITEMS)
        {
            zserio::detail::write(writer, item.valueA);
            zserio::detail::write(writer, item.valueB);
        }

        writer.writeUnsignedBits32(pos, 16);
    }

    void fillData(StructureArray& structureArray, uint16_t pos)
    {
        structureArray.numElements = static_cast<uint16_t>(ITEMS.size());

        auto& values = structureArray.values;
        values.assign(ITEMS.begin(), ITEMS.end());

        structureArray.pos = pos;
    }

    void checkStructureArray(uint16_t pos)
    {
        StructureArray data;
        fillData(data, pos);
        zserio::View<StructureArray> view(data);

        const auto& readElement = view.getElement();
        ASSERT_EQ(zserio::View<Item>(ITEMS.at(pos)), readElement);

        zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
        zserio::BitBuffer expectedBitBuffer = zserio::BitBuffer(bitSize);
        zserio::BitStreamWriter expectedWriter(expectedBitBuffer);
        writeData(expectedWriter, pos);

        test_utils::readTest(expectedWriter, data);
        test_utils::writeReadTest(data);
    }

private:
    static const std::array<Item, 3> ITEMS;
};

const std::array<Item, 3> StructureArrayTest::ITEMS = {Item{1, 2}, Item{3, 4}, Item{5, 6}};

TEST_F(StructureArrayTest, checkStructureArrayElement0)
{
    checkStructureArray(0);
}

TEST_F(StructureArrayTest, checkStructureArrayElement1)
{
    checkStructureArray(1);
}

TEST_F(StructureArrayTest, checkStructureArrayElement2)
{
    checkStructureArray(2);
}

} // namespace structure_array
} // namespace functions
