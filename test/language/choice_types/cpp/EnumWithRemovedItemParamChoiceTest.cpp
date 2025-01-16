#include "choice_types/enum_with_removed_item_param_choice/EnumWithRemovedItemParamChoice.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace choice_types
{
namespace enum_with_removed_item_param_choice
{

using ChoiceTag = EnumWithRemovedItemParamChoice::ChoiceTag;

TEST(EnumWithRemovedItemParamChoiceTest, writeRead)
{
    EnumWithRemovedItemParamChoice data(zserio::in_place_index<ChoiceTag::CHOICE_greyData>, 0xCAFE);
    test_utils::writeReadTest(data, Selector::ZSERIO_REMOVED_GREY);
}

} // namespace enum_with_removed_item_param_choice
} // namespace choice_types
