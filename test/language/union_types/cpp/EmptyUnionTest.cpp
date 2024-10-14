#include "gtest/gtest.h"
#include "union_types/empty_union/EmptyUnion.h"

namespace union_types
{
namespace empty_union
{

using allocator_type = EmptyUnion::allocator_type;

TEST(EmptyUnionDataTest, emptyConstructor)
{
    {
        EmptyUnion emptyUnion;
    }
    {
        EmptyUnion emptyUnion = {};
    }
    {
        EmptyUnion emptyUnion(allocator_type{});
    }
}

TEST(EmptyUnionDataTest, copyConstructor)
{
    EmptyUnion emptyUnion;
    EmptyUnion emptyUnionCopy(emptyUnion);
    ASSERT_EQ(emptyUnion, emptyUnionCopy);
}

TEST(EmptyUnionDataTest, assignmentOperator)
{
    EmptyUnion emptyUnion;
    EmptyUnion emptyUnionCopy;
    emptyUnionCopy = emptyUnion;
    ASSERT_EQ(emptyUnion, emptyUnionCopy);
}

TEST(EmptyUnionDataTest, moveConstructor)
{
    EmptyUnion emptyUnion;
    EmptyUnion emptyUnionMoved(std::move(emptyUnion));
    (void)emptyUnionMoved;
}

TEST(EmptyUnionDataTest, moveAssignmentOperator)
{
    EmptyUnion emptyUnion;
    EmptyUnion emptyUnionMoved;
    emptyUnionMoved = std::move(emptyUnion);
    (void)emptyUnionMoved;
}

TEST(EmptyUnionDataTest, operatorEquality)
{
    EmptyUnion emptyUnion1;
    EmptyUnion emptyUnion2;
    ASSERT_TRUE(emptyUnion1 == emptyUnion2);
}

TEST(EmptyUnionDataTest, operatorLessThan)
{
    EmptyUnion emptyUnion1;
    EmptyUnion emptyUnion2;
    ASSERT_FALSE(emptyUnion1 < emptyUnion2);
    ASSERT_FALSE(emptyUnion2 < emptyUnion1);
}

TEST(EmptyUnionDataTest, stdHash)
{
    std::hash<EmptyUnion> hasher;

    EmptyUnion emptyUnion1;
    EmptyUnion emptyUnion2;
    ASSERT_EQ(hasher(emptyUnion1), hasher(emptyUnion2));

    // TODO[Mi-L@]: hashCode not stable across extensions!
    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(23, hasher(emptyUnion1));
}

} // namespace empty_union
} // namespace union_types
