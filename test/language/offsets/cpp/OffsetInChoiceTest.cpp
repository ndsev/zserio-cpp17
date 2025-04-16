#include "gtest/gtest.h"
#include "offsets/offset_in_choice/TestStruct.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace offset_in_choice
{

class OffsetInChoiceTest : public ::testing::Test
{
protected:
    static void writeData(zserio::BitStreamWriter& writer)
    {
        writer.writeBool(HAS_OFFSET);
        writer.writeUnsignedBits32(OFFSET);
        writer.alignTo(8);
        writer.writeUnsignedBits32(U32_FIELD);
    }

    static void fillData(TestStruct& data, bool setOffset = false)
    {
        data.hasOffset = HAS_OFFSET;
        data.choiceField.emplace<TestChoice::ChoiceTag::CHOICE_offset>(setOffset ? OFFSET : 0);
        data.u32Field = U32_FIELD;
    }

    static void checkOffset(const zserio::View<TestStruct>& view)
    {
        ASSERT_EQ(OFFSET, view.choiceField().offset());
    }

private:
    static constexpr bool HAS_OFFSET = true;
    static constexpr uint32_t OFFSET = zserio::alignTo(8, 1 + 32) / 8;
    static constexpr uint32_t U32_FIELD = 13;
};

TEST_F(OffsetInChoiceTest, read)
{
    TestStruct data;
    fillData(data, true);

    test_utils::readTest(writeData, data);
}

TEST_F(OffsetInChoiceTest, writeRead)
{
    TestStruct data;
    fillData(data);
    zserio::View view(data);

    test_utils::writeReadTest(data);
    checkOffset(view);
}

} // namespace offset_in_choice
} // namespace offsets
