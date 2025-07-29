#include "gtest/gtest.h"
#include "offsets/offset_in_nested_choice_in_template/OffsetInNestedChoiceInTemplate.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace offset_in_nested_choice_in_template
{

class OffsetInNestedChoiceInTemplateTest : public ::testing::Test
{
protected:
    static OffsetInNestedChoiceInTemplate makeData()
    {
        OffsetInNestedChoiceInTemplate data;
        data.testStruct.hasOffset = true;
        NestedChoice<zserio::UInt32> nested(
                zserio::in_place_index<NestedChoice<zserio::UInt32>::Tag::offset>, 10);
        data.testStruct.choiceField.emplace<TestChoice<NestedChoice<zserio::UInt32>>::Tag::nestedChoice>(
                nested);
        return data;
    }
};

TEST_F(OffsetInNestedChoiceInTemplateTest, writeRead)
{
    auto data = makeData();
    test_utils::writeReadTest(data);
}

TEST_F(OffsetInNestedChoiceInTemplateTest, hash)
{
    auto data = makeData();
    ASSERT_EQ(std::hash<OffsetInNestedChoiceInTemplate>()(data), static_cast<size_t>(3500385));
}

} // namespace offset_in_nested_choice_in_template
} // namespace offsets
