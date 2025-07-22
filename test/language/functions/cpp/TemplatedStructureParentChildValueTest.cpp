#include "functions/templated_structure_parent_child_value/TemplatedStructureParentChildValue.h"
#include "gtest/gtest.h"
#include "test_utils/TestUtility.h"

namespace functions
{
namespace templated_structure_parent_child_value
{

TEST(TemplatedStructureParentChildValueTest, writeReadTest)
{
    TemplatedStructureParentChildValue data;
    test_utils::writeReadTest(data);
}

TEST(TemplatedStructureParentChildValueTest, hashTest)
{
    TemplatedStructureParentChildValue data;
    ASSERT_EQ(std::hash<TemplatedStructureParentChildValue>()(data), static_cast<size_t>(2553));
}

} // namespace templated_structure_parent_child_value
} // namespace functions
