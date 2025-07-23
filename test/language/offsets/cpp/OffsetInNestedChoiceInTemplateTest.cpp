#include "gtest/gtest.h"
#include "offsets/offset_in_nested_choice_in_template/OffsetInNestedChoiceInTemplate.h"
#include "test_utils/TestUtility.h"

namespace offsets
{
namespace offset_in_nested_choice_in_template
{

using AllocatorType = OffsetInNestedChoiceInTemplate::allocator_type;
using String = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;
template <typename T>
using Vector = zserio::Vector<T, zserio::RebindAlloc<AllocatorType, T>>;

TEST(OffsetInNestedChoiceInTemplateTest, writeReadTest)
{
    OffsetInNestedChoiceInTemplate data;
    ASSERT_EQ(data.testStruct.hasOffset, false);
    data.testStruct.hasOffset = true;
    NestedChoice<zserio::UInt32> nested(zserio::in_place_index<NestedChoice<zserio::UInt32>::Tag::offset>, 10);
    data.testStruct.choiceField.emplace<TestChoice<NestedChoice<zserio::UInt32>>::Tag::nestedChoice>(nested);
    test_utils::writeReadTest(data);
}

TEST(OffsetInNestedChoiceInTemplateTest, hashTest)
{
    OffsetInNestedChoiceInTemplate data;
    ASSERT_EQ(std::hash<OffsetInNestedChoiceInTemplate>()(data), static_cast<size_t>(2394862));
}

} // namespace offset_in_nested_choice_in_template
} // namespace offsets
