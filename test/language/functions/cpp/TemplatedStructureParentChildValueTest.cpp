#include "functions/templated_structure_parent_child_value/TemplatedStructureParentChildValue.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace functions
{
namespace templated_structure_parent_child_value
{

TEST(TemplatedStructureParentChildValueTest, writeRead)
{
    TemplatedStructureParentChildValue data;
    data.parentValue.childValue.val = 26;
    test_utils::writeReadTest(data);
}

TEST(TemplatedStructureParentChildValueTest, hash)
{
    TemplatedStructureParentChildValue data;
    ASSERT_EQ(std::hash<TemplatedStructureParentChildValue>()(data), static_cast<size_t>(2553));
}

TEST(TemplatedStructureParentChildValueTest, checkFunctions)
{
    TemplatedStructureParentChildValue data;
    data.parentValue.childValue.val = 26;

    zserio::View childView(data.parentValue.childValue);
    ASSERT_EQ(childView.getValue(), data.parentValue.childValue.val);

    zserio::View parentView(data.parentValue);
    ASSERT_EQ(parentView.getValue(), childView.getValue());
}

} // namespace templated_structure_parent_child_value
} // namespace functions
