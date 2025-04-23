#include "gtest/gtest.h"
#include "offsets/offset_in_nested_union/TestStruct.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace offset_in_nested_union
{

class OffsetInNestedUnionTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeVarSize(0); // unionField choiceTag
        writer.writeVarSize(0); // nestedUnion choiceTag
        writer.writeUnsignedBits32(OFFSET);
        writer.writeUnsignedBits32(U32_FIELD);
    }

    static void fillData(TestStruct& data, bool setOffset = false)
    {
        data.unionField.emplace<TestUnion::Tag::nestedUnion>().emplace<NestedUnion::Tag::offset>(
                setOffset ? OFFSET : 0);
        data.u32Field = U32_FIELD;
    }

    static void checkOffset(const zserio::View<TestStruct>& view)
    {
        ASSERT_EQ(OFFSET, view.unionField().nestedUnion().offset());
    }

private:
    static constexpr uint32_t OFFSET = 1 + 1 + 4;
    static constexpr uint32_t U32_FIELD = 13;
};

TEST_F(OffsetInNestedUnionTest, read)
{
    TestStruct data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(OffsetInNestedUnionTest, writeRead)
{
    TestStruct data;
    fillData(data);
    zserio::View view(data);

    test_utils::writeReadTest(data);
    checkOffset(view);
}

} // namespace offset_in_nested_union
} // namespace offsets
