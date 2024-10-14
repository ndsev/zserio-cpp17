#include "gtest/gtest.h"
#include "union_types/simple_union/SimpleUnion.h"

namespace union_types
{
namespace simple_union
{

using allocator_type = SimpleUnion::allocator_type;
using string_type = zserio::basic_string<allocator_type>;

class SimpleUnionDataTest : public ::testing::Test
{
protected:
    static constexpr int8_t CASE1_FIELD = 13;
    static constexpr uint16_t CASE2_FIELD = 65535;
    static constexpr std::string_view CASE3_FIELD = "SimpleUnion";
    static constexpr int8_t CASE4_FIELD = 42;
};

TEST_F(SimpleUnionDataTest, emptyConstructor)
{
    {
        SimpleUnion simpleUnion;
        ASSERT_EQ(SimpleUnion::UNDEFINED_CHOICE, simpleUnion.variant.index());
    }
    {
        SimpleUnion simpleUnion = {};
        ASSERT_EQ(SimpleUnion::UNDEFINED_CHOICE, simpleUnion.variant.index());
    }
    {
        SimpleUnion simpleUnion(allocator_type{});
        ASSERT_EQ(SimpleUnion::UNDEFINED_CHOICE, simpleUnion.variant.index());
    }
}

TEST_F(SimpleUnionDataTest, copyConstructor)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    ASSERT_EQ(CASE1_FIELD, zserio::get<SimpleUnion::CHOICE_case1Field>(simpleUnion.variant));

    SimpleUnion simpleUnionCopy(simpleUnion);
    ASSERT_EQ(CASE1_FIELD, zserio::get<SimpleUnion::CHOICE_case1Field>(simpleUnion.variant));
}

TEST_F(SimpleUnionDataTest, assignmentOperator)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case2Field>(CASE2_FIELD);
    ASSERT_EQ(CASE2_FIELD, zserio::get<SimpleUnion::CHOICE_case2Field>(simpleUnion.variant));

    SimpleUnion simpleUnionCopy;
    simpleUnionCopy = simpleUnion;
    ASSERT_EQ(CASE2_FIELD, zserio::get<SimpleUnion::CHOICE_case2Field>(simpleUnionCopy.variant));
}

TEST_F(SimpleUnionDataTest, moveConstructor)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case3Field>(CASE3_FIELD);
    ASSERT_EQ(CASE3_FIELD, zserio::get<SimpleUnion::CHOICE_case3Field>(simpleUnion.variant));

    SimpleUnion simpleUnionMoved(std::move(simpleUnion));
    ASSERT_EQ(CASE3_FIELD, zserio::get<SimpleUnion::CHOICE_case3Field>(simpleUnionMoved.variant));
}

TEST_F(SimpleUnionDataTest, moveAssignmentOperator)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE4_FIELD);
    ASSERT_EQ(CASE4_FIELD, zserio::get<SimpleUnion::CHOICE_case4Field>(simpleUnion.variant));

    SimpleUnion simpleUnionMoved;
    simpleUnionMoved = std::move(simpleUnion);
    ASSERT_EQ(CASE4_FIELD, zserio::get<SimpleUnion::CHOICE_case4Field>(simpleUnionMoved.variant));
}

TEST_F(SimpleUnionDataTest, index)
{
    SimpleUnion simpleUnion;
    ASSERT_EQ(SimpleUnion::UNDEFINED_CHOICE, simpleUnion.variant.index());
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    ASSERT_EQ(SimpleUnion::CHOICE_case1Field, simpleUnion.variant.index());
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case2Field>(CASE2_FIELD);
    ASSERT_EQ(SimpleUnion::CHOICE_case2Field, simpleUnion.variant.index());
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case3Field>(CASE3_FIELD);
    ASSERT_EQ(SimpleUnion::CHOICE_case3Field, simpleUnion.variant.index());
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE4_FIELD);
    ASSERT_EQ(SimpleUnion::CHOICE_case4Field, simpleUnion.variant.index());
}

TEST_F(SimpleUnionDataTest, getCase1Field)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    ASSERT_EQ(CASE1_FIELD, zserio::get<SimpleUnion::CHOICE_case1Field>(simpleUnion.variant));
}

TEST_F(SimpleUnionDataTest, getCase2Field)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case2Field>(CASE2_FIELD);
    ASSERT_EQ(CASE2_FIELD, zserio::get<SimpleUnion::CHOICE_case2Field>(simpleUnion.variant));
}

TEST_F(SimpleUnionDataTest, getCase3Field)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case3Field>(CASE3_FIELD);
    ASSERT_EQ(CASE3_FIELD, zserio::get<SimpleUnion::CHOICE_case3Field>(simpleUnion.variant));

    string_type movedString(1000, 'a'); // long enough to prevent small string optimization
    const void* ptr = movedString.data();
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case3Field>(std::move(movedString));
    const void* movedPtr = zserio::get<SimpleUnion::CHOICE_case3Field>(simpleUnion.variant).data();
    ASSERT_EQ(ptr, movedPtr);
    string_type& case3 = zserio::get<SimpleUnion::CHOICE_case3Field>(simpleUnion.variant);
    case3 = CASE3_FIELD;
    ASSERT_EQ(CASE3_FIELD, zserio::get<SimpleUnion::CHOICE_case3Field>(simpleUnion.variant));
}

TEST_F(SimpleUnionDataTest, getCase4Field)
{
    SimpleUnion simpleUnion;
    simpleUnion.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE4_FIELD);
    ASSERT_EQ(CASE4_FIELD, zserio::get<SimpleUnion::CHOICE_case4Field>(simpleUnion.variant));
}

TEST_F(SimpleUnionDataTest, operatorEquality)
{
    SimpleUnion simpleUnion11;
    SimpleUnion simpleUnion12;
    SimpleUnion simpleUnion13;
    ASSERT_TRUE(simpleUnion11 == simpleUnion11);
    ASSERT_TRUE(simpleUnion11 == simpleUnion12);
    simpleUnion11.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    simpleUnion12.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    simpleUnion13.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD + 1);
    ASSERT_TRUE(simpleUnion11 == simpleUnion11);
    ASSERT_TRUE(simpleUnion11 == simpleUnion12);
    ASSERT_FALSE(simpleUnion11 == simpleUnion13);

    SimpleUnion simpleUnion21;
    SimpleUnion simpleUnion22;
    SimpleUnion simpleUnion23;
    simpleUnion21.variant.emplace<SimpleUnion::CHOICE_case2Field>(CASE2_FIELD);
    simpleUnion22.variant.emplace<SimpleUnion::CHOICE_case2Field>(CASE2_FIELD);
    simpleUnion23.variant.emplace<SimpleUnion::CHOICE_case2Field>(CASE2_FIELD - 1);
    ASSERT_TRUE(simpleUnion21 == simpleUnion21);
    ASSERT_TRUE(simpleUnion21 == simpleUnion22);
    ASSERT_FALSE(simpleUnion21 == simpleUnion23);
    ASSERT_FALSE(simpleUnion21 == simpleUnion11);

    SimpleUnion simpleUnion4;
    // same value as simpleUnion11, but different choice
    simpleUnion4.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE1_FIELD);
    ASSERT_FALSE(simpleUnion11 == simpleUnion4);
}

TEST_F(SimpleUnionDataTest, operatorLessThan)
{
    SimpleUnion simpleUnion1;
    SimpleUnion simpleUnion2;
    ASSERT_FALSE(simpleUnion1 < simpleUnion2);
    ASSERT_FALSE(simpleUnion2 < simpleUnion1);

    simpleUnion1.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    ASSERT_FALSE(simpleUnion1 < simpleUnion2);
    ASSERT_TRUE(simpleUnion2 < simpleUnion1);

    simpleUnion2.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    ASSERT_FALSE(simpleUnion1 < simpleUnion2);
    ASSERT_FALSE(simpleUnion2 < simpleUnion1);

    simpleUnion2.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD + 1);
    ASSERT_TRUE(simpleUnion1 < simpleUnion2);
    ASSERT_FALSE(simpleUnion2 < simpleUnion1);

    simpleUnion2.variant.emplace<SimpleUnion::CHOICE_case2Field>(CASE2_FIELD);
    ASSERT_TRUE(simpleUnion1 < simpleUnion2);
    ASSERT_FALSE(simpleUnion2 < simpleUnion1);

    // same value as simpleUnion11, but different choice
    simpleUnion2.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE1_FIELD);
    ASSERT_TRUE(simpleUnion1 < simpleUnion2);
    ASSERT_FALSE(simpleUnion2 < simpleUnion1);
}

TEST_F(SimpleUnionDataTest, stdHash)
{
    std::hash<SimpleUnion> hasher;

    SimpleUnion simpleUnion1;
    SimpleUnion simpleUnion2;
    ASSERT_EQ(hasher(simpleUnion1), hasher(simpleUnion2));
    simpleUnion1.variant.emplace<SimpleUnion::CHOICE_case1Field>(CASE1_FIELD);
    ASSERT_NE(hasher(simpleUnion1), hasher(simpleUnion2));
    simpleUnion2.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE4_FIELD);
    ASSERT_NE(hasher(simpleUnion1), hasher(simpleUnion2));

    // TODO[Mi-L@]: hashCode not stable across extensions!
    // use hardcoded values to check that the hash code is stable
    ASSERT_EQ(31537, hasher(simpleUnion1));
    ASSERT_EQ(31677, hasher(simpleUnion2));

    simpleUnion2.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE1_FIELD); // same value as simpleUnion1
    ASSERT_NE(hasher(simpleUnion1), hasher(simpleUnion2));
    simpleUnion1.variant.emplace<SimpleUnion::CHOICE_case4Field>(CASE1_FIELD); // same value as simpleUnion2
    ASSERT_EQ(hasher(simpleUnion1), hasher(simpleUnion2));
}

} // namespace simple_union
} // namespace union_types
