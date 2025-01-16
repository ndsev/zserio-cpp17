#include "choice_types/function_selector_choice/TestChoice.h"
#include "gtest/gtest.h"

namespace choice_types
{
namespace function_selector_choice
{

using ChoiceTag = TestChoice::ChoiceTag;

TEST(FunctionSelectorChoiceTest, field8)
{
    Selector selector(8);
    zserio::View selectorView(selector);
    TestChoice data(zserio::in_place_index<ChoiceTag::CHOICE_field8>, 0x7F);
    zserio::View view(data, selectorView);

    ASSERT_EQ(0x7F, view.field8());
    ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
}

TEST(FunctionSelectorChoiceTest, field16)
{
    Selector selector(16);
    zserio::View selectorView(selector);
    TestChoice data(zserio::in_place_index<ChoiceTag::CHOICE_field16>, 0x7F7F);
    zserio::View view(data, selectorView);

    ASSERT_EQ(0x7F7F, view.field16());
    ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
}

} // namespace function_selector_choice
} // namespace choice_types
