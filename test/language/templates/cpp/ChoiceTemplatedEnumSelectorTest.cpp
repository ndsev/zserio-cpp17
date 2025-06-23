#include "gtest/gtest.h"
#include "templates/choice_templated_enum_selector/ChoiceTemplatedEnumSelector.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace choice_templated_enum_selector
{

using AllocatorType = ChoiceTemplatedEnumSelector::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(ChoiceTemplatedEnumSelectorTest, writeRead)
{
    ChoiceTemplatedEnumSelector data;
    data.selectorFromZero = EnumFromZero::ONE;
    data.selectorFromOne = EnumFromOne::THREE;
    {
        TemplatedChoice<EnumFromZero> fromZeroChoice;
        fromZeroChoice.emplace<TemplatedChoice<EnumFromZero>::Tag::uint16Field>(42);
        data.fromZeroChoice = std::move(fromZeroChoice);
    }
    {
        TemplatedChoice<EnumFromOne> fromOneChoice;
        fromOneChoice.emplace<TemplatedChoice<EnumFromOne>::Tag::stringField>("string");
        data.fromOneChoice = fromOneChoice; // copy
    }

    test_utils::writeReadTest(data);
}

} // namespace choice_templated_enum_selector
} // namespace templates
