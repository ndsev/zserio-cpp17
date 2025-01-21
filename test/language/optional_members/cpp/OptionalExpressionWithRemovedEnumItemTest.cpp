#include "gtest/gtest.h"
#include "optional_members/optional_expression_with_removed_enum_item/Compound.h"
#include "test_utils/TestUtility.h"

namespace optional_members
{
namespace optional_expression_with_removed_enum_item
{

TEST(OptionalExpressionWithRemovedEnumItemTest, writeRead)
{
    Compound data(12, {{1, 2}});
    test_utils::writeReadTest(data);
}

} // namespace optional_expression_with_removed_enum_item
} // namespace optional_members
