#include "gtest/gtest.h"
#include "templates/instantiate_type_as_choice_field/InstantiateTypeAsChoiceField.h"
#include "test_utils/TestUtility.h"

namespace templates
{
namespace instantiate_type_as_choice_field
{

TEST(InstantiateTypeAsChoiceFieldTest, readWrite)
{
    InstantiateTypeAsChoiceField data;
    data.emplace<InstantiateTypeAsChoiceField::Tag::test>(Test32{13});

    test_utils::writeReadTest(data, true);
}

} // namespace instantiate_type_as_choice_field
} // namespace templates
