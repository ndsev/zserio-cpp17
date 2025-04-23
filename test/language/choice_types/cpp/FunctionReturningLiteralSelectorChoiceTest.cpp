#include "choice_types/function_returning_literal_selector_choice/TestChoice.h"
#include "gtest/gtest.h"

namespace choice_types
{
namespace function_returning_literal_selector_choice
{

TEST(FunctionReturningLiteralSelectorChoiceTest, field8)
{
    Selector selector;
    zserio::View selectorView(selector, false);
    TestChoice data(zserio::in_place_index<TestChoice::Tag::field8>, 0x7F);
    zserio::View view(data, selectorView);

    ASSERT_EQ(0x7F, view.field8());
    ASSERT_EQ(8, zserio::detail::bitSizeOf(view));
}

TEST(FunctionReturningLiteralSelectorChoiceTest, field16)
{
    Selector selector;
    zserio::View selectorView(selector, true);
    TestChoice data(zserio::in_place_index<TestChoice::Tag::field16>, 0x7F7F);
    zserio::View view(data, selectorView);

    ASSERT_EQ(0x7F7F, view.field16());
    ASSERT_EQ(16, zserio::detail::bitSizeOf(view));
}

} // namespace function_returning_literal_selector_choice
} // namespace choice_types
