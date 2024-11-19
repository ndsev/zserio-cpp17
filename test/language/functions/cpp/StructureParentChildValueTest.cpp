#include <vector>

#include "functions/structure_parent_child_value/ParentValue.h"
#include "gtest/gtest.h"
#include "test_utils/ReadTest.h"
#include "test_utils/WriteReadTest.h"
#include "zserio/BitStreamWriter.h"

namespace functions
{
namespace structure_parent_child_value
{

class StructureParentChildValueTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeUnsignedBits32(CHILD_VALUE, 32);
    }

    static void fillData(ParentValue& data)
    {
        data.childValue.val = CHILD_VALUE;
    }

    static constexpr uint32_t CHILD_VALUE = 0xABCD;
};

TEST_F(StructureParentChildValueTest, checkParentValue)
{
    ParentValue data;
    fillData(data);
    zserio::View<ParentValue> view(data);

    ASSERT_EQ(CHILD_VALUE, view.getValue());

    test_utils::readTest(writeData, data);
    test_utils::writeReadTest(data);
}

} // namespace structure_parent_child_value
} // namespace functions
