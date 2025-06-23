#include "gtest/gtest.h"
#include "templates/choice_templated_field/ChoiceTemplatedField.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace choice_templated_field
{

TEST(ChoiceTemplatedFieldTest, writeRead)
{
    ChoiceTemplatedField data;
    data.selector = 0;
    {
        TemplatedChoice<zserio::UInt32, zserio::UInt16> choice1;
        choice1.emplace<TemplatedChoice<zserio::UInt32, zserio::UInt16>::Tag::templatedField1>(42);
        data.choice1 = std::move(choice1);
    }
    {
        TemplatedChoice<Compound<zserio::UInt32>, zserio::UInt16> choice2;
        choice2.emplace<TemplatedChoice<Compound<zserio::UInt32>, zserio::UInt16>::Tag::templatedField1>(
                Compound<zserio::UInt32>{42});
        data.choice2 = choice2; // copy
    }

    test_utils::writeReadTest(data);
}

} // namespace choice_templated_field
} // namespace templates
