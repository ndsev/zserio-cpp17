#include "choice_types/expression_selector_choice/ExpressionSelectorChoice.h"
#include "gtest/gtest.h"

namespace choice_types
{
namespace expression_selector_choice
{

using ChoiceTag = ExpressionSelectorChoice::ChoiceTag;

TEST(ExpressionSelectorChoiceTest, field8)
{
    const uint16_t selector = 0;
    const zserio::UInt8 value = 0x7F;
    ExpressionSelectorChoice data(zserio::in_place_index<ChoiceTag::CHOICE_field8>, value);
    zserio::View view(data, selector);

    ASSERT_EQ(value, view.field8());
    ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
}

TEST(ExpressionSelectorChoiceTest, field16)
{
    const uint16_t selector = 1;
    const zserio::UInt16 value = 0x7F7F;
    ExpressionSelectorChoice data(zserio::in_place_index<ChoiceTag::CHOICE_field16>, value);
    zserio::View view(data, selector);

    ASSERT_EQ(value, view.field16());
    ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
}

TEST(ExpressionSelectorChoiceTest, field32)
{
    const uint16_t selector = 2;
    const zserio::UInt32 value = 0x7F7F7F7F;
    ExpressionSelectorChoice data(zserio::in_place_index<ChoiceTag::CHOICE_field32>, value);
    zserio::View view(data, selector);

    ASSERT_EQ(value, view.field32());
    ASSERT_EQ(32, zserio::detail::bitSizeOf(view));
}

} // namespace expression_selector_choice
} // namespace choice_types
