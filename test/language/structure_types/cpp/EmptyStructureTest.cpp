#include "gtest/gtest.h"
#include "structure_types/empty_structure/EmptyStructure.h"

namespace structure_types
{
namespace empty_structure
{

using allocator_type = EmptyStructure::allocator_type;

TEST(EmptyStructureDataTest, emptyConstructor)
{
    {
        EmptyStructure emptyStructure;
    }
    {
        EmptyStructure emptyStructure = {};
    }
    {
        EmptyStructure emptyStructure(allocator_type{});
    }
}

TEST(EmptyStructureDataTest, copyConstructor)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureCopy(emptyStructure);
    ASSERT_EQ(emptyStructure, emptyStructureCopy);
}

TEST(EmptyStructureDataTest, assignmentOperator)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureCopy;
    emptyStructureCopy = emptyStructure;
    ASSERT_EQ(emptyStructure, emptyStructureCopy);
}

TEST(EmptyStructureDataTest, moveConstructor)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureMoved(std::move(emptyStructure));
    (void)emptyStructureMoved;
}

TEST(EmptyStructureDataTest, moveAssignmentOperator)
{
    EmptyStructure emptyStructure;
    EmptyStructure emptyStructureMoved;
    emptyStructureMoved = std::move(emptyStructure);
    (void)emptyStructureMoved;
}

TEST(EmptyStructureDataTest, operatorEquality)
{
    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;
    ASSERT_TRUE(emptyStructure1 == emptyStructure2);
}

TEST(EmptyStructureDataTest, operatorLessThan)
{
    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;
    ASSERT_FALSE(emptyStructure1 < emptyStructure2);
    ASSERT_FALSE(emptyStructure2 < emptyStructure1);
}

TEST(EmptyStructureDataTest, stdHash)
{
    std::hash<EmptyStructure> hasher;

    EmptyStructure emptyStructure1;
    EmptyStructure emptyStructure2;
    ASSERT_EQ(hasher(emptyStructure1), hasher(emptyStructure2));

    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(23, hasher(emptyStructure1));
}

} // namespace empty_structure
} // namespace structure_types
