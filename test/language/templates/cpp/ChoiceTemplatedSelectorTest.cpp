#include "gtest/gtest.h"
#include "templates/choice_templated_selector/ChoiceTemplatedSelector.h"
#include "test_utils/TestUtility.h"
#include "zserio/RebindAlloc.h"

namespace templates
{
namespace choice_templated_selector
{

using AllocatorType = ChoiceTemplatedSelector::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<AllocatorType, char>>;

TEST(ChoiceTemplatedSelectorTest, writeRead)
{
    ChoiceTemplatedSelector data;
    data.selector16 = 0;
    data.selector32 = 1;
    {
        TemplatedChoice<zserio::UInt16, Shift16> uint16Choice;
        uint16Choice.emplace<TemplatedChoice<zserio::UInt16, Shift16>::Tag::uint16Field>(42);
        data.uint16Choice = std::move(uint16Choice);
    }
    {
        TemplatedChoice<zserio::UInt32, Shift32> uint32Choice;
        uint32Choice.emplace<TemplatedChoice<zserio::UInt32, Shift32>::Tag::stringField>("string");
        data.uint32Choice = uint32Choice; // copy
    }

    test_utils::writeReadTest(data);
}

} // namespace choice_templated_selector
} // namespace templates
